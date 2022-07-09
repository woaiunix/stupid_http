#include "../headers/mime.h"

char* get_mime_type(char* filename) {
    char* ext = strrchr(filename, '.');

    if(ext == NULL)
        return "application/octet-stream";

    if(!strcmp(".html", ext) || !strcmp(".htm", ext))
        return "text/html";

    if(!strcmp(".xhtml", ext))
        return "application/xhtml+xml";

    if(!strcmp(".xml", ext))
        return "application/xml";

    if(!strcmp(".css", ext))
        return "text/css";

    if(!strcmp(".js", ext) || !strcmp(".mjs", ext))
        return "text/javascript";

    if(!strcmp(".json", ext))
        return "application/json";

    if(!strcmp(".jsonlod", ext))
        return "application/ld+json";
    
    if(!strcmp(".txt", ext))
        return "text/plain";

    if(!strcmp(".pdf", ext))
        return "application/pdf";

    if(!strcmp(".jpeg", ext) || !strcmp(".jpg", ext))
        return "image/jpeg";

    if(!strcmp(".png", ext))
        return "image/png";

    if(!strcmp(".gif", ext))
        return "image/gif";

    if(!strcmp(".webp", ext))
        return "image/webp";

    if(!strcmp(".svg", ext))
        return "image/svg+xml";

    if(!strcmp(".ico", ext))
        return "image/vnd.microsoft.icon";

    if(!strcmp(".tiff", ext))
        return "image/tiff";

    if(!strcmp(".bmp", ext))
        return "image/bmp";

    if(!strcmp(".avif", ext))
        return "image/avif";

    if(!strcmp(".swf", ext))
        return "application/x-shockwave-flash";

    if(!strcmp(".mp3", ext))
        return "audio/mp3";

    if(!strcmp(".mp4", ext))
        return "video/mp4";

    if(!strcmp(".mpeg", ext))
        return "video/mpeg";

    if(!strcmp(".wav", ext))
        return "audio/wav";

    if(!strcmp(".avi", ext))
        return "video/x-msvideo";

    if(!strcmp(".weba", ext))
        return "audio/weba";
    
    if(!strcmp(".webm", ext))
        return "video/webm";
    
    if(!strcmp(".oga", ext))
        return "audio/ogg";

    if(!strcmp(".ogv", ext))
        return "video/ogg";

    if(!strcmp(".ogx", ext) || !strcmp(".ogg", ext))
        return "application/ogg";

    if(!strcmp(".opus", ext))
        return "audio/opus";

    if(!strcmp(".ttf", ext))
        return "font/ttf";

    if(!strcmp(".otf", ext))
        return "font/otf";

    if(!strcmp(".woff", ext))
        return "font/woff";

    if(!strcmp(".woff2", ext))
        return "font/woff2";
    
    return "application/octet-stream";
}