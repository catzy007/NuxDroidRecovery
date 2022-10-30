#ifndef STATUSCHECKER_H_INCLUDED
#define STATUSCHECKER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isDevicePaired(char *adbmode);
int IsDeviceRooted();
int isBusyboxInstalled();
int availableBlockDevice(int mode);

#endif