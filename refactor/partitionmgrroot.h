#ifndef PARTITIONMGRROOT_H_INCLUDED
#define PARTITIONMGRROOT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statuschecker.h"

void printPartitionList();
int partitionCopyManual(char *targetPartition);
int partitionCopyTwrpDd(char *targetPartition);
char *partitionSelector();
int partitionExtractor();

#endif