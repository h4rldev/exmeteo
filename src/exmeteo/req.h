#ifndef REQ_H_INCLUDED
#define REQ_H_INCLUDED

#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <curl/easy.h>
#include <stdio.h>


char *req(const char *url);

#endif // !REQ_H_INCLUDED 
