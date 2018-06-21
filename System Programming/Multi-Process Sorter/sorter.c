#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mergesort.c"
#include "mergesort_num.c"
#include "mergesort_f.c"
#include "sorter.h"
#include <dirent.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int check_c = 0;//check -c;
char* c_name=NULL;
int check_d = 0;//check -d;
char* d_name=NULL;
char* new_dir = NULL;
int check_o=NULL;//check -o;
char* o_name=NULL;//check oinput;
int ini=0;/*initail pid*/
int vv=100;/*size of the path*/
int count=98;
int *glob_var;
void  setting(char*,int,int,int);
void sort(char*,char*,char*);

int* main(int argc, char* argv[])
{
    if(argc < 3 && argc > 7){
        fprintf(stderr,"%s","Not a right inputs\n");
        return 0;
    }
    if(argc%2 != 1){
        fprintf(stderr,"%s","Not a right inputs\n");
        return 0;
    }
    new_dir = malloc(1000* sizeof(char));
    if(argc == 3){
        if(strcmp(argv[1],"-c") == 0){
            check_c = 1;
            c_name = argv[2];
            d_name = ".";
            DIR *d;
        }else{
            fprintf(stderr,"%s","wrong parameter: need -c \n");
        }
    }
    if(argc == 5){
        if(strcmp(argv[1],"-c") == 0){
            check_c = 1;
            c_name = argv[2];
        }else{
            fprintf(stderr,"%s","wrong parameter: need -c \n");
        }
        if(strcmp(argv[3],"-d") == 0){
            check_d = 1;
            d_name = argv[4];
            strcpy(new_dir, "newDir");
            mkdir("newDir",0700);
            strcat(new_dir,"/");
        }else if(strcmp(argv[3],"-o") == 0){
            check_o = 1;
            o_name = argv[4];
            mkdir(o_name,0700);
            strcpy(new_dir,o_name);
            strcat(new_dir,"/");
        }else{
            fprintf(stderr,"%s","wrong parameter: need -d \n");
        }
    }
    if(argc ==7){
        if(strcmp(argv[1],"-c") == 0){
            check_c = 1;
            c_name = argv[2];
        }else{
            fprintf(stderr,"%s","wrong parameter: need -c \n");
        }
        if(strcmp(argv[3],"-d") == 0){
            check_d = 1;
            d_name = argv[4];
        }else{
            fprintf(stderr,"%s","wrong parameter: need -d \n");
        }
        if(strcmp(argv[5],"-o") == 0){
            check_o = 1;
            o_name = argv[6];
            mkdir(o_name,0700);
            strcpy(new_dir,o_name);
            strcat(new_dir,"/");
        }else{
            fprintf(stderr,"%s","wrong parameter: need -o \n");
        }
    }
  glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *glob_var = 1;
  int ini_pid = getpid();
  printf("initial pid: %d\n",ini_pid);
  printf("children pids: \n");
  int pid;
  setting(d_name,vv,count,pid);
  printf("total pids: %d\n", *glob_var);
  free(new_dir);
}
void setting(char* cdi,int vv,int count, int pid) {
  DIR *dir;
  struct dirent *ent; 
  if((dir=opendir(cdi))==NULL){
      printf("no dir\n");
      exit(1);
  }
  if ((dir = opendir (cdi)) != NULL){
  
      while((ent = readdir (dir)) != NULL){
          if(strcmp(ent->d_name,".")==0||strcmp(ent->d_name,"..")==0){
            continue;
          }
          if(strstr(ent->d_name,".csv")!=NULL&&(ent->d_type==8) && strstr(ent->d_name,"-sorted-")==NULL){/*if it is a file*/
              int pid=fork();
              if(pid<0){
                  printf("pid");
              }else if(pid==0){
                  printf("%d\n",getpid());   
                  *glob_var = *glob_var+1;
                  munmap(glob_var, sizeof *glob_var); 
                  /*mergesort*/
                  char* current_file = malloc(50 * sizeof(char));
                  current_file = strtok(ent->d_name,".");
                  sort(cdi,current_file,c_name);
                  break;
              }
              continue;
          }if(ent->d_type==4 && (strcmp(ent->d_name,"NewDir") != 0 || strcmp(ent->d_name, o_name) != 0)){/*if it is a folder*/
              int pid=fork();
              if(pid<0){
                  printf("pid");
              }
              if(pid==0){
                  char* path=malloc (vv*sizeof(char));
                  int kk=0;
                  kk=strlen(path);
                  if(kk>=count){
                      vv=vv+1000;
                      path=(char*) realloc(path,vv);
                      count=count+1000;
                  }
                  strcpy(path,cdi);
                  kk++;
                  if(kk>=count){
                      vv=vv+1000;
                      path=(char*) realloc(path,vv);
                      count=count+1000;
                  }
                  strcat(path,"/");
                  kk=kk+strlen(ent->d_name);
                  if(kk>=count){
                      vv=vv+1000;
                      path=(char*) realloc(path,vv);
                      count=count+1000;
                  }
                  strcat(path,ent->d_name);
                  printf("%d\n",getpid());              
                  setting(path,vv,count,pid);
                  free(path);           
                  *glob_var = *glob_var+1;
                  munmap(glob_var, sizeof *glob_var);
                  break;
              }
          }//ol 
      }//while 
      closedir(dir);
      while(pid = waitpid(-1,NULL,0)){
          if(errno = ECHILD){
              break;
          }           
   }//else
}
}

// mergesorting 
void sort(char* cdi,char* name,char* column)
{
    char* tmp_str = malloc(100 * sizeof(char));
    char* path = malloc(sizeof(char*) * 1000);
    char* new_csv = malloc(1000 * sizeof(char*));
    strcpy(tmp_str,column);
    char* tok = strtok(tmp_str,",");
    int num_tok = 0; //count tokens
    while(tok != NULL){
        if(num_tok != 0){
          strcpy(path,cdi);
          strcat(path,"/");
          strcat(path,new_csv);
        }else{
            strcpy(path,cdi);
            strcat(path,"/");
            strcat(path,name);
            strcat(path,".csv");
        }    
    FILE *fp = fopen(path, "r");
    fseek(fp,0,SEEK_END);
    int len = ftell(fp);
    
    if(len == 0){
        fclose(fp);
        return;
    }else{
      //finding if the number of column is correct
      char* column_name = malloc(1000 * sizeof(char*) +1);      
      fseek(fp,0,SEEK_SET); //reset to the beginning
      fgets(column_name,1000,fp);
      for(i = 0;column_name[i];column_name[i] == ',' ? i++ : *column_name++);
      if(i != 27){
          fclose(fp);
          return;
      }else{
          fseek(fp,0,SEEK_SET); //reset to the beginning
      }  
    }
    int record= 0;
    int max = 1000;//max_row
    CSVFILE *data = malloc(max * sizeof(*data)+1);
    char** input = malloc(max * sizeof(char*)+1);
    char hello [1000];
    for(i=0;i < max;i++)
    {
       input[i] = malloc(sizeof(char*) + num);
    }
    i = 0;
    while(fgets(input[i],1000,fp)){
        data[record].quote = 0;
        data[record].color = parse_string(input[i]);
        data[record].director_name = parse_string(input[i]);
        data[record].num_critic_for_reviews = parse_string(input[i]);
        data[record].duration = parse_string(input[i]);
        data[record].director_facebook_likes = parse_string(input[i]);//5
        data[record].actor_3_facebook_likes = parse_string(input[i]);
        data[record].actor_2_name = parse_string(input[i]);
        data[record].actor_1_facebook_likes = parse_string(input[i]);
        data[record].gross = parse_string(input[i]);
        data[record].genres = parse_string(input[i]);//10
        data[record].actor_1_name = parse_string(input[i]);
        data[record].movie_title = parse_string(input[i]);
        if(prev_quote == 1){
            data[record].quote = 1;
        }     
        data[record].num_voted_users = parse_string(input[i]);
        data[record].cast_total_facebook_likes = parse_string(input[i]);
        data[record].actor_3_name = parse_string(input[i]);//15
        data[record].facenumber_in_poster = parse_string(input[i]);
        data[record].plot_keywords = parse_string(input[i]);
        data[record].movie_imdb_link = parse_string(input[i]);
        data[record].num_user_for_reviews = parse_string(input[i]);
        data[record].language = parse_string(input[i]);//20
        data[record].country = parse_string(input[i]);
        data[record].content_rating = parse_string(input[i]);
        data[record].budget = parse_string(input[i]);
        data[record].title_year = parse_string(input[i]);
        data[record].actor_2_facebook_likes = parse_string(input[i]);//25
        data[record].imdb_score = parse_string(input[i]);
        data[record].aspect_ratio = parse_string(input[i]);
        data[record].movie_facebook_likes = parse_string(input[i]); 
        data[record].row = record;
        data[record].check = 0; 
        record++;i++;
        first_token = 0;
        //reallocating memory every 1000 rows
        if(record == max)
        {
          int tmp = max;
          int count = i;
          max += 1000;
          data = realloc(data,max*sizeof(*data)+1);
          input = (char**)realloc(input, max *sizeof(char*));
          for(; count < max;count++){
              input[count] = malloc(sizeof(char*) + 1000);
          }
        }
    }
    //MERGESORTING
    unsorted = malloc(max*sizeof(char*)+1);
    unsorted_num = malloc(max * sizeof(int)+1);
    unsorted_num_f = malloc(max * sizeof(float)*max);
    row_num = malloc(max*sizeof(int)+1);
    row_num[0] = 0;
    int tmp;
    int ptr = 1;
    if(strcmp(tmp_str,"color") == 0){//////////////////color
      for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].color);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].color) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"director_name") == 0){///////////////director_name
      for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].director_name);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].director_name) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"num_critic_for_reviews") == 0){////////////////num_critic_for_review
      
     for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].num_critic_for_reviews,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].num_critic_for_reviews);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].num_critic_for_reviews,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].num_critic_for_reviews) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
        
      }
    }else if(strcmp(tmp_str,"duration") == 0){//////////////duration
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].duration,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].duration);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].duration,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].duration) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
      
    }else if(strcmp(tmp_str,"director_facebook_likes") == 0){/////////////director_facebook_likes
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].director_facebook_likes,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].director_facebook_likes);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].director_facebook_likes,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].director_facebook_likes) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
      
    }else if(strcmp(tmp_str,"actor_3_facebook_likes") == 0){///////actor_3_facebook_likes
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].actor_3_facebook_likes,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].actor_3_facebook_likes);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].actor_3_facebook_likes,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].actor_3_facebook_likes) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
      
    }else if(strcmp(tmp_str,"actor_2_name") == 0){////////////actor_2_name
      
      for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].actor_2_name);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].actor_2_name) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"actor_1_facebook_likes") == 0){///////////////actor_1_facebook_likes
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].actor_1_facebook_likes,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].actor_1_facebook_likes);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].actor_1_facebook_likes,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].actor_1_facebook_likes) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"gross") == 0){///////////////gross
      
       for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].gross,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].gross);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].gross,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].gross) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"genres") == 0){///////////////genres
      
      for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].genres);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].genres) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"actor_1_name") == 0){///////////////actor_1_name
      
     for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].actor_1_name);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].actor_1_name) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"movie_title") == 0){///////////////movie_title
      
      for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].movie_title);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].movie_title) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"num_voted_users") == 0){///////////////num_voted_users
    
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].num_voted_users,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].num_voted_users);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].num_voted_users,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].num_voted_users) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
     
    }else if(strcmp(tmp_str,"cast_total_facebook_likes") == 0){///////////cast_total_facebook_likes
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].cast_total_facebook_likes,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].cast_total_facebook_likes);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].cast_total_facebook_likes,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].cast_total_facebook_likes) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"actor_3_name") == 0){///////////////actor_3_name
      
      for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].actor_3_name);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].actor_3_name) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"facenumber_in_poster") == 0){///////////////facenumber_in_poster
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].facenumber_in_poster,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].facenumber_in_poster);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].facenumber_in_poster,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].facenumber_in_poster) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"plot_keywords") == 0){///////////////plot_keywords
      
     for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].plot_keywords);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].plot_keywords) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"movie_imdb_link") == 0){///////////////movie_imdb_link
      
     for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].movie_imdb_link);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].movie_imdb_link) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"language") == 0){///////////////language
    for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].language);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].language) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"country") == 0){///////////////country
       for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].country);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].country) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"content_rating") == 0){///////////////content_rating
       for(i = 0;i < record;i++)
      {
        unsorted[i] = malloc(sizeof(char*)+600);
        strcpy(unsorted[i],data[i].content_rating);
      }
      mergechar(unsorted,1,record-1);
      for(i = 0;i < record;i++){
        for(tmp = 1; tmp<record;tmp++){
          if(strcmp(unsorted[i],data[tmp].content_rating) == 0 && data[tmp].check == 0){
            data[tmp].check = 1;
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"budget") == 0){///////////////budget
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].budget,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].budget);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].budget,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].budget) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"title_year") == 0){///////////////title_year
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].title_year,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].title_year);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].title_year,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].title_year) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"actor_2_facebook_likes") == 0){///////////////actor_2_facebook_likes
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].actor_2_facebook_likes,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].actor_2_facebook_likes);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].actor_2_facebook_likes,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].actor_2_facebook_likes) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"imdb_score") == 0){///////////////imdb_score
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].imdb_score,"") == 0){
          unsorted_num_f[i-1] = -1;
        }else
          unsorted_num_f[i-1]=atof(data[i].imdb_score);
      }
      mergechar_f(unsorted_num_f,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if((float)atof(data[tmp].imdb_score) == (float)unsorted_num_f[i] && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"aspect_ratio") == 0){///////////////aspect_ratio
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].aspect_ratio,"") == 0){
          unsorted_num_f[i-1] = -1;
        }else
          unsorted_num_f[i-1]=atof(data[i].aspect_ratio);
      }
      mergechar_f(unsorted_num_f,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
        if(unsorted_num_f[i] == -1 && strcmp(data[tmp].aspect_ratio,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if((float)atof(data[tmp].aspect_ratio) == (float)unsorted_num_f[i]&& data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else if(strcmp(tmp_str,"movie_facebook_likes") == 0){///////////////movie_facebook_likes
      
      for(i = 1;i < record;i++)
      {
        if(strcmp(data[i].movie_facebook_likes,"") == 0){
          unsorted_num[i-1] = -1;
        }else
          unsorted_num[i-1]=atoi(data[i].movie_facebook_likes);
      }
      mergechar_i(unsorted_num,0,record-1);
      for(i = 0;i < record;i++)
      {
        for(tmp = 1; tmp<record;tmp++)
        {
          if(unsorted_num[i] == -1 && strcmp(data[tmp].movie_facebook_likes,"") == 0 && data[tmp].check == 0){
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
          }else if(atoi(data[tmp].movie_facebook_likes) == unsorted_num[i] && data[tmp].check == 0)
          {
            data[tmp].check = 1; //check if it is already used
            row_num[ptr++] = data[tmp].row;
            break;
          }
        }
      }
    }else{///////////////error
       fprintf(stderr,"%s ","Not a existing column name!\n");
       //free spaces
       for(i = 0; i < 1000; i++){
         free(input[i]);
         free(unsorted[i]);
       }
       free(path);
       free(row_num);
       free(unsorted_num);
       free(unsorted);
       free(data);
       free(input);
       return;
   
   }
   
    //create new CSVfil
    FILE *new_fp;
    if(num_tok == 0){
       strcpy(new_csv,new_dir);
       strcat(new_csv,name);       
       strcat(new_csv,"-sorted-");
       strcat(new_csv,column);
       strcat(new_csv,".csv");
       new_fp = fopen(new_csv,"w+");  
    }else{
        new_fp = fopen(new_csv,"w+");
    }
    
   
    
    //print 
    for(i = 0; i < record;i++)
    {
      fprintf(new_fp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,",data[row_num[i]].color,data[row_num[i]].director_name,data[row_num[i]].num_critic_for_reviews,data[row_num[i]].duration,data[row_num[i]].director_facebook_likes,data[row_num[i]].actor_3_facebook_likes,data[row_num[i]].actor_2_name,data[row_num[i]].actor_1_facebook_likes,data[row_num[i]].gross,data[row_num[i]].genres,data[row_num[i]].actor_1_name);
      if(data[row_num[i]].quote ==1)
      {
        fprintf(new_fp,"\"%s\",%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",data[row_num[i]].movie_title,data[row_num[i]].num_voted_users,data[row_num[i]].cast_total_facebook_likes,data[row_num[i]].actor_3_name,data[row_num[i]].facenumber_in_poster,data[row_num[i]].plot_keywords,data[row_num[i]].movie_imdb_link,data[row_num[i]].num_user_for_reviews,data[row_num[i]].language,data[row_num[i]].country,data[row_num[i]].content_rating,data[row_num[i]].budget,data[row_num[i]].title_year,data[row_num[i]].actor_2_facebook_likes,data[row_num[i]].imdb_score,data[row_num[i]].aspect_ratio,data[row_num[i]].movie_facebook_likes);
      }else{
        fprintf(new_fp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",data[row_num[i]].movie_title,data[row_num[i]].num_voted_users,data[row_num[i]].cast_total_facebook_likes,data[row_num[i]].actor_3_name,data[row_num[i]].facenumber_in_poster,data[row_num[i]].plot_keywords,data[row_num[i]].movie_imdb_link,data[row_num[i]].num_user_for_reviews,data[row_num[i]].language,data[row_num[i]].country,data[row_num[i]].content_rating,data[row_num[i]].budget,data[row_num[i]].title_year,data[row_num[i]].actor_2_facebook_likes,data[row_num[i]].imdb_score,data[row_num[i]].aspect_ratio,data[row_num[i]].movie_facebook_likes);
      }
    }
    fclose(fp);
    fclose(new_fp);
    for(i = 0; i < 1000; i++){
          free(input[i]);
          free(unsorted[i]);
    }
    free(input);
    tok = strtok(NULL,",");
    num_tok++;
    }
    //free spaces
        
        free(row_num);
        free(unsorted_num);
        free(unsorted);
        free(data);
        free(path);
        
 }
 
 //tokenizing the string
 char* parse_string(char* array){
     char *endToken;
     if(array == NULL){
       return NULL;
     }
     
     if(first_token == 0){
       endToken = strchr(array, ',');
       if (endToken)
         *endToken = '\0'; 
       first_token = 1;
     }else{
       array = endToken+1;
       if(array[0] == '"')
       {
         prev_quote = 1;
         endToken = strchr(array,'"');
         array = endToken+1;
         endToken = strchr(array,'"');
       }else{
         if(prev_quote == 1){
             endToken = strchr(array, ',');
             array = endToken + 1;
             prev_quote = 0;
         }
         endToken = strchr(array, ',');
       }
         if (endToken)
           *endToken = '\0';
     }
     return array;
}

