

//Alan Achtenberg
//ECEN 449-505
//Problem 3
#include <stdio.h>
#include <string.h>
#define WORD_SIZE 60 //arbitrary buffer size of individual words

int main(){

    printf("HOWDY were going to reverse a string from IN.txt\n");

    FILE* infile =fopen("IN.txt","r"); //open file for reading

    char* buf= malloc(sizeof(char)*WORD_SIZE);//input buffer, getline will allocate mem for us
    char** string_array=malloc(sizeof(char*)*1); //allocate now so realloc doesnt signal abort
    //assume we have at least one word or symbol
    fscanf(infile,"%s",buf);//read first word
    int words=1;
    printf("The input string is: ");
    while(1)
    {
        string_array= (char**)realloc(string_array,sizeof(char*)*(words));//allocate mem for bigger array, preserves old values
        string_array[words-1]=buf;//set string value
        printf("%s ",buf);//print words to console
        buf= malloc (sizeof(char)*WORD_SIZE);//allocate new memory for next string
        if (fscanf(infile,"%s",buf)==EOF)//read next word
            break;//if end of file break loop
        ++words;//increase size of words
    }
    printf("\n");

    FILE* outfile=fopen("OUT.txt","w"); //open file for writing
    printf("the reversed string is: ");
    int j=0;
    for(j;j<words;++j){
        fprintf(outfile,"%s ",string_array[words-1-j]); //write to output file
        printf("%s ", string_array[words-1-j]);//show output on console
        free(string_array[words-1-j]);//free memory for strings
    }
    printf("\n");
    free(string_array);//deallocate string array
    close(outfile);

    char* _buf=NULL;
    size_t _buf_size=256;
    printf("input any char to exit\n");
    getline(&_buf,&_buf_size,stdin); // keep window open in debugger

    return 0;
}

