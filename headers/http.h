#ifndef __HTTP_H__
#define __HTTP_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "utils.h"
#include "mime.h"

#define BUFF_SIZE 10240
#define ERR_BUFF_SIZE 256
#define HEAD_BUFF_SIZE 256
#define MAX_REQUEST_TIME 60
#define SERVER_NAME "Stupid HTTP"

#define HTTP_RES_OK 200
#define HTTP_RES_BAD_REQUEST 400
#define HTTP_RES_FORBIDDEN 403
#define HTTP_RES_NOT_FOUND 404
#define HTTP_RES_METHOD_NOT_ALLOWED 405

void* request_handler(void* _connfd);

#endif