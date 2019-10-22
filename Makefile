CC= /usr/bin/gcc

all:	tcpclient tcpserver 

tcpclient: tcpclient.c;
	${CC} tcpclient.c pkt_header.c str.c -o tcpclient

tcpserver: tcpserver.c;
	${CC} tcpserver.c pkt_header.c str.c -o tcpserver
clean:
	rm tcpclient tcpserver
