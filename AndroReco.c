#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(){
	int menu;
	char partition[30];
	char command[255];
	while(1){
		printf("Select options :\n1.Pair\n2.Show Partitions\n3.Copy Partitions\n");
		scanf("%d",&menu);
		switch(menu){
			case 1:		
				printf("pairing the devices\n");
				system("sleep 0.2");
				system("adb devices");
				system("adb forward tcp:8888 tcp:8888");
				break;
			case 2:
				getc(stdin);
				system("adb shell \"cat /proc/partitions\"");
				printf("Select the partition you want to copy : ");
				fgets(partition,sizeof(partition),stdin);
				printf("copying the partitions %s",partition);
				strcpy(command,"adb shell \"su -c dd if=/dev/block/");
				strcat(command,partition);
				strcat(command," | busybox nc -l -p 8888\"");
				printf("%s",command);
				/*
				system("sleep 0.2");
				system("adb shell \"su -c dd if=/dev/block/mmcblk0p6 | busybox nc -l -p 8888\"");
				system("nc 127.0.0.1 8888 > ~/mmcblk0p6");
				system("pgrep -x nc | xargs kill -SIGINT");
				printf("done!");
				*/
				break;
			default:
				printf("Input Error!\n");
				break;
		}
	}
}
