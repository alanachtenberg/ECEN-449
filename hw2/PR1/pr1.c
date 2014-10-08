//Alan Achtenberg
//ECEN 449-505
//Problem 1
#include <stdio.h>
#include <string.h>

//mallocs a float double array
float** initialize_matrix(int rows, int columns){

         float** new_matrix=malloc(rows*sizeof(float*)); //allocate row pointers
            int i=0;
            for (i;i<rows;++i){
                new_matrix[i]=malloc(columns*sizeof(float)); //allocate floats
            }
        return new_matrix;
}
// matrix A and B is an MxN matrix where M is the number of rows and N is the number of columns
float** matrix_multiply(float** A,float** B, int a_rows, int a_columns, int b_rows, int b_columns){ //returns a double aray of floats

    if (a_columns!=b_rows){//make sure we can multiply the matricies
        printf("Err, matrix A columns not equal to matrix B rows");
        return 0;
    }

    float** new_matrix=initialize_matrix(a_rows,b_columns);
    float temp;
    //calculate new matrix
    int i=0;
    for(i;i<a_rows;++i){
        int j=0;
        for (j;j<b_columns;++j){
            int k=0;
            for (k;k<b_rows;++k)
                temp=temp+A[i][k]*B[k][j];//sums multiples of elements of row A and column B
            new_matrix[i][j]=temp; //
            temp=0;//reset sum back to 0;
        }
    }

    return new_matrix;
}

void print_matrix(float** matrix, int rows,int columns){
    int i=0;
    for (i;i<rows;++i){
        int j=0;
        for (j;j<columns;++j)
            printf("%f ", matrix[i][j]);
        printf("\n");
    }
}


int main(){

    printf("HOWDY lets multiply\n");
    FILE* inA=fopen("inputA.txt","r"); //open files for reading and writing
    FILE* inB=fopen("inputB.txt", "r");// r is read
    FILE* outC= fopen("outputC.txt", "w");//w is write

    int a_rows, a_columns, b_rows, b_columns;
    fscanf(inA,"%d %d",&a_rows,&a_columns); // get number of rows and columns of A and B
    fscanf(inB,"%d %d",&b_rows,&b_columns);

     if (a_columns!=b_rows){//make sure we can multiply the matricies
        printf("Err, matrix A columns not equal to matrix B rows");
        return 0;
    }

    int c_rows=a_rows;
    int c_columns=b_columns;

    float** A=initialize_matrix(a_rows,a_columns);//initialize A
    float** B=initialize_matrix(b_rows,b_columns);//initialize B

    float temp;
    int i=0; //populate A
    for (i;i<a_rows;++i){
        int j=0;
        for(j;j<a_columns;++j){
            fscanf(inA,"%f",&temp); //get single value from file
            A[i][j]=temp;//store it in A
        }
    }
    printf("Matrix A\n\n"); //show A to user
    print_matrix(A,a_rows,a_columns);

    i=0; //populate B
    for (i;i<b_rows;++i){
        int j=0;
        for(j;j<b_columns;++j){
            fscanf(inB,"%f",&temp); //get single value from file
            B[i][j]=temp;//store it in A
        }
    }

    printf("Matrix B\n\n"); //show B to user
    print_matrix(B,b_rows,b_columns);

    float** C=matrix_multiply(A,B,a_rows,a_columns,b_rows,b_columns); //Multiplies matrix, mallocs for C must delete later

    printf("Matrix C=A*B\n\n"); //show C to user
    print_matrix(C,c_rows,c_columns);

    fprintf(outC,"%d %d\n",c_rows,c_columns);//write C rows and columns to output file

    i=0;
    for (i;i<c_rows;++i){
        int j=0;
        for (j;j<c_columns;++j){
            fprintf(outC,"%f ",(float)C[i][j]); //write C values
        }
    }

    char* _buf=NULL;
    size_t _buf_size=256;
    printf("input any char to exit\n");
    getline(&_buf,&_buf_size,stdin); // keep window open in debugger

    return 0;

}
