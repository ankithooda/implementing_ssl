#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>


#define BUFFER_SIZE 256



/* Print All IP Addresses for a resolved address
 */

void print_all_addr(struct addrinfo *resolved_host) {
  // Trying all resolved addresses
  char ipstr[INET6_ADDRSTRLEN];


  for (struct addrinfo *cur_addr = resolved_host; cur_addr != NULL; cur_addr = cur_addr->ai_next) {
    void *addr;
    char *ipver;

    if (cur_addr->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)cur_addr->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else { // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)cur_addr->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(cur_addr->ai_family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
  }
}

/*
  Parses HTTP urls of the form - http://www.example.com/example.txt

  *host points to www.example.com
  *path points to example.txt

  ignores http for now.
 */

int parse_url( char *uri, char **host, char **path )
{
  char *pos;

  pos = strstr( uri, "//" );

  if (!pos)
  {
    return -1;
  }

  *host = pos + 2;

  pos = strchr( *host, '/' );

  if (!pos)
  {
    *path = NULL;
  }
  else
  {
    *path = pos + 1;
    *pos = '\0';
  }

  return 0;
}

int http_get(int sockfd, char *host, char *path) {
  char message_buffer[BUFFER_SIZE];

  snprintf(message_buffer, BUFFER_SIZE, "GET /%s HTTP 1.1\r\n", path);
  if (send(sockfd, message_buffer, strlen(message_buffer), 0) == -1) {
    return -1;
  }

  snprintf(message_buffer, BUFFER_SIZE, "Host: %s\r\n", host);
  if (send(sockfd, message_buffer, strlen(message_buffer), 0) == -1) {
    return -1;
  }

  snprintf(message_buffer, BUFFER_SIZE, "Connection: Close\r\n\r\n");
  if (send(sockfd, message_buffer, strlen(message_buffer), 0) == -1) {
    return -1;
  }

  return 0;
}

int display_response(int sockfd) {
  // Extra byte for storing \0 so that printf works.
  // Other printf will run outside the buffer
  char response_buffer[BUFFER_SIZE + 1];

  ssize_t received = 0;

  while ((received = recv(sockfd, response_buffer, BUFFER_SIZE, 0)) > 0) {
    response_buffer[received] = '\0';
    fprintf(stdout, "%s", response_buffer);
  }
  fprintf(stdout, "\n");
}

int main(int argc, char **argv) {
  const char *service = "http";
  if (argc < 2) {
    fprintf(stderr, "Usage : http_client <url>\n");
    exit(1);
  }

  char *host, *path;
  host = NULL;
  path = NULL;
  if (parse_url(argv[1], &host, &path) != -1) {
    printf("Parsed host - %s\n", host);
    printf("Parsed path - %s\n", path);
  } else {
    fprintf(stderr, "Error : Malformed URL\n");
    exit(2);
  }

  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags    = AI_PASSIVE;

  struct addrinfo *resolved_host;
  int resolve_code = getaddrinfo(host, service, &hints, &resolved_host);

  if (resolve_code != 0) {
    fprintf(stderr, "Error : Could not resolved hostname - %s\n", gai_strerror(resolve_code));
    exit(3);
  }

  print_all_addr(resolved_host);

  int sockfd = 0;
  bool connected = false;

  for (struct addrinfo *cur_addr = resolved_host; cur_addr != NULL && !connected; cur_addr = cur_addr->ai_next) {

    sockfd = socket(cur_addr->ai_family, cur_addr->ai_socktype, cur_addr->ai_protocol);

    if (sockfd == -1) {
      fprintf(stderr, "Warning : Could not create socket - %s\n", strerror(errno));
      continue;
    }

    if (connect(sockfd, cur_addr->ai_addr, cur_addr->ai_addrlen) == -1) {
      fprintf(stderr, "Warning : Could not connect to the host - %s\n", strerror(errno));
      // Before continuing to the next address.
      // Close the socket.
      close(sockfd);
      continue;
    }
    // Flag to store whether we have successfully connected
    // to atleast one address.
    connected = true;
  }

  if ( connected == false) {
    fprintf(stderr, "Error : Could not connect to the host\n");
    exit(4);
  }

  if (http_get(sockfd, host, path) == -1) {
    fprintf(stderr, "Error : Could send GET HTTP message\n");
    exit(5);
  }

  display_response(sockfd);

  // Free addrinfo struct
  freeaddrinfo(resolved_host);
  close(sockfd);

  return 0;
}
