#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

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

  struct hostent *hostname;
  hostname = getaddrinfo(host, 80,const struct addrinfo* __req,struct addrinfo** __pai)
  return 0;
}
