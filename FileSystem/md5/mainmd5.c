#include "md5.h"
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: %s 'string'\n", argv[0]);
        return 1;
    }
 
    char *msg = argv[1];
    size_t len = strlen(msg);

    char * MD5String = (char *)malloc(sizeof(MD5String) * 32);
    md5(msg, len, MD5String);

    printf("\nEl MD5 es:\n");

    printf("%s", MD5String);
    printf("\n");

    free(MD5String);
    return 0;
}