# Scheduling - A Secret Sauce For Resource Disaggregation

This repo contains the source code for a constraint-based ([Gecode][gecode-website])  framework describe in [here][technical-report-link]. 

- **DDC-RA** The framework for finding the optimal memory allocation in an extended rack-scale architecture.
- **CPU-RA** The source code for a NUCA-aware CPU allocator. 

Each folder has a `Makefile` that can be used to compile the source code. 

## Requirement

You need to install [Gecode][gecode-website] to be able to use our source code. A quick recepie would be to run the following commands:

```bash
sudo apt-get install libgecode-dev
sudo add-apt-repository ppa:rock-core/qt4
sudo apt-get update
sudo apt-get install libqtcore4 libqt4-dev
sudo ldconfig
```

Note that Gecode also requires QT libraries. For more information, please refer to the [Gecode documentation][gecode-doc]. 

## Citing our technical report

If you use our framework in any context, please cite our [technical report][technical-report-link]:

```bibtex
@inproceedings {farshin-ddio,
author = {Farshin, Alireza and Roozbeh, Amir and {Maguire Jr.}, Gerald Q. and Kosti\'{c}, Dejan},
title = {{Reexamining Direct Cache Access to Optimize I/O Intensive Applications for Multi-hundred-gigabit Networks}},
booktitle = {2020 {USENIX} Annual Technical Conference ({USENIX} {ATC} 20)},
year = {2020},
isbn = {978-1-939133-14-4},
pages = {673--689},
url = {https://www.usenix.org/conference/atc20/presentation/farshin},
publisher = {{USENIX} Association},
month = jul,
}
```

[technical-report-link]: https://people.kth.se/~farshin/documents/ddc-ra-technical-report.pdf
[gecode-website]: https://www.gecode.org/
[gecode-doc]: https://www.gecode.org/doc-latest/MPG.pdf

