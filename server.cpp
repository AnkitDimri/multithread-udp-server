/*****************************************************************************************************************************************
 *                                                                                                                                       *
 *  author: Ankit Dimri                                                                                                                  *
 *  Multithreaded udp server to reverse a given input string by the client.                                                              *
 *                                                                                                                                       *
 *****************************************************************************************************************************************/

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

 pthread_t threads [100];                   // at max 100 threads
 int threadno = 0, fd;

 /* Signal handler to handle ctrl+c to close server socket before terminating the server */
 void sig_handler(int signo) {

     if (signo == SIGINT) {
         std::cout << "\t Exiting..." << '\n';
         close (fd);
         exit (1);
     }
 }

 /* Structure to hold the necessary parameters to pass into the threaded reverse_string function */
 struct req {
     int reqno;
     int des;
     char str [2048];
     socklen_t addlen;
     sockaddr_in clientaddr;
 };

 /*
  * Function called in threads to reverse the string and send the packet back to the respective
  * client socket with the string sent reversed
  */
 void* reverse_string (void*);

 int main(int argc, char const *argv[]) {

     /* srver name : Misty */
     sockaddr_in mistaddr; // server address
     sockaddr_in clientaddr; // client address

     socklen_t addrlen = sizeof(clientaddr);
     int recvlen, msgcnt = 0;
     char buf [2048]; // Hold buffer sent in udp packet

     /* Create socket */
     if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
         std::cout << "\n\t Socket creation failed...\n\t Exiting..." << '\n';
         return 0;
     }

     std::cout << "\n\t Socket created..." << '\n';

     /* clear the memory allocated */
     memset ((sockaddr*)&mistaddr, 0, sizeof (mistaddr));
     mistaddr.sin_family = AF_INET;   // IPv4 address family
     mistaddr.sin_addr.s_addr = htonl (INADDR_ANY);  // Give the local machine address
     mistaddr.sin_port = htons (1721); // Port at which server listens to the requests

     /* Bind the IP address and the port number to craete the socket */
     if (bind (fd, (sockaddr*)&mistaddr, sizeof (mistaddr)) == -1) {
         std::cout << "\n\t Binding failed...\n\t Exiting..." << '\n';
         return 0;
     }

     std::cout << "\n\t Binding succesful" << '\n';

     /* Signal catching */
     signal(SIGINT, sig_handler);
     signal(SIGTSTP, sig_handler);

     /* Infinte server loop */
     while (1) {
         /* Server listing */
         std::cout << "\n\t Waiting on port " << ntohs(mistaddr.sin_port) << '\n';
         /* waiting to recieve the requests from client at port */
         /* Code blocking function */
         recvlen = recvfrom (fd, buf, 2048, 0, (sockaddr*) &clientaddr, &addrlen);
         /* Counting number of messages recieved */
         msgcnt++;

         /* Filling the parameter values of the threaded function */
         req *r = new req;  // allocate memory
         bzero (r, sizeof (req));  // Clear memory
         r->reqno = msgcnt;
         r->addlen = addrlen;
         r->clientaddr = clientaddr;
         r->des = fd;
         strcpy (r->str, buf);

         /* Create thread to execute the client requests paralelly */
         pthread_create (&threads [threadno++], NULL, reverse_string, (void*)r);
         if (threadno == 100)
             threadno = 0;

         memset (buf, 0, sizeof (buf)); // clear buffer
     }

 }

 // Function to reverse the string and send back the packet */
 void* reverse_string (void* r) {

     req rq = *((req*)r); // Type casting
     std::cout << "\n String: " << rq.str << "  | From: " << inet_ntoa (rq.clientaddr.sin_addr) << ":" << ntohs (rq.clientaddr.sin_port);
     std::cout << "  (message count: " << rq.reqno << ")" << '\n';
     std::reverse( rq.str, &rq.str[ strlen( rq.str ) ] ); // reversing the string
     /* Send the reversed string back to the client at its socket */
     sendto (rq.des, rq.str, strlen (rq.str), 0, (sockaddr*) &rq.clientaddr, rq.addlen);

 }
