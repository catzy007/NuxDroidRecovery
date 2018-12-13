#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int IsPaired(){
	FILE *buffer;
	char text[255];
	int signal;
	int loop;
	int i;
//plugged device status checker
	buffer = popen("adb devices", "r");
	if(buffer==NULL){
		return -1;
	}
	loop=1;
	while(fgets(text,sizeof(text),buffer)!= NULL){
		if(loop>1){
		i=0;
			//comparator algorithm begin
			while(text[i]!='\0'){
				if(text[i]=='d' && text[i+1]=='e' 
				&& text[i+2]=='v' && text[i+3]=='i' 
				&& text[i+4]=='c' && text[i+5]=='e'){
					signal=1;
				}
				i++;
			}
		}
		loop++;
		//printf("%.2d - %s",loop,text); //debug_line_can_be_removed
	}
	pclose(buffer);
	if(signal==1){
		return 1;
	}
	return 0;
}

int IsRooted(){
	FILE *buffer;
	char text[255];
	int signal;
	int loop;
	int i;
//plugged device status checker
	//adb shell "su -c ls /data/app" //debug_line_can_be_removed
	buffer = popen("adb shell \"su -c ls /data/app\"", "r");
	if(buffer==NULL){
		return -1;
	}
	loop=1;
	while(fgets(text,sizeof(text),buffer)!= NULL){
		if(loop>1){
		i=0;
			//comparator algorithm begin
			while(text[i]!='\0'){
				if(text[i]=='g' && text[i+1]=='o' 
				&& text[i+2]=='o' && text[i+3]=='g' 
				&& text[i+4]=='l' && text[i+5]=='e'){
					signal=1;
				}
				i++;
			}
		}
		loop++;
		//printf("%.2d - %s",loop,text); //debug_line_can_be_removed
	}
	pclose(buffer);
	if(signal==1){
		return 1;
	}
	return 0;
}

int ShowPartitions(){
	FILE *buffer;
	char *partitions;
	char *partsize;
	int loop1,loop2;
	char text[255];
	if(!IsPaired()){
		printf("The Device Aren't Ready!, Pair First\n");
	}else{
		//adb shell "cat /proc/partitions" | awk '{print $4}' //debug_line_can_be_removed
		buffer = popen("adb shell \"cat /proc/partitions\" | awk '{print $4}'","r");
		if(buffer==NULL){
			return -1;
		}
		loop1=1,loop2=1;
		while(fgets(text,sizeof(text),buffer)!= NULL){
			if(loop1>2){
				printf("%.2d %s",loop2,text);
				loop2++;
			}
			loop1++;
		}
		pclose(buffer);
	}
}

void CopyPartitions(char *partition, char *path){
	char command[255];
	int i=0;
	printf("copying the partitions %s", partition);
	
	//adb forward tcp:8888 tcp:8888 & xterm -e 'echo Sending Data ; adb shell "su -c dd if=/dev/block/loop0 | busybox nc -l -p 8888"' & xterm -e 'echo Receiving Data ; nc 127.0.0.1 8888 > loop0.img'
	//forward the android tcp to host tcp
	strcpy(command, "adb forward tcp:8888 tcp:8888 & ");
	//run xterm, and add message
	strcat(command, "xterm -e 'echo Sending Data \073 ");
	//run adb shell and dd
	strcat(command, "adb shell \"su -c dd if=/dev/block/");
	strcat(command, partition);
	//pipe the dd output to port 8888
	strcat(command, " | busybox nc -l -p 8888\"' & ");
	//run xterm and add message
	strcat(command, "xterm -e 'echo Receiving Data \073 ");
	//get the dd output
	strcat(command, "nc 127.0.0.1 8888 >");
	//save the output to .img file
	strcat(command, partition);
	strcat(command, ".img' & ");
	//run nc killer
	strcat(command, "./nckiller");
	
	//printf("\n%s\n",command); //debug_line_can_be_removed
	system(command);
	system("adb forward --remove-all");
}

int main(){
	int menu;
	FILE *buffer1;
	FILE *buffer2;
	char partition[30]="\0"	;
	char command[255];
	char text[255];
	int i=0;
	int loop;
	int mark1=0;
	int mark2=0;
	while(1){
		printf("1.Pair\n2.Show Partitions\n3.Copy Partitions\n4.exit\nSelect options : ");
		scanf("%d",&menu);
		switch(menu){
		//pair device
			case 1:		
				printf("pairing the devices\n");
				system("sleep 0.2");
				if(IsPaired()){
					//if device has been plugged in and root
					if(IsRooted()){
						printf("Device Are Ready To Use\n");
					}else{
						printf("Please Root Your Device!\n");
					}
				//if device wasn't plugged in
				}else{
					printf("Remove Sdcard, Plug The Device, Enable ADB and Try Again\n");
				}
				break;
		//select partitions
			case 2:
				//ShowPartitions();
				system("adb shell \"cat /proc/partitions\"");
				break;
		//copy partitions
			case 3:
				getc(stdin);
				printf("Select the partition you want to copy : ");
				scanf("%s",partition);
				CopyPartitions(partition,NULL);
				break;
			case 4:
				system("adb kill-server");
				goto exit;
			default:
				printf("Input Error!\n");
				break;
		}
	}
	exit:
	return 0;
}
