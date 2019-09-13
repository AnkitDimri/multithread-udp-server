/*****************************************************************************************************************************************
 *                                                                                                                                       *
 *  author: Ankit Dimri                                                                                                                  *
 *  udp client which runs infinitely to take a user string input and sends it to multitheaded udp server to reverse it and send back     *
 *  to client to display.                                                                                                                *
 *                                                                                                                                       *
 *****************************************************************************************************************************************/

#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>

 int fd; // global client socket descriptor

 /* Signal handlre for ctrl+c to terminate the infinite client properly by closing the socket */
 void sig_handler(int signo) {

     /* signal handler */
     if (signo == SIGINT) {
         std::cout << "\t Exiting..." << '\n';
         close (fd);
         exit (1);
     }
 }

 int main(int argc, char const *argv[]) {

     sockaddr_in myaddr,remaddr;
     int slen = sizeof (remaddr), recvlen;
     char buf [2048]; // buffer to take user input and send to the server and recieve the reversed string */
     int rcvlen;
     char *server = "127.0.0.1"; // Enter the server address

     /* create socket */
     if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) == -1 ) {
         std::cout << "\n\t Socket creation failed...\n\t Exiting..." << '\n';
         return 0;
     }

     signal(SIGINT, sig_handler); // handle ctrl+c
     signal(SIGTSTP, sig_handler); // handle ctrl+z

     std::cout << "\n\t Socket created..." << '\n';

     memset ((char*)&myaddr, 0, sizeof (myaddr));  // clear memory allocated
     myaddr.sin_family = AF_INET; // IPv4 address family
     myaddr.sin_addr.s_addr = htonl (INADDR_ANY); // give local machine address automatically
     myaddr.sin_port = htons (3071); // a port number for the socket

     // bind the IP address and the port to the descritor to create socket */
     if (bind(fd, (sockaddr*) &myaddr, sizeof (myaddr)) < 0) {
         std::cout << "\n\t Bind failed...\n\t Exiting..." << '\n';
         return 0;
     }

     std::cout << "\n\t Bind succesful..." << '\n';

     memset ((char*) &remaddr, 0, sizeof (remaddr)); // clear memory allocated
     remaddr.sin_family = AF_INET;  // IPv4 address family
     remaddr.sin_port = htons (1721);  // Port number at which the server is listning
     if (inet_aton (server, &remaddr.sin_addr) == 0) {
         std::cout << "\n\t inet_aton() failed...\n\t Exiting..." << '\n';
         return 0;
     }

     int i = 0; // to count the number of packet

     /* Infinite client loop */
     while (1) {

         std::cout << "\n\t Enter the string to be reversed : ";
         std::cin.getline (buf, 2048); // user input for string
         std::cout << "\n\t Sending packet " << i++ << " to " << server << " on port " << ntohs(remaddr.sin_port) << '\n';
         /* Send udp packet to server */
         sendto (fd, buf, strlen (buf), 0, (sockaddr*) &remaddr, slen);

         memset (buf, 0, sizeof (buf)); // clear the buffer to hold the reversed value */
         socklen_t len;
         /* Wait for the reply from the server */
         recvlen = recvfrom (fd, buf, 2048, 0, (sockaddr*) &remaddr, &len); // wait; code blocking segment
         /* Print the reversed string */
         std::cout << "\n\t recieved: " << buf << '\n';
         memset (buf, 0, sizeof (buf)); // clear the buffer

     } // end of while

 } // end of main
