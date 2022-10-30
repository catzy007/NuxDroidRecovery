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
    printf("Done! Press enter to continue\n");
    return 0;
}

char *partitionSelector(char *adbmode){
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

    if(!isDevicePaired(adbmode)){
        printf("The device is not paired!\n");
        return partitionName;
    }

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
    char choice;
    char text[255];
    char command[255];
    char *token, *ptr;

    char startSector[50] = "0\0";
    char endSector[50] = "0\0";
    char tempStartSector[50] = "0\0";
    char tempEndSector[50] = "0\0";
    unsigned long int largestSector = 0;
    unsigned long int tempLargestSector = 0;

    buffer = popen("sfdisk -q -l -uS deviceImage.img", "r");
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

    if(largestSector == 0){
        return 0;
    }

    system("sfdisk -q -l -uS deviceImage.img");
    printf("Largest partition is sector %ld, start %s, end %s.\n", largestSector, startSector, endSector);
    printf("Do you want to extract this partition or specify your own (Y/N)? ");
    scanf("%c", &choice); getc(stdin);

    if(choice == 'n' || choice == 'N'){
        printf("\nSpecify Start sector : ");
        scanf("%s", startSector); getc(stdin);
        printf("Specify End sector : ");
        scanf("%s", endSector); getc(stdin);
    }

    strcpy(command, "\ndd if=deviceImage.img of=deviceImage.dd skip=");
    strcat(command, startSector);
	strcat(command, " count=");
	strcat(command, endSector);
    printf("%s\n", command);
    system(command);
    printf("Done!, press enter to continue!\n");

    return 1;
}