/*
** tlmUtil.c -- Read and display UDP telemetry packets
*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close() */
#include <string.h> /* memset() */


#define LOCAL_SERVER_PORT 1235
#define MAX_MSG 200
#define EVENT_PORT 0x0808

int main(int argc, char *argv[]) 
{
  
  int                 sd, rc, n, cliLen;
  int                 i;
  struct sockaddr_in  cliAddr, servAddr;
  unsigned short int  msg[MAX_MSG/2];

  /* 
  ** socket creation 
  */
  sd=socket(AF_INET, SOCK_DGRAM, 0);
  if(sd < 0) 
  {
    printf("%s: cannot open socket \n",argv[0]);
    exit(1);
  }

  /*
  ** bind local server port 
  */
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(LOCAL_SERVER_PORT);
  rc = bind (sd, (struct sockaddr *) &servAddr,sizeof(servAddr));
  if(rc<0) 
  {
    printf("%s: cannot bind port number %d \n", 
	   argv[0], LOCAL_SERVER_PORT);
    exit(1);
  }

  printf("%s: waiting for data on port UDP %u\n", 
	   argv[0],LOCAL_SERVER_PORT);

  /* server infinite loop */
  while(1) 
  {
    
    /* 
    ** init buffer 
    */
    memset((char *)msg,0x0,MAX_MSG);

    /* 
    ** receive message 
    */
    cliLen = sizeof(cliAddr);
    n = recvfrom(sd, (char *)msg, MAX_MSG, 0, 
		 (struct sockaddr *) &cliAddr, (socklen_t *) &cliLen);

    if(n<0) 
    {
      printf("%s: cannot receive data \n",argv[0]);
      continue;
    }
      
    /* 
    ** print received message 
    */
    
    /*
    ** Check for event message. If it's not an event
    **  then print the packet data.
    */
    if ( msg[0] == EVENT_PORT )
    {
       printf("Event --> %s\n",(char *)&(msg[4]) );
    }
    else
    {
       printf("Telemetry Packet From: %s:UDP%u : \n", 
	      inet_ntoa(cliAddr.sin_addr),
	      ntohs(cliAddr.sin_port));
       for ( i = 0; i < (n/2); i++ )
          printf("%04X ",msg[i]);
       printf("\n");
    }
    
  }/* end of server infinite loop */

  return 0;

}
