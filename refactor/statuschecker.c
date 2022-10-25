#include "statuschecker.h"

int isDevicePaired(){
    const char checkstring1[] = "List";
    const char checkstring2[] = "device";
    FILE *buffer;
    char text[255];
    int status = 0;
    char *token;

    //adb devices
    buffer = popen("adb devices", "r");
    if(buffer == NULL){
		return -1;
	}

	while(fgets(text, sizeof(text), buffer) != NULL){
        token = strtok(text, "\t");
        while(token != NULL) {
            // printf("%s\n", token);
            if(strstr(token, checkstring1) == NULL && 
                strstr(token, checkstring2) != NULL){
                printf("Found : %s\n", token);
                status = 1;
            }
            token = strtok(NULL, "\t");
        }
	}
	pclose(buffer);
    return status;
}

int IsDeviceRooted(){
	const char checkstring[] = "google";
    FILE *buffer;
    char text[255];
    int status = 0;
    char *token;

	//adb shell "su -c ls /data/app"
	buffer = popen("adb shell 'su -c ls /data/app'", "r");
	if(buffer == NULL){
		return -1;
	}

	while(fgets(text, sizeof(text), buffer) != NULL){
        token = strtok(text, " ");
        while(token != NULL) {
            // printf("%s\n", token);
            if(strstr(token, checkstring) != NULL){
                printf("Found : %s\n", token);
                status = 1;
            }
            token = strtok(NULL, " ");
        }
	}
	pclose(buffer);
    return status;
}