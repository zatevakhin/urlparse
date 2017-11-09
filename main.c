
#include "url.h"


int main ()
{
  url_t* url = url_parse("https://admin:1234@example.com/topic/?q=42");

  if (url == NULL) {
    return EXIT_FAILURE;
  }

  printf("url:      %s\n", url->url);
  printf("scheme:   %s\n", url->scheme);
  printf("host:     %s\n", url->host);
  printf("port:     %s\n", url->port);
  printf("ip:       %s\n", url->ip);
  printf("path:     %s\n", url->path);
  printf("query:    %s\n", url->query);
  printf("login:    %s\n", url->login);
  printf("password: %s\n", url->password);

  return EXIT_SUCCESS;
}
