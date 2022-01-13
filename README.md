# Uni signal router for massage distribution

## Summary

summary

## Environment

* gnu g++ version
* os requirement
* external dependency.
* build steps.

## Note

* Pass the macro USH_QNX/USH_DEBUG/USH_TEST to make, to select the build type you want.
* When using USH_QNX macro, the qnx license must be deployed on your host machine.

## Build

```
$ make clean all # for x86-gnu-gcc
$ source $PATH_OF_YOUR_QNX_ENV_SETUP; make all USH_QNX=1 # for aarch64-gnu-gcc/qcc
$ make all USH_DEBUG=1 # for x86 debug version
$ make all USH_TEST=1 # run all test cases
```
