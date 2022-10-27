#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "statuschecker.h"
#include "partitionmanager.h"
#include "partitioncopy.h"

int main(int argc, char** argv){

    printf("%s\n", isDevicePaired()?"Connected":"Disconnected!");
    
    printf("%s\n", IsDeviceRooted()?"Root OK":"No root!");

    char *value = partitionSelector();
    printf("largest partition is %s\n", value);
    
    if(strcmp(value, "NULL") != 0){
        partitionCopy(value);
    }

    partitionExtractor();

    free(value);
    return 0;
}