#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "statuschecker.h"
#include "partitionselector.h"

int main(int argc, char** argv){
    // printf("hello world!\n");
    printf("%s\n", isDevicePaired()?"Connected":"Disconnected!");
    printf("%s\n", IsDeviceRooted()?"Root OK":"No root!");

    char *value = partitionSelector();
    printf("%s\n", value);
    free(value);

    return 0;
}