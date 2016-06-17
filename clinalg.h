#include <complex.h>


//Structs
struct cmatrix{
	int rows;
	int cols;
	double complex** entries;
};
typedef struct cmatrix cmatrix;

struct cvector{
	int size;
	double complex* entries;
};
typedef struct cvector cvector;



//Matrix creation
cmatrix identity(int size);
cmatrix zeromat(int rows, int cols);
cmatrix matcpy(cmatrix input);
cmatrix arytomat(int rows, int cols, double complex array[rows][cols]);

void matprint(cmatrix input);

//Matrix operations
double complex det(cmatrix input);
cmatrix inv(cmatrix input);
cmatrix transpose(cmatrix input);
cmatrix matmul(cmatrix leftmatrix, cmatrix rightmatrix);
cmatrix matadd(cmatrix input1, cmatrix input2);
cmatrix scalarmatmul(double complex scalar, cmatrix input);

//matrix and vector operations
cmatrix vectomat(cvector input);
cvector mattovec(cmatrix input);
cvector matmulvec(cmatrix muliplier, cvector input);
cmatrix outprod(cvector leftvector, cvector rightvector);

//Vector creation
cvector zerovec(int size);
cvector veccpy(cvector input);
cvector arytovec(int size, double complex array[size]);

void vecprint(cvector input);

//vector operations
double complex dot(cvector input1, cvector input2);
double norm(cvector input);
cvector normalize(cvector input);
cvector vecadd(cvector input1, cvector input2);
cvector cross(cvector leftvector, cvector rightvector);
cvector scalarvecmul(double complex scalar, cvector input);
cvector conjvec(cvector input);
