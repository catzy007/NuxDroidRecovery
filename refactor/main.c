#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "statuschecker.h"
#include "partitionmanager.h"

int main(int argc, char** argv){
    int menu;
    char ch;

    while(true){
        printf("\e[1;1H\e[2J");
        printf("NuxDroidRecovery Menu\n");
        printf(" 1. Connect and check device\n");
        printf(" 5. Exit\n");

        scanf("%d", &menu); getc(stdin);
        switch(menu){
            case 1:
                printf("\e[1;1H\e[2J");
                printf("A. Check ADB USB Debugging\n");
                printf(" - Please enable 'Developer options'\n");
                printf(" - and turn on 'USB debugging',\n");
                printf(" - Then plug your device, press the checkbox\n");
                printf(" - and press 'OK' to 'Allow USB debugging' pop-up.\n");
                printf("\n");
                if(!isDevicePaired()){
                    printf("Please enable USB Debugging and try again!\n");
                    scanf("%c", &ch);
                    break;
                }
                printf("USB Debugging is enabled, press enter to continue.\n");
                scanf("%c", &ch);
                
                printf("\e[1;1H\e[2J");
                printf("B. Check root/superuser access\n");
                printf(" - root access is required in order to\n");
                printf(" - read device partition block.\n");
                printf("\n");
                printf(" - WARNING! rooting your devices may causes\n");
                printf(" - lost of warranty, render your device suspectible\n");
                printf(" - to malware or even brick your device!\n");
                printf("\n");
                if(!IsDeviceRooted()){
                    printf("Please enable root access and try again!\n");
                    scanf("%c", &ch);
                    break;
                }
                printf("Root access is granted, press enter to continue.\n");
                scanf("%c", &ch);
                break;
            case 5:
                goto exit;
            default:
				printf("Input Error!\n");
				break;
        }
    }
    
    // printf("%s\n", IsDeviceRooted()?"Root OK":"No root!");

    // char *value = partitionSelector();
    // printf("largest partition is %s\n", value);
    
    // if(strcmp(value, "NULL") != 0){
    //     partitionCopy(value);
    // }

    // partitionExtractor();

    // free(value);
    exit:
    return 0;
}