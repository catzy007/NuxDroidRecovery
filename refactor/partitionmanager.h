#ifndef PARTITIONMANAGER_H_INCLUDED
#define PARTITIONMANAGER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statuschecker.h"

int partitionCopy(char *targetPartition);
char *partitionSelector();
int partitionExtractor();

#endif