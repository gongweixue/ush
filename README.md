# Uni signal router for massage distribution

## Summary

* The project with two parts: a service and a client-lib
* The app calling the client-lib can send the msg to the service and subscribe any msg pre-defined
* Msg could be defined by the auto-generated script.
* Connect action is sync and other operations are all async since it should be un-blocked for the communications.
* Multi-thread is used in the service to listen, to process, to publish msg, and to schedule all the actions internal.
* All these threads cooperate by the queue(consumer-producer) which defined by the macro in the header file.
* Posix MQ is used for IPC between client and service processes.
* Multi-link could be used with all client, only if the number of links is under the limitations you define.
* msg could be sent to service itself also, to do some trick(clean up that the client could not do when disconnect happends).

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
