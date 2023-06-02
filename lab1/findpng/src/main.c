#include "../include/f_util.h"

#include <stdio.h>

int main(int argc, char *argv[]) 
{
    if(argc == 1){
        perror("Please enter a valid directory!");
        return -1;
    }

    bool pngFound = false; 

    directoryPngCheck(argv[1], &pngFound);

    if(!pngFound){
        printf("findpng: No PNG file found\n");
    }
    return 0;
}