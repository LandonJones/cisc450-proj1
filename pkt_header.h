typedef struct pkt_header{ 
    unsigned short count; 
    unsigned short seq_num; 
} pkt_header_t; 


pkt_header_t* new_pkt(unsigned short, unsigned short); 

void delete_pkt(pkt_header_t*); 


