#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "statuschecker.h"
#include "partitionmgrroot.h"
#include "partitionmgrtwrp.h"
#include "recoverdata.h"

int main(int argc, char** argv){
    int menu;
    char ch;
    char *partitionName;
    char targetPartition[17];

    while(true){
        printf("\e[1;1H\e[2J");
        printf("NuxDroidRecovery Menu\n");
        printf(" 1. Connect and check device (TWRP)\n");
        printf(" 2. Clone the device partition (MODE1) (TWRP)\n");
        printf(" 3. Clone the device partition (MODE2) (TWRP)\n");
        printf(" 4. Connect and check device (ROOT)\n");
        printf(" 5. Clone the device partition (ROOT) (manual)\n");
        printf(" 6. Extract userdata from disk image\n");
        printf(" 7. Perform data recovery from userdata\n");
        printf(" 0. Exit\n");

        printf("Your Choice : ");
        scanf("%d", &menu); getc(stdin);
        switch(menu){
            case 1:
            //check TWRP ADB
                printf("\e[1;1H\e[2J");
                printf("A. Check ADB via TWRP custom recovery\n");
                printf(" - Please boot into TWRP custom recovery\n");
                printf(" - this mode require TWRP installed on your device\n");
                printf(" - WARNING! installing TWRP is going to void your device\n");
                printf(" - warranty. Failure while installing TWRP may render your\n");
                printf(" - device bricked or unusable.\n");
                printf("\n");
                if(!isDevicePaired("recovery")){
                    printf("Rebooting device to recovery\n");
                    system("adb reboot recovery");
                    printf("press enter after device is ready\n");
                    scanf("%c", &ch);
                    if(!isDevicePaired("recovery")){
                        printf("Could not boot into recovery, please install TWRP\n");
                        printf("Press enter to continue");
                        scanf("%c", &ch);
                        break;
                    }
                }
                printf("ADB TWRP is enabled, press enter to continue.\n");
                scanf("%c", &ch);
            //check partition
                printf("\e[1;1H\e[2J");
                printf("B. Reading partition table\n");
                if(availableBlockDevice(1) < 4){
                    printf("\n");
                    printf("Number of block device visible does not seem to be valid!");
                    printf("Press enter to continue");
                    scanf("%c", &ch);
                    break;
                }
                printPartitionList();
                partitionName = partitionSelector("recovery");
                printf("Largest partition is : %s\n", partitionName);
                printf(" - please unplug any expansion storage\n");
                printf(" - in your device to ensure accurate reading.\n");
                printf("\n");
                printf("All set, you can continue to the next step\n");
                free(partitionName);
                scanf("%c", &ch);
                break;
            case 2:
                partitionName = partitionSelector("recovery");
                printf("\e[1;1H\e[2J");
                if(strcmp(partitionName, "NULL") != 0){
                    printf("\n");
                    printPartitionList();
                    printf("Largest partition is : %s\n", partitionName);
                    printf("Enter which partition to clone using TWRP MODE1\n");
                    printf("Your Choice : ");
                    scanf("%17s", targetPartition); getc(stdin);
                    partitionCopyTwrpRawDd(targetPartition);
                    scanf("%c", &ch);
                }else{
                    printf("\n");
                    printf("Could not read partition table, connect and check device!\n");
                    printf("Press enter to continue");
                    scanf("%c", &ch);
                }
                free(partitionName);
                break;
            case 3:
                partitionName = partitionSelector("recovery");
                printf("\e[1;1H\e[2J");
                if(strcmp(partitionName, "NULL") != 0){
                    printf("\n");
                    printPartitionList();
                    printf("Largest partition is : %s\n", partitionName);
                    printf("Enter which partition to clone using TWRP MODE2\n");
                    printf("Your Choice : ");
                    scanf("%17s", targetPartition); getc(stdin);
                    partitionCopyTwrpNetcat(targetPartition);
                    scanf("%c", &ch);
                }else{
                    printf("\n");
                    printf("Could not read partition table, connect and check device!\n");
                    printf("Press enter to continue");
                    scanf("%c", &ch);
                }
                free(partitionName);
                break;
            case 4:
            //check ABB
                printf("\e[1;1H\e[2J");
                printf("A. Check ADB USB Debugging\n");
                printf(" - Please enable 'Developer options'\n");
                printf(" - and turn on 'USB debugging',\n");
                printf(" - Then plug your device, press the checkbox\n");
                printf(" - and press 'OK' to 'Allow USB debugging' pop-up.\n");
                printf("\n");
                if(!isDevicePaired("device")){
                    printf("Please enable USB Debugging and try again!\n");
                    printf("Press enter to continue");
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
                    printf("Press enter to continue");
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
                    printf("Press enter to continue");
                    scanf("%c", &ch);
                    break;
                }
                printf("BusyBox is installed, press enter to continue.\n");
                scanf("%c", &ch);
            //check Partition
                printf("\e[1;1H\e[2J");
                printf("D. Reading partition table\n");
                if(availableBlockDevice(0) < 4){
                    printf("\n");
                    printf("Number of block device visible does not seem to be valid!\n");
                    printf("Press enter to continue");
                    scanf("%c", &ch);
                    break;
                }
                printPartitionList();
                partitionName = partitionSelector("device");
                printf("Largest partition is : %s\n", partitionName);
                printf(" - please unplug any expansion storage\n");
                printf(" - in your device to ensure accurate reading.\n");
                printf("\n");
                printf("All set, you can continue to the next step\n");
                free(partitionName);
                scanf("%c", &ch);
                break;
            case 5:
                partitionName = partitionSelector("device");
                printf("\e[1;1H\e[2J");
                if(strcmp(partitionName, "NULL") != 0){
                    printf("\n");
                    printPartitionList();
                    printf("Largest partition is : %s\n", partitionName);
                    printf("Enter which partition to clone using Root Manual\n");
                    printf("Your Choice : ");
                    scanf("%17s", targetPartition); getc(stdin);
                    partitionCopyManual(targetPartition);
                    scanf("%c", &ch);
                }else{
                    printf("\n");
                    printf("Could not read partition table, connect and check device!\n");
                    printf("Press enter to continue");
                    scanf("%c", &ch);
                }
                free(partitionName);
                break;
            case 6:
                printf("\e[1;1H\e[2J"); printf("\n");
                partitionExtractor();
                scanf("%c", &ch);
                break;
            case 7:
                printf("\e[1;1H\e[2J"); printf("\n");
                recoverData();
                scanf("%c", &ch);
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