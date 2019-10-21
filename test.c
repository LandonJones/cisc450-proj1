#include <stdio.h> 
#include <stdlib.h> 
typedef struct str { 
    int num_chars; 
    char* content; 
} str_t; 

#define MAX_STR 80
str_t* readlines(FILE* f){
    /* 
    Allocate space and set up blank result
    */ 
    str_t* result = (str_t* )malloc(sizeof(str_t)); 
    result->content = (char *)malloc((MAX_STR + 1)*sizeof(char)); 
    result->num_chars = 0; 
    char c = fgetc(f); 
    if (c != EOF){
        result->num_chars = result->num_chars + 1; 
	result->content[0] = c; 
    } 
    int chars_read = 1; 
    while (chars_read < MAX_STR && c != '\n' && c != EOF){
	c = fgetc(f); 
	chars_read += 1; 
	result->content[result->num_chars] = c; 
	result->num_chars = result->num_chars + 1;
    } 
          
    result->content[result->num_chars] = '\0'; 
     
    return result;  

} 
int main(int argc, char* argv[]){
	FILE* f = fopen("test2.txt", "r"); 
	
	str_t* lad = readlines(f); 
	
	while (lad->num_chars != 0){ 
		int i = 0; 
		while (lad->content[i] != '\0'){
			i += 1; 
		} 
		if (lad->content[0] == '\n'){
			printf("newline\n"); 
		} 
		
		free(lad->content); 
		free(lad); 
		lad = readlines(f); 
	} 
	
	return 0; 
} 
