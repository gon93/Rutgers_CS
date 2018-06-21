/*
*********************************************
*  214 System Programming                   *
*  Seong Kon Kim                            *
*  Zitian Qin                               *
*  Alon Flor                                *
*  Dec 14,2017                              *
*  sorter_client.c                          *
*********************************************
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "sorter_client.h"

void* traverseDir(void*args);
void* sort(void*args);
void final(char*param,char*outputDir);
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
struct sortInfo{
	pthread_t id;
	char theFile[100];
	char *param;
};
typedef struct sortInfo Info;
void** retval;		//dummy variable for join

char hostName [30];
int port;

int main(int argc, char** argv) {
	
	//initID=pthread_self();
	

	if(argc<7 || argc>11 ||(argc%2==0)){
		printf("Wrong number of arguments\n");
		return 1;
	}
	
	int c=0;
	int h=0;
	int p=0;
	int d=0;
	int o=0;
	int readIndex;
	for(readIndex=1; readIndex<argc; readIndex++){
		if(readIndex%2 == 0)continue;		//only odd numbered
		if(strcmp(argv[readIndex],"-c")==0)c=readIndex;
		if(strcmp(argv[readIndex],"-h")==0)h=readIndex;
		if(strcmp(argv[readIndex],"-p")==0)p=readIndex;
		if(strcmp(argv[readIndex],"-d")==0)d=readIndex;
		if(strcmp(argv[readIndex],"-o")==0)o=readIndex;
	}

	if(c==0){
		printf("Not sorting by columns\n");
		return 1;
	}
	if(p==0){
		printf("Port not specified\n");
		return 1;
	}
	if(h==0){
		printf("Host name not specified\n");
		return 1;
	}



	//printf("c=%d,d=%d,o=%d\n",c,d,o);

	int directoryChange=0;
	int outputChange=0;
	int procCount=1;

	if(d != 0){
		directoryChange=1;	//Non-default directory behavior
	}

	if(o != 0){
		outputChange=1;	//Output directory specified
	}

	//int initialPid=getpid();

	//printf("Initial PID: %d\nTIDs of all child threads: ",getpid());
	
	char directory[70];
	if(directoryChange==0){
		getcwd(directory,70);
		//printf("%s",directory);
	}else{
		strcpy(directory,argv[d+1]);
	}

	if(directory[strlen(directory)-1] != '/'){
		strcat(directory,"/");
	}

	DIR* testdir;
	testdir = opendir(directory);
	if(testdir==NULL){	
		printf("Directory to open not found\n");
		return;
	}
	closedir(testdir);

	if(outputChange!=0){
		testdir = opendir(argv[o+1]);
		if(testdir==NULL){	
		printf("Directory to place file not found\n");
		return;
	}
	closedir(testdir);

	}

	char theC[20];
	strcpy(theC,argv[c+1]);
	strcpy(hostName, argv[h+1]);
	port= atoi(argv[p+1]);
	//printf("host name = %s\tport = %d\n",hostName,port);

	Info initDirInfo = {pthread_self(),"", theC};
	strcat(initDirInfo.theFile,directory);

	traverseDir(&initDirInfo);

	//printf("%s",header);
	char finOutputDir[70];
									//printf("");
	if(outputChange!=0){
		strcpy(finOutputDir,argv[o+1]);
	}else{
		getcwd(finOutputDir,70);
	}
	strcat(finOutputDir,"/");
	//printf("\nput in %s\n",finOutputDir);
	final(theC,finOutputDir);
	
	
}

void* sort(void* args){
								//printf("Got to sorting\n");
	Info *theInfo = args;
	char *myFile = theInfo->theFile;				//printf("myParam: %s\n",theInfo->param);
								//printf("myFile: %s\tStated Thread: %d Thread: %d\n",myFile,theInfo->id,pthread_self());
	char secondArg[strlen(theInfo->param)];			//parameter by which we are sorting
	strcpy(secondArg,theInfo->param);

	int anInd;
	int argCount=1;
	int stringLength = strlen(secondArg);
	for(anInd=0; anInd<stringLength; anInd++){
		if(secondArg[anInd]==','){
			argCount++;
			secondArg[anInd]='\0';
		}
	}
	char theArgs[argCount][20];
	int aTempInt=0;
	for(anInd=0; anInd<argCount; anInd++){
		strcpy(theArgs[anInd],&secondArg[aTempInt]);
		while(secondArg[aTempInt] !='\0')aTempInt++;				//get to next string
		//printf("theArgs are %s\n",theArgs[anInd]);
		aTempInt++;								//get to it
	}

	//pthread_mutex_lock(&m);
	//printf("%u,",pthread_self());
	//numThreads++;							//update the number of threads, which is a global variable (and thus it is shared)
	//pthread_mutex_unlock(&m);

																//printf("Totally a race condition: %s\n",myFile);




																	//printf("Got here 101, %d\n",pthread_self());
	if(strlen(myFile)==0){
		//printf("No csv file found in folder. Terminating\n");
		return ;//procCount;
	}
	FILE * input;
	input = fopen(myFile, "r");
																	//printf("Got here 108, %d\n",pthread_self());


	char line[10000];
	char AllLines[6000][701];
	Row table[6000];
																	//printf("Got here 114, %d\n",pthread_self());
	int i;
	int j;
	int size=0;
	int columnCompare[argCount];
	for(i=0; i<argCount; i++)columnCompare[i]=-1;
	int colInd;
	i=0;												//printf("gonnaSort, %d\n",pthread_self());
	
	//get initial line
	fgets(line,sizeof(line),input);
														//printf("Got here 125, %d\n",pthread_self());
	//copy it
	if(line[strlen(line)-2]=='\n' || line[strlen(line)-2]=='\r'){line[strlen(line)-2]=0;		//no newlines!
	}else if(line[strlen(line)-1]=='\n' || line[strlen(line)-1]=='\r')line[strlen(line)-1]=0;		//no newlines!
	strcpy(AllLines[0],line);
	//AllLines[0][strlen(line)]=0;

																//printf("Got here 128, %d\n",pthread_self());
	//look for columns
	int linestart=0;
	int k=0;
	while(line[linestart+k]!='\0'){
		
		char *elem = malloc(200*sizeof(char));
		k=0;
			
									//printf("bkpt k=%d linestart=%d line=%d\n",k,linestart,strlen(line));
		while(k>=0){
			//printf("%s");
									//printf("\tbkpt1 k=%d linestart=%d line=%d\tletter is%c\n",k,linestart,strlen(line),line[linestart+k]);
			if(line[linestart+k]=='\0'){
				break;
			}else if(line[linestart+k] != ','){
				elem[k] = line[linestart+k];
				k++;
			}else{
				elem[k]='\0';
				linestart=linestart +k+1;
				
				k=-1;
			}

			
			
			
			
		}
		if((elem[strlen(elem)-1]=='\n') || (elem[strlen(elem)-1]=='\r'))elem[strlen(elem)-1]='\0';		//no newlines

		for(colInd=0; colInd<argCount; colInd++){
			//printf("theArgs where it matters: theArgs[%d]=\"%s\"\telem=\"%s\"\ti=%d\n",colInd,theArgs[colInd],elem,i);
			if(strcmp(elem,theArgs[colInd])==0)columnCompare[colInd]=i;
		}
			
		for(j=0;elem[j]!='\0';j++){
			elem[j]='\0';
		}
		j=0;

		i++;
		free(elem);

		
									//printf("bkpt2 k=%d linestart=%d line=%d\n",k,linestart,strlen(line));
	}


	//printf("reached 1st checkpoint\n");
	int columnCompareMax=0;
	for(colInd=0; colInd<argCount; colInd++){
		if(columnCompare[colInd]<0){
			fclose(input);
			//printf("Thread # %u found a bad csv file and is doing nothing\n",pthread_self());
			return;// procCount;
		}
		if(columnCompare[colInd] > columnCompareMax)columnCompareMax=columnCompare[colInd];		//need this so we do not look at elements after all the columns we need to sort
		//printf("columnCompare[0]=%d\n",columnCompare[colInd]);
	}



 	colInd=0;
	int currentRow=0;
	int stop=0;
	while(1==1){
		fgets(line,sizeof(line),input);
		//printf("%s\nc= %c%c endc\n",line,line[strlen(line)-2],line[strlen(line)-1]);
		if(line[strlen(line)-2]=='\n' || line[strlen(line)-2]=='\r'){line[strlen(line)-2]=0;		//no newlines!
		}else if(line[strlen(line)-1]=='\n' || line[strlen(line)-1]=='\r')line[strlen(line)-1]=0;		//no newlines!
		if(strlen(line)==0)break;		//done reading?
		currentRow=currentRow+1;
		

		strcpy(AllLines[currentRow],line);
		//printf("No newln detected\n");
		//printf("Line: %s|",AllLines[currentRow]);
			
		//printf("%s\n",line);
		
		k=0;
		int foundelem=0;
		int linestart=0;
		int currentElem=0;
		int commaInQuotes=-1;
		while(currentElem<=columnCompareMax){
			
			char *elem = malloc(200*sizeof(char));
			k=0;
			
			
			while(k>=0){
				//printf("%s");
				//if(currentElem==11)printf("%c",line[linestart + k]);

				if(line[linestart+k] == '\0'){					//end of line
					break;
				}else if(line[linestart+k] == '"'){				//quotes
					commaInQuotes = -1*commaInQuotes;
					elem[k] = ' ';			//This will be trimmed in mergesort

					k++;
				}else if((line[linestart+k] != ',') || (commaInQuotes>0)){	//standard case
					elem[k] = tolower(line[linestart+k]);
					k++;
				}else{								//comma
					elem[k]='\0';
					linestart=linestart +k+1;
					commaInQuotes=-1;
					k=-1;
				}
	
			}
			
			for(colInd=0; colInd<argCount; colInd++){	
				if(currentElem==columnCompare[colInd]){
					table[currentRow-1].argCount=argCount;
					elem[50]='\0';								//trauncate overlong strings
					strcpy(table[currentRow-1].colterm,elem);
					table[currentRow-1].originalPos=currentRow;
											//printf("table[%d].originalPos=%d\ttheArgs[%d]=%s\n",currentRow-1,currentRow,colInd,theArgs[colInd]);
				}
			}
			
			currentElem++;
			
			free(elem);
		}
		
		j++;
		size++;
		
		line[0]='\0';	//if there is more to read, this will get overridden
	}
		
	fclose(input);
	
	
	//write to the socket
	pthread_mutex_lock(&m);

	char export[1600000];

	//if(type==0)universalType=type;
	//
	//
	//
	//Make a connection
	struct sockaddr_in addr;
	memset(&addr,0,	sizeof(addr));
	int sock = socket(AF_INET,SOCK_STREAM,0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	struct hostent* nameIP;
	nameIP = gethostbyname(hostName);
	//printf("name is %s\n",nameIP->h_name);
	struct in_addr **al = (struct in_addr **) nameIP->h_addr_list;
	int addrIndex;
	for(addrIndex=0; al[addrIndex]!=NULL; addrIndex++){
		int didItWork = inet_pton(AF_INET, inet_ntoa(*al[addrIndex]), &addr.sin_addr);//create the address		//addr.sin_add = 
		//printf("Did it work? %d\t%s\n",didItWork,inet_ntoa(*al[addrIndex]));
		int connectwork;
		//printf("All about the addr:\n\tfamily=%d,port=%d,ain_addr=")
		connectwork=connect(sock, (struct sockaddr *) &addr,sizeof(struct sockaddr_in));//connect
		//printf("Connection: %d\n",connectwork);		
		if(connectwork!=-1)break;
	}
	//
	//
	//write down the sort request
	//int col = columnCompare[0];
	//printf("col=%d\n",col);
	char colStr [50];
	strcpy(colStr,theInfo->param);
	strcat(colStr,",");
	//sprintf(colStr,"%d,",col);
	//strcpy(export,colStr);	//column number
	//int headerNum;
	char rowAmount [10];
	sprintf(rowAmount,"%d,",size+1);
	strcat(colStr,rowAmount);
	
	int sent=-1;
	do{
		sent =write(sock,colStr,strlen(colStr));
	}while(sent==-1);
	
	for(i=0; i<size+1; i++){
		//int originalPosition = table[i].originalPos;
		int headerNum=strlen(AllLines[i]);		//get header for the row
		//printf("%d\n",atoi(&(AllLines[i][strlen(AllLines[i])-2])));
		char hNstr[4];
		sprintf(hNstr,"%d,",headerNum);
		strcat(export,hNstr);			//write it down
		strcat(export,AllLines[i]);
		
		//int resend=0;
		/*char* toSend = strcat(AllLines[i],",");
		sent=write(sock, toSend,strlen(toSend));			//write down the row
		while(sent==-1){
			sent=write(sock, toSend,strlen(toSend));			//write down the row. resend=1
			//printf("Yeah, error detected!!!\n");
		}
		printf("Sent row %d\ttoSend length:%d\tstring length:%d\n",i+1,strlen(toSend),strlen(AllLines[i]));*/
	}
	strcat(export,"+++");
	//printf("export = %s\n",export);


	
	
	//send it
	//printf("Going to do the thing.\n");
	//char* message = "Hello there!";
	//printf("%d\n",strlen(message));
	//char sizeSend[10];
	//sprintf(sizeSend,"%d",strlen(export));
	//write(sock, sizeSend, strlen(sizeSend));

	sent = write(sock, export, strlen(export));
	//printf("Sent = %d\n",sent);

	/*while(sent<strlen(export)){
		int sendr = write(sock, &export[sent], strlen(export)-sent);
		if(sendr>=0)sent+=sendr;
		printf("Sent = %d	change=%d\n",sent,sendr);
		//if(sendr<0)break;
	}*/
	
	//wait for response	(cancelled due to bugginess)
	//printf("Done sending\n");
	//fclose(out);
	char response[1600000];
	//int received=0;
	int receivedNow=-1;
	//do{
		//printf("Begin reading\n");
		memset(response,0,1600000);
		//printf("Nowish\n");
		//receivedNow=read(sock,response, 1600000/*,MSG_DONTWAIT*/);
		//if(receivedNow>=0)received+=receivedNow;
		//printf("drgjekr\n");
		//printf("RN: %d\n",receivedNow);
		//printf("Response: %s\n",response);
		
	//}while(receivedNow>0);
	//printf("Response: %s\n",response);

	//close(sock);
	pthread_mutex_unlock(&m);
	//finished writing to the socket

	
	//if(getpid()==initialPid)return 0;
	return 0;//procCount;
}


void* traverseDir(void* args){
																//printf("Got here 316\n");
	Info * di = args;													//printf("Got here 67	dirput =%s\n",di->dirput);
	char* directory = di->theFile;
	
	DIR *dir;
	dir = opendir(directory);
	if(dir==NULL){	
		dir = opendir(strcat("./",di->theFile));		//is relative path
		if(dir==NULL){
			printf("Bad starting directory, %s\n",di->theFile);
			return;
		}
	}
	//printf("Traverse Dir: %s\n",directory);
	pthread_mutex_lock(&m);
	//if(initID!=pthread_self())printf("%u,",pthread_self());
	pthread_mutex_unlock(&m);

	struct dirent *direntity;
		
	//fork on all csv files but one
	//fork on all directories
	int threads=0;
	Info myThreads [250];
	int nameStart;	//for outputChange>0
	
	while ((direntity = readdir(dir))!=NULL){						//go through the current directory
														//printf("PID=%d\t%s\t",getpid(),direntity->d_name);
		char * name = direntity->d_name;
		
		if(direntity->d_type == DT_DIR){						//found a directory
			//printf("directory\t");
			if(strcmp(direntity->d_name,".")==0	||	strcmp(direntity->d_name,"..")==0){
				//printf("\tignore");
			}else{										//directory is neither . nor ..	
				//create new thread, start over with a new directory. Set dir to direntity and go to start of loop;
				char tempDir[50];
				strcpy(tempDir,directory);	
				strcat(tempDir,direntity->d_name);
				strcat(tempDir,"/");
				pthread_t id;
				Info tempInfo = {id,"",di->param};
				strcat(tempInfo.theFile,tempDir);
				myThreads[threads] = tempInfo;
				pthread_create(&(myThreads[threads].id), NULL,traverseDir,&(myThreads[threads]));
				threads++;
	
			}
		}else if(strlen(name)>4){
			name = &name[strlen(name)-4];
			if(strcmp(name,".csv")==0){							//found a .csv file
													//Create a new thread here.
				char tempDir[strlen(directory)+strlen(direntity->d_name)];
				strcpy(tempDir,directory);
				
				strcat(tempDir,direntity->d_name);				//printf("Threads = %d\n",threads);
				pthread_t id;
				Info tempInfo = {id,"",di->param};
				strcat(tempInfo.theFile,tempDir);
				myThreads[threads] = tempInfo;
				//myThreads[threads].theFile = tempDir;
															//printf("Going to call, File=%s\n",myThreads[threads].theFile);
				//sort(&myThreads[threads]);
				//supertemp(&myThreads[threads]);
				int check = pthread_create(&myThreads[threads].id,NULL,sort,&myThreads[threads]);	//printf("check=%d\n",check);
				threads++;
			
			}
		}
				//printf("\n");
	}
	closedir(dir);
	//SortInfo si= {myFile,di->paramC};
	int cThreads=0;
	while(cThreads<threads){
		//printf("Join threads\n");
		
		pthread_join(myThreads[cThreads].id,retval);
		cThreads++;
	}
	return 0;
}


void final(char*param,char*outputDir){
	//RowLLE *current=start;
	//if(FilesToSort==0)return;
	//
	//
	pthread_mutex_lock(&m);	
	////Make a connection	(copied from sort)
	struct sockaddr_in addr;
	memset(&addr,0,	sizeof(addr));
	int sock = socket(AF_INET,SOCK_STREAM,0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	struct hostent* nameIP;
	nameIP = gethostbyname(hostName);
	//printf("name is %s\n",nameIP->h_name);
	struct in_addr **al = (struct in_addr **) nameIP->h_addr_list;
	int addrIndex;
	for(addrIndex=0; al[addrIndex]!=NULL; addrIndex++){
		int didItWork = inet_pton(AF_INET, inet_ntoa(*al[addrIndex]), &addr.sin_addr);//create the address		//addr.sin_add = 
		//printf("Did it work? %d\t%s\n",didItWork,inet_ntoa(*al[addrIndex]));
		int connectwork;
		//printf("All about the addr:\n\tfamily=%d,port=%d,ain_addr=")
		connectwork=connect(sock, (struct sockaddr *) &addr,sizeof(struct sockaddr_in));//connect
		//printf("Connection: %d\n",connectwork);		
		if(connectwork!=-1)break;
	}
	//printf("Got to this point!\t1\n");
	//
	//
	//make and send dump request
	int sent;
	char dumpRequest [50];
	strcpy(dumpRequest, "***");
	//printf("Got to this point!\t2\n");
	strcat(dumpRequest,param);
	strcat(dumpRequest,"+++");
	//printf("Got to this point!\t3\n");
	sent = write(sock, dumpRequest, strlen(dumpRequest));
	//printf("Sent = %d\n",sent);

	/*while(sent<strlen(dumpRequest)){
		int sendr = write(sock, &dumpRequest[sent], strlen(dumpRequest)-sent);
		if(sendr>=0)sent+=sendr;
		printf("Sent = %d	change=%d\n",dumpRequest,sendr);
		//if(sendr<0)break;
	}*/

	
	//get stuff
	char sorted[1000000];
	memset(sorted,0,1000000);
	char buffer[1000];
	int r =0;
	int exit=0;
	while(exit==0){
		memset(buffer,0,1000);
		r = read(sock,buffer,1000);
		//printf("%d\t%s\nbigger? %s\n",r,sorted,r>0 ? "yes":"no");
		if(r>=0)strcat(sorted,buffer);
		if(strcmp(&sorted[strlen(sorted)-3],"+++")==0)exit=1;
	};
	sorted[strlen(sorted)-3]='\0';
	//printf("\n\n%s\n\n\nSorted\n%s\n\n\nr=%d\n",outputDir,sorted,r);
	


	FILE * output;
	
	char addro[80];
	strcpy(addro,outputDir);
	strcat(addro,"AllFiles-sorted-");
	strcat(addro,param);
	strcat(addro,".csv");
	output=fopen(addro, "w");

	
	char rowstr [1000];
	int stop=0;
	int indexSorted=0;
	int indexCap=0;
	int skip=0;
	
	//write file
	while(stop==0){
		memset(rowstr,0,1000);
		while(sorted[indexSorted+indexCap]!=',')indexCap++;		//printf("sigh,\tindexSorted=%d\n",indexSorted);
		strncpy(rowstr,&sorted[indexSorted],indexCap);			//printf("sigh2\n");	//copy the number to rowstr
		skip=atoi(rowstr);
		indexSorted=indexSorted+indexCap+1;
		indexCap=0;							//printf("sigh3\n");
		memset(rowstr,0,60);						//printf("sigh4\n");
		strncpy(rowstr,&sorted[indexSorted],skip);			//printf("sigh5\n");
		indexSorted=indexSorted+skip;
		//printf("try2: %d\n%s\n\n\n\n",skip,rowstr);	//copy the row to rowstr
		//printf("try2 continued\n%s\n",&sorted[indexSorted]);
		strcat(rowstr,"\n");
		fputs(rowstr,output);
		if(sorted[indexSorted]==0)stop=1;
	}
	
	fclose(output);
	pthread_mutex_unlock(&m);
	return;
	//printf("Exit\n");
	//printf("Equal?	%s\n",current==last ? "true":"false");
}



