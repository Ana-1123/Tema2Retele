#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
extern int errno;
int port=2024;
int main (int argc, char *argv[])
{
  int sd;			
  struct sockaddr_in server;
  char msg_trimis[1000];
  char msg_primit[1000];
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Error at socket().\n");
      return errno;
    }
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons (port);
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("-->Error at connect().\n");
      return errno;
    }

  printf("Aplicatia Offline Messenger, pentru mai multe detalii tastati Ajutor\n ");
  fflush (stdout); 
  while(1) 
  {
    bzero (msg_primit, 1000);
    bzero(msg_trimis,1000);
    fflush(stdout);
    read(0,msg_trimis,1000);    
    msg_trimis[strlen(msg_trimis)-1]=0;
     if (write (sd, msg_trimis,sizeof(msg_trimis)) <= 0)
      {
      perror ("-->Error at write() to server.\n");
      return errno;
      }
      if(strcmp(msg_trimis,"EXIT")==0) return 0;
      if (read (sd, msg_primit, sizeof(msg_primit)) < 0)
      {
       perror ("-->Error at read() from server.\n");
      return errno;
      }
     printf ("-->: %s\n", msg_primit);
  }
  close (sd);
  return 0;
}
