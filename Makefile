CFLAGS = -pg -g -Wall -std=c++14 -mpopcnt -march=native

hashes : hashes.c
	gcc hashes.c -o hashes 

bf : validity.cpp bloom_filter.h 
	g++ $(CFLAGS) -Ofast -o validity validity.cpp  

bfc_test : bfc_test.cpp BloomFilter.h bfcascade.h
	g++ $(CFLAGS) -Ofast -o bfc_test bfc_test.cpp  

cfc_test : cfc_test.cpp cfcascade.h ModifiedCuckooFilter/src/cuckoofilter.h ModifiedCuckooFilter/src/singletable.h
	g++ $(CFLAGS) -Ofast -o cfc_test cfc_test.cpp  

cfssc_test : cfssc_test.cpp cfsscascade.h ModifiedCuckooFilter/src/cuckoofilter.h ModifiedCuckooFilter/src/packedtable.h
	g++ $(CFLAGS) -Ofast -o cfssc_test cfssc_test.cpp  

vfc_test : vfc_test.cpp vfcascade.h ModifiedCuckooFilter/src/cuckoofilter.h ModifiedCuckooFilter/src/singletable.h
	g++ $(CFLAGS) -Ofast -o vfc_test vfc_test.cpp  

vfssc_test : vfssc_test.cpp vfsscascade.h ModifiedCuckooFilter/src/cuckoofilter.h ModifiedCuckooFilter/src/packedtable.h
	g++ $(CFLAGS) -Ofast -o vfssc_test vfssc_test.cpp  


format:
	clang-format -i fc.cpp
clean:
	rm -f fc
	rm -f hashes