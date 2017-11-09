
#include "url.h"



url_t* url_parse(const char* url)
{
  url_t* purl = (url_t*)malloc(sizeof(url_t));
  int auth_flag = 0;
  int dns_flag = 0;

  if (purl == NULL) {
    return NULL;
  }

  purl->url      = NULL;
  purl->scheme   = NULL;
  purl->host     = NULL;
  purl->port     = NULL;
  purl->ip       = NULL;
  purl->path     = NULL;
  purl->query    = NULL;
  purl->login    = NULL;
  purl->password = NULL;

  const char* urlstr = url;
  const char* tmp;

  tmp = strchr(urlstr, ':');

  if (tmp == NULL) {
    url_free(purl);
    return NULL;
  }

  for (int i = 0; i < (tmp - urlstr); ++i)
  {
    if (!url_check_scheme_char(urlstr[i])) {
      url_free(purl);
      return NULL;
    }
  }

  purl->scheme = strndup(urlstr, tmp - urlstr);

  if (purl->scheme == NULL) {
    url_free(purl);
    return NULL;
  }

  for (int i = 0; i < (tmp - urlstr); ++i)
  {
    purl->scheme[i] = (char)tolower(purl->scheme[i]);
  }

  // skip ':'
  urlstr = (++tmp);

  // check if next two char is '/' and skip it
  for (int i = 0; i < 2; ++i, urlstr++)
  {
    if (*urlstr != '/') {
      url_free(purl);
      return NULL;
    }
  }

  // check if url have (user, password) data
  for (tmp = urlstr; *tmp != '\0' && *tmp != '/'; ++tmp)
  {
    if (*tmp == '@') {
      auth_flag = 1;
      break;
    }
  }


  if (auth_flag) {

    for (tmp = urlstr; *tmp != '\0' && *tmp != ':' && *tmp != '@'; ++tmp)
      ;

    purl->login = strndup(urlstr, tmp - urlstr);

    if (purl->login == NULL) {
      url_free(purl);
      return NULL;
    }

    urlstr = tmp;
    if (*urlstr == ':') {
      urlstr++;

      for (tmp = urlstr; *tmp != '\0' && *tmp != '@'; ++tmp)
        ;

      purl->password = strndup(urlstr, tmp - urlstr);

      if (purl->password == NULL) {
        url_free(purl);
        return NULL;
      }
      urlstr = tmp;
    }

    if (*urlstr != '@') {
      url_free(purl);
      return NULL;
    }

    urlstr++;
  }

  for (tmp = urlstr; *tmp != '\0' && *tmp != ':' && *tmp != '/'; ++tmp)
    ;

  purl->host = strndup(urlstr, tmp - urlstr);

  if (purl->host == NULL) {
    url_free(purl);
    return NULL;
  }

  for (int i = 0; i < (tmp - urlstr); ++i)
  {
    if (!(isdigit(urlstr[i]) || urlstr[i] == '.')) {
      dns_flag = 1;
      break;
    }
  }

  urlstr = tmp;

  if (*urlstr == ':') {
    urlstr++;

    for (tmp = urlstr; *tmp != '\0' && *tmp != '/'; ++tmp)
      ;

    purl->port = strndup(urlstr, tmp - urlstr);

    if (purl->port == NULL) {
      url_free(purl);
      return NULL;
    }

  }
  else {
    purl->port = (char*)calloc(3, sizeof(char));
    strncpy(purl->port, "80", 2);
  }

  if (dns_flag) {
    purl->ip = url_resolve_dns(purl->host);
  }

  purl->url = (char*)url;

  if (*urlstr == '\0') {
    return purl;
  }

  if (*urlstr != '/') {
    url_free(purl);
    return NULL;
  }

  urlstr++;

  for (tmp = urlstr; *tmp != '\0' && *tmp != '?'; ++tmp)
    ;

  purl->path = strndup(urlstr, tmp - urlstr);

  if (purl->path == NULL) {
    url_free(purl);
    return NULL;
  }

  urlstr = tmp;

  if (*urlstr == '?') {
    urlstr++;

    for (tmp = urlstr; *tmp != '\0'; ++tmp)
      ;

    purl->query = strndup(urlstr, tmp - urlstr);

    if (purl->query == NULL) {
      url_free(purl);
      return NULL;
    }
  }

  return purl;
}



inline int url_check_scheme_char(int c)
{
  return isalpha(c);
}



char* url_resolve_dns(char* host)
{
  struct hostent *h;

  if ((h = gethostbyname(host)) == NULL) {
    return NULL;
  }

  return inet_ntoa(*((struct in_addr*)h->h_addr));
}



void url_free(url_t* purl)
{
  if (purl != NULL) {
    if (purl->url      != NULL) free(purl->url);
    if (purl->scheme   != NULL) free(purl->scheme);
    if (purl->host     != NULL) free(purl->host);
    if (purl->port     != NULL) free(purl->port);
    if (purl->ip       != NULL) free(purl->ip);
    if (purl->path     != NULL) free(purl->path);
    if (purl->query    != NULL) free(purl->query);
    if (purl->login    != NULL) free(purl->login);
    if (purl->password != NULL) free(purl->password);
    free(purl);
  }
}
