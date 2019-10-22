#include <stdio.h> 
#include <stdlib.h> 
#include "str.h" 


int main(int argc, char* argv[]){
    FILE* f = fopen("test2.txt", "r"); 
    str_t* str = readlines(f); 
    while (str->num_chars != 0){ 
        printf("%s\n", str->content); 
        str = readlines(f); 
    } 
    
} 
