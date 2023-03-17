#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

int sockfd;

void sigtermHandler(int a)
{
    close(sockfd);
    printf("Exit");
    exit(0);
}



void tcp()
{
    int sockfd;
    struct sockaddr_in server_addr;
    char server_message[2000];
    std::string client_message, check;

    // Clean buffers:
    memset(server_message,'\0',sizeof(server_message));

    // Create socket:
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        printf("Unable to create socket\n");
        exit(-1);
    }

    printf("Socket created successfully\n");

    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2023);
    server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    // Send connection request to server:
    if(connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect\n");
        exit(-1);
    }
    printf("Connected with server successfully\n");

    while(true) {
        // Get input from the user:
        printf("Enter message: ");
        std::cin.clear();
        std::cin.sync();
        std::getline(std::cin, client_message);
        client_message = client_message + '\n';

        // Send the message to server:
        if (send(sockfd, client_message.c_str(), client_message.length(), 0) < 0) {
            printf("Unable to send message\n");
            exit(-1);
        }

        // Receive the server's response:
        if (recv(sockfd, server_message, sizeof(server_message), 0) < 0) {
            printf("Error while receiving server's msg\n");
            exit(-1);
        }

        printf("Server's response: %s\n", server_message);
        check = server_message;
        strcpy(server_message, "");
        // std::cout << check;

        if(std::strcmp(check.c_str(), "BYE") == 0) printf("BYE WAS CHECKCED");

    }
    // Close the socket:
    close(sockfd);

}


void udp(const std::string& hostname, int port)
{
    char buffer[1024];

    size_t input_size = 0;
    std::string hello;
    struct sockaddr_in servaddr;

    //std::cout << hello << std::endl;
    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    // Filling server information

    /*if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
     {
         printf("\n Error : Connect Failed \n");
         exit(0);
     }*/

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(hostname.c_str());

    int n;
    socklen_t len;

    //hello = "\\x00\\x07(+ 1 1)";
    //hello = to_ascii(hello);
    //hello.insert(0, "\0");

    while(true) {
        std::cin.clear();
        std::cin.sync();
        std::getline(std::cin, hello);
        // std::cout << hello;
        char msg[hello.length() + 2];
        msg[0] = '\0';
        msg[1] = (char) hello.length();
        for (int i = 2; i < hello.length() + 2; i++) {
            //  std::cout << hello[i];
            msg[i] = hello[i - 2];
        }

        //std::cout << hello.length();
        sendto(sockfd, msg, hello.length() + 2,
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        //std::cout << "Hello message sent." << std::endl;

        n = recvfrom(sockfd, (char *) buffer, 1024,
                     MSG_WAITALL, (struct sockaddr *) &servaddr,
                     &len);
        buffer[n] = '\0';

        //std::cout << buffer[3] << std::endl;
        for (int i = 0; i < n; i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;

    }
    close(sockfd);
}


int main(int argc,char *argv[]) {

    int opt, port;
    std::string mode, host;
    // std::cout<< argc;
    if(argc != 7) { std::cout<<"Invalid number of arguments";
        exit(-1);}

    signal(SIGTERM, sigtermHandler);

    while((opt = getopt(argc, argv, "h:p:m:")) != -1)
    {
        switch(opt) {
            case 'h':
                host = optarg;
                break;
            case 'p':
                //TODO: validate port
                port = atoi(optarg);

                //  std::cout << port;
                break;
            case 'm':
                mode = optarg;

                if (mode != "udp" && mode != "tcp") {
                    std::cout << "Invalid mode";
                    exit(-1);
                }
                break;
            default:
                printf("error");
                exit(-1);
                break;
        }

    }


    if(mode == "udp")
    {
        udp(host, port);
    }
    else
    {
        tcp();
    }

    return 0;
}
