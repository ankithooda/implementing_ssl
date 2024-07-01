#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define INCOMING_QUEUE_SIZE 3
#define BUFFER_SIZE 256

void _exit_with_error(char *msg, int code) {
  perror( msg );
  exit( code );
}

void send_failure_message(int socket) {
  char *out_buffer = (char *)malloc( BUFFER_SIZE );
  sprintf( out_buffer, "HTTP/1.1 500 Error Occurred\r\n\r\n" );

  if ( send( socket, out_buffer, strlen( out_buffer ), 0 ) < strlen( out_buffer ) ) {
    perror("Could not send reply\n");
  }
  free(out_buffer);
  return;
}

void send_success_message(int socket) {
  char *out_buffer = (char *)malloc( BUFFER_SIZE );
  sprintf( out_buffer, "HTTP/1.1 200 Success\r\nConnection: Close\r\n\
Content-Type:text/html\r\n\
\r\n<html><head><title>Test Page</title></head><body>Nothing here</body></html>\
\r\n" );

  if ( send( socket, out_buffer, strlen(out_buffer), 0 ) < strlen( out_buffer ) ) {
    perror( "Could not send reply" );
  }
  free(out_buffer);
  return;
}

void process_http_request(int server_socket) {
  // Read the line from the connection

  char *in_buffer = (char *)malloc( BUFFER_SIZE );
  int bytes_read;

  bytes_read = recv( server_socket, in_buffer, BUFFER_SIZE, 0 );

  if ( strncmp( in_buffer, "GET", 3 ) == -1) {
    send_failure_message( server_socket );
  } else if (bytes_read == -1) {
    send_failure_message( server_socket );
  } else {
    send_success_message( server_socket );
  }
  free(in_buffer);
  return;
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
      _exit_with_error( "Error: Could not accept", 3 );
    }
    process_http_request( incoming_socket );
    close( incoming_socket );
  }
  free(res);
  return 0;
}
