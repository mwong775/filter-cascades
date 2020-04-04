# filter-cascades
Comparisons and evaluations for performance and memory efficiency of various filter cascade data structures.

Overview
--------
A filter cascade is a sequence of compact, probabilistic data structures (e.g. Bloom filters) used to achieve zero false positives.

Performance comparisons were made for filter cascades constructed using the following space-efficient data structures:

+ Bloom filters
+ Cuckoo filters
+ Cuckoo filters with semi-sorting implementation
+ Vacuum filters
+ Vacuum filtesr with semi-sorting implementation

