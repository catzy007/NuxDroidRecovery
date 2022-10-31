#ifndef PARTITIONMGRROOT_H_INCLUDED
#define PARTITIONMGRROOT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statuschecker.h"

void printPartitionList();
int partitionCopyAuto(char *targetPartition);
int partitionCopyManual(char *targetPartition);
char *partitionSelector(char *adbmode);
int partitionExtractor();

#endif