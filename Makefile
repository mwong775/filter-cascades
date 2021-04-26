CFLAGS = -pg -g -Wall -std=c++14 -mpopcnt -march=native

all: bf bfc cfc cfssc vfc vfssc

hashes : hashes_test/hashes.c
	gcc hashes.c -o hashes 

# bf : validity.cpp bloom_filter.h 
# 	g++ $(CFLAGS) -Ofast -o validity validity.cpp  

bfc : bf_cascade/bfc_test.cpp bf_cascade/BloomFilter.h bf_cascade/bfcascade.h
	g++ $(CFLAGS) -Ofast -o bf_cascade/bfc_test bf_cascade/bfc_test.cpp  

cfc : cf_cascade/cfc_test.cpp cf_cascade/cfcascade.h ModifiedCuckooFilter/src/cuckoofilter.h ModifiedCuckooFilter/src/singletable.h
	g++ $(CFLAGS) -Ofast -o cfc_test cfc_test.cpp  

cfssc : cfss_cascade/cfssc_test.cpp cfss_cascade/cfsscascade.h ModifiedCuckooFilter/src/cuckoofilter.h ModifiedCuckooFilter/src/packedtable.h
	g++ $(CFLAGS) -Ofast -o cfssc_test cfssc_test.cpp  

vfc : vf_cascade/vfc_test.cpp vf_cascade/vfcascade.h ModifiedCuckooFilter/src/cuckoofilter.h ModifiedCuckooFilter/src/singletable.h
	g++ $(CFLAGS) -Ofast -o vfc_test vfc_test.cpp  

vfssc : vfss_cascade/vfssc_test.cpp vfss_cascade/vfsscascade.h ModifiedCuckooFilter/src/cuckoofilter.h ModifiedCuckooFilter/src/packedtable.h
	g++ $(CFLAGS) -Ofast -o vfssc_test vfssc_test.cpp  


clean:
	rm -f hashes
	rm -f bf
	rm -f bfc_test
	rm -f cfc_test
	rm -f cfssc_test
	rm -f vfc_test
	rm -f vfssc_test

# format:
# 	clang-format -i fc.cpp
