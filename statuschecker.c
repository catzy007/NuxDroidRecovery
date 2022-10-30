#include "statuschecker.h"

// printf("%s\n", isDevicePaired("device")?"Connected":"Disconnected!");
int isDevicePaired(char *adbmode){
    const char checkstring1[] = "List";
    // const char checkstring2[] = "device";
    FILE *buffer;
    char text[255];
    int status = 0;
    char *token;

    //adb devices
    buffer = popen("adb devices", "r");
    if(buffer == NULL){
		return 0;
	}

	while(fgets(text, sizeof(text), buffer) != NULL){
        token = strtok(text, "\t");
        while(token != NULL) {
            // printf("%s\n", token);
            if(strstr(token, checkstring1) == NULL && 
                strstr(token, adbmode) != NULL){
                // printf("Found : %s\n", token);
                status = 1;
            }
            token = strtok(NULL, "\t");
        }
	}
	pclose(buffer);

    return status;
}

// printf("%s\n", IsDeviceRooted()?"Root OK":"No root!");
int IsDeviceRooted(){
	const char checkstring[] = "blocks";
    FILE *buffer;
    char text[255];
    int status = 0;
    char *token;

	//adb shell 'su -c cat /proc/partitions'
	buffer = popen("adb shell 'su -c cat /proc/partitions'", "r");
	if(buffer == NULL){
		return 0;
	}

	while(fgets(text, sizeof(text), buffer) != NULL){
        token = strtok(text, " ");
        while(token != NULL) {
            // printf("%s\n", token);
            if(strstr(token, checkstring) != NULL){
                // printf("Found : %s\n", token);
                status = 1;
            }
            token = strtok(NULL, " ");
        }
	}
	pclose(buffer);

    return status;
}

// printf("%s\n", isBusyboxInstalled()?"BusyBox OK":"No BusyBox!");
int isBusyboxInstalled(){
    const char checkstring[] = "function";
    FILE *buffer;
    char text[255];
    int status = 0;
    char *token;

    //adb shell 'su -c busybox'
    buffer = popen("adb shell 'su -c busybox'", "r");
    if(buffer == NULL){
        return 0;
    }

    while(fgets(text, sizeof(text), buffer) != NULL){
        token = strtok(text, " ");
        while(token != NULL) {
            if(strstr(token, checkstring) != NULL){
                status = 1;
            }
            token = strtok(NULL, " ");
        }
    }
    pclose(buffer);

    return status;
}

//mode 1=twrp, 0=root
int availableBlockDevice(int mode){
    FILE *buffer;
    char text[255];
    int block = 0;

    if(mode == 0){
        buffer = popen("adb shell 'su -c ls -l /dev/block'", "r");
    }else{
        buffer = popen("adb shell 'ls -l /dev/block'", "r");
    }

    if(buffer == NULL){
        return 0;
    }

    while(fgets(text, sizeof(text), buffer) != NULL){
        block++;
    }
    pclose(buffer);

    return block;
}