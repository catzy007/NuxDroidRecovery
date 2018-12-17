#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//ps -eaf | grep "adb shell" | cut -d ":" -f4-s  //debug_line_can_be_removed

int main(){
	FILE *fp1;
	int i,j,signal=0;
	char strcmd[255];
	char search[]="xterm -e echo Receiving Data";
	char lensrc=sizeof(search)/sizeof(char);
	sleep(2);
	printf("Copying Partition, Please Wait\n");
	while(1){
		fp1 = popen("ps -at | grep \"xterm -e echo\" | cut -d \":\" -f2-", "r");
		if(fp1==NULL){
			return -1;
		}
		fgets(strcmd,sizeof(strcmd)-1,fp1);
		//printf("%s",strcmd); //debug_line_can_be_removed
		//printf("%s",search); //debug_line_can_be_removed
		pclose(fp1);
		signal=0;i=0;j=0;
		while(strcmd[i]!='\0'){
			signal=0;
			for(j=0; j<lensrc; j++){
				if(search[j]==strcmd[i+j]){
					signal++;
				}
			}
			//printf("\n%d %d %d %d",i,j,signal,lensrc); //debug_line_can_be_removed
			if(signal==lensrc-1){
				system("pgrep -x xterm | xargs kill -SIGINT");
				goto exit;
			}
			i++;
		}
		sleep(1);
	}
	exit:
	return 0;
}
