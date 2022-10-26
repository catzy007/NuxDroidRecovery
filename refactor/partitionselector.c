#include "partitionselector.h"

char *partitionSelector(){
    FILE *buffer;
    char *ptr;
    char *token;
    char text[255];
    int lineNum = 0, lineSkip = 2;

    unsigned long int partitionBlocks = 0;
    unsigned long int tempPartitionBlocks = 0;
    char *partitionName = malloc(sizeof(char) * 16+1);
    strcpy(partitionName, "NULL\0");

    if(!isDevicePaired()){
        printf("The device is not paired!");
        return partitionName;
    }

    // system("rm *.img >/dev/null");

    //adb shell 'cat /proc/partitions' | awk '{print $4}' 
    buffer = popen("sh -c 'cat /proc/partitions'", "r");
    // buffer = popen("adb shell 'cat /proc/partitions'", "r");
    if(buffer == NULL){
        return partitionName;
    }

    while(fgets(text, sizeof(text), buffer) != NULL){
        if(lineNum >= lineSkip){
            token = strtok(text, " ");
            int i=0;
            while(token != NULL || i<3){
                if(i == 2){
                    //partition blocks 
                    tempPartitionBlocks = strtoul(token, &ptr, 10);
                    if(tempPartitionBlocks > partitionBlocks){
                        partitionBlocks = tempPartitionBlocks;
                        //partition name
                        token = strtok(NULL, " ");
                        strcpy(partitionName, token);
                        // printf("%ld - %s\n", partitionBlocks, token);
                    }
                }
                token = strtok(NULL, " ");
                i++;
            }
        }
        lineNum++;
    }
    pclose(buffer);

    //clean the partition name from newline character
    for(int j=0; j<(16+1); j++){
        if(partitionName[j] == '\n'){
            partitionName[j] = '\0';
        }
    }

    return partitionName;
}