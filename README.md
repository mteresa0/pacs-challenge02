# Second challenge - PACS
Second challenge of the course Advaced Programming for Scientific Computing, 23/24, Politecnico di Milano.

## Installation 
To clone the repository run:
```
git clone git@github.com:mteresa0/pacs-challenge02.git
```

## Configuration
First, you have to set `PACS_ROOT` as your path to the [pacs-examples]() repo. You could edit the `Makefile`; for example:
```
PACS_ROOT=/home/myname/pacs/Examples/
```

Alternatively, you may set an environmental variable with the same name using the command 
```
export PACS_ROOT=/home/myname/pacs/Examples/ 
```

## Usage
In the terminal run:
- `make all` to compile and run the program;
- `make main` to compile;
- `make run` to run the program.

## Files
- `src/main.cpp` - main file
- `inc/Matric.hpp` - `Matrix` class declaration
- `inc/Matrix.inc` - `Matrix` class definitions
- `inc/Operations.inc` - definition of friend operations methods
- `inc/TestPerformance.hpp` - `TestPerformance` class declaration and definitions
- in folder `data` there are the matrix market files



