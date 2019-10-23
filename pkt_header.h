#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <netinet/in.h>

typedef struct pkt_header{ 
    unsigned short count; 
    unsigned short seq_num; 
} pkt_header_t; 

typedef struct pkt{ 
    pkt_header_t* header; 
    char* content; 
} pkt_t; 

/* 
new_pkt_header: Creates a new packetheader 
Parameters: 
	count: an unsigned short representing the count of bytes 
	seq_num: an unsigned short representing the seq_num of the packet
Return: a new pkt header struct
*/ 
pkt_header_t* new_pkt_header(unsigned short, unsigned short); 

/*
delete_pkt_header: frees a packet_header
Parameters:
	pkt_header: a packet to be freed
*/ 
void delete_pkt_header(pkt_header_t*); 

/* 
new_pkt: creates a new packet
Parameters: 
	packet_header: the packet header
	content: the content of the packet
*/ 
pkt_t* new_pkt(pkt_header_t*, char*);

/*
delete_pkt: frees a packet
Parameters: 
	packet: the packet to be freed
*/ 
void delete_pkt(pkt_t*); 

/*
send_pkt: sends the packet header and data of a packet
Parameters: 
	sock_id: the socket to send the packet to 
	content: the data to send in the packet
	count: the number of data bytes 
	seq_num: the sequence number of the packet
*/ 
pkt_header_t* send_pkt(int, char*, unsigned short, unsigned short); 

/* 
recv_pkt: receives the packet header and data of a packet 
Parameters: 
	sock_id: the id of the socket to receive from 
Return: 
	the full packet
*/ 
pkt_t* recv_pkt(int); 
