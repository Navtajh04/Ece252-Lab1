#include "../include/catpng.h"

#include <stdio.h>
#include "../include/catpng.h"

#include <stdio.h>

int main(int argc, char *argv[]){
    if(argc == 1){
        printf("Please enter a valid png file\n");
    }
    catPng(argv + 1, argc - 1);
}