#include "partitionmgrroot.h"

void printPartitionList(){
    system("adb shell \"cat /proc/partitions\"");
}

int partitionCopyManual(char *targetPartition){
    char command1[256] = "\0";
    char command2[256] = "\0";

    //forward the android tcp to host tcp
    strcpy(command1, "  adb forward tcp:8175 tcp:8175\n");

    //run adb shell, dd the selected partition and pipe 
    //the output to netcat 8175
    strcat(command1, "  adb shell su -c \"\"dd if=/dev/block/");
    strcat(command1, targetPartition);
    strcat(command1, " | nc -l -p 8175\"\" ");
    // strcat(command, "&& pgrep -x nc | xargs kill -SIGINT");

    //run netcat and save device partition to deviceImage.img
    strcat(command2, "  nc localhost 8175 > deviceImage.img");

    printf("\nOpen new terminal window and enter the command below\n");
    printf("%s\n\n", command1);
    printf("If you get 'nc: Address already in use', turn airplane mode\n");
    printf("on or off. Then repeat 'adb shell su' command\n");
    printf("\nThen open another terminal window and enter the command below\n");
    printf("%s\n\n", command2);

    printf("Check your working directory\n");
    printf("if 'deviceImage.img' file stop increase in size\n");
    printf("it is likely that clone process is complete\n\n");
    printf("Done!\n");
    return 0;
}

char *partitionSelector(){
    FILE *buffer;
    char *ptr;
    char *token;
    char text[255];
    int lineNum = 0, lineSkip = 2;

    unsigned long int partitionBlocks = 0;
    unsigned long int tempPartitionBlocks = 0;
    char *partitionName = malloc(sizeof(char) * 16+1);
    for(int j=0; j<(16+1); j++){
        partitionName[j] = '\0';
    }
    strcpy(partitionName, "NULL");

    if(!isDevicePaired()){
        printf("The device is not paired!");
        return partitionName;
    }

    // system("rm *.img >/dev/null");

    //adb shell 'cat /proc/partitions' | awk '{print $4}' 
    // buffer = popen("sh -c 'cat /proc/partitions'", "r");
    buffer = popen("adb shell 'cat /proc/partitions'", "r");
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
    for(int k=0; k<(16+1); k++){
        if(partitionName[k] == '\n'){
            partitionName[k] = '\0';
        }
    }

    return partitionName;
}

int partitionExtractor(){
    FILE *buffer;
    char text[255];
    char command[255];
    char *token, *ptr;

    char startSector[50] = "0\0";
    char endSector[50] = "0\0";
    char tempStartSector[50] = "0\0";
    char tempEndSector[50] = "0\0";
    unsigned long int largestSector = 0;
    unsigned long int tempLargestSector = 0;

    //sfdisk -q -l -uS *.img 
    buffer = popen("sfdisk -q -l -uS *.img", "r");
    // buffer = popen("sfdisk -q -l -uS", "r");
    if(buffer == NULL){
        return -1;
    }

    while(fgets(text, sizeof(text), buffer) != NULL){
        token = strtok(text, " ");
        int i = 0;
        while(token != NULL){
            if(i == 1){
                strcpy(tempStartSector, token);
                token = strtok(NULL, " ");
                strcpy(tempEndSector, token);
                token = strtok(NULL, " ");
                tempLargestSector = strtoul(token, &ptr, 10);
                if(tempLargestSector > largestSector){
                    largestSector = tempLargestSector;
                    strcpy(startSector, tempStartSector);
                    strcpy(endSector, tempEndSector);
                }
            }
            // printf("%s\n", token);
            token = strtok(NULL, " ");
            i++;
        }
    }
    pclose(buffer);

    printf("start %s, end %s, sector %ld\n", startSector, endSector, largestSector);
    if(largestSector == 0){
        return 0;
    }

    strcpy(command, "dd if=deviceImage.img of=deviceImage.dd skip=");
    strcat(command, startSector);
	strcat(command, " count=");
	strcat(command, endSector);
    printf("%s\n", command);
    // system(command);

    return 1;
}