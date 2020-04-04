#ifndef VFSSCASCADE_H
#define VFSSCASCADE_H

#include <cstdio>
#include <cstdint>
#include <cstring>
// #include "cuckoo.h"
#include "hashutil.h"
#include "ModifiedCuckooFilter/src/cuckoofilter.h"
using namespace std;

class VFSSCascade {
	public :

    vector<cuckoofilter::CuckooFilter<size_t, 13, cuckoofilter::PackedTable>> vfssc;
	vector<int> hash_lvl;
	int a[20];

    ~VFSSCascade() {}

	void insert(vector<uint64_t> ins, vector<uint64_t> lup, vector<uint64_t> fp, FILE *file) {
	    int n = ins.size()/0.95; // fill up to 95%
	    // cout << "n: " << n << endl;
	    cuckoofilter::CuckooFilter<size_t, 13, cuckoofilter::PackedTable> vf(n);
		int i = 0;
	    for(auto c : ins) { 
			if(vf.Add(c) == cuckoofilter::Ok) {
				i++;
				assert(vf.Contain(c) == cuckoofilter::Ok);
			}
	    }
		cout << "finish inserting " << i << " of r, lf : " << vf.LoadFactor() << endl;
	    vfssc.push_back(vf);
		if(vfssc.size() & 1) { // odd = revoked
			cout << "vf contains REVOKED" << " level " << vfssc.size() << endl;
		} 
		else { // even = unrevoked
			cout << "vf contains UNREVOKED" << " level " << vfssc.size() << endl;
		}

	    cout << "checking fp's: " << endl;
	    for(uint64_t l : lup) { // int j = 0; j < int(lup.size()); j++
	        // __uint64_t l = lup.at(j);
	        if(vf.Contain(l) == cuckoofilter::Ok) {
				// 	cout << "fp: " << l << " index: " << j << endl;
	            fp.push_back(l);
	        }
	    }
		double fpratio = (double) fp.size() / lup.size();
		// fprintf(file, "level, insert, n, load factor, lookup, # fp's, fp, memory(bytes), bits per item\n");
		double mem = vf.SizeInBytes();
		fprintf(file, "%lu, %lu, %d, %.2f, %lu, %lu, %.9f, %d, %.5f\n", vfssc.size(), ins.size(), n, vf.LoadFactor(), lup.size(), fp.size(), fpratio, int(mem), mem * 8 / ins.size());
		cout << "# fp's " << fp.size() << " fp " << fpratio << endl;

		if(fpratio == 1) {
			cout << "***ERROR*** fp = " << fpratio << endl;
			hash_lvl.push_back(vfssc.size() + 1);
			// cout << "before: " << fp.at(0) << endl;
			mt19937 rd(123);
			for (int i = 0; i < 20; i++) {
				a[i] = rd();
			}
			// fp.at(0) = HashUtil::MurmurHash64(fp.at(0) ^ a[0]);
			for(int i = 0; i < int(fp.size()); i++) {
				fp.at(i) = HashUtil::MurmurHash64(fp.at(i) ^ a[fp.at(i) % 20]);
			}
			// cout << "after: " << fp.at(0) << endl;
			for(int k = 0; k < int(ins.size()); k++) {
				ins.at(k) = HashUtil::MurmurHash64(ins.at(k) ^ a[ins.at(k) % 20]);
			}
		}

	    if(fp.size() > 0) {
			lup.clear();
			// cout << "cleared lookup vector, size = " << lup.size() << '\n';
	        insert(fp, ins, lup, file);
	    }
	}

	bool lookup(size_t e) { // true = revoked, false = unrevoked
	    int l = 1;
		for (cuckoofilter::CuckooFilter<size_t, 13, cuckoofilter::PackedTable> vf : vfssc) {
			for(int i : hash_lvl) {
				if(l == i)
					e = HashUtil::MurmurHash64(e ^ a[e % 20]); 
			}
	        if(vf.Contain(e) == cuckoofilter::NotFound) {
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
    	if(vfssc.size() & 1) { // odd
        	// cout << "r (revoked)" << endl;
    		return true;
    	}
    	else { // even
        	// cout << "s (unrevoked)" << endl;
    		return false;
    	}
	}


    void print_cascade() {
	    cout << "\nprinting vfssc, # levels " << vfssc.size() << endl;
	    int i = 1;
		for (cuckoofilter::CuckooFilter<size_t, 13, cuckoofilter::PackedTable> vf : vfssc) {
	        cout << "level " << i << ":\n" << vf.Info() << endl;
	        i++;
	    }
	}

};

#endif
