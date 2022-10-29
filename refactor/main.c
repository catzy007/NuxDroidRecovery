#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "statuschecker.h"
#include "partitionmgrroot.h"

int main(int argc, char** argv){
    int menu;
    char ch;
    char *partitionName;
    char targetPartition[17];

    while(true){
        printf("\e[1;1H\e[2J");
        printf("NuxDroidRecovery Menu\n");
        printf(" 1. Connect and check device (ROOT)\n");
        printf(" 2. Clone the device partition (ROOT) (manual)\n");
        printf(" 0. Exit\n");

        scanf("%d", &menu); getc(stdin);
        switch(menu){
            case 1:
            //check ABB
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
            //check ROOT
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
            //check BusyBox
                printf("\e[1;1H\e[2J");
                printf("C. Check is BusyBox installed\n");
                printf(" - BusyBox is needed in order to output the partition\n");
                printf(" - to your receiving computer. You can install BusyBox\n");
                printf(" - via PlayStore or APK file.\n");
                printf("\n");
                if(!isBusyboxInstalled()){
                    printf("Please install BusyBox and try again!\n");
                    scanf("%c", &ch);
                    break;
                }
                printf("BusyBox is installed, press enter to continue.\n");
                scanf("%c", &ch);
            //check Partition
                printf("\e[1;1H\e[2J");
                printf("D. Reading partition table\n");
                if(availableBlockDevice() < 4){
                    printf("\n");
                    printf("Number of block device visible does not seem to be valid!");
                    scanf("%c", &ch);
                    break;
                }
                printPartitionList();
                partitionName = partitionSelector();
                printf("Largest partition is : %s\n", partitionName);
                printf(" - please unplug any expansion storage\n");
                printf(" - in your device to ensure accurate reading\n");
                printf(" - if Largest partition shown above is partition\n");
                printf(" - you want to recover, please press enter\n");
                printf(" - then select option 2 in the menu\n");
                printf("\n");
                printf("All set, you can continue to the next step\n");
                free(partitionName);
                scanf("%c", &ch);
                break;
            case 2:
                partitionName = partitionSelector();
                if(strcmp(partitionName, "NULL") != 0){
                    printf("\e[1;1H\e[2J");
                    printf("\n");
                    printPartitionList();
                    printf("Enter which partition to clone\n");
                    scanf("%17s", targetPartition); getc(stdin);
                    partitionCopyManual(targetPartition);
                    scanf("%c", &ch);
                }
                free(partitionName);
                break;
            case 0:
                goto exit;
            default:
				printf("Input Error!\n");
				break;
        }
    }
    exit:
    return 0;
}