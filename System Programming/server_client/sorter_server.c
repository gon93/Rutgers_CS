#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

/* Socket Stuff */
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h> 

/* Pthread Library */
#include <pthread.h>
#include "sorter_server.h"





struct arg_struct {/*struct */
   int arg1;
   
int arg2;
};
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;




int main(int argc, char* argv[]){
header=(char*)malloc(sizeof(char)*10000);
header[0]=0;
singleclient =malloc(thesize*sizeof(char*));
    iparry=malloc(10*sizeof(char*));
 iparryse=malloc(100*sizeof(int));
    //allocating memories
    ttid=malloc(ttid_size*sizeof(pthread_t));
    int* arrayosocketfd=malloc(ttid_size*sizeof(int));
    c_data = malloc(100 * sizeof(char*));
    for(i = 0; i < c_data_size; i++){
        c_data[i] =  malloc(1000 * sizeof(char));
    }
    
    /*init the lock*/
    pthread_mutex_init(&connection_lock,NULL);
     //pthread_mutex_init(&m,NULL);
    //having wrong arguments
    if(argc != 3){
        perror("ERROR: Wrong format for arguments");
        return (-1);
    }

    if(strcmp(argv[1], "-p") != 0){
        perror("ERROR: Need -p argument to load");
        return (-1);
    }
    
    
    //creating & setting socket
    int s_socket, c_socket,size;//server socket, client socket
    struct sockaddr_in server, client; //server address, client address

    //checking error
    if((s_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("ERROR Socket: ");
        return (-1);
    }

    int port_num;
    port_num = atoi(argv[2]);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
	  server.sin_port = htons(port_num);
    //puts("Created socket");
    
    //bind
    //check error
    if(bind(s_socket, (struct sockaddr *)&server,sizeof(server)) < 0){
        perror("ERROR Bind: ");
        close(s_socket); //closing socket
        return (-1);
    }
    //puts("bind done");
    //listen
    //check error
    if(listen(s_socket,100) <0){
        perror("ERROR Listen: ");
        close(s_socket); //closing socket
        return (-1);
    }
    //puts("waiting for connection");
  i=0;
o=0;
    //accepting the connection
    size = sizeof(struct sockaddr_in);
/*i set a loop for this but i do not know how to stop the loop.*/



	printf("Recieved connection from: ");
     while(   1){


c_socket = accept(s_socket,(struct sockaddr *)&client,&size);

printf("<%s>,", inet_ntoa(client.sin_addr));
fflush(stdout);

/*struct sockaddr_in* address = (struct sockaddr_in*)&client;
	struct in_addr ipAddr = address->sin_addr;
	char str[INET_ADDRSTRLEN];
	inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
	
	if(print_ip == 0){
		
		printf("client_ipaddr: %s\n",str);
		print_ip =1;
}*/	
	
        //puts("Connection accepted");

/*it print out the ip address */


	
/*
int newloopc=0;
int tureornot=0;
//printf("here %d,%d\n",newloopc,iparryc);
while(newloopc<iparryc+1){
//printf("herennnnnnn\n");
if(iparryc==0){

iparry[iparryc]=str;
iparryc++;
tureornot=1;
//printf("hereasaswq\n");
break;
}
if (tureornot==0 && strcmp(str,iparry[newloopc])==0){
tureornot=1;
//printf("heyyyyyyywq\n");
break;
}
if (tureornot==0 && newloopc==iparryc){
iparry[iparryc]=str;
iparryc++;
break;
}
newloopc++;
}
*/
struct arg_struct *args=malloc(sizeof(struct arg_struct)) ;
args->arg1 = c_socket;
//printf("nnnn%d\n",newloopc);
        pthread_create(&ttid[i],NULL,connection_handler,args);
	
i=i+1;
o=o+1;



}

/*int counterjoin=0;

while(counterjoin<i){
        

pthread_join(ttid[counterjoin],NULL);

counterjoin=counterjoin+1;
}

*/
    close(s_socket);
    return 0;
}







void* connection_handler(void *arguments){
    // this is the socket for our server
    // need to find tid # (use self())
   pthread_mutex_lock(&connection_lock);
    int c_sock,j,column_num,ch_num ;

    char *message;
int wholesize;


//printf("this is print\n");

    

 struct arg_struct *ar = (struct arg_struct *)arguments;
    c_sock = ar->arg1;
int thesizeofcds=2000000;


char** datasort=malloc(thesizeofcds*sizeof(char*));
int countofdatasort=0;

//printf("this is print1\n");
char *recv_buff=malloc(thesizeofcds*sizeof(char));

int csizeo=0;
int tft=0;
int theturnofrecv =0;
//printf("this is print3\n");



while(theturnofrecv ==0){
//printf("this is print4 ,,,,%d\n",tft);
   char *recv_bu;
recv_bu = malloc(c_data_size * sizeof(char)); 
memset(recv_bu,0,c_data_size);
    //put read
//printf("befor\n");
int ook= read(c_sock,recv_bu,c_data_size);
//printf("after\n");
//printf("%s\n",recv_bu);
//printf(" %s\n",recv_bu);
//printf("ook %d\n",ook);
if(ook==0||ook<0){
theturnofrecv =1;
//printf("break point\n");
}
//printf("break point1\n");

if(tft==0){
/*
if(strlen(recv_bu)>thesizeofcds){
recv_buff=realloc(recv_buff,thesizeofcds*2*sizeof(char));
thesizeofcds=thesizeofcds*2;
}
*/
strcpy(recv_buff,recv_bu);
//csizeo=csizeo+strlen(recv_bu);
}
else{
/*
if(strlen(recv_bu)+csizeo>thesizeofcds){
recv_buff=realloc(recv_buff,thesizeofcds*2*sizeof(char));
thesizeofcds=thesizeofcds*2;
}
*/
strcat(recv_buff,recv_bu);
//csizeo=csizeo+strlen(recv_bu);
}
//printf("break point2\n");
free(recv_bu);
//printf("the end of the big big loop\n");
tft++;

if(strcmp(&recv_buff[strlen(recv_buff)-3],"+++")==0){
	recv_buff[strlen(recv_buff)-3]='\0';
//printf(" recv bufff    %s",&recv_buff[strlen(recv_buff)-10]);
	theturnofrecv=1;
}


}
/*
int nope;
for(nope=0; nope<20;nope++){
	printf("%c",recv_buff[nope]);
}
printf("\n");
*/
//printf("Recvbuff: %s\n",recv_buff);
//fflush(stdout);


//printf("outloop\n");
/*
char* mess="go!!";
int seeent=write(c_sock,mess,strlen(mess));
printf("seent  %d\n",seeent);
*/
//printf("%s\n",recv_buff);
int  iii = 0;

if(*(recv_buff+iii)=='*'){
if(*(recv_buff+iii+1)=='*'){
if(*(recv_buff+iii+2)=='*'){
iii=iii+3;
//printf("%s\n",&recv_buff[iii]);
char* socl=malloc(30*sizeof(char));
int elo=strlen(recv_buff)-3;
int elocount=0;
while(elocount<elo){
*(socl+elocount)=*(recv_buff+iii);
//printf("%c\n",*(socl+elocount));
iii++;
//printf("Problem here?\n");
elocount++;
}
socl[elocount]=0;
//printf("%s\n",socl);
sort(singleclient,socl,ar->arg1);
/*
int kloiu=0;
while(kloiu<singleclientcounter){
//printf("%s\n",singleclient[kloiu++]);
}
*/
//printf("Exiting connection handler - dump request\n");
pthread_mutex_unlock(&connection_lock);

return;
}
}
}



//printf("here\n");

 /* 
char * sc=malloc(10*sizeof(char));
*(sc+9)='\0';
while(*(recv_buff+iii)!=','){
*(sc+iii)=*(recv_buff+iii);
iii++;
}
iii++;
int scc=atoi(sc);
*(iparryse+ar->arg2)=scc;
free(sc);
*/

char * sc=malloc(100*sizeof(char));
//*(sc+9)='\0';
while(*(recv_buff+iii)!=','){
*(sc+iii)=*(recv_buff+iii);
iii++;
}
*(sc+iii)='\0';
iii++;
//int scc=atoi(sc);
//*(iparryse+ar->arg2)=scc;
//printf("sc   %s\n",sc);
//return;
//free(sc);



//printf("%d\n",scc);

int countscs=0;
char * scs=malloc(10*sizeof(char));
while(*(recv_buff+iii)!=','){
*(scs+countscs)=*(recv_buff+iii);

//printf("%c        %c\n",*(scs+countscs),*(recv_buff+iii));
iii++;
countscs++;
}
iii++;
scs[countscs]='\0';
//printf("%d\n",iii);
int scsc=atoi(scs);
free(scs);
//printf("scsc%d\n",scsc);






int lopk=0;
while(lopk<scsc){
char * ss=malloc(10*sizeof(char));

int intnumber;
int counterofsc=0;
while(*(recv_buff+iii)!=','){
*(ss+counterofsc)=*(recv_buff+iii);
//printf("compare chars: %c\t%c\n",*(recv_buff+iii),*(ss+counterofsc));
iii++;
counterofsc++;
}
*(ss+counterofsc)='\0';
//printf("%s\n",ss);
iii++;
intnumber=atoi(ss);
//printf("Intnumber = %d\n",intnumber);
int loopcount=0;
char* data=malloc((intnumber+1)*sizeof(char));
memset(data,0,intnumber+1);
*(data+intnumber+1)='\0';
while(loopcount<intnumber){
*(data+loopcount)=*(recv_buff+iii);
iii++;
loopcount++;
}
//printf("data= %s\n",data);
if((theheader==0&&lopk==0) || (theheader==1&&lopk!=0)){
//*(datasort+countofdatasort)=data;

*(singleclient+singleclientcounter)=data;
//printf("data= %s    \n   %s\n",*(singleclient+singleclientcounter),data);
//printf("the counter of master list: %d\n",singleclientcounter);
singleclientcounter++;

//countofdatasort++;
if(theheader == 0)
	theheader=1;
}
/*
if(singleclientcounter==thesize){
thesize=thesize*2;
singleclient=realloc(singleclient,thesize*sizeof(char*));
}
*/
lopk++;
//printf("%d\n",lopk);
}

//printf("outlqq\n");


//sort(datasort,sc,c_sock);

int u=0;

/*
while(u==0){
printf("%s\n",*(singleclient+u));
u++;
}
*/
//printf("Exiting connection handler - done with thread\n");
pthread_mutex_unlock(&connection_lock);
return;



}





























void sort(char** raw,char*secondArg,int c_sock){
								//printf("Got to sorting\n");
								//printf("Got here 108, %d\n",pthread_self());


	char** AllLines=raw;
	Row table[60000];
	int argCount=1;											//printf("Got here 114, %d\n",pthread_self());

	int anInd;
	int stringLength = strlen(secondArg);
	for(anInd=0; anInd<stringLength;anInd++){
		if(secondArg[anInd]==','){
			argCount++;
			secondArg[anInd]='\0';
		}
	}
	char theArgs [argCount][20];

	int aTempInt=0;
	for(anInd=0; anInd<argCount; anInd++){
		strcpy(theArgs[anInd],&secondArg[aTempInt]);
		while(secondArg[aTempInt] !='\0')aTempInt++;
		aTempInt++;
	}

	int i;
	int j;
	int size=0;
	int columnCompare[argCount];
	for(i=0; i<argCount; i++)columnCompare[i]=-1;
	int colInd;
	i=0;												//printf("gonnaSort\n");
	
																	//printf("Got here 128, %d\n",pthread_self());
	//look for columns
	int linestart=0;
	int k=0;
	while(raw[0][linestart+k]!='\0'){
		
		char *elem = malloc(200*sizeof(char));
		k=0;
			
									//printf("bkpt k=%d linestart=%d line=%d\n",k,linestart,strlen(line));
		while(k>=0){
			//printf("%s");
									//printf("\tbkpt1 k=%d linestart=%d line=%d\tletter is%c\n",k,linestart,strlen(line),line[linestart+k]);
			if(raw[0][linestart+k]=='\0'){
				break;
			}else if(raw[0][linestart+k] != ','){
				elem[k] = raw[0][linestart+k];
				k++;
			}else{
				elem[k]='\0';
				linestart=linestart +k+1;
				
				k=-1;
			}

			
			
			
			
		}
		if(elem[strlen(elem)-1]=='\n')elem[strlen(elem)-1]='\0';

		for(colInd=0; colInd<argCount; colInd++){
			//printf("theArgs where it matters: theArgs[%d]=\"%s\"\telem=\"%s\"\ti=%d\n",colInd,theArgs[colInd],elem,i);
			//printf("%s\t%s\n",elem,theArgs[colInd]);
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
			//fclose(input);
			//printf("found a bad csv file and is doing nothing\n");
			return;// procCount;
		}
		if(columnCompare[colInd] > columnCompareMax)columnCompareMax=columnCompare[colInd];		//need this so we do not look at elements after all the columns we need to sort
		//printf("columnCompare[0]=%d\n",columnCompare[colInd]);
	}


	//printf("checkpoint 1.2\t%d\n",singleclientcounter);
 	colInd=0;
	int currentRow=0;
	int stop=0;
	while(currentRow<singleclientcounter-1){
		//fgets(line,sizeof(line),input);
		if(AllLines[currentRow]==NULL)break;		//done reading?
		currentRow=currentRow+1;
		
		//printf("Does AllLines exist?\n");
		//printf("%s\n",AllLines[currentRow]);
		
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

				if(AllLines[currentRow][linestart+k] == '\0'){					//end of line
					break;
				}else if(AllLines[currentRow][linestart+k] == '"'){				//quotes
					commaInQuotes = -1*commaInQuotes;
					elem[k] = ' ';			//This will be trimmed in mergesort

					k++;
				}else if((AllLines[currentRow][linestart+k] != ',') || (commaInQuotes>0)){	//standard case
					elem[k] = tolower(AllLines[currentRow][linestart+k]);
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
		
		
	}
		
	//fclose(input);

	//printf("2\tColumnCompare = %d\n",columnCompare);
	//for(i=0; i<size; i++){
		//printf("%s	%d\n",table[i].colterm,table[i].originalPos);
	//}

	//printf("3\t\tsize=%d\n\n\n\n",size);

	
	//determine what type is the column
	int type;	//0 = String, 1 = number;
	
	for(i=0; i<size; i++){

		if(table[i].colterm==NULL)continue;
		if(strcmp("0",table[i].colterm)!=0){
			if(atoi(table[i].colterm)==0){
				type=0;
				break;
			}else{
				type=1;
			}
		}else{
			type=1;
		}
		//printf("Column is %s\t\t\ttype is %d\t\t%d\n",table[i].colterm,type,atoi(table[i].colterm));		
	}
	
	//printf("enter mergesort\n");
	
															//printf("Got here 318\t%d\n",type);
	mergesort(table,0,size,type);

															//printf("Got here 321\n");
	
	/*FILE * output;
	char addr[50];
	myFile[strlen(myFile)-4]='\0';
//	if(outputChange==0){
		strcpy(addr,strcat(strcat(strcat(myFile,"-sorted-"),secondArg),".csv"));
//	}else{														//run with no output change please
		//char newAddr[strlen(argv[outputChange])+1];
		//strcpy(newAddr,argv[outputChange]);
		//strcat(newAddr,"/");
		//printf("%s\n",newAddr);
		//strcpy(addr,strcat(strcat(strcat(strcat(newAddr,&myFile[nameStart]),"-sorted-"),secondArg),".csv"));
//	}
	output = fopen(addr, "w");
	
	fputs(AllLines[0],output);
	for(i=1; i<size+1; i++){
		int originalPosition = table[i-1].originalPos;
		fputs(AllLines[originalPosition],output);
	}

	fclose(output);*/

	//printf("chkpt2\n");
	//write to the shared data
	//pthread_mutex_lock(&m);
														//just dump the darn data to the client!!!!!!!!!
	if(strlen(header)==0)strcpy(header,AllLines[0]);

	//;if(type==0)universalType=type;

	//FSRow *exportLines= (FSRow*)malloc((sizeof(FSRow))*size);
	int originalPosition;
	char export [1600000];
	memset(export,0,1600000);
	int length=0;
	char tempL[10];
	
	length=strlen(header);
	memset(tempL,0,10);
	sprintf(tempL,"%d,",length);	
	strcat(export,tempL);
	strcat(export,header);
	for(i=0; i<size; i++){
										//printf("here\n");
		originalPosition = table[i].originalPos;
		length=strlen(AllLines[originalPosition]);
		memset(tempL,0,10);										//printf("here1\n");
		sprintf(tempL,"%d,",length);										//printf("here2\n");
		strcat(export,tempL);										//printf("here3 %d\n%s\n",originalPosition,AllLines[originalPosition]);
		strcat(export,AllLines[originalPosition]);										//printf("here4\n");
		//printf("export size: %d\n",strlen(export));

 		//write(c_sock , export , strlen(export));
		//write(export);
		//strcpy(exportLines[i].row,AllLines[originalPosition]);
		//strcpy(exportLines[i].colTerm,table[i].colterm);
		//printf("%s\n",exportLines[i].colTerm);
	}
										//printf("here end of loop\n");

	int sent=0;
	while(sent<strlen(export)){
		
		int sentr=send(c_sock,export,strlen(export),MSG_DONTWAIT);
		if(sentr>=0)sent+=sentr;
		//printf("Sent = %d\t%d\t%d\t,so %s\n",sent,sentr,strlen(export), sent<strlen(export) ? "yes":"no");
	}
	write(c_sock,"+++",3);
	//printf("end of loop\n");
	//pointers[FilesToSort]= &exportLines[0];
	sizeRemaining[FilesToSort]=size;	

	FilesToSort++;							//increases number of pointers to files

	//pthread_mutex_unlock(&m);
	//finished writing to shared data

	
	//printf("finished sorting %s.\n",strcat(strcat(strcat(myFile,"-sorted-"),secondArg),".csv"));

	//if(getpid()==initialPid)return 0;
	return;//procCount;
}
