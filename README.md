# ParallelRootFinding

### A simple algorithm which can compute the root of a polynomial.


# Usage

In the Makefile you can choose about compiling running and deleting obsolete files. Is available a sequential version of software which can be runned without any installation and a parallel version where you need to install a software that is available in Makefile as make install (now aviable only for ubuntu but you have to install manually mpich)

- make compile to compile parallel version
- make compile-seq to compile sequential version
- make run-seq to run an example of sequential
- make run [params] where have to add param that you need which [N] for the number of process (--oversubscriber is inserted so you can run the parogram for all process you want, not suggested)
- make run-pres to run a parallel version with some preset value
- make clean to clean all binaries
