/* tcp_ client.c */ 
/* Programmed by Landon Jones and Ben Conrad */
/* Sept. 19, 2019 */     

#include <stdio.h>          /* for standard I/O functions */
#include <stdlib.h>         /* for exit */
#include <string.h>         /* for memset, memcpy, and strlen */
#include <netdb.h>          /* for struct hostent and gethostbyname */
#include <sys/socket.h>     /* for socket, connect, send, and recv */
#include <netinet/in.h>     /* for sockaddr_in */
#include <unistd.h>         /* for close */
#include "pkt_header.h" 
#define STRING_SIZE 1024
int main(void) {

   int sock_client;  /* Socket used by client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE] = "cisc450.cis.udel.edu"; /* Server's hostname */
   unsigned short server_port = 65000;  /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];  /* send message */
   char modifiedSentence[STRING_SIZE]; /* receive message */
   unsigned int msg_len;  /* length of message */                      
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   
   int total_bytes_recd; /* total bytes received from start of connection until the end*/ 
   int total_packets_recd; /* total number of packets received*/ 
   
   int done; /* condition for the client to loop on*/ 
       
   unsigned short seq_num; /* the sequence number for the pkt*/ 
   /* open a socket */

   if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      perror("Client: can't open stream socket");
      exit(1);
   }

   /* Note: there is no need to initialize local client address information 
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* initialize server address information */

   
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname");
      close(sock_client);
      exit(1);
   }

   
   /* Clear server address structure and initialize with server address */
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   memcpy((char *)&server_addr.sin_addr, server_hp->h_addr,
                                    server_hp->h_length);
   server_addr.sin_port = htons(server_port);

    /* connect to the server */
 		
   if (connect(sock_client, (struct sockaddr *) &server_addr, 
                                    sizeof (server_addr)) < 0) {
      perror("Client: can't connect to server");
      close(sock_client);
      exit(1);
   }
  
   /* user interface */

   printf("Enter the file you want to receive\n");
   scanf("%s", sentence);
   msg_len = strlen(sentence);
    
   
   send_pkt(sock_client, sentence, msg_len, 0); /* send the desired filename*/  
   done = 0; 
   FILE* f = fopen("out.txt", "w"); /* open out.txt file handler*/ 
   while (!done){ 
      
      pkt_t* line = recv_pkt(sock_client); /*recvs the packet header and content (a line of the file)*/ 
      
      total_bytes_recd += line->header->count; /* increment the bytes received*/ 
      
      if (line->header->count == 0){ /* EOT packet*/ 
         done = 1; /* stop condition is true*/ 
          
         printf("End of Transmission Packet with sequence number %hu and data bytes %hu received\n", line->header->seq_num, line->header->count); 
         delete_pkt(line); /* free the packet*/ 
         fclose(f); /* close the file at end of transmission*/ 
         break; 
      } 
      total_packets_recd++;  /* increment total packets if it's not the EOT*/ 
      fprintf(f, "%s", line->content); /* */ 
      printf("Packet %hu received with %hu data bytes\n", line->header->seq_num, line->header->count); 
      delete_pkt(line); 
   }
   /* print end statistics*/  
   printf("Total packets received: %hu\nTotal bytes received: %hu\n", total_packets_recd, total_bytes_recd); 
   
   
   /* close the socket */

   close (sock_client);
}
