/***************************************************************
  Student Name: Alan Benitez
  File Name: HttpServer.cpp
  Project 1

  In this file, I create the server, and handle the requests and responses
***************************************************************/
#include "HttpServer.hpp"

//Default constructor, creates the sockets and do the connections, binding, listening, accepting, among other jobs
HttpServer::HttpServer(){
    addrLen = sizeof(address);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
    cout << "Server started." << endl << endl;
    cout << "Server ready." << endl << endl;

    while(true) {

        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrLen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        handleResponses();

    }
}

//This method will handle the responses from the server
void HttpServer::handleResponses(){
    if(fork() == 0) {
        cout << "In forked process\n" << endl;
        char * request = new char [400];
        char * pointer = request;
        int numBytes = 0;
        int max = 400;
        int length = 0;
        FILE* file;

        string received;
        cout << "A client has connected\n" << endl;

        //Will read the request from the client
        while((numBytes = read(new_socket,pointer,max)) > 0) {
            pointer += numBytes;
            max -= numBytes;
            length += numBytes;
        }

        //Will print an error if there is an error reading
        if(numBytes < 0){
            cout << "Error reading"<< endl;
        }

        received = request;

        //Will print the request packets from the client
        cout << "Request from client\n\n" + received +" \n\nend of what was received\n" << endl;
        if(received.find("GET /index.html") != string::npos) {
            //Opening the index file
            file = fopen("index.html","rb");
            char * buffer = new char [1000];
            char * point = buffer;
            int numBytesRead = fread(buffer,sizeof(char),1000,file);

            //Adding contents to the header
            string header = manageCommunicationErrors(received);
            header += "Content-Type: ";
            header += manageContentType(received);
            header += "Connection: close\n";

            string contentLength = "Content-Length: ";
            contentLength += to_string(numBytesRead);
            contentLength += "\n\n" ;
            header += contentLength;

            //printing what was read from the html file for testing purposes
            cout << point << endl << endl;

            //Sending what's in the header
            send(new_socket, (char*)header.c_str(), (int)header.size(), 0);
            cout << header << endl;
            int sent = send(new_socket,point,numBytesRead,0);
            //Sending what's on the buffer to the browser
            sendToBrowser(new_socket, point, numBytesRead, sent);

            //deleting the buffer and closing the file
            delete []buffer;
            fclose(file);
            cout << "Sent index.html\n" << endl;
        }
        else if(received.find("GET /olimpia.png") != string::npos) {
            //Opening the image file
            file = fopen("olimpia.png","r");      //image.html
            char * buffer = new char [400000];
            char * point = buffer;
            int numBytesRead = fread(point,sizeof(char),400000,file);

            //Adding contents to the header
            string header = manageCommunicationErrors(received);
            header += "Content-Type: ";
            header += manageContentType(received);
            header += "Connection: close\n";

            string contentLength = "Content-Length: ";
            contentLength += to_string(numBytesRead);
            contentLength += "\n\n";
            header += contentLength;

            cout << point << endl << endl;

            //Sending what's in the header
            send(new_socket, (char*)header.c_str(), (int)header.size(), 0);
            cout << header << endl;
            int sent = send(new_socket,point,numBytesRead,0);

            //Sending what's on the buffer to the browser
            sendToBrowser(new_socket, point, numBytesRead, sent);

            delete []buffer;
            fclose(file);

            cout << "Sent olimpia.png\n" << endl;
        }
        else if(received.find("GET /normal.html") != string::npos){
            //Opening the normal file that shows that is working
            file = fopen("normal.html","rb");
            char * buffer = new char [1000];
            char * point = buffer;
            int numBytesRead = fread(buffer,sizeof(char),1000,file);

            //Adding contents to the header
            string header = manageCommunicationErrors(received);
            header += "Content-Type: ";
            header += manageContentType(received);
            header += "Connection: close\n";

            string contentLength = "Content-Length: ";
            contentLength += to_string(numBytesRead);
            contentLength += "\n\n";
            header += contentLength;

            cout << point << endl << endl;

            //Sending what's in the header
            send(new_socket, (char*)header.c_str(), (int)header.size(), 0);
            cout << header << endl;
            int sent = send(new_socket,point,numBytesRead,0);

            //Sending what's on the buffer to the browser
            sendToBrowser(new_socket, point, numBytesRead, sent);

            delete []buffer;
            fclose(file);
            cout << "Sent normal.html\n" << endl;
        }
        else if(received.find("GET /error.html") != string::npos) {
            file = fopen("error.html","rb");
            char * buffer = new char [1000];
            char * point = buffer;
            int numBytesRead = fread(buffer,sizeof(char),1000,file);

            //Adding contents to the header
            string header = manageCommunicationErrors(received);
            header += "Content-Type: ";
            header += manageContentType(received);
            header += "Connection: close\n";

            string contentLength = "Content-Length: ";
            contentLength += to_string(numBytesRead);
            contentLength += "\n\n";
            header += contentLength;
            header += point;

            //Sending what's in the header
            send(new_socket, (char*)header.c_str(), (int)header.size(), 0);
            cout << header << endl;
            int sent = send(new_socket,point,numBytesRead,0);

            //Sending what's on the buffer to the browser
            sendToBrowser(new_socket, point, numBytesRead, sent);

            delete []buffer;
            fclose(file);
            cout << "Sent error.html\n" << endl;
        }

        delete[] request;
        close(new_socket);
        exit(0);
    }
    else
        cout << "Parent going back to wait" << endl;
}

//Will send the content of the buffer to the browser
void HttpServer::sendToBrowser(int socket, char * point, int numBytesRead, int sent){
    while(sent > 0) {
        numBytesRead -= sent;
        point += sent;
        sent = send(socket,point,numBytesRead,0);
    }
}

//Helper method that will return the correct status code
string HttpServer::manageCommunicationErrors(string received){
    if(received.find("error") != string::npos ){
        return "HTTP/1.1 404 NOT FOUND\n";
    }else{
        return "HTTP/1.1 200 OK\n";
    }
}

//Helper method that will return the correct content type of the file
string HttpServer::manageContentType(string received) {
    if(received.find("png") != string::npos ){
        return "image/png\n";
    }else{
        return "text/html\n";
    }
}

