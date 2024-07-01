#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define INCOMING_QUEUE_SIZE 3

void _exit_with_error(char *msg, int code) {
  fprintf(stderr, "%s\n", msg);
  exit(code);
}

int main(int argc, char *argv[]) {
  char *port;
  if (argc == 2) {
    port = argv[1];
  } else {
    port = "80";
  }

  struct addrinfo hints, *res;
  struct sockaddr_storage incoming_addr;
  socklen_t incoming_addr_len;
  int incoming_socket;

  memset( &hints, 0, sizeof(struct addrinfo) );
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo( NULL, port, &hints, &res );

  int server_socket = socket( res->ai_family, res->ai_socktype, res->ai_protocol );

  if ( bind( server_socket, res->ai_addr, res->ai_addrlen ) == -1 ) {
    _exit_with_error( "Error: Could not bind", 1 );
  };
  if ( listen( server_socket, INCOMING_QUEUE_SIZE) == -1) {
    _exit_with_error( "Error: Could not listen", 2);
  }
  while (1) {
    incoming_addr_len = sizeof(incoming_addr);
    incoming_socket = accept(server_socket, (struct sockaddr *)&incoming_addr, &incoming_addr_len);
    if (incoming_socket == -1) {
      _exit_with_error( "Error: Could not accept", 3);
    }
  }
  free(res);
  return 0;
}
