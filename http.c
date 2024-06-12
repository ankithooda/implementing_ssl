#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

  if ( !pos )
  {
    return -1;
  }

  *host = pos + 2;

  pos = strchr( *host, '/' );

  if ( !pos )
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


int main() {
  char *url = (char *)malloc(256);
  url = "http://www.example.com/testing/a.txt";
  char *host, *path;
  parse_url(url, &host, &path);
  printf("Parsed host - %s\n", host);
  printf("Parsed path - %s\n", path);
  return 0;
}
