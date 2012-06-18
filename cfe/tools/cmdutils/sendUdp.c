/*
** Udp packet send routine
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* memset() */
#include <sys/time.h> /* select() */ 


/*
** Defines
*/
#define BIG_E 1
#define LIT_E 2

/*
** Prototypes
*/
unsigned short int hexChars2word ( char c1, char c2, char c3, char c4 );
int                hexChar2int(char hex);
int                SendUdp( const char *endian, char *hostname, char *portNum, char *packetAsciiData );


/*
** SendUdp
*/
int SendUdp( const char *endian, char *hostname, char *portNum, char *packetAsciiData ) 
{
  
  int                 sd, rc, i,j;
  struct sockaddr_in  cliAddr;
  struct sockaddr_in  remoteServAddr;
  struct hostent     *hostID;
  int                 port;
  unsigned short int  message_buffer[64];
  int                 endian_setting;
  

  /* 
  ** check arguments
  */
  if( endian == NULL )
  {
     return(-1);
  }
  
  if( hostname == NULL ) 
  {
    return(-1);
  }
  
  /*
  ** Set the endianness based on the users option
  */
  if (strncmp("LE", endian, 2 ) == 0 )
  {
     printf("Setting Little Endian output\n");
     endian_setting = LIT_E;
   }
   else
   {
      endian_setting = BIG_E;
   }
  
  /* 
  ** get server IP address (no check if input is IP address or DNS name 
  */
  hostID = gethostbyname(hostname);
  if(hostID == NULL) 
  {
    return(-2);
  }

  /* 
  ** Check port
  */
  port = atoi(portNum);
  if(port == -1) 
  {
    return(-3);
  }
  
  printf("sending data to '%s' (IP : %s); port %d\n", hostID->h_name,
	 inet_ntoa(*(struct in_addr *)hostID->h_addr_list[0]), port);

  /*
  ** Setup socket structures
  */
  remoteServAddr.sin_family = hostID->h_addrtype;
  memcpy((char *) &remoteServAddr.sin_addr.s_addr, 
	 hostID->h_addr_list[0], hostID->h_length);
  remoteServAddr.sin_port = htons(port);

  /* 
  ** Create Socket
  */
  sd = socket(AF_INET,SOCK_DGRAM,0);
  if(sd<0) 
  {
    return(-4);
  }
  
  /* 
  ** bind any port 
  */
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(0);
  
  rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
  if(rc<0) 
  {
    printf("%s: cannot bind port\n", portNum);
    return(-5);
  }
    
  printf("Data to send: %s\n",packetAsciiData);
  /*
  ** Clear the message buffer
  */
  memset((void *)message_buffer, 0, 128);
             
  /* 
  ** Convert the ASCII message to binary  
  */  
   i = 0;
   j = 0;
   while ( packetAsciiData[i] != '\0' )
   {
      if ( packetAsciiData[i] == 'x' )
	  {
         if ( endian_setting == BIG_E )
         {
		    message_buffer[j] = hexChars2word (packetAsciiData[i+1], packetAsciiData[i+2], 
                                packetAsciiData[i+3], packetAsciiData[i+4]);
         }
         else 
         {
            message_buffer[j] = hexChars2word (packetAsciiData[i+3], packetAsciiData[i+4], 
                                packetAsciiData[i+1], packetAsciiData[i+2]);
         }
	     i+=4;
         j++;
	  }
	  else
	  {
	     i++;
	  }
   }  
  
  /* 
  ** send the event 
  */
  rc = sendto(sd, &message_buffer, sizeof(message_buffer), 0, 
    (struct sockaddr *) &remoteServAddr, 
    sizeof(remoteServAddr));

  if(rc<0) 
  {
    close(sd);
    return(-6);
  }

  close(sd);
  return 0;

}

/*
** Convert a hex ASCII character to integer
*/
int hexChar2int(char hex)
{
   int value;
   
   switch (hex)
   {
      case '0':
      case '1':
	  case '2':
	  case '3':
	  case '4':
      case '5':
	  case '6':
	  case '7':
	  case '8':
	  case '9':
	     value = (hex - '0');
		 break;
	  case 'a':
	  case 'b':
	  case 'c':
	  case 'd':
	  case 'e':
	  case 'f':
		  value = (hex - 'a') + 10;
		  break;
	  case 'A':
	  case 'B':
	  case 'C':
	  case 'D':
	  case 'E':
	  case 'F':
		  value = (hex - 'A') + 10;
		  break;
	   default:
	       value = 0;
	}

    return(value);
}

/*
** Convert 4 'hex' integers ( 0..15 ) to a 16 bit unsigned integer
*/
unsigned short int hexChars2word ( char c1, char c2, char c3, char c4 )
{
   int  value;
   char nibval1, nibval2, nibval3, nibval4;
      
   nibval1 = hexChar2int(c1);
   nibval2 = hexChar2int(c2);
   nibval3 = hexChar2int(c3);
   nibval4 = hexChar2int(c4);

   value = (nibval1 << 12) + (nibval2 << 8) + (nibval3 << 4) + nibval4;

  return value;

}
