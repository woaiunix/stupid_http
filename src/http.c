#include "../headers/http.h"

struct http_response_data {
    int res_code;
    char* content_type;
    long content_lenght;
    struct tm time;
};

struct http_request_data {
    char* method;
    char* url;
    int headers_c;
    char* headers_str;
};

void secure_file_path(char* unsec, char* sec) {
    bool first_slashes = true;
    while (*unsec != '\0') {
        if(*unsec != '/')
            first_slashes = false;
        else if(first_slashes) {
            unsec++;
            continue;
        }

        if(*unsec == '.' && *(unsec + 1) == '.') {
            unsec++;
            continue;
        }

        *sec++ = *unsec++;
    }
    *sec = '\0';
}

void decode_url(char* url, char* dest) {
    char buff[3];
    buff[2] = '\0';

    while(*url != '\0') {
        if(*url == '?' || *url == '#') 
            break;

        if(*url == '%') {
            url++;
            if(*url == '\0')
                break;
            buff[0] = *url++;
            if(*url == '\0')
                break;
            buff[1] = *url++;

            char decoded_char;
            sscanf(buff, "%hhX", &decoded_char);
            if(decoded_char == '\0')
                break;
            
            *dest++ = decoded_char;
        } else
            *dest++ = *url++;
    }
    *dest = '\0';
}

char* get_http_response_string(int code) {
    switch (code)
    {
        case HTTP_RES_OK:
            return "200 OK";

        case HTTP_RES_BAD_REQUEST:
            return "400 Bad Request";

        case HTTP_RES_FORBIDDEN:
            return "403 Forbidden";

        case HTTP_RES_NOT_FOUND:
            return "404 Not Found";

        case HTTP_RES_METHOD_NOT_ALLOWED:
            return "405 Method Not Allowed";

        default:
            return NULL;
    }
}

void generate_http_response_head(char* res_head, struct http_response_data res_data) {  
    char date[50];
    strftime(date, 50, "%a, %d %b %Y %H:%M:%S GMT", &res_data.time);

    sprintf(res_head, "HTTP/1.1 %s\r\n\
Server: " SERVER_NAME "\r\n\
Date: %s\r\n\
Content-Type: %s\r\n\
Content-Length: %ld\r\n\
Connection: close",
    get_http_response_string(res_data.res_code), date, res_data.content_type, res_data.content_lenght);
}

void generate_error_body(char* body, char* msg) {
    sprintf(body, "<!DOCTYPE html>\
<html>\
<head>\
<title>%s</title></head>\
<body>\
<center><h1>%s</h1></center>\
<hr>\
</body>\
</html>", 
    msg, msg);
}

char* get_header_value(char* name, char* headers_str) {
    if(headers_str == NULL)
        return "";

    char headers_str_copy[strlen(headers_str) + 1];
    strcpy(headers_str_copy, headers_str);

    char *header = strtok(headers_str_copy, "\r\n");

    while(header != NULL) {
        char header_lower[strlen(header) + 1];
        strcpy(header_lower, header);
        to_lower_case(header_lower);

        if(!strcmp(header_lower, name)) 
            break;

        int colon_location = find_pattern(header_lower, ":");
        if(colon_location < 0) 
            break;
            

        if(!strncmp(header_lower, name, colon_location)) {
            char* header_value_ptr = strchr(header, ' ');
            header_value_ptr++;

            if(!strcmp(header_value_ptr, ""))
                break;

            char* header_value = (char*)malloc(strlen(header_value_ptr) + 1);
            strcpy(header_value, header_value_ptr);
            return header_value;
        }
            
        header = strtok(NULL, "\r\n");
    }

    return "";
}

int get_http_request_data(struct http_request_data* request_data, char* head) {
    request_data->method = NULL;
    request_data->url = NULL;
    request_data->headers_str = NULL;

    char head_copy[strlen(head) + 1];
    strcpy(head_copy, head);

    char *line = strtok(head_copy, "\r\n");

    // method and url
    char request_line[strlen(line) + 1];
    strcpy(request_line, line);

    char* request_line_part = strtok(request_line, " ");

    if(request_line_part == NULL)
        return -1;

    //char method[strlen(request_line_part) + 1];
   
    request_data->method = (char*)malloc(strlen(request_line_part) + 1);
    strcpy(request_data->method, request_line_part);

    request_line_part = strtok(NULL, " ");

    if(request_line_part == NULL)
        return -1;  

    // char url[strlen(request_line_part) + 1];
    request_data->url = (char*)malloc(strlen(request_line_part) + 1);
    strcpy(request_data->url, request_line_part);

    int headers_c = count_pattern(head, "\n");
    request_data->headers_c = headers_c;
    
    if(headers_c > 0) {
        char* headers = strchr(head, '\n');
        headers++;
        request_data->headers_str = headers;
    } else
        request_data->headers_str = NULL;

    return 0;
}

void handle_error(int connfd, int res_code, struct http_request_data request_data) {
    if(res_code == HTTP_RES_BAD_REQUEST) 
        printf("Request: BAD REQUEST\n");

    char* body = (char*)malloc(ERR_BUFF_SIZE);
    generate_error_body(body, get_http_response_string(res_code));

    size_t body_size = strlen(body);

    time_t t = time(NULL);

    struct http_response_data res_data;
    res_data.res_code = res_code;
    res_data.content_type = "text/html";
    res_data.content_lenght = body_size;
    res_data.time = *gmtime(&t);

    char* head = (char*)malloc(HEAD_BUFF_SIZE);
    generate_http_response_head(head, res_data);

    send(connfd, head, strlen(head), 0);
    send(connfd, "\r\n\r\n", 4, 0);

    if(strcmp(request_data.method, "HEAD"))
        send(connfd, body, body_size, 0);

    free(body);
    free(head);

    printf("Response: %s\n", get_http_response_string(res_code));
    if(request_data.method != NULL)
        free(request_data.method);
    if(request_data.url != NULL)
        free(request_data.url);

    close(connfd);
}

void* request_handler(void* _connfd) {
    int connfd = *(int*)_connfd;

    char* buff = (char*)malloc(BUFF_SIZE);
    memset(buff, 0, BUFF_SIZE);

    //processing request
    struct http_request_data request_data;

    recv(connfd, buff, BUFF_SIZE, 0);

    int headEnd = find_pattern(buff, "\r\n\r\n");
    if(headEnd < 0)
        headEnd = find_pattern(buff, "\n\n");
    
    if(headEnd < 0) {
        handle_error(connfd, HTTP_RES_BAD_REQUEST, request_data);
        free(buff);
        return NULL;
    }

    buff[headEnd] = '\0';

    if(get_http_request_data(&request_data, buff) < 0) {
        handle_error(connfd, HTTP_RES_BAD_REQUEST, request_data);
        free(buff);
        return NULL;
    }

    char decoded_url[strlen(request_data.url) + 1];

    char* user_agent = get_header_value("user-agent", request_data.headers_str);
    printf("Request: %s %s \"%s\"\n", request_data.method, request_data.url, user_agent);
        
    decode_url(request_data.url, decoded_url);

    if(user_agent[0] != '\0')
        free(user_agent);

    //generating response
    
    if(strcmp("GET", request_data.method) && strcmp("HEAD", request_data.method)) {
        handle_error(connfd, HTTP_RES_METHOD_NOT_ALLOWED, request_data);
        return NULL;
    }

    memset(buff, 0, BUFF_SIZE);

    char* unsec_file_path = decoded_url;
    unsec_file_path++;

    char sec_file_path[strlen(unsec_file_path) + 1];
    secure_file_path(unsec_file_path, sec_file_path);

    char* file_path = *sec_file_path == '\0' ? "index.html" : sec_file_path;

    FILE* file = fopen(file_path, "rb");

    if(file == NULL) {
        if(errno == ENOENT) 
            handle_error(connfd, HTTP_RES_NOT_FOUND, request_data);
        else
            handle_error(connfd, HTTP_RES_FORBIDDEN, request_data);

        free(buff);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    if(file_size == LONG_MAX || file_size < 0) {
        handle_error(connfd, HTTP_RES_NOT_FOUND, request_data);
        free(buff);
        fclose(file);
        return NULL;
    }

    fseek(file, 0, SEEK_SET);

    time_t t = time(NULL);

    struct http_response_data res_data;
    res_data.res_code = HTTP_RES_OK;
    res_data.content_type = get_mime_type(file_path);
    res_data.content_lenght = file_size;
    res_data.time = *gmtime(&t);

    generate_http_response_head(buff, res_data);

    send(connfd, buff, strlen(buff), 0);
    send(connfd, "\r\n\r\n", 4, 0);

    if(strcmp(request_data.method, "HEAD")) {
        long file_position = file_size;

        do {
            memset(buff, 0, BUFF_SIZE);
            fread(buff, 1, BUFF_SIZE, file);
            send(connfd, buff, file_position >= BUFF_SIZE ? BUFF_SIZE : file_position, 0);
            file_position -= BUFF_SIZE;
        } while(file_position >= 0);
    }

    fclose(file);
    free(buff);

    free(request_data.method);
    free(request_data.url);

    printf("Response: %s\n", get_http_response_string(HTTP_RES_OK));
    
    //closing connection
    close(connfd);
   
    return NULL;
}