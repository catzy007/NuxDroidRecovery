#include "partitionmgrtwrp.h"

int partitionCopyTwrpRawDd(char *targetPartition){
    char command[64] = "\0";

    strcpy(command, "adb shell 'stty raw && dd if=/dev/block/");
    strcat(command, targetPartition);
    strcat(command, "' > deviceImage.img");

    // printf("%s\n", command);
    system(command);
    printf("Done! Press enter to continue\n");
    return 0;
}

int partitionCopyTwrpNetcat(char *targetPartition){
    char command[512] = "\0";

    //forward the android tcp to host tcp
    system("adb forward tcp:8175 tcp:8175");

    //run adb shell, dd the selected partition and pipe 
    //the output to netcat 8175
    strcpy(command, "xterm -e 'echo Sending Data && ");
    strcat(command, "adb shell \"dd if=/dev/block/");
    strcat(command, targetPartition);
    strcat(command, " | nc -l -p 8175\"' && ");

    //close android forward and kill netcat
    strcat(command, "pgrep -x nc | xargs kill -SIGINT && ");
    strcat(command, "adb forward --remove-all & ");

    //run netcat and save device partition to deviceImage.img
    strcat(command, "sleep 5 && xterm -e 'echo Receiving Data && ");
    strcat(command, "nc localhost 8175 > deviceImage.img'");

    // printf("%s\n", command);
    system(command);
    printf("Done! Press enter to continue\n");
    return 0;
}