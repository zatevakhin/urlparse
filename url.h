
#ifndef URL_URL_H
#define URL_URL_H

#include <ctype.h>
#include <stdio.h>
#include <dnet.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct {
  char* url;
  char* scheme;
  char* host;
  char* ip;
  char* port;
  char* path;
  char* query;
  char* login;
  char* password;
} url_t;



url_t* url_parse(const char* url);

int url_check_scheme_char(int c);
char* url_resolve_dns(char* host);

void url_free(url_t* url_struct);


#endif //URL_URL_H
