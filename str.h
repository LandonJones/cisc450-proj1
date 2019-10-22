typedef struct str {
    int num_chars;
    char* content;
} str_t;

#define MAX_STR 80 

str_t* readlines(FILE*); 
void delete_str_t(str_t*); 
