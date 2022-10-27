#include "partitionmanager.h"

int partitionCopy(char *targetPartition){
    char command[512];

    //forward the android tcp to host tcp
    strcpy(command, "adb forward tcp:8888 tcp:8888 && ");

    //run netcat and save device partition to deviceImage.img
    strcat(command, "xterm -e 'echo Receiving Data && sleep 5 && ");
    strcat(command, "nc 127.0.0.1 8888 > deviceImage.img' & ");

    //run adb shell, dd the selected partition and pipe 
    //the output to netcat 8888
    strcat(command, "xterm -e 'echo Sending Data && sleep 5 && ");
    strcat(command, "adb shell \"su -c dd if=/dev/block/");
    strcat(command, targetPartition);
    strcat(command, " | busybox nc -l -p 8888\"' && ");

    //close android forward and kill netcat
    strcat(command, "adb forward --remove-all && ");
    strcat(command, "pgrep -x nc | xargs kill -SIGINT");

    printf("\n%s\n",command);
    system(command);    
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
    for(int j=0; j<strlen(partitionName); j++){
        if(partitionName[j] == '\n'){
            partitionName[j] = '\0';
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