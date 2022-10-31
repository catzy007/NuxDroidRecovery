#ifndef PARTITIONCOPYROOT_H_INCLUDED
#define PARTITIONCOPYROOT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statuschecker.h"

int partitionCopyAuto(char *targetPartition);
int partitionCopyManual(char *targetPartition);

#endif