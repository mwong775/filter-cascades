#include <bits/stdc++.h>
#include <random>
#include <unistd.h>
#include "hashutil.h"
#include "cuckoo.h"
#include <time.h>
#include <ratio>
#include <chrono>

#define debug(a) cerr << #a << " = " << a << ' '
#define deln(a)  cerr << #a << " = " << a << endl
#define memcle(a) memset(a, 0, sizeof(a))

using namespace std;

double time_cost(chrono::steady_clock::time_point &start,
                 chrono::steady_clock::time_point &end)
{
     /* Return the time elapse between start and end
      * count by seconds
      */
    double elapsedSeconds = ((end - start).count()) * chrono::steady_clock::period::num / static_cast<double>(chrono::steady_clock::period::den);
    return elapsedSeconds;
}

// generate n 64-bit random numbers
void random_gen(int n, vector<uint64_t> &store, mt19937 &rd)
{
    store.resize(n); // uses Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits..?
    printf("generating dem random num nums\n");
    for (int i = 0; i < n; i++) {
        store[i] = (uint64_t(rd()) << 32) + rd();
        // printf("%lu\n", store[i]);
    }
}

void test_ins(int n = 0, int q = 0, int rept = 1)
{
    FILE *out = fopen("bloom_throughput_ins.csv", "a");
    assert(out != NULL);

    if (n == 0)
        n = (1 << 27) * 0.95;
    if (q == 0)
        q = 10000000;

    int seed = 1;

    mt19937 rd(seed);
    double mop[6][30], mop1[6][30];
    int cnt[6][30], cnt1[6][30];

    memcle(mop); // I think it sets all elements to 0
    memcle(cnt);
    memcle(mop1);
    memcle(cnt1);

    printf("bloom insert\n");
    for (int t = 0; t < rept; t++)
    {
        vector<uint64_t> insKey;
        vector<uint64_t> lupKey;
        random_gen(n, insKey, rd);
        random_gen(q, lupKey, rd);

        BloomFilter<uint16_t, 15> bf;
        bf.init(n, 1);

        int i = 0;
        int j = 0;
        int lim;

        auto tot_start = chrono::steady_clock::now();

        for (double r = 0.05; r <= 0.96; r += 0.05, ++j) // increasing occupancy
        {
            lim = int(n * r); // limit for filling bf
            printf("lim: %d\n", lim);
            int ins_cnt = 0;

            auto start = chrono::steady_clock::now(); // times inserts
            for (; i < lim; i++) 
            {
                //printf("%d\n", i);
                bf.insert(insKey[i]);
                ++ins_cnt;
                // printf("insert count: %d\n", ins_cnt);
            }

            auto end = chrono::steady_clock::now();
            double cost = time_cost(start, end);
            mop[0][j] += double(ins_cnt) / 1000000.0 / cost; // larger than cost, which is less than 1 -> 0.000...
            cnt[0][j] += 1;
            printf("limit: %d, occup: %.2f, total insert time: %.5f, time: %.5f\n", lim, r, cost, double(ins_cnt) / 1000000.0 / cost);
        }

        auto tot_end = chrono::steady_clock::now();

        mop[0][j] += double(lim) / 1000000.0 / time_cost(tot_start, tot_end);
        cnt[0][j] += 1;
    }


    fprintf(out, "occupancy, bloom, key-Size = %d\n", n); // wtf is bloom

    for (int j = 0; j < 20; j++) 
    {
        fprintf(out, "%.2f, ", (j + 1) * 0.05); // occupancy
        for (int k = 0; k < 1; k++)
            fprintf(out, "%.5f, ", mop[k][j] / cnt[k][j]);
        fprintf(out, "\n");
    }
    fclose(out);
}

void test_lookup(int n = 0, int q = 0, int rept = 1)
{

    FILE *out = fopen("bloom_throughput_lookup.csv", "a");
    assert(out != NULL);

    if (n == 0)
        n = (1 << 27) * 0.95;
    if (q == 0)
        q = 10000000;
    int seed = 1;
    double neg_frac = 0.5; // what is this

    mt19937 rd(seed);
    double mop[6][20], mop1[6][20];
    int cnt[6][20], cnt1[6][20];
    memcle(mop);
    memcle(cnt);
    memcle(mop1);
    memcle(cnt1);

    printf("bloom lookup\n");
    for (int t = 0; t < rept; t++)
    {
        vector<uint64_t> insKey;
        vector<uint64_t> lupKey;
        random_gen(n, insKey, rd);
        random_gen(q, lupKey, rd);
       
        BloomFilter<uint16_t, 15> bf;
        bf.init(n, 1);

        int i = 0;
        int ins_cnt = 0;
        int j = 0;

        for (double r = 0.05; r <= 0.96; r += 0.05, j++) // testing increasing occupancy
        {
            printf("%.2f\n", r);
            int lim = int(n * r); // limit? to fill bf
            printf("lim: %d\n", lim);
            for (; i < lim; i++) 
                if (bf.insert(insKey[i]) == true) {
                    ++ins_cnt;
                    printf("insert count: %d\n", ins_cnt); // never printed? maybe true = collision thing
                }

            auto start = chrono::steady_clock::now();

            int lookup_number = 0;
            int p = min(q, lim);
            int k = 0;

            for (k = 0; k < p * neg_frac; k++) // neg_frac = 0.5
                if (bf.lookup(lupKey[k]) == false) {
                    lookup_number++;
                    // printf("neg lookup: %d false/miss\n", lookup_number);
                }
            printf("lupkey count: %d\n", lookup_number); // this has half of limit

            for (; k < p; k++)
                if (bf.lookup(insKey[k]) == true) { // first lookup hits, has other half 
                    lookup_number++;
                    // printf("lookup: %d hit\n", lookup_number);
                }

            printf("neg lookup_count: %d\n", lookup_number);

            auto end = chrono::steady_clock::now();
            double cost = time_cost(start, end);

            mop[0][j] += double(p) / 1000000.0 / cost; // are these seriously 2d arrays that only use the first row -__-
            cnt[0][j] += 1;

            start = chrono::steady_clock::now();

            int t = min(q, lim);
            for (int i = 0; i < t; i++)
                if (bf.lookup(insKey[i]) == true) { // second lookup hits
                    lookup_number++;
                    // printf("pos lookup: %d hit\n", lookup_number);
                }

            end = chrono::steady_clock::now();
            cost = time_cost(start, end);
            mop1[0][j] += double(t) / 1000000.0 / cost;
            cnt1[0][j] += 1;
            printf("time: %.5f\n", double(t) / 1000000.0 / cost);

            printf("pos lookup count: %d\n", lookup_number); // double of the limit
        }
    }

    fprintf(out, "occupancy, bloom neg, bloom pos, negative fraction = %.2f, item numbers = %d, query number = %d\n", neg_frac, n, q);

    for (int j = 0; j < 19; j++)
    {
        fprintf(out, "%.2f, ", (j + 1) * 0.05);
        for (int k = 0; k < 1; k++)
            fprintf(out, "%.5f, %.5f, ", mop[k][j] / cnt[k][j], mop1[k][j] / cnt1[k][j]); // time over count?
        fprintf(out, "\n");
    }

    fclose(out);
}
/* 
void test_del(int n = 0, int q = 0, int rept = 1)
{
    FILE *out = fopen("morton_throughput_del.csv", "a");
    assert(out != NULL);

    if (n == 0)
        n = (1 << 27) * 0.95;
    if (q == 0)
        q = 1;
    int seed = 1;

    mt19937 rd(seed);
    double mop[6][20], mop1[6][20];
    int cnt[6][20], cnt1[6][20];
    memcle(mop);
    memcle(cnt);
    memcle(mop1);
    memcle(cnt1);
    
    printf("morton delete\n");
    for (int t = 0; t < rept; t++)
    {
        vector<uint64_t> insKey;
        vector<uint64_t> lupKey;
        random_gen(n, insKey, rd);
        random_gen(q, lupKey, rd);

        Morton3_8 mf(n);

        for (int i = 0; i < 0.95 * n; i++) mf.insert(insKey[i]);

        int i = 0;
        int j = 18;
        for (double r = 0.05; r <= 0.96; r += 0.05, --j)
        {
            int del_cnt = 0;
            int lim = int(n * r);
            //deln(lim);
            auto start = chrono::steady_clock::now();
            for (; i < lim; i++) 
            {
                if (mf.delete_item(insKey[i]) == true)
                    ++del_cnt;
            }

            auto end = chrono::steady_clock::now();
            double cost = time_cost(start, end);

            mop[0][j] += double(del_cnt) / 1000000.0 / cost;
            cnt[0][j] += 1;

            printf("%.2f, %.5f\n", r, mop[0][j]);
        }
    }

    fprintf(out, "occupancy, mf del, item numbers = %d\n", n);
    for (int j = 0; j < 19; j++) 
    {
        fprintf(out, "%.2f, ", (j + 1) * 0.05);
        for (int k = 0; k < 1; k++)
            fprintf(out, "%.5f, ", mop[k][j] / cnt[k][j]);
        fprintf(out, "\n");
    }
    fclose(out);
}
*/

void test_fp(int n = 0, int q = 0, int rept = 1)
{
    FILE *out = fopen("bloom_false_positive.csv", "a");
    assert(out != NULL);


    if (n == 0)
        n = (1 << 23); // 8388608
    if (q == 0)
        q = 10000000;  // 10000000
    printf("n = %d q = %d\n", n, q);
    int seed = 1;

    mt19937 rd(seed);

    for (int len = 10; len <= 22; len++)
    {
        double fp = 0;
        double mem = 0;
        for (int i = 0; i < rept; i++)
        {
            vector<uint64_t> insKey;
            vector<uint64_t> lupKey;
            random_gen(n * 0.95, insKey, rd);
            random_gen(q, lupKey, rd);

            BloomFilter<uint16_t, 15> bf;
            bf.init(n, 1, len);

            for (auto key : insKey) 
                if (bf.insert(key) != 0)
                    cout << "ins false" << endl;

            printf("insert finish\n");
            for (auto key : insKey)
                if (bf.lookup(key) == false) // ensures all items properly inserted, should never print I'm pretty sure ;)86
                    cout << "lookup false negative" << endl;

            printf("pos lookup finish\n");
            int cnt = 0;

            for (auto key : lupKey) 
                if (bf.lookup(key) == true) //counts hits from lookup
                    cnt++;

            mem += bf.memory_consumption;
            fp += double(cnt) / q;
            printf("hits: %d fp = %.5f\n", cnt, fp);
        }

        fp /= rept; // gets average for # of reps, for each increasing len
        mem /= rept;

        fprintf(out, "key numbers, query number, false positive rate, memory(bytes), bits per item\n");
        fprintf(out, "%d, %d, %.5f, %d, %.5f\n", n, q, fp, int(mem), mem * 8 / (0.95 * n));
    } 
}



int main(int argc, char *argv[])
{
    int rept = 5; // original = 5
    int n = 8000; // n = # items in set?
    int q = 10000;
    // ( n , q , rept )
    // test_fp(0, 0, rept);
    // test_ins(0, 0, rept);
    // test_lookup(0, 0, rept);

    test_fp(n, q, rept);
    // test_ins(n, q, rept);
    // test_lookup(n, q, rept);

    return 0;

}



