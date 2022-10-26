#include "partitioncopy.h"

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
    strncat(command, targetPartition, strlen(targetPartition) - 1);
    strcat(command, " | busybox nc -l -p 8888\"' && ");

    //close android forward and kill netcat
    strcat(command, "adb forward --remove-all && ");
    strcat(command, "pgrep -x nc | xargs kill -SIGINT");

    printf("\n%s\n",command);
    system(command);    
    return 0;
}