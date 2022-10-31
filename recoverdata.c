#include "recoverdata.h"

//mode 0=userdata, 1=diskimage
int recoverData(int mode){
    char command[256] = "\0";

    strcpy(command, "xterm -e 'foremost -i ./deviceImage.");
    if(mode == 0){
        if(access("deviceImage.dd", F_OK) != 0){
            printf("Could not find userdata image to recover!\n");
            printf("Press enter to continue");
            return 0;
        }
        strcat(command, "dd");
    }else{
        if(access("deviceImage.img", F_OK) != 0){
            printf("Could not find raw disk image to recover!\n");
            printf("Press enter to continue");
            return 0;
        }
        strcat(command, "img");
    }
    strcat(command, " -o ./recovery-$(date +'%Y.%m.%d-%H.%M.%S')'");

    printf("Begin recovery using Foremost\n");
    // printf("%s\n", command);
    system(command);
    printf("Done! Press enter to continue\n");
    return 1;
}