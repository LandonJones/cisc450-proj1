#include <stdio.h> 
#include <stdlib.h> 
#include "pkt_header.h"
pkt_header_t* new_pkt(unsigned short count, unsigned short seq_num){
    pkt_header_t* new_pkt = (pkt_header_t *)malloc(sizeof(pkt_header_t)); 
    new_pkt->count = count; 
    new_pkt->seq_num = seq_num; 
    return new_pkt; 
} 

void delete_pkt(pkt_header_t* pkt){ 
    free(pkt); 
} 



 


