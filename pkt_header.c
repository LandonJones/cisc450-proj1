#include "pkt_header.h"
/*
new_pkt_header: Creates a new packetheader
Parameters:
        count: an unsigned short representing the count of bytes
        seq_num: an unsigned short representing the seq_num of the packet
Return: a new pkt header struct
*/
pkt_header_t* new_pkt_header(unsigned short count, unsigned short seq_num){
    pkt_header_t* new_pkt_header = (pkt_header_t *)malloc(sizeof(pkt_header_t)); 
    new_pkt_header->count = count; 
    new_pkt_header->seq_num = seq_num; 
    return new_pkt_header; 
} 
/*
delete_pkt_header: frees a packet_header
Parameters:
        pkt_header: a packet header to be freed
*/
void delete_pkt_header(pkt_header_t* pkt_header){ 
    free(pkt_header); 
} 
/*
new_pkt: creates a new packet
Parameters:
        packet_header: the packet header
        content: the content of the packet
*/
pkt_t* new_pkt(pkt_header_t* packet_header, char* content){ 
    pkt_t* packet = (pkt_t* )malloc(sizeof(pkt_t)); 
    packet->header = packet_header; 
    packet->content = content; 
    
    return packet; 
} 
/*
delete_pkt: frees a packet
Parameters:
        packet: the packet to be freed
*/
void delete_pkt(pkt_t* packet){ 
    free(packet->content); 
    free(packet->header); 
    free(packet); 
} 
/*
send_pkt: sends the packet header and data of a packet
Parameters:
        sock_id: the socket to send the packet to
        content: the data to send in the packet
        count: the number of data bytes
        seq_num: the sequence number of the packet
Return: a new pkt_header_t
*/
pkt_header_t* send_pkt(int sock_id, char* content, unsigned short count, unsigned short seq_num){
    pkt_header_t* header; /* the header of the packet */ 
    int bytes_sent; /* number of bytes sent in transmission */ 
    
    /* Create new pkt with values converted from host to network shorts*/ 
    header = new_pkt_header(htons(count), htons(seq_num));
    /* send message */
    
    bytes_sent = send(sock_id, header, sizeof(pkt_header_t), 0); /*send the packet header*/ 
    bytes_sent = send(sock_id, content, count, 0); /* send the packet contents*/ 
    
    header->count = bytes_sent; /*count used for send statistics*/ 
    header->seq_num = ntohs(header->seq_num); 
    return header; 
}  
/*
recv_pkt: receives the packet header and data of a packet
Parameters:
        sock_id: the id of the socket to receive from
Return:
        the full packet
*/
pkt_t* recv_pkt(int sock_id){ 
    int bytes_recd; /* bytes received from the socket */
    char* response; /* response received from the server*/
    pkt_header_t* resp_header; /* header for the packet to be received*/ 
    pkt_t* packet; /* the complete packet received during transmission */ 

    resp_header = (pkt_header_t *)malloc(sizeof(pkt_header_t)); /* allocate memory for the header*/ 

    bytes_recd = recv(sock_id, resp_header, sizeof(pkt_header_t), 0); /* receive the header */ 
    
    /* convert values from network to host values */ 
    resp_header->count = ntohs(resp_header->count);
    resp_header->seq_num = ntohs(resp_header->seq_num); 
    
    response = (char *)malloc((resp_header->count + 1)*sizeof(char));
   
    
    bytes_recd = recv(sock_id, response, resp_header->count, 0); /* recv the data */ 
    response[bytes_recd] = '\0';   /* null-terminate the string */ 
         
    resp_header->count = bytes_recd; /*count used for recv statistics"*/ 

    packet = new_pkt(resp_header, response); /* create resulting packet with header and content */ 

    return packet; 
} 






 


