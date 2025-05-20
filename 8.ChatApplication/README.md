# Chat Application Peer-to-Peer TCP Communication

## Overview

- Develop a simple chat application for message exchange among remote peers.
- Language used: C 
- Requirement:
  1. Use TCP Sockets in your peer connection implementation.
  2. Handle multiple socket connections.
  3. Integrate both client-side and server-side code into one program and run on each peer.

## Features

- View available commands (`help`)
- Get the actual IP address of the machine (`myip`)
- Display the listening port number (`myport`)
- Establish TCP connections with other peers (`connect`)
- View all active connections (`list`)
- Terminate specific connections (`terminate`)
- Send messages to connected peers (`send`)
- Exit the program and clean up all active connections (`exit`)

---

## Demonstration:

Build program:
- Target binary file is ./bin/chat
``` terminal
make all
```

- run appication with port.
run program with port 2000
``` terminal
./bin/chat 2000
```

## Commands

### 1. `help`
Displays information about the available commands and their usage.
Ex
``` terminal
help
1. help
2. myip
3. myport
4. connect <destination> <port no>
5. list
6. terminate <connection id.>
7. send <connection id.> <message>
```

### 2. `myip`
Displays the actual (non-localhost) IP address of this process.
Ex
``` terminal
myip
IP: 0.0.0.0
```

### 3. `myport`
Displays the port number on which this process is listening for incoming TCP connections.
Ex
``` terminal
myport
Port: 2000
```

### 4. `connect <destination> <port no>`
Establishes a TCP connection to a peer identified by the IP address and port number.
- Rejects invalid IP addresses.
- Displays success or failure messages for both ends of the connection.
- Prevents self-connections and duplicate connections.
Ex
``` terminal
connect 127.0.0.1 2000
Address 127.0.0.1:2000 Connected
```

### 5. `list`
Displays a numbered list of all active connections, including:
- IP address
- Port number
Ex
``` terminal
list
id:     IP address      Port No.
 1:     127.0.0.1       2000
```

### 6. `terminate <connection id>`
Terminates a connection based on the `list` command’s ID.
- Displays error if the connection ID is invalid.
- Notifies both peers on successful termination.
Ex
``` terminal
terminate 1
127.0.0.1:2000 disconnected
```

### 7. `send <connection id> <message>`
Sends a text message (up to 100 characters) to the specified connection.
- Displays confirmation after sending.
- Receiver displays:
  - Sender IP
  - Sender Port
  - Received message
Ex
``` terminal
send 1 Hi!
Sent to 127.0.0.1:2000
```
The listen app get message
``` terminal
Message received from 127.0.0.1
Sender's Port: 38384
Message: Hi!
```

## Directory structure:
```
.
├── inc/
│   ├── common.h           
│   ├── control.h           
│   ├── gui.h
│   ├── queue.h
│   └── tcpsocket.h
├── src/
│   ├── control.c
│   ├── gui.c
│   ├── main.c
│   ├── queue.c
│   └── tcpsocket.c
├── Makefile
└── README.md
```

  1. Module `common`: contain define and macro for purpose general using
  2. Modole `control`: process main task of the program
  3. Modole `gui`: draw the CLI output for terminal
  4. Modole `tcpsocket`: contain API of handling of TCP socket
  5. Modole `queue`: data structure used in program
