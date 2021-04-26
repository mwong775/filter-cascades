#ifndef CVSSCASCADE_H
#define CVSSCASCADE_H

#include <cstdio>
#include <cstdint>
#include <cstring>
// #include "cuckoo.h"
#include "hashutil.h"
#include "ModifiedCuckooFilter/src/cuckoofilter.h"
using namespace std;

class CFSSCascade {
	public :

    vector<cuckoofilter::CuckooFilter<size_t, 13, cuckoofilter::PackedTable>> cfssc;
	vector<int> hash_lvl;
	int a[30];

    ~CFSSCascade() {}

	void insert(vector<uint64_t> ins, vector<uint64_t> lup, vector<uint64_t> fp, FILE *file) {
	    int n = ins.size()/0.95; // fill up to 95%
	    // cout << "n: " << n << endl;
	    cuckoofilter::CuckooFilter<size_t, 13, cuckoofilter::PackedTable> cf(n);
		int i = 0;
	    for(auto c : ins) { 
			if(cf.Add(c) == cuckoofilter::Ok) {
				i++;
				assert(cf.Contain(c) == cuckoofilter::Ok);
			}
	    }
		cout << "finish inserting " << i << " of r, lf : " << cf.LoadFactor() << endl;
	    cfssc.push_back(cf);
		if(cfssc.size() & 1) { // odd = revoked
			cout << "cf contains REVOKED" << " level " << cfssc.size() << endl;
		} 
		else { // even = unrevoked
			cout << "cf contains UNREVOKED" << " level " << cfssc.size() << endl;
		}

	    cout << "checking fp's: " << endl;
	    for(uint64_t l : lup) { // int j = 0; j < int(lup.size()); j++
	        // __uint64_t l = lup.at(j);
	        if(cf.Contain(l) == cuckoofilter::Ok) {
				// 	cout << "fp: " << l << " index: " << j << endl;
	            fp.push_back(l);
	        }
	    }
		double fpratio = (double) fp.size() / lup.size();
		// fprintf(file, "level, insert, n, load factor, lookup, # fp's, fp, memory(bytes), bits per item\n");
		double mem = cf.SizeInBytes();
		fprintf(file, "%lu, %lu, %d, %.2f, %lu, %lu, %.9f, %d, %.5f\n", cfssc.size(), ins.size(), n, cf.LoadFactor(), lup.size(), fp.size(), fpratio, int(mem), mem * 8 / ins.size());
		cout << "# fp's " << fp.size() << " fp " << fpratio << endl;

		if(fpratio == 1) {
			cout << "***ERROR*** fp = " << fpratio << endl;
			hash_lvl.push_back(cfssc.size() + 1);
			// cout << "before: " << fp.at(0) << endl;
			mt19937 rd(123);
			for (int i = 0; i < 30; i++) {
				a[i] = rd();
			}
			// fp.at(0) = HashUtil::MurmurHash64(fp.at(0) ^ a[0]);
			for(int i = 0; i < int(fp.size()); i++) {
				fp.at(i) = HashUtil::MurmurHash64(fp.at(i) ^ a[fp.at(i) % 30]); // BITWISE EXCLUSIVE OR
			}
			// cout << "after: " << fp.at(0) << endl;
			for(int k = 0; k < int(ins.size()); k++) {
				ins.at(k) = HashUtil::MurmurHash64(ins.at(k) ^ a[ins.at(k) % 30]); // XOR
			}
		}

	    if(fp.size() > 0) {
			lup.clear();
			// cout << "cleared lookup vector, size = " << lup.size() << '\n';
	        insert(fp, ins, lup, file);
	    }
		// cout << "final # levels: " << cfssc.size() << endl;
		// cout << "insert " << ins.size() << " lookup " << lup.size() << " fp " << fp.size() << endl;
	}

	bool lookup(size_t e) { // true = revoked, false = unrevoked
	    int l = 1;
		for (cuckoofilter::CuckooFilter<size_t, 13, cuckoofilter::PackedTable> cf : cfssc) {
			for(int i : hash_lvl) {
				if(l == i)
					e = HashUtil::MurmurHash64(e ^ a[e % 30]); 
			}
	        if(cf.Contain(e) == cuckoofilter::NotFound) {
	        	// cout << "not found" << endl;
	        	if(l & 1) { // odd # of levels
		        	// cout << "s (unrevoked)" << endl;
	        		return false;
	        	}
	        	else { // even
		        	// cout << "r (revoked)" << endl;
	        		return true;
	        	}
	        }
	        l++;
	    }
	    // cout << "in all bf's" << endl;
    	if(cfssc.size() & 1) { // odd
        	// cout << "r (revoked)" << endl;
    		return true;
    	}
    	else { // even
        	// cout << "s (unrevoked)" << endl;
    		return false;
    	}
	}


    void print_cascade() {
	    cout << "\nprinting cfssc, # levels " << cfssc.size() << endl;
	    int i = 1;
		for (cuckoofilter::CuckooFilter<size_t, 13, cuckoofilter::PackedTable> cf : cfssc) {
	        cout << "level " << i << ":\n" << cf.Info() << endl;
	        i++;
	    }
	}

};

#endif
