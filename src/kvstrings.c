#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "./debug.h"
#include "./types.h"


#define READ_BUFFER_SIZE 4096
#define NUM_BYTES_TO_READ 4095
#define LEN_LOW_BOUNDARY 3


void exit_with_error_mess(u8 * mess){
	debug(mess,DEBUG_STATUS_ERROR);
	exit(0);
}


void display_help(void){
	display("\n Help:\n\n",DISPLAY_COLOR_GREEN_YELLOW);
	display("	kvstrings FILE [MIN_LEN:3][-h/--help] \n\n",DISPLAY_COLOR_WHITE);	
}	


int main(int argc , char **argv){

	u8 * fileName;
	u32 threshold = LEN_LOW_BOUNDARY;

	if(argc==1)
		exit_with_error_mess("Not provided filename as argv[1], see -h/--help\n");
	else{
	
		fileName = argv[1];
		if (strcmp(fileName,"-h")==0 || strcmp(fileName,"--help")==0){
			display_help();
			exit(0);
		}
	
		if (argc==3){
			threshold = atoi(argv[2]);
			if(threshold > NUM_BYTES_TO_READ)
				exit_with_error_mess("Invalid threshold, give <4096\n");
		}
	}


	FILE * fp;
	fp = fopen(fileName, "r");
	
	if (!fp)
		exit_with_error_mess("Cannot open the file\n");
	
	/* reading file's content */
	u32 numOfBytesRead;
	u32 thresholdChecker;
	u8 readBuffer[READ_BUFFER_SIZE];

	u8 * startString = NULL ;

	printf("\n    Address			String\n\n");

	while((numOfBytesRead = fread(readBuffer,1,NUM_BYTES_TO_READ,fp))){

		thresholdChecker = 0;

		for( u32 i=0 ; i<=numOfBytesRead ; i++){

			if (i!=numOfBytesRead && (readBuffer[i]=='\t' || (readBuffer[i]>=32 && readBuffer[i]<127))){
				
				if(!startString){
					startString = &readBuffer[i];
				}
			
				// printable character
				thresholdChecker++;
				
			}else{
				// Non-printable character encountered
				if (thresholdChecker>=threshold){

					// we have to print what is in the buffer

					// Nulling the end of the string
					readBuffer[i]=0;

					// Creating colored offset string
					u8 offsetBuffer[20];
					snprintf(offsetBuffer,20,"0x%016lx:",ftell(fp) - NUM_BYTES_TO_READ + i - (&readBuffer[i]-startString));
					display(offsetBuffer,DISPLAY_COLOR_ORANGE);
					printf("  %s\n",startString);
				}

				thresholdChecker = 0;
				startString=NULL;
			}
		}
	}
}




/*

Notes of the implemnetation
---------------------------


The buffer size is really important. In the case of using 1024, some strings were
deleted since they were reseided in the las positions of the buffer and hence, were splitted. The
rest of those strings were read in the next iteration but due to the threshold constraint, they 
were not considered. So we are using a PAGE_SIZE of 4096 bytes.


*/








