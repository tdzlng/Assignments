
# IPv4 Stream Socket
b1:
	gcc client_socket.c -o IPv4Stream_Client -DD_IPV4_STREAM
	gcc server_socket.c -o IPv4Stream_Server -DD_IPV4_STREAM
# Ipv4 Datagram Socket
b2:
	gcc client_socket.c -o IPv4Datagram_Client -DD_IPV4_DATAGRAM
	gcc server_socket.c -o IPv4Datagram_Server -DD_IPV4_DATAGRAM	
# Unix Stream Socket
b3:
	gcc client_socket.c -o UnixStream_Client -DD_UNIX_STREAM
	gcc server_socket.c -o UnixStream_Server -DD_UNIX_STREAM	
# Unix Datagram Socket
b4:
	gcc client_socket.c -o UnixDatagram_Client -DD_UNIX_DATAGRAM
	gcc server_socket.c -o UnixDatagram_Server -DD_UNIX_DATAGRAM

clean:
	rm -rfv IPv4Stream_Client IPv4Stream_Server IPv4Datagram_Client IPv4Datagram_Server UnixStream_Client UnixStream_Server UnixDatagram_Client UnixDatagram_Server
	rm -rfv a.out