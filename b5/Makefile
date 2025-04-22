
# IPv4 Stream Socket done
b1:
	gcc client_socket_INT.c -o Client_IPv4Stream -DD_IPV4_STREAM
	gcc server_socket_INT.c -o Server_IPv4Stream -DD_IPV4_STREAM
# Ipv4 Datagram Socket done
b2:
	gcc client_socket_INT.c -o Client_IPv4Datagram -DD_IPV4_DATAGRAM
	gcc server_socket_INT.c -o Server_IPv4Datagram -DD_IPV4_DATAGRAM	
# Unix Stream Socket done
b3:
	gcc client_socket_UDP.c -o Client_UnixStream -DD_UNIX_STREAM
	gcc server_socket_UDP.c -o Server_UnixStream -DD_UNIX_STREAM	
# Unix Datagram Socket hotfix
b4:
	gcc client_socket_UDP.c -o Client_UnixDatagram -DD_UNIX_DATAGRAM
	gcc server_socket_UDP.c -o Server_UnixDatagram -DD_UNIX_DATAGRAM

all: b1 b2 b3 b4

clean:
	rm -rfv Client_IPv4Stream Server_IPv4Stream Client_IPv4Datagram Server_IPv4Datagram Client_UnixStream Server_UnixStream Client_UnixDatagram Server_UnixDatagram
	rm -rfv a.out
	rm -rf Server_Socket