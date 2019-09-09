#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>

 pthread_t threads [100];
 int threadno = 0, fd;

 void sig_handler(int signo) {

     if (signo == SIGINT) {
         std::cout << "\t Exiting..." << '\n';
         close (fd);
         exit (1);
     }
 }

 struct req {
     int reqno;
     int des;
     char str [2048];
     socklen_t addlen;
     sockaddr_in clientaddr;
 };

 void* reverse_string (void*);

 int main(int argc, char const *argv[]) {

     sockaddr_in mistaddr; // server address
     sockaddr_in clientaddr; // client address

     socklen_t addrlen = sizeof(clientaddr);
     int recvlen, msgcnt = 0;
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

     signal(SIGINT, sig_handler);
     signal(SIGTSTP, sig_handler);

     while (1) {
         std::cout << "\n\t Waiting on port " << ntohs(mistaddr.sin_port) << '\n';
         recvlen = recvfrom (fd, buf, 2048, 0, (sockaddr*) &clientaddr, &addrlen);
         msgcnt++;

         req *r = new req;
         bzero (r, sizeof (req));
         r->reqno = msgcnt;
         r->addlen = addrlen;
         r->clientaddr = clientaddr;
         r->des = fd;
         strcpy (r->str, buf);

         pthread_create (&threads [threadno++], NULL, reverse_string, (void*)r);
         if (threadno == 100)
             threadno = 0;

         memset (buf, 0, sizeof (buf));
     }

     return 0;
 }

 void* reverse_string (void* r) {

     req rq = *((req*)r);
     std::cout << "\n String: " << rq.str << "  | From: " << inet_ntoa (rq.clientaddr.sin_addr);
     std::cout << "  (message count: " << rq.reqno << ")" << '\n';
     std::reverse( rq.str, &rq.str[ strlen( rq.str ) ] );
     sendto (rq.des, rq.str, strlen (rq.str), 0, (sockaddr*) &rq.clientaddr, rq.addlen);

 }
