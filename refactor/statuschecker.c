#include "statuschecker.h"

int isDevicePaired(){
    const char checkstring1[] = "List";
    const char checkstring2[] = "device";
    FILE *buffer;
    char text[255];
    int status = 0;
    char *token;

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