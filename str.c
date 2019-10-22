#include <stdio.h> 
#include <stdlib.h> 
#include "str.h" 
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
void delete_str_t(str_t* s){ 
    free(s->content);
    free(s); 
}  
