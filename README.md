Implementation of TSDs that support degenerate input line segments.

Checkout the branch ``experiments`` for code of ``An Experimental Evaluation of Window Query Data Structures``.
- Install CGAL and Boost headers, as well as their lib dependencies.
- Put patched version of Jansens' Persistent Treap in the subfolder `persist_range`, or disable use in `experiments.h`.
- Patch the headers of the Boost R-Tree implementation for comp_count statistics, or disable use in `experiments.h`.

Compile: `mkdir build && cd build && cmake ../ && make`

Run:
- Populate subfolders in `build` with input and query segments:
  `./data/osm`,`./data/query_segments`,`./data/results` 
- Call `./run_experiments 512 random_horizontal 2 seg_ds`, for example, to run two queries.
