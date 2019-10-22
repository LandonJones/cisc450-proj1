/* tcp_ client.c */ 
/* Programmed by Adarsh Sethi */
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

/* 
recvs a line and from the server using sock_client 
writes the line from the server to the file pointer f
*/ 
int recv_line(FILE* f, int sock_client){
    int bytes_recd; /* bytes received from the socket */ 
    int ret; /* value to be returned */ 
    pkt_header_t* resp_header = (pkt_header_t *)malloc(sizeof(pkt_header_t));
    bytes_recd = recv(sock_client, resp_header, sizeof(pkt_header_t), 0);
    char* response = (char *)malloc((resp_header->count + 1)*sizeof(char));
    
    bytes_recd = recv(sock_client, response, resp_header->count, 0);
    response[resp_header->count] = '\0'; 
    if (bytes_recd == resp_header->count){ 
        if (bytes_recd > 0){ 
            printf("Packet %hu received with %hu data bytes\n", resp_header->seq_num, resp_header->count); 
            fprintf(f, "%s", response); 
            ret = 0; 
	} 
        else if (strlen(response) == 0){ 
            printf("End of Transmission Packet with sequence number %hu received with %hu data bytes\n", resp_header->seq_num, resp_header->count); 
            ret = -1; 
        } 
    }
    else{ 
        printf("Error in transmission: Expected %d bytes, but received %d", resp_header->count, bytes_recd); 
    }  
    fflush(stdout); 
    delete_pkt(resp_header); 
    free(response);  
    return ret; 
       
} 
int main(void) {

   int sock_client;  /* Socket used by client */

   struct sockaddr_in server_addr;  /* Internet address structure that
                                        stores server address */
   struct hostent * server_hp;      /* Structure to store server's IP
                                        address */
   char server_hostname[STRING_SIZE]; /* Server's hostname */
   unsigned short server_port;  /* Port number used by server (remote port) */

   char sentence[STRING_SIZE];  /* send message */
   char modifiedSentence[STRING_SIZE]; /* receive message */
   unsigned int msg_len;  /* length of message */                      
   int bytes_sent, bytes_recd; /* number of bytes sent or received */
   
   unsigned short seq_num = 0; 
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

   printf("Enter hostname of server: ");
   scanf("%s", server_hostname);
   if ((server_hp = gethostbyname(server_hostname)) == NULL) {
      perror("Client: invalid server hostname");
      close(sock_client);
      exit(1);
   }

   printf("Enter port number for server: ");
   scanf("%hu", &server_port);

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
   
   pkt_header_t* header = new_pkt((unsigned short)msg_len, seq_num); 
   /* send message */
   
   bytes_sent = send(sock_client, header, sizeof(pkt_header_t), 0);
   printf("%s\n",sentence);    
   bytes_sent = send(sock_client, &sentence, msg_len, 0);   

   printf("bytes sent %d\n", bytes_sent);  
   /* get response from server */
   FILE* f = fopen("out.txt", "w"); 
   while (recv_line(f, sock_client) != -1) {} 
   
   /*
   pkt_header_t* resp_header = (pkt_header_t *)malloc(sizeof(pkt_header_t)); 
   bytes_recd = recv(sock_client, header, sizeof(pkt_header_t), 0); 
   char* response = (char *)malloc(header->count*sizeof(char)); 
   
   bytes_recd = recv(sock_client, response, header->count, 0); 
   printf("\nThe response from server is:\n");
   printf("%s", response);
   fflush(stdout);
   */  
   /* close the socket */

   close (sock_client);
}
