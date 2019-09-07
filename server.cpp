#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <algorithm>

 int main(int argc, char const *argv[]) {

     sockaddr_in mistaddr; // server address
     sockaddr_in clientaddr; // client address

     socklen_t addrlen = sizeof(clientaddr);
     int recvlen, fd, msgcnt = 0;
     char buf [2048];

     if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
         std::cout << "\n\t Socket creation failed...\n\t Exiting..." << '\n';
         return 0;
     }

     std::cout << "\n\t Socket created..." << '\n';

     memset ((sockaddr*)&mistaddr, 0, sizeof (mistaddr));
     mistaddr.sin_family = AF_INET;
     mistaddr.sin_addr.s_addr = htonl (INADDR_ANY);
     mistaddr.sin_port = htons (1721);

     if (bind (fd, (sockaddr*)&mistaddr, sizeof (mistaddr)) == -1) {
         std::cout << "\n\t Binding failed...\n\t Exiting..." << '\n';
         return 0;
     }

     std::cout << "\n\t Binding succesful" << '\n';

     while (1) {
         std::cout << "\n\t Waiting on port 1721" << '\n';
         recvlen = recvfrom (fd, buf, 2048, 0, (sockaddr*) &mistaddr, &addrlen);
         std::cout << "\n\t Message: " << buf << '\n';
         //memset (buf, 0, sizeof (buf));
         //std::cout << "reply: ";
         //std::cin.getline (buf, 2048);
         std::reverse( buf, &buf[ strlen( buf ) ] );
         if (strcmp ("exit", buf) == 0) {
             close (fd);
             std::cout << "\n\t Exiting..." << '\n';
             return 0;
         }
        // getline (std::cin, buf);
        // std::cin >> buf;
         msgcnt++;
         std::cout << "\n\t (message count: " << msgcnt << ")" << '\n';
         sendto (fd, buf, strlen (buf), 0, (sockaddr*) &mistaddr, addrlen);
         memset (buf, 0, sizeof (buf));
     }

     return 0;
 }
