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
@techreport{farshin-ddc-ra,
   author = {Farshin, Alireza and Roozbeh, Amir and Schulte, Christian and Maguire Jr., {Maguire Jr.}, Gerald Q. and Kosti\'{c}, Dejan},
   institution = {KTH, Network Systems Laboratory (NS Lab)},
   institution = {KTH, Software and Computer systems, SCS},
   institution = {Ericsson Research},
   title = {{Scheduling - A Secret Sauce For Resource Disaggregation}},
   keywords = {Disaggregated Data center, Scheduling, Resource Allocation, Constraint Programming, Gecode},
   year={2021},
   abstract = {This technical report describes the design &amp; implementation of a constraint-based framework for scheduling &amp; resource allocation in a disaggregated data center (DDC) where we build logical servers from disaggregated resources. We show that an Service LevelObjective (SLO)-aware constraint-based solver could improve a data center’s resource utilization by finding better solutions based on provided workload characteristics. },
   series = {Technical Report},
   url={http://kth.diva-portal.org/smash/record.jsf?pid=diva2%3A1599234&dswid=-9580},
}
```

[technical-report-link]: https://people.kth.se/~farshin/documents/ddc-ra-technical-report.pdf
[gecode-website]: https://www.gecode.org/
[gecode-doc]: https://www.gecode.org/doc-latest/MPG.pdf

