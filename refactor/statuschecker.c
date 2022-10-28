#include "statuschecker.h"

// printf("%s\n", isDevicePaired()?"Connected":"Disconnected!");
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
		return 0;
	}

	while(fgets(text, sizeof(text), buffer) != NULL){
        token = strtok(text, "\t");
        while(token != NULL) {
            // printf("%s\n", token);
            if(strstr(token, checkstring1) == NULL && 
                strstr(token, checkstring2) != NULL){
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