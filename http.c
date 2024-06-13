#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
//#include <netinet/in.h>


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
  freeaddrinfo(resolved_host);
  return 0;
}
