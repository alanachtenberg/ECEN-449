//Alan Achtenberg
//ECEN 449-505
//Problem 4
#include <stdio.h>
#include <string.h>
#define INFILE "ARRAY3.txt"
#define OUTFILE "SUBARRAY3.txt"

//implementation of kadanes algorithm
int* find_max_subarray(int* array, int size, int* new_array_size){
    int max_so_far=0, max_ending_here=0;
    int start=0, end=0;
    int i=0;
    for(i;i<size;++i){
        max_ending_here = max_ending_here + array[i];
     if(max_ending_here < 0){
         max_ending_here = 0;
         start=i+1;
         }
     else if (max_so_far < max_ending_here){
            max_so_far = max_ending_here;
            end=i;
     }
   }

   if (max_so_far==0){//must be the case where all are neg or all 0
        int val=array[0];//must give comparison value an initial array value
        start=0; //give intial value to end and start
        end=0;
        for(i=0;i<size;++i){
            if(array[i]>val){
                val=array[i]; //solve for case where all are negative, the answer is just the maximum single value in the array
                start=i;
                end=i;
            }
        }
    }
   //now that we know are begining and starting index lets create our new sub array
   int* sub_array= malloc(sizeof(int)*(end+1-start));//allocates memory and points our new pointer to
   int j=0;
   for(i=start;i<end+1;++i){
        sub_array[j]=array[i];//just indexing intermediate values of array into the first values of sub_array
   }
   *new_array_size=end+1-start;//calculate new sub array size and store it in memory thats given by pointer
   return sub_array;
}

int main(){

    printf("HOWDY were going to find the max sum substring\n");

    FILE* infile=fopen(INFILE,"r"); //open file for reading

    int* int_array = malloc(sizeof(int));//allocate for at least one int

    int array_size=0;
    int temp_int=0;
    printf("The input array is: ");
    while(1)
    {
        ++array_size;//increase size of word
        int_array= (int*)realloc(int_array,sizeof(int)*(array_size));//allocate mem for bigger array, preserves old values
        if (fscanf(infile,"%d",&temp_int)==EOF){//read next word
            --array_size;
            break;//if end of file break loop
            }
        int_array[array_size-1]=temp_int;
        printf("%d ",int_array[array_size-1]);//print ints to console as we build the array

    }
    printf("\n");

    int sub_array_size=0;
    int* sub_array= find_max_subarray(int_array,array_size,&sub_array_size);//this function returns the sub array and modifys the 3rd arguemt to contain the size of the array
    FILE* outfile=fopen(OUTFILE,"w"); //open file for writing

    printf("the max sum subarray is: ");
    int j=0;
    for(j;j<sub_array_size;++j){
        fprintf(outfile,"%d ",sub_array[j]); //write to output file
        printf("%d ", sub_array[j]);//show output on console
    }
    printf("\n");
    free(sub_array);//deallocate find_max_subarray allocates mem for the array
    free(int_array);
    close(outfile);

    char* _buf=NULL;
    size_t _buf_size=256;
    printf("input any char to exit\n");
    getline(&_buf,&_buf_size,stdin); // keep window open in debugger

    return 0;
}


