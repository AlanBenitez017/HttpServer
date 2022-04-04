/***************************************************************
  Student Name: Alan Benitez
  File Name: HttpServer.hpp
  Project 1

  This is the header file of the HttpServer.cpp
***************************************************************/
#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 60010

using namespace std;

class HttpServer{

public:
    HttpServer();
    int server_fd;
    int new_socket;
    struct sockaddr_in address;
    int addrLen;

    void handleResponses();
    void sendToBrowser(int socket, char * point, int numBytes, int sent);
    string manageCommunicationErrors(string received);
    string manageContentType(string received);
};



#endif