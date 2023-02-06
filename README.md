# CPP-Python_SharedMemory
Inter-script communication between C++ and Python using Shared Memory for Unix system.

## How to use:
Build the ```CMAKELists.txt``` file in a designated build directory. Then run ```shm_test.cpp``` first, followed by ```shmpy.py```.
```bash
\\ Inside a directory where this repository is cloned to or the files are saved at
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```
To run the C++ (shared memory creator) script:
```bash
$ cd build // if not yet in build directory
$ ./shm_test
```
To run the Python script (shared memory reader), open a new terminal, access the project directory:</br>
If python is configured as python3:
```bash
$ python shmpy.py
```
Otherwise, use this command:
```bash
$ python3 shmpy.py
```

## Note:
Only Python >= 3.8 supports ```shared_memory``` module. So make sure to have the right version installed. In the ```CMAKELists.txt``` file, ```shmpy.py``` was added as an executables. </br>

If this script is omitted, running this script independently will result in failure to access the shared memory created by C++.
