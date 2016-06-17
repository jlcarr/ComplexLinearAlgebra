#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include "clinalg.h"

/*
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



int main(){
	
	
	
	double complex test2[3]={1,2,3};
	cvector anew = arytovec(3, test2);
	printf("\ntest1: \n");
	vecprint(anew);
	
	printf("\ntest2: \n");
	anew = normalize(anew);
	vecprint(anew);

	 
}
*/


static void cprint(double complex z){
	if (fabs(creal(z))>fabs(cimag(z))*pow(10,15) || cimag(z)==0){
		printf("%g",creal(z));
	}
	else if (fabs(creal(z))*pow(10,15)<fabs(cimag(z)) || creal(z)==0){
		if (cimag(z)==1) {
			printf("i");
		}
		else if (cimag(z)==-1) {
			printf("-i");
		}
		else{
			printf("%gi",cimag(z));
		}
	}
	else{
		if (cimag(z)==1) {
			printf("%g+i",creal(z));
		}
		else if (cimag(z)==-1) {
			printf("%g-i",creal(z));
		}
		else if (cimag(z)>0) {
			printf("%g+%gi",creal(z),cimag(z));
		}
		else{
			printf("%g-%gi",creal(z),-cimag(z));
		}
	}
}


//Matrices

//Creation of matrices

//print matrix
void matprint(cmatrix input){
	for(int i=0; i<input.rows; i++,printf("\n")) for(int j=0; j<input.cols; j++) {
		cprint(input.entries[i][j]);
		printf("\t");
	}
}

//Array to matrix
cmatrix arytomat(int rows, int cols, double complex array[rows][cols]){
	if(rows<=0 || cols<=0){
		fprintf(stderr, "Error in function 'arytomat': array rows or cols invalid\nPossible reasons:\nValue passed to either 'rows' or 'cols' was <=0\n");
		exit(1);
	}
	cmatrix returnVal;
	returnVal.rows = rows;
	returnVal.cols = cols;
	returnVal.entries = malloc(rows*sizeof(double complex *));
	//check for error
	if(returnVal.entries == NULL){
		fprintf(stderr, "Error in function 'arytomat': pointer to allocated memory was NULL\nPossible reasons:\nOut of memory\n");
		exit(1);
	}
	for (int i=0; i<rows; i++) returnVal.entries[i] = (double complex *) array[i];
	return returnVal;
}

//generate identity matrix
cmatrix identity(int size){
	if(size<=0){
		fprintf(stderr, "Error in function 'identity': input invalid\nPossible reasons:\nValue passed to 'size' was <=0\n");
		exit(1);
	}
	//create new matrix
	cmatrix returnVal;
	//create rows
	returnVal.entries = malloc(size*sizeof(double complex *));
	//check for error
	if(returnVal.entries == NULL){
		fprintf(stderr, "Error in function 'identity': pointer to allocated memory was NULL\nPossible reasons:\nOut of memory\n");
		exit(1);
	}
	//create columns full of zeros
	for(int i=0;i<size;i++) returnVal.entries[i] = calloc(size,sizeof(double complex));
	//fill diagonal with 1s to create identity matrix
	for(int i=0;i<size;i++) returnVal.entries[i][i] = 1;
	// set rows and cols
	returnVal.rows = size;
	returnVal.cols = size;
	//return the matrix
	return returnVal;
}

//generate zeros matrix
cmatrix zeromat(int rows, int cols){
	if(rows<=0 || cols<=0){
		fprintf(stderr, "Error in function 'zeromat': array rows or cols invalid\nPossible reasons:\nValue passed to either 'rows' or 'cols' was <=0\n");
		exit(1);
	}
	//create new matrix
	cmatrix returnVal;
	//create rows
	returnVal.entries = malloc(rows*sizeof(double complex*));
	//create columns full of zeros
	for(int i=0;i<rows;i++) returnVal.entries[i] = calloc(cols,sizeof(double complex));
	// set rows and cols
	returnVal.rows = rows;
	returnVal.cols = cols;
	//return the matrix
	return returnVal;
}

//deep copy a matrix
cmatrix matcpy(cmatrix input){
	//create new matrix
	cmatrix returnVal;
	//create rows
	returnVal.entries = malloc(input.rows*sizeof(double complex *));
	//create columns full of zeros
	for(int i=0;i<input.rows;i++) returnVal.entries[i] = malloc(input.cols*sizeof(double complex));
	//fill matrix with input
	for(int i=0;i<input.rows;i++) for(int j=0;j<input.cols;j++) returnVal.entries[i][j] = input.entries[i][j];
	// set rows and cols
	returnVal.rows = input.rows;
	returnVal.cols = input.cols;
	//return the matrix
	return returnVal;
}


//Operations on matrices

//Determinant
double complex det(cmatrix input){
	if(input.rows!=input.cols){
		fprintf(stderr, "Error in function 'det': input rows and cols do not match\nPossible reasons:\n'input' was not a square matrix\n");
		exit(1);
	}
    int i, j, index, zero, indexzero;
    double complex tempvector[input.rows], multi, detnum=1, detdenom=1;
	
	//Copy input so as not to overwrite entries
	cmatrix temp = matcpy(input);
	
    for(index=0, indexzero=0;index<temp.rows;index++){
		//bring rows with a zero in column 'index' down to the bottom (ignoring rows above 'index' as they have already been used.
        for(i=index;i<temp.rows;i++)if(temp.entries[i][index]==0){
            for(j=i, zero=i;j<temp.rows;j++) if(temp.entries[j][index]!=0) zero=j;
            for(j=indexzero;j<index;j++) if(temp.entries[j][index]!=0) zero=j;
            if(i!=zero) detnum*=-1;
            for(j=0;j<temp.rows;j++) tempvector[j]=temp.entries[i][j];
            for(j=0;j<temp.rows;j++) temp.entries[i][j]=temp.entries[zero][j];
            for(j=0;j<temp.rows;j++) temp.entries[zero][j]=tempvector[j];
        }
		//if there is a leading 1, then use it to remove all leading 1s below
        if(temp.entries[index][index]!=0) for (indexzero=index+1, i=0; i<temp.rows; i++) if(i!=index && temp.entries[i][index]!=0){
			multi=temp.entries[index][index];
            detdenom*=temp.entries[i][index]*multi;
			for (j=0; j<temp.rows; j++) temp.entries[index][j]*=temp.entries[i][index];
			for (j=0; j<temp.rows; j++) temp.entries[i][j]*=multi;
			for (j=0; j<temp.rows; j++) temp.entries[i][j]-=temp.entries[index][j];
		}
    }
	//ensure that there are all no
    for(index=0;index<temp.rows;index++) if(temp.entries[index][index]!=0){
        for(j=0, multi=temp.entries[index][index];j<temp.rows;j++) temp.entries[index][j]/=multi; //final division
        detnum*=multi;
    }
	//get rid of -0
    for(i=0;i<temp.rows;i++)for(j=0;j<temp.rows;j++) if(temp.entries[i][j]==0) temp.entries[i][j]=cabs(temp.entries[i][j]);
	//if there is an entry without a leading 1, then the matrix is degenerate an the determinant is 0.
    for(i=0;i<temp.rows;i++) if(temp.entries[i][i]==0) detnum=0;
    return detnum/detdenom;
}

//Matrix Inverse
cmatrix inv(cmatrix input){
	if(input.rows!=input.cols){
		fprintf(stderr, "Error in function 'inv': input rows and cols do not match\nPossible reasons:\n'input' was not a square matrix\n");
		exit(1);
	}
    int i, j, index, zero, indexzero;
    double complex tempvector[input.rows], multi, detnum=1, detdenom=1;
	
	//Copy input so as not to overwrite entries, initialize 'temp' to identity matrix
	cmatrix output = identity(input.rows);
	cmatrix temp = matcpy(input);
	
    for(index=0, indexzero=0;index<temp.rows;index++){
        for(i=index;i<temp.rows;i++)if(temp.entries[i][index]==0){
            for(j=i, zero=i;j<temp.rows;j++) if(temp.entries[j][index]!=0) zero=j;
            for(j=indexzero;j<index;j++) if(temp.entries[j][index]!=0) zero=j;
            if(i!=zero) detnum*=-1;
            for(j=0;j<temp.rows;j++) tempvector[j]=temp.entries[i][j];
            for(j=0;j<temp.rows;j++) temp.entries[i][j]=temp.entries[zero][j];
            for(j=0;j<temp.rows;j++) temp.entries[zero][j]=tempvector[j];
            
            for(j=0;j<temp.rows;j++) tempvector[j]=output.entries[i][j];
            for(j=0;j<temp.rows;j++) output.entries[i][j]=output.entries[zero][j];
            for(j=0;j<temp.rows;j++) output.entries[zero][j]=tempvector[j];
            
        }
        if(temp.entries[index][index]!=0) for (indexzero=index+1, i=0; i<temp.rows; i++) if(i!=index && temp.entries[i][index]!=0){
			multi=temp.entries[index][index];
            detdenom*=temp.entries[i][index]*multi;
			for (j=0; j<temp.rows; j++){
                output.entries[index][j]*=temp.entries[i][index];
                temp.entries[index][j]*=temp.entries[i][index];
            }
			for (j=0; j<temp.rows; j++){
                output.entries[i][j]*=multi;
                temp.entries[i][j]*=multi;
            }
			for (j=0; j<temp.rows; j++){
                output.entries[i][j]-=output.entries[index][j];
                temp.entries[i][j]-=temp.entries[index][j];
            }
		}
    }
    for(index=0;index<temp.rows;index++) if(temp.entries[index][index]!=0){
        for(j=0, multi=temp.entries[index][index];j<temp.rows;j++){
            temp.entries[index][j]/=multi;
            output.entries[index][j]/=multi;
        } //final division
        detnum*=multi;
    }
    for(i=0;i<temp.rows;i++)for(j=0;j<temp.rows;j++) if(temp.entries[i][j]==0) temp.entries[i][j]=cabs(temp.entries[i][j]);
    for(i=0;i<temp.rows;i++) if(temp.entries[i][i]==0) detnum=0;
    return output;
}

//Matrix transpose
cmatrix transpose(cmatrix input){
	cmatrix returnVal = zeromat(input.cols, input.rows);
	for(int i=0;i<input.rows;i++) for(int j=0;j<input.cols;j++) returnVal.entries[j][i]=input.entries[i][j];
	return returnVal;
}

//Matrix multiplication
cmatrix matmul(cmatrix leftmatrix, cmatrix rightmatrix){
	if(leftmatrix.cols!=rightmatrix.rows){
		fprintf(stderr, "Error in function 'matmul': leftmatrix cols and rightmatrix rows do not match\nPossible reasons:\n'leftmatrix' and 'rightmatrix' cannot be multiplied\n");
		exit(1);
	}
	int i,j,k;
	//create output
	cmatrix returnVal = zeromat(leftmatrix.rows, rightmatrix.cols);
	for(i=0;i<leftmatrix.rows;i++) for(j=0;j<rightmatrix.cols;j++) {
		for(k=0;k<leftmatrix.cols;k++) returnVal.entries[i][j]+=rightmatrix.entries[i][k]*leftmatrix.entries[k][j];
	}
	return returnVal;
}

//Matrix addition
cmatrix matadd(cmatrix input1, cmatrix input2){
	if(input1.rows!=input2.rows || input1.cols!=input2.cols){
		fprintf(stderr, "Error in function 'matadd': input1 cols and cols do not match input2 rows and cols\nPossible reasons:\n'input1' and 'input2' cannot be added because they are not the same size\n");
		exit(1);
	}
	int i,j;
	//create output
	cmatrix returnVal = zeromat(input1.rows, input1.cols);
	for(i=0;i<input1.rows;i++) for(j=0;j<input1.cols;j++) returnVal.entries[i][j] = input1.entries[i][j] + input2.entries[i][j];
	return returnVal;
}

//Matrix multiplication by a scalar
cmatrix scalarmatmul(double complex scalar, cmatrix input){
	int i,j;
	//create output
	cmatrix returnVal = zeromat(input.rows, input.cols);
	for(i=0;i<input.rows;i++) for(j=0;j<input.cols;j++) returnVal.entries[i][j] = scalar * input.entries[i][j];
	return returnVal;
}




//Vectors

//creation of vectors

//create zero vector
cvector zerovec(int size){
	//create new vector
	cvector returnVal;
	//create entries
	returnVal.entries = calloc(size,sizeof(double complex));
	// set rows and cols
	returnVal.size = size;
	//return the vector
	return returnVal;
}

//deep copy vector
cvector veccpy(cvector input){
	//create new vector
	cvector returnVal;
	//create entries
	returnVal.entries = malloc(input.size*sizeof(double complex*));
	//fill vector with input
	for(int i=0;i<input.size;i++) returnVal.entries[i] = input.entries[i];
	// set rows and cols
	returnVal.size = input.size;
	//return the matrix
	return returnVal;
}

//convert array to vector
cvector arytovec(int size, double complex array[size]){
	cvector returnVal;
	returnVal.size = size;
	returnVal.entries = malloc(size*sizeof(double complex));
	for (int i=0; i<size; i++) returnVal.entries[i] = array[i];
	return returnVal;
}

//print a vector
void vecprint(cvector input){
	for (int i=0; i<input.size; i++){
		cprint(input.entries[i]);
		printf("\n");
	}
}


//operations on vectors

//dot product
double complex dot(cvector leftvector, cvector rightvector){
	double complex returnVal=0;
	for (int i=0; i<leftvector.size; i++) returnVal += conj(leftvector.entries[i])*rightvector.entries[i];
	return returnVal;
}

//norm (magnitude) of vector
double norm(cvector input){
	double returnVal=0;
	for (int i=0; i<input.size; i++) returnVal += cabs(input.entries[i])*cabs(input.entries[i]);
	return sqrt(returnVal);
}

//normalize vector
cvector normalize(cvector input){
	//create output
	return scalarvecmul(1.0/norm(input), input);
}

//add vectors
cvector vecadd(cvector input1, cvector input2){
	int i;
	//create output
	cvector returnVal = zerovec(input1.size);
	//fill entries
	for(i=0;i<input1.size;i++) returnVal.entries[i] = input1.entries[i] + input2.entries[i];
	return returnVal;
}

//cross product
cvector cross(cvector leftvector, cvector rightvector){
	if(leftvector.size!=3 || rightvector.size!=3){
		fprintf(stderr, "Error in function 'cross': Either leftvector size or cols rightvector size is not 3\nPossible reasons:\ncross product of 'leftvector' and 'rightvector' cannot be taken because they must both be of size 3\n");
		exit(1);
	}
	int i;
	//create output
	cvector returnVal = zerovec(3);
	returnVal.entries[0] = leftvector.entries[1]*rightvector.entries[2] - leftvector.entries[2]*rightvector.entries[1];
	returnVal.entries[1] = leftvector.entries[2]*rightvector.entries[0] - leftvector.entries[0]*rightvector.entries[2];
	returnVal.entries[2] = leftvector.entries[0]*rightvector.entries[1] - leftvector.entries[1]*rightvector.entries[0];
	return returnVal;
}

//multiply vector by a scalar
cvector scalarvecmul(double complex scalar, cvector input){
	int i;
	//create output
	cvector returnVal = zerovec(input.size);
	for(i=0;i<input.size;i++) returnVal.entries[i] = scalar * input.entries[i];
	return returnVal;
}

//vector complex conjugate
cvector conjvec(cvector input){
	int i;
	//create output
	cvector returnVal = zerovec(input.size);
	for(i=0;i<input.size;i++) returnVal.entries[i] = conj(input.entries[i]);
	return returnVal;
}


//vector and matrix operations

//convert a vector struct to a matrix struct
cmatrix vectomat(cvector input){
	cmatrix returnVal;
	returnVal.rows = input.size;
	returnVal.cols = 1;
	returnVal.entries = malloc(sizeof(double complex*));
	for (int i=0; i<input.size; i++){
		returnVal.entries[i] = malloc(sizeof(double complex));
		returnVal.entries[i][0] = input.entries[i];
	}
	return returnVal;
}

//convert a 1d matrix to a vector
cvector mattovec(cmatrix input){
	//create new vector
	cvector returnVal;
	returnVal.size = input.rows;
	returnVal.entries = malloc(returnVal.size*sizeof(double complex));
	//fill the entries
	for (int i=0; i<returnVal.size; i++) returnVal.entries[i] = input.entries[i][0];
	return returnVal;
}

//multiply a vector by a matrix
cvector matmulvec(cmatrix muliplier, cvector input){
	cvector returnVal = zerovec(input.size);
	for(int i=0;i<input.size;i++) for(int j=0;j<input.size;j++) returnVal.entries[i] += muliplier.entries[i][j]*input.entries[j];
	return returnVal;
}

//outter product
cmatrix outprod(cvector leftvector, cvector rightvector){
	int i,j;
	//create new matrix
	cmatrix returnVal = zeromat(leftvector.size,rightvector.size);
	//fill entries
	for(i=0;i<leftvector.size;i++) for(j=0;j<rightvector.size;j++) returnVal.entries[i][j] = leftvector.entries[i] * conj(rightvector.entries[j]);
	return returnVal;
}


