#include "partitionselector.h"

char *partitionSelector(){
    FILE *buffer1;
    int lineNum = 0, lineSkip = 2;
    char *token;
    char text[255];

    // if(!isDevicePaired()){
    //     printf("The device is not paired!");
    //     return 0;
    // }

    // system("rm *.img >/dev/null");

    //adb shell 'cat /proc/partitions' | awk '{print $4}' 
    buffer1 = popen("sh -c 'cat /proc/partitions'", "r");
    if(buffer1 == NULL){
        return "Error!";
    }

    while(fgets(text, sizeof(text), buffer1) != NULL){
        if(lineNum >= lineSkip){
            token = strtok(text, " ");
            for(int i=0; i<4; i++){
                //partition blocks 
                if(i == 2){
                    printf("%s\n", token);
                }
                //partition name
                if(i == 3){
                    printf("%s\n", token);
                }
                token = strtok(NULL, " ");
            }
            // while(token != NULL){
            //     printf("%s\n", token);
            //     token = strtok(NULL, " ");
            // }
        }
        lineNum++;
    }
    pclose(buffer1);

    char *str = malloc(sizeof(char) * 6+1);
    strcpy(str, "hello\0");
    return str;
}