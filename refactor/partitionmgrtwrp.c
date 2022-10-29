#include "partitionmgrtwrp.h"

int partitionCopyTwrpDd(char *targetPartition){
    char command[512] = "\0";

    //forward the android tcp to host tcp
    system("adb forward tcp:8175 tcp:8175");

    //run adb shell, dd the selected partition and pipe 
    //the output to netcat 8175
    strcpy(command, "xterm -e 'echo Sending Data && ");
    strcat(command, "adb shell \"dd if=/dev/block/");
    // strcat(command, targetPartition);
    strcat(command, "mmcblk0");
    strcat(command, " | nc -l -p 8175\"' && ");

    //close android forward and kill netcat
    strcat(command, "pgrep -x nc | xargs kill -SIGINT && ");
    strcat(command, "adb forward --remove-all & ");

    //run netcat and save device partition to deviceImage.img
    strcat(command, "sleep 5 && xterm -e 'echo Receiving Data && ");
    strcat(command, "nc localhost 8175 > deviceImage.img'");

    // printf("%s\n", command);
    system(command);
    printf("Done!\n");
    return 0;
}