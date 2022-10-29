#ifndef PARTITIONMGRTWRP_H_INCLUDED
#define PARTITIONMGRTWRP_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statuschecker.h"

int partitionCopyTwrpRawDd(char *targetPartition);
int partitionCopyTwrpNetcat(char *targetPartition);

#endif