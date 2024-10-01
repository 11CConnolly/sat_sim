# sat_sim
Satellite and Simulator Challenge

## Compilation and Execution

#### Satellite
- Developed in C, compiled using CMake with CTest for UTs
- Target: x86_64-linux-gnu
- gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
- Thread model: posix

Ensure that: Cmake, Check, Gcov, Lcov are installed

Then perform the following:

```
mkdir build
cd build
cmake ..
make
./src/main
```

See later section on issues for futher help or solving of issues

##### Assumptions
- TCs can execute simulatenously
- Duplicate TCs (TCs received whilst the associated task is running) will be discarded and won't cause new TM reception.
- Whilst it would be possible to create a new space on the heap for each task, in the time given it was chosen to make a simplifying assumption and that tasks will be stored statically in 'code'

#### Simulator
Developed in Python3

To run, please use `python3 sim.py`

## Run Satellite and Sat. Sim
Compile and Run the Satellite Server as previously instructed.

Run the Python program with python3 as previously instructed. 

Once both programs are compiled and running, the Python Menu will present, initially, the three options. Reception from TMs will be provided in the terminal as console output. 

## Run UTs
CTest was the framework for execution of UTs. To run UTs, please do similar to previous in the build directory:

```
cmake ..
make
ctest
```

## Issues
Some issues that may arise during the compilation and/or execution of the program & their solutions: 

- To install Check, please refer to https://github.com/getdnsapi/getdns/issues/498 if any issues arise
- To install pkg-config please try `sudo apt install pkg-config`
