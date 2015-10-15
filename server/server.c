#include <stdio.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include "ws2812-RPi.h"

#define PORT  9999
#define LEDS_COUNT  90
#define BYTES_FOR_LED 3
#define MAXBUF (LEDS_COUNT * BYTES_FOR_LED) 

int sockfd, clientfd;

void error(char * msg)
{
  perror(msg);
  exit(errno);
}

void ctrl_c_hook(int s){
  printf("Caught signal %d\n", s);
  
  if (clientfd >= 0)  
	 close(clientfd);
  
  if (sockfd >= 0) 
	 close(sockfd);
   
  /*---Terminate PWM on Raspberry Pi---*/
  dispose_pwm();
   
   exit(1);
}

void send_data(unsigned char data[], ssize_t size) 
{
  unsigned int i;

  if (LEDS_COUNT * BYTES_FOR_LED > size) {
    printf("Need more data. Now recive %zu\n", size);
    return;
  }

  for (i = 0; i < LEDS_COUNT; i++) {
    //printf("#%x%x%x \n", data[i], data[i + 1], data[i + 2]);
    setPixelColor(i, data[i], data[i+1], data[i+2]);
  }
  
  show();
}

int main(int argc, char *argv[])
{ 
  struct sigaction sigIntHandler;  
	struct sockaddr_in sock_addr, client_addr;
  socklen_t client_len;
	unsigned char buffer[MAXBUF];
  ssize_t dataSize;
  bool connected;
  
  /*---Set hook for Ctrl+c signal---*/    
  sigIntHandler.sa_handler = ctrl_c_hook;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;  
  sigaction(SIGINT, &sigIntHandler, NULL);

	/*---Create streaming socket---*/
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)	{
		error("ERROR socket creation");		
	}

	/*---Initialize address/port structure---*/
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(PORT);
	//sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	/*---Assign a port---*/
  if (bind(sockfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) != 0)	{
		error("ERROR socket bind");		
	}

	/*---Make it a "listening socket"---*/
	if (listen(sockfd, 20) != 0) {
		error("ERROR socket listen");		
	}
  
  /*---Initialize PWM on Raspberry Pi---*/
  init_pwm();

	/*---Forever... ---*/
	client_len = sizeof(client_addr);
  connected = false;
  
  while(1) {
    /*---Accept a connection---*/
    clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (clientfd < 0)
      error("ERROR on accept");
    else {
      connected = true;
      printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }      
    
    while(connected) {
      dataSize = recv(clientfd, buffer, MAXBUF, MSG_WAITALL);      
      if (dataSize <= 0){
        printf("%s:%d disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        close(clientfd);
        connected = false;
      }        
      if (dataSize > 0)
        send_data(buffer, dataSize);       
    }    
  }
  
	exit(0);
}