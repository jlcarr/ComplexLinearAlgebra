# Complex Linear Algebra

A c library for performing matrix and vector computations with complex numbers.

## Features
* Uses standard c complex number library.
* Easily integrated with other libraries.
* Simple design and usage.

## Installation
None required; simply dowload the library, compile it, then include it in your main.c

```console
$ gcc -c -g clinalg.c
$ gcc -c -g clinalg.c
$ gcc -g -o main main.o clinalg.o
```

## Usage
### Creating Basic Matrices and Vectors

```c
cmatrix newMatrix = identity(3); //3 is the size of the square identity matrix
qprint(newMatrix);

cvector newVector = zerovec(3); //3 is the size of the zero vector
vecprint(newVector);
```

Output will be:
```console
1	0 	0
0	1	0
0	0	1
0
0
0
```

### Creating Custom Matrices and Vectors

```c
double complex newArrayMatrix[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
cmatrix newMatrix = arytomat(3,3,newArrayMatrix); //3 and 3 are the dimensions of the matrix
matprint(newMatrix);

double complex newArrayVector[3] = {1,2,3};
cvector newVector = arytovec(3,newArrayVector); //3 is the dimension vector
vecprint(newVector);
```

Output will be:
```console
1	2 	3
4	5	6
7	8	9
1
2
3
```
