
//Alan Achtenberg
//ECEN 449-505
//Problem 2
#include <stdio.h>
#include <string.h>

#include <sys/types.h> // mode_t declaration
#include <fcntl.h> //for open function
#include <sys/mman.h>//for mmap function
#include <unistd.h>//for lseek
#define NUM_INTS 99 //99 ints because we start count from 1ĵ
int main(){

    printf("HOWDY time to count from 1 to 100 and save it to a file\n\n");
    printf("Creating mmappedfile.bin for writing\n");
    int file =open("mmappedfile.bin", O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);// mode 0600 means user has write(0200) and read(0400) permissions

    printf("extending file to correct size\n");//we need to extend the file for mapping, cannot map an empty file
    lseek(file, NUM_INTS*sizeof(int), SEEK_SET); //repositions r/w file offset to the size of our file that we want 100 is offset in bytes
    write(file, "", 1);
    //The lseek() function allows the file offset to be set beyond the end of the file (but this does not change the size of the file). If data is
    //later written at this point, subsequent reads of the data in the gap return null bytes until data is actually written into the gap.

    printf("mapping file\n");
    int* file_map = mmap(NULL, NUM_INTS*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, file, 0); //NULL allows kernel to decide where to map the memory
    //PROT_WRITE and PROT_READ flags means pages may be read or written
    //MAP_SHARED flag allows other processes to see the changes and changes are carried through to the underlying file
    //0 is our offset

    int i=0;
    printf("writing to file\n");
    for(i;i<100;++i)
        file_map[i]=i+1;//write to mapped file integers from 1 to 100
    printf("unmapping file\n");
    munmap(file_map,NUM_INTS*sizeof(int));//unmaps file

    printf("closing file\n");
    close(file);

    char* _buf=NULL;
    size_t _buf_size=256;
    printf("input any char to exit\n");
    getline(&_buf,&_buf_size,stdin); // keep window open in debugger

    return 0;
}
