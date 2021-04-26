#include <bits/stdc++.h>
#include <random>
#include <unistd.h>
#include "hashutil.h"
#include "cuckoo.h"
#include <time.h>
#include <ratio>
#include <chrono>
#include "./ModifiedCuckooFilter/src/cuckoofilter.h"

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
    store.resize(n);
    for (int i = 0; i < n; i++)
        store[i] = (uint64_t(rd()) << 32) + rd();
}

void test_ins(int n = 0, int q = 0, int rept = 1)
{
    FILE *out = fopen("cf_throughput_ins.csv", "a");
    assert(out != NULL);

    if (n == 0)
        n = (1 << 27);
    if (q == 0)
        q = 10000000;

    int seed = 1;

    mt19937 rd(seed);
    double mop[6][30], mop1[6][30];
    int cnt[6][30], cnt1[6][30];

    memcle(mop);
    memcle(cnt);
    memcle(mop1);
    memcle(cnt1);

    printf("cf insert\n");
    for (int t = 0; t < rept; t++)
    {
        vector<uint64_t> insKey;
        vector<uint64_t> lupKey;
        random_gen(n, insKey, rd);
        random_gen(q, lupKey, rd);


        cuckoofilter::CuckooFilter<size_t, 12> cf(n);

        int i = 0;
        int j = 0;
        int lim;

        auto tot_start = chrono::steady_clock::now();

        for (double r = 0.05; r <= 0.96; r += 0.05, ++j)
        {
            //printf("%.2f\n", r);
            lim = int(n * r);
            int ins_cnt = 0;

            auto start = chrono::steady_clock::now();
            for (; i < lim; i++) 
            {
                //printf("%d\n", i);
                cf.Add(insKey[i]);
                ++ins_cnt;
            }

            auto end = chrono::steady_clock::now();
            double cost = time_cost(start, end);
            mop[0][j] += double(ins_cnt) / 1000000.0 / cost;
            cnt[0][j] += 1;
            printf("%d, %.2f, %.5f, %.5f\n", lim, r, cost, double(ins_cnt) / 1000000.0 / cost);
        }

        auto tot_end = chrono::steady_clock::now();

        mop[0][j] += double(lim) / 1000000.0 / time_cost(tot_start, tot_end);
        cnt[0][j] += 1;
    }


    fprintf(out, "occupancy, cf, key-Size = %d\n", n);

    for (int j = 0; j < 20; j++) 
    {
        fprintf(out, "%.2f, ", (j + 1) * 0.05);
        for (int k = 0; k < 1; k++)
            fprintf(out, "%.5f, ", mop[k][j] / cnt[k][j]);
        fprintf(out, "\n");
    }
    fclose(out);
}

void test_lookup(int n = 0, int q = 0, int rept = 1)
{

    FILE *out = fopen("cf_throughput_lookup.csv", "a");
    assert(out != NULL);

    if (n == 0)
        n = (1 << 27);
    if (q == 0)
        q = 10000000;
    int seed = 1;
    double neg_frac = 0.5;

    mt19937 rd(seed);
    double mop[6][20], mop1[6][20];
    int cnt[6][20], cnt1[6][20];
    memcle(mop);
    memcle(cnt);
    memcle(mop1);
    memcle(cnt1);

    printf("cf lookup\n");
    for (int t = 0; t < rept; t++)
    {
        vector<uint64_t> insKey;
        vector<uint64_t> lupKey;
        random_gen(n, insKey, rd);
        random_gen(q, lupKey, rd);
       
        cuckoofilter::CuckooFilter<size_t, 12> cf(n);

        int i = 0;
        int ins_cnt = 0;
        int j = 0;

        for (double r = 0.05; r <= 0.96; r += 0.05, j++)
        {
            printf("%.2f\n", r);
            int lim = int(n * r);
            for (; i < lim; i++) 
                if (cf.Add(insKey[i]) == cuckoofilter::Ok)
                    ++ins_cnt;

            auto start = chrono::steady_clock::now();

            int lookup_number = 0;
            int p = min(q, lim);
            int k = 0;

            for (k = 0; k < p * neg_frac; k++)
                if (cf.Contain(lupKey[k]) != cuckoofilter::Ok)
                    lookup_number++;

            for (; k < p; k++)
                if (cf.Contain(insKey[k]) == cuckoofilter::Ok)
                    lookup_number++;

            auto end = chrono::steady_clock::now();
            double cost = time_cost(start, end);

            mop[0][j] += double(p) / 1000000.0 / cost;
            cnt[0][j] += 1;

            start = chrono::steady_clock::now();

            int t = min(q, lim);
            for (int i = 0; i < t; i++)
                if (cf.Contain(insKey[i]) == cuckoofilter::Ok)
                    lookup_number++;

            end = chrono::steady_clock::now();
            cost = time_cost(start, end);
            mop1[0][j] += double(t) / 1000000.0 / cost;
            cnt1[0][j] += 1;
            printf("%.5f\n", double(t) / 1000000.0 / cost);

            printf("%d\n", lookup_number);
        }
    }

    fprintf(out, "occupancy, cf neg, cf pos, negative fraction = %.2f, item numbers = %d, query number = %d\n", neg_frac, n, q);

    for (int j = 0; j < 19; j++)
    {
        fprintf(out, "%.2f, ", (j + 1) * 0.05);
        for (int k = 0; k < 1; k++)
            fprintf(out, "%.5f, %.5f, ", mop[k][j] / cnt[k][j], mop1[k][j] / cnt1[k][j]);
        fprintf(out, "\n");
    }

    fclose(out);
}

void test_del(int n = 0, int q = 0, int rept = 1)
{
    FILE *out = fopen("cf_throughput_del.csv", "a");
    assert(out != NULL);

    if (n == 0)
        n = (1 << 27);
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
    
    double fail_del_rate = 0;
    printf("cf delete\n");
    for (int t = 0; t < rept; t++)
    {
        vector<uint64_t> insKey;
        vector<uint64_t> lupKey;
        random_gen(n, insKey, rd);
        random_gen(q, lupKey, rd);

        int fail_del = 0;

        cuckoofilter::CuckooFilter<size_t, 12> cf(n);

        for (int i = 0; i < 0.95 * n; i++) cf.Add(insKey[i]);

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
                if (cf.Delete(insKey[i]) == cuckoofilter::Ok)
                    ++del_cnt;
                /*
                if (cf.Contain(insKey[i]) == cuckoofilter::Ok)
                    fail_del++;
                */
            }

            auto end = chrono::steady_clock::now();
            double cost = time_cost(start, end);

            mop[0][j] += double(0.05 * n) / 1000000.0 / cost;
            cnt[0][j] += 1;

            printf("%.2f, %.5f\n", r, double(0.05 * n) / 1000000.0 / cost);
        }

        fail_del_rate += fail_del / (0.95 * n) / rept;
    }

    fprintf(out, "occupancy, cf del, item numbers = %d, fail del rate = %.5f\n", n, fail_del_rate);
    for (int j = 0; j < 19; j++) 
    {
        fprintf(out, "%.2f, ", (j + 1) * 0.05);
        for (int k = 0; k < 1; k++)
            fprintf(out, "%.5f, ", mop[k][j] / cnt[k][j]);
        fprintf(out, "\n");
    }
    fclose(out);
}

void test_fp(int n = 0, int q = 0, int rept = 1)
{
    FILE *out = fopen("cf_false_positive.csv", "a");
    assert(out != NULL);

    if (n == 0)
        n = (1 << 27);
    if (q == 0)
        q = 10000000;
    int seed = 1;

    mt19937 rd(seed);

        double fp = 0;
        double mem = 0;
        for (int i = 0; i < rept; i++)
        {
            vector<uint64_t> insKey;
            vector<uint64_t> lupKey;
            random_gen(n * 0.95, insKey, rd);
            random_gen(q, lupKey, rd);

             
             /*
            CuckooFilter<uint16_t, 16> cf;
            int m = (int) (n / 4 + 1);
            m += m & 1;
            cf.init(m, 4, 500);
            */
            
            cuckoofilter::CuckooFilter<size_t, 16> cf(n);

            for (auto key : insKey) 
                //if (cf.insert(key) != 0)
                if (cf.Add(key) != cuckoofilter::Ok)
                {
                    cout << "lf : " << cf.LoadFactor() << endl;
                    //cout << "lf : " << cf.get_load_factor() << endl;
                    cout << "ins false" << endl;
                    break;
                }

            printf("insert finish\n");
            for (auto key : insKey)
                //if (cf.lookup(key) == false)
                if (cf.Contain(key) != cuckoofilter::Ok)
                {
                    continue;
                    cout << "lookup false negative" << endl;
                }

            printf("pos lookup finish\n");
            int cnt = 0;

            for (auto key : lupKey) 
                //if (cf.lookup(key) == true)
                if (cf.Contain(key) == cuckoofilter::Ok)
                    cnt++;

            //mem += cf.memory_consumption;
            mem += cf.SizeInBytes();
            fp += double(cnt) / q;
            printf("fp = %.9f\n", double(cnt) / q);
        }

        fp /= rept;
        mem /= rept;

        fprintf(out, "cf(fb) key numbers, query number, false positive rate, memory(bytes), bits per item\n");
        fprintf(out, "%d, %d, %.9f, %d, %.5f\n", n, q, fp, int(mem), mem * 8 / (0.95 * n));
}

int main()
{
    int rept = 5;
    //test_load_factor_new();
    test_fp(0, 0, rept);
    //test_ins(0, 0, rept);
    //test_lookup(0, 0, rept);
    //test_del(0, 0, rept);
    return 0;

}



