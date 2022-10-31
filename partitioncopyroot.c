#include "partitioncopyroot.h"

int partitionCopyAuto(char *targetPartition){
    char command[512] = "\0";

    //forward the android tcp to host tcp
    system("adb forward tcp:8175 tcp:8175");

    //run adb shell, dd the selected partition and pipe 
    //the output to netcat 8175
    strcpy(command, "xterm -e 'echo Sending Data && ");
    strcat(command, "adb shell su -c \"\"dd if=/dev/block/");
    strcat(command, targetPartition);
    strcat(command, " | nc -l -p 8175\"\"' && ");

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