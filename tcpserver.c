/* tcpserver.c */
/* Programmed by Adarsh Sethi */
/* Sept. 19, 2019 */    

#include <ctype.h>          /* for toupper */
#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset */
#include <sys/socket.h>     /* for socket, bind, listen, accept */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */
#include "pkt_header.h" 
#define STRING_SIZE 1024   
#define MAX_LINE 80 
/* SERV_TCP_PORT is the port number on which the server listens for
   incoming requests from clients. You should change this to a different
   number to prevent conflicts with others in the class. */

#define SERV_TCP_PORT 65000

int main(void) {

   int sock_server;  /* Socket on which server listens to clients */
   int sock_connection;  /* Socket on which server exchanges data with client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   unsigned int server_addr_len;  /* Length of server address structure */
   unsigned short server_port;  /* Port number used by server (local port) */

   struct sockaddr_in client_addr;  /* Internet address structure that
                                        stores client address */
   unsigned int client_addr_len;  /* Length of client address structure */

   char sentence[STRING_SIZE];  /* receive message */
   char modifiedSentence[STRING_SIZE]; /* send message */
   unsigned int msg_len;  /* length of message */
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   unsigned int i;  /* temporary loop variable */
   
   int seq_num; /* sequence number of the packet*/ 
   int total_bytes_trans; /* total transmitted bytes*/ 
   int total_packets; /* total packets transmitted*/ 
   /* open a socket */

   if ((sock_server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Server: can't open stream socket");
      exit(1);                                                
   }

   /* initialize server address information */
    
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl (INADDR_ANY);  /* This allows choice of
                                        any host interface, if more than one
                                        are present */ 
   server_port = SERV_TCP_PORT; /* Server will listen on this port */
   server_addr.sin_port = htons(server_port);

   /* bind the socket to the local server port */

   if (bind(sock_server, (struct sockaddr *) &server_addr,
                                    sizeof (server_addr)) < 0) {
      perror("Server: can't bind to local address");
      close(sock_server);
      exit(1);
   }                     

   /* listen for incoming requests from clients */

   if (listen(sock_server, 50) < 0) {    /* 50 is the max number of pending */
      perror("Server: error on listen"); /* requests that will be queued */
      close(sock_server);
      exit(1);
   }
   printf("I am here to listen ... on port %hu\n\n", server_port);
  
   client_addr_len = sizeof (client_addr);

   /* wait for incoming connection requests in an indefinite loop */

   for (;;) {

      sock_connection = accept(sock_server, (struct sockaddr *) &client_addr, 
                                         &client_addr_len);
                     /* The accept function blocks the server until a
                        connection request comes from a client */
      if (sock_connection < 0) {
         perror("Server: accept() error\n"); 
         close(sock_server);
         exit(1);
      }
 
      /* receive the message */
      
      pkt_t* pkt = recv_pkt(sock_connection); 
      printf("%s\n", pkt->content); 
      FILE* f = fopen(pkt->content, "r"); 
      
      /* initiate values */ 
      seq_num = 0; 
      total_bytes_trans = 0; 
      total_packets = 0; 
      if (f == NULL){ 
          perror("Server: error opening file"); 
          pkt_header_t* EOT = send_pkt(sock_connection, "", strlen(""), ++seq_num); 
          delete_pkt_header(EOT); 
          close(sock_connection);
          continue; 
      } 
      delete_pkt(pkt); 
      
      char input[MAX_LINE + 1]; /* input buffer*/ 
      
      while(fgets(input, MAX_LINE, f)){ /* while not EOF */ 
         seq_num++; /* increment the seq num*/ 
         pkt_header_t* header = send_pkt(sock_connection, input, strlen( input ), seq_num); /* send the line*/
         
         /* print statistics */  
         printf("Packet %hu transmitted with %hu data bytes\n", header->seq_num, header->count); 
         
         total_bytes_trans += header->count; /* increment the count to the host short*/ 
         total_packets++; /* increment total packets transmitted */ 
         delete_pkt_header(header); /* free the header*/ 
      } 
      pkt_header_t* EOT = send_pkt(sock_connection, "", strlen(""), 0); /* transmit the EOT*/ 
      
      printf("End of Transmission Packet with sequence number %hu sent with %hu data bytes\n", EOT->seq_num, EOT->count);       
      /* print statistics*/ 
      printf("There were %d packets transmitted and %d bytes transmitted\n", total_packets, total_bytes_trans); 

      fclose(f); 
      /* close the connection*/     
      close(sock_connection); 
      exit(0); /* exit after successful transmission */ 
    } 
}
