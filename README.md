About TACLe Benchmarks
======================

This is the starting point for the repository of the TACLe benchmark
collection.
See also: http://www.tacle.eu/index.php/activities/taclebench

Documentation
=============

The TACLe benchmark collection has been described in a paper at the
WCET 2016 workshop. The paper is included as PDF in the doc folder.
If you use the TACLe benchmarks please cite following paper:

    @INPROCEEDINGS{TACLeBench,
      author = {Heiko Falk and Sebastian Altmeyer and Peter Hellinckx and Bj{\"o}rn
    	Lisper and Wolfgang Puffitsch and Christine Rochange and Martin Schoeberl
    	and Rasmus Bo S{\o}rensen and Peter W{\"a}gemann and Simon Wegener},
      title = {{TACLeBench}: A Benchmark Collection to Support Worst-Case Execution
    	Time Research},
      booktitle = {16th International Workshop on Worst-Case Execution Time Analysis
    	(WCET 2016)},
      year = {2016},
      editor = {Martin Schoeberl},
      volume = {55},
      series = {OpenAccess Series in Informatics (OASIcs)},
      pages = {2:1--2:10},
      address = {Dagstuhl, Germany},
      publisher = {Schloss Dagstuhl--Leibniz-Zentrum f\"ur Informatik}
    }


Getting Started
===============

All benchmarks are self-contained and can be compiled just with

    cc/gcc/clang *.c

Current Status
==============

the TACLeBench group is currently in the process of cleaning up and unifying
the benchmarks. A version 2.0 will be available in the relative near future.

The WCET paper is based on the version V1.9. To switch to this version use:
```
git checkout V1.9
```

Regression Tests
================

https://www4.cs.fau.de/Research/TACLeBench

Getting Involved
================

You are welcome to contribute and help. To get involved contact:

Heiko for the mailing list (http://www.tuhh.de/es/esd/people/hfalk) and
Martin for GitHub access (http://www2.imm.dtu.dk/~masca/).
