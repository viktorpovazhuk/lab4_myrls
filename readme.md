# Lab work: Recursive directory listing
Authors (team): <a href="https://github.com/bogdanmagometa">Bohdan Mahometa</a>,
<a href="https://github.com/viktorpovazhuk">Viktor Povazhuk</a>,
<a href="https://github.com/shevdan">Bohdan Shevchuk</a>
<br>

Currently, the project has been seen to work on Linux and MacOS.

## Prerequisites

Tools:
- cmake, GCC

Libraries:
- boost collection (`program_options` library is used in the project)

### Compilation

#### Compile with `./compile.sh`

Execute the following command in the project root directory:
```bash
$ ./compile.sh
```

The executable will be built and copied to `./bin/myrls`.

#### Compile manually with cmake

Run the following in the project root directory:
```bash
$ mkdir build
$ cmake -S . -B build
$ cmake --build build
```

The executable will be available by the path `./build/myrls`.

### Installation

Assuming `./build` is the directory with compiled `myrls` and `./install` is the install directory:
```bash
$ cp ./build/myrls ./install/myrls
```

### Usage

Assuming the `myrls` executable is available by the path `./bin/myrls`:
```bash
$ ./bin/myrls ./myrls
./myrls:
rwxrwxr-x bohdan 4096 2022-11-03 15:56:23 /include
rwxrwxr-x bohdan 4096 2022-11-03 16:48:11 /src

./myrls/include:
rw-rw-r-- bohdan  378 2022-11-03 15:56:23 entry_info.h
rw-rw-r-- bohdan 1407 2022-11-03 13:00:49 options_parser.h

./myrls/src:
rw-rw-r-- bohdan 6352 2022-11-03 16:48:11 main.cpp
rw-rw-r-- bohdan 1661 2022-11-03 15:56:23 options_parser.cpp
```

### Important!

### Results

We implemented recursive directory listing command.

# Additional tasks

Not yet.
