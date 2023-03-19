/**
 * @Author: Rostislav Kral xkralr06
 *
 *
 * */


#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

// Made it global due to sigtint handler
int sockfd;


// Closing opened socket and exiting program
void sigtermHandler(int a) {
    close(sockfd);
    printf("Exit");
    exit(0);
}


void tcp(const std::string &hostname, int port) {
    // server setup
    struct sockaddr_in server_addr;

    //buffer
    char server_message[1024];

    std::string client_message, check, foo;

    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));

    // Create socket:
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Was socket creation successful?
    if (sockfd < 0) {
        printf("Unable to create socket\n");
        exit(-1);
    }

    // Server setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(hostname.c_str());

    // Send connection request to server:
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Unable to connect\n");
        exit(-1);
    }

    while (true) {
        //clearing cache of stdin objects
        std::cin.clear();
        std::cin.sync();


        // Get input from the user
        std::getline(std::cin, client_message);
        client_message.append("\n"); //last char must be \n to end the line


        //Trying to send socket to server
        if (send(sockfd, client_message.c_str(), client_message.length(), 0) < 0) {
            perror("UNABLE TO SEND MESSAGE");
            exit(-1);
        }

        do {
            //null the buffer
            memset(server_message, '\0', sizeof(server_message));

            //Try to get the message from the server
            if (recv(sockfd, server_message, sizeof(server_message), 0) < 0) {
                perror("Error while receiving server's msg\n");
                exit(-1);
            }

            check.append(server_message);

        } while (check.find('\n') == std::string::npos);

        std::cout << check;

        if (check == "BYE\n") break;
        check = "";
    }
    close(sockfd);
}


void udp(const std::string &hostname, int port) {

    //server message
    char buffer[1024];

    // client message
    std::string hello;
    struct sockaddr_in servaddr;

    int n;
    socklen_t len;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket create failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(hostname.c_str());



    while (true) {
        hello = "";
        memset(buffer, ' ', sizeof(buffer));

        std::cin.clear();
        std::cin.sync();
        std::getline(std::cin, hello);

        char msg[hello.length() + 2];
        msg[0] = '\0';
        msg[1] = (char) hello.length();
        for (int i = 2; i < hello.length() + 2; i++) {
            msg[i] = hello[i - 2];
        }

        if (sendto(sockfd, msg, hello.length() + 2,
                   MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr)) < 0) {
            perror("SEND:");
        }

        n = recvfrom(sockfd, (char *) buffer, 1024,
                     MSG_WAITALL, (struct sockaddr *) &servaddr,
                     &len);

        if (n < 0) {
            perror("RECEIVE:");
        }

        if (buffer[1] == 0) {
            std::cout << "OK: ";
        } else {
            std::cout << "ERR: ";
        }

        for (int i = 3; i < n; i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;

    }
    close(sockfd);
}


int main(int argc, char *argv[]) {

    int opt, port;
    std::string mode, host;
    // std::cout<< argc;
    if (argc != 7) {
        std::cout << "Invalid number of arguments\nUsage: ipkcpc -h <host> -p <port> -m <mode>";
        exit(-1);
    }

    signal(SIGTERM, sigtermHandler);
    signal(SIGINT, sigtermHandler);


    while ((opt = getopt(argc, argv, "h:p:m:")) != -1) {
        switch (opt) {
            case 'h':
                host = optarg;
                break;
            case 'p':
                //TODO: validate port
                port = atoi(optarg);

                if(port < 1023) {
                    std::cout << "Invalid port number";
                    exit(-1);
                }
                //std::cout << port;
                break;
            case 'm':
                mode = optarg;

                if (mode != "udp" && mode != "tcp") {
                    std::cout << "Invalid mode\nUsage: ipkcpc -h <host> -p <port> -m <mode>";
                    exit(-1);
                }
                break;
            default:
                printf("Usage: ipkcpc -h <host> -p <port> -m <mode>");
                exit(-1);
                break;
        }

    }


    if (mode == "udp") {
        udp(host, port);
    } else {
        tcp(host, port);
    }

    return 0;
}
