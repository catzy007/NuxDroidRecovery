#include "recoverdata.h"

int recoverData(){
    if(access("deviceImage.dd", F_OK) != 0){
        printf("Could not find disk image to recover!\n");
        printf("Press enter to continue");
        return 0;
    }
    printf("Begin recovery using Foremost\n");
    system("foremost -i ./deviceImage.dd -o ./recovery-$(date +'%Y.%m.%d-%H.%M.%S')");
    printf("Done! Press enter to continue\n");
    return 1;
}