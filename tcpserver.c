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
#include "str.h" 

#define STRING_SIZE 1024   

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
   int seq_num = 0; 
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
      pkt_header_t* pkt = (pkt_header_t*)malloc(sizeof(pkt_header_t)); 
      bytes_recd = recv(sock_connection, pkt, sizeof(pkt_header_t), 0);
      
      char filename[STRING_SIZE]; 
      printf("file size %hu sequence num %hu", pkt->count, pkt->seq_num); 
      bytes_recd = recv(sock_connection, &filename, pkt->count, 0); 
      filename[pkt->count] = '\0'; 
      printf("The filename is: %s and %d", filename, bytes_recd);  
      fflush(stdout); 
      FILE* f = fopen(filename, "r"); 
      if (f == NULL ){
          perror("Server: error opening file"); 
	  close(sock_connection); 
          continue; 
      } 
      
      fflush(stdout); 
      if (bytes_recd > 0){
         printf("Received Sentence is:\n");
         printf("%d %d", pkt->seq_num, pkt->count);
         printf("\nwith length %d\n\n", bytes_recd);
	 
        /* prepare the message to send */

         msg_len = bytes_recd;

         str_t* resp = readlines(f); 
         int done = 0; 
         while (!done){ 
	     pkt_header_t* header = new_pkt(resp->num_chars, ++seq_num); 

             /* send message */
 	     bytes_sent = send(sock_connection, header, sizeof(pkt_header_t), 0); 
             bytes_sent = send(sock_connection, resp->content, resp->num_chars, 0); 
             if (resp->num_chars == 0){
                 done = 1; 
             }              
             delete_str_t(resp); 
             delete_pkt(header); 
             resp = readlines(f); 
             
         } 
         
      }

      /* close the socket */

      close(sock_connection);
   } 
}
