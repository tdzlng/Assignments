
# IPv4 Stream Socket done
b1:
	gcc client_socket.c -o Client_IPv4Stream -DD_IPV4_STREAM
	gcc server_socket.c -o Server_IPv4Stream -DD_IPV4_STREAM
# Ipv4 Datagram Socket hotfix
b2:
	gcc client_socket.c -o Client_IPv4Datagram -DD_IPV4_DATAGRAM
	gcc server_socket.c -o Server_IPv4Datagram -DD_IPV4_DATAGRAM	
# Unix Stream Socket to do
b3:
	gcc client_socket.c -o Client_UnixStream -DD_UNIX_STREAM
	gcc server_socket.c -o Server_UnixStream -DD_UNIX_STREAM	
# Unix Datagram Socket to do
b4:
	gcc client_socket.c -o Client_UnixDatagram -DD_UNIX_DATAGRAM
	gcc server_socket.c -o Server_UnixDatagram -DD_UNIX_DATAGRAM

clean:
	rm -rfv Client_IPv4Stream Server_IPv4Stream Client_IPv4Datagram Server_IPv4Datagram Client_UnixStream Server_UnixStream Client_UnixDatagram Server_UnixDatagram
	rm -rfv a.out