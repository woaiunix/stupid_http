#include "../headers/utils.h"

char* to_lower_case(char* s) {
  for(char *p=s; *p; p++) *p=tolower(*p);
  return s;
}
char* to_upper_case(char* s) {
  for(char *p=s; *p; p++) *p=toupper(*p);
  return s;
}

int find_pattern(char* str, char* pat) {
    int M = strlen(pat);
    int N = strlen(str);
    for (int i = 0; i <= N - M; i++){
        int j;
        for (j = 0; j < M; j++)
            if (str[i + j] != pat[j])
                break;
      if (j == M)
        return i;
   }
   return -1;
}

int count_pattern(char* str, char* pat) {
    int M = strlen(pat);
    int N = strlen(str);
    int c = 0;
    for (int i = 0; i <= N - M; i++){
        int j;
        for (j = 0; j < M; j++)
            if (str[i + j] != pat[j])
                break;
      if (j == M)
        c++;
   }
   return c;
}