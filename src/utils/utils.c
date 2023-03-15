#include <sys/stat.h>
#include <linux/stat.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

int strEndsWith(char *str, char *suffix){
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

int strStartsWith(char *str, char *prefix){
    if (!str || !prefix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lenprefix = strlen(prefix);
    if (lenprefix >  lenstr)
        return 0;
    return strncmp(str, prefix, lenprefix) == 0;
}

char *strReplace(char* str, char* old, char* new){
    char *ret;
    int i, count = 0;
    int newlen = strlen(new);
    int oldlen = strlen(old);
    for (i = 0; str[i] != '\0'; i++) {
        if (strstr(&str[i], old) == &str[i]) {
            count++;
            i += oldlen - 1;
        }
    }
    ret = (char *)malloc(i + count * (newlen - oldlen) + 1);
    i = 0;
    while (*str) {
        if (strstr(str, old) == str) {
            strcpy(&ret[i], new);
            i += newlen;
            str += oldlen;
        }
        else
            ret[i++] = *str++;
    }
    ret[i] = '\0';
    return ret;
}

int isFile(char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
