CC= /usr/bin/gcc

all:	tcpclient tcpserver 

tcpclient: tcpclient.c pkt_header.o
	${CC} -g tcpclient.c pkt_header.o -o tcpclient

tcpserver: tcpserver.c pkt_header.o
	${CC} -g tcpserver.c pkt_header.o -o tcpserver
pkt_header.o: pkt_header.c pkt_header.h
	$(CC) -g -c pkt_header.c 
clean:
	rm tcpclient tcpserver pkt_header.o
