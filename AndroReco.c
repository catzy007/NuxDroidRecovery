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

int SelectPartition(char **partition){
	FILE *buffer1;
	unsigned long int *partsize;
	int idxlarge;
	int partlen;
	int i,j,loop1,loop2;
	char text[255];
	char dict[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	if(!IsPaired()){
		printf("The Device Aren't Ready!, Pair First\n");
	}else{
		//adb shell "cat /proc/partitions" | awk '{print $4}' //debug_line_can_be_removed
		buffer1 = popen("adb shell \"cat /proc/partitions\" | awk '{print $4}'","r");
		if(buffer1==NULL){
			return -1;
		}
		
	//calculate partition number and allocate memmory accordingly
		loop1=1,loop2=1,partlen=0;
		while(fgets(text,sizeof(text),buffer1)!=NULL){
			if(loop1>2){
				partlen++;
				loop2++;
			}
			loop1++;
		}
		pclose(buffer1);
		partsize=malloc(sizeof(unsigned long int)*partlen);
		
	//get partition size and store as float
		buffer1 = popen("adb shell \"cat /proc/partitions\" | awk '{print $3}'","r");
		loop1=1,loop2=0;
		while(fgets(text,sizeof(text),buffer1)!=NULL){
			if(loop1>2){
				partsize[loop2]=strtoul(text, NULL, 0);
				loop2++;
			}
			loop1++;
		}
		pclose(buffer1);
		
		//for(i=0;i<partlen;i++){printf(" %d %d %.0f\n",i,partlen,partsize[i]);} //debug_line_can_be_removed
		
	//get the largest partition size
		for(idxlarge=0, i=0 ;i<partlen; i++){
			if(partsize[i] > partsize[idxlarge]){
				idxlarge=i;
			}
		}
		//printf("%lu %d\n", partsize[idxlarge], idxlarge); //debug_line_can_be_removed
		
	//get the largest partition name and save to memmory
		buffer1 = popen("adb shell \"cat /proc/partitions\" | awk '{print $4}'","r");
		loop1=1,loop2=0;
		while(fgets(text,sizeof(text),buffer1)!=NULL){
			if(loop1>2){
				if(loop2==idxlarge){
				//some f*ckin filter to filter some sh*t
					i=0;partlen=0;
					while(text[i]!='\0'){
						for(j=0;j<sizeof(dict)/sizeof(char);j++){
							if(text[i]==dict[j]){
								partlen++;
							}
						}
						i++;
					}
				//allocate memory and copy each character
					*partition=malloc(sizeof(char)*partlen);
					for(i=0;i<partlen;i++){
						(*partition)[i]=text[i];
						//printf("%c",text[i]);
					}
					//printf(" %s\n",*partition); //debug_line_can_be_removed
					//strcpy(partition, text); //debug_line_can_be_removed
				}
				loop2++;
			}
			loop1++;
		}
		pclose(buffer1);
	}
	//printf("%s\n",partition); //debug_line_can_be_removed
	//system("adb shell \"cat /proc/partitions\""); //debug_line_can_be_removed
}

void CopyPartitions(char *partition){
	char dict[]="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char command[255];
	int i=0,j=0;
	char *cleanpart;
	int lencleanpart=0;
	
	printf("copying the partitions %s\n", partition);
	
	//adb forward tcp:8888 tcp:8888 & xterm -e 'echo Sending Data ; adb shell "su -c dd if=/dev/block/loop0 | busybox nc -l -p 8888"' & xterm -e 'echo Receiving Data ; nc 127.0.0.1 8888 > loop0.img'
	//forward the android tcp to host tcp
	strcpy(command, "adb forward tcp:8888 tcp:8888 & ");
	//run xterm, and add message
	strcat(command, "xterm -e 'echo Sending Data \073 ");
	//run adb shell and dd
	strcat(command, "adb shell \"su -c dd if=/dev/block/");
	strcat(command, partition);
	//pipe the dd output to port 8888
	strcat(command, " | busybox nc -l -p 8888\" \073 sleep 3' & ");
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

void recover(){
	printf("This Might Take a While. . .\n");
	system("foremost -i *.img -o ./recovered-$(date +'%Y.%m.%d-%H.%M.%S')");
}

int main(){
	int menu;
	FILE *buffer1;
	FILE *buffer2;
	char *partition;
	char command[255];
	char text[255];
	int i=0;
	int loop;
	int mark1=0;
	int mark2=0;
	
	while(1){
		printf("1.Pair\n2.Show Partitions\n3.Copy Partitions\n4.Recover Data\n5.Exit\nSelect options : ");
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
				system("adb shell \"cat /proc/partitions\"");
				break;
		//copy partitions
			case 3:
				//getc(stdin);
				//printf("Select the partition you want to copy : ");
				//scanf("%s",partition);
				SelectPartition(&partition);
				//printf("Largest Partition Is : %s\n", partition);
				CopyPartitions(partition);
				break;
			case 4:
				recover();
				break;
			case 5:
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
