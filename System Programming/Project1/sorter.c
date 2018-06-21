/*
*********************************************
*  213 System Programming                   *
*  Seong Kon Kim                            *
*  Oct 2,2017                               *
*  sorter.c                                 *
*********************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mergesort.c"
#include "mergesort_num.c"
#include "mergesort_f.c"
#include "sorter.h"

int* main(int argc, char* argv[])
{
  if(strcmp(argv[1],"-c")){
    fprintf(stderr,"%s ","only \"-c\" is available\n");
    return 0;
  }else{
   int record= 0;
   int max = 1000;//max_row
   CSVFILE *data = malloc(max * sizeof(*data)+1);
   char** input = malloc(max * sizeof(char*)+1);
   for(;i < max;i++)
   {
      input[i] = malloc(sizeof(char*) + num);
   }
   i = 0;
   while(fgets(input[i],1000,stdin)){
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
   
   if(strcmp(argv[2],"color") == 0){//////////////////color
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
   }else if(strcmp(argv[2],"director_name") == 0){///////////////director_name
     
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
   }else if(strcmp(argv[2],"num_critic_for_reviews") == 0){////////////////num_critic_for_review
     
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
   }else if(strcmp(argv[2],"duration") == 0){//////////////duration
     
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
     
   }else if(strcmp(argv[2],"director_facebook_likes") == 0){/////////////director_facebook_likes
     
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
     
   }else if(strcmp(argv[2],"actor_3_facebook_likes") == 0){///////actor_3_facebook_likes
     
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
     
   }else if(strcmp(argv[2],"actor_2_name") == 0){////////////actor_2_name
     
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
   }else if(strcmp(argv[2],"actor_1_facebook_likes") == 0){///////////////actor_1_facebook_likes
     
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
   }else if(strcmp(argv[2],"gross") == 0){///////////////gross
     
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
   }else if(strcmp(argv[2],"genres") == 0){///////////////genres
     
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
   }else if(strcmp(argv[2],"actor_1_name") == 0){///////////////actor_1_name
     
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
   }else if(strcmp(argv[2],"movie_title") == 0){///////////////movie_title
     
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
   }else if(strcmp(argv[2],"num_voted_users") == 0){///////////////num_voted_users
   
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
    
   }else if(strcmp(argv[2],"cast_total_facebook_likes") == 0){///////////cast_total_facebook_likes
     
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
   }else if(strcmp(argv[2],"actor_3_name") == 0){///////////////actor_3_name
     
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
   }else if(strcmp(argv[2],"facenumber_in_poster") == 0){///////////////facenumber_in_poster
     
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
   }else if(strcmp(argv[2],"plot_keywords") == 0){///////////////plot_keywords
     
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
   }else if(strcmp(argv[2],"movie_imdb_link") == 0){///////////////movie_imdb_link
     
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
   }else if(strcmp(argv[2],"language") == 0){///////////////language
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
   }else if(strcmp(argv[2],"country") == 0){///////////////country
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
   }else if(strcmp(argv[2],"content_rating") == 0){///////////////content_rating
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
   }else if(strcmp(argv[2],"budget") == 0){///////////////budget
     
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
   }else if(strcmp(argv[2],"title_year") == 0){///////////////title_year
     
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
   }else if(strcmp(argv[2],"actor_2_facebook_likes") == 0){///////////////actor_2_facebook_likes
     
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
   }else if(strcmp(argv[2],"imdb_score") == 0){///////////////imdb_score
     
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
   }else if(strcmp(argv[2],"aspect_ratio") == 0){///////////////aspect_ratio
     
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
   }else if(strcmp(argv[2],"movie_facebook_likes") == 0){///////////////movie_facebook_likes
     
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
       free(row_num);
       free(unsorted_num);
       free(unsorted);
       free(data);
       free(input);
       return 0;
   
   }
   
   //print
   
   for(i = 0; i < record;i++)
   {
     printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,",data[row_num[i]].color,data[row_num[i]].director_name,data[row_num[i]].num_critic_for_reviews,data[row_num[i]].duration,data[row_num[i]].director_facebook_likes,data[row_num[i]].actor_3_facebook_likes,data[row_num[i]].actor_2_name,data[row_num[i]].actor_1_facebook_likes,data[row_num[i]].gross,data[row_num[i]].genres,data[row_num[i]].actor_1_name);
     if(data[row_num[i]].quote ==1)
     {
       printf("\"%s\",%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",data[row_num[i]].movie_title,data[row_num[i]].num_voted_users,data[row_num[i]].cast_total_facebook_likes,data[row_num[i]].actor_3_name,data[row_num[i]].facenumber_in_poster,data[row_num[i]].plot_keywords,data[row_num[i]].movie_imdb_link,data[row_num[i]].num_user_for_reviews,data[row_num[i]].language,data[row_num[i]].country,data[row_num[i]].content_rating,data[row_num[i]].budget,data[row_num[i]].title_year,data[row_num[i]].actor_2_facebook_likes,data[row_num[i]].imdb_score,data[row_num[i]].aspect_ratio,data[row_num[i]].movie_facebook_likes);
     }else{
       printf("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",data[row_num[i]].movie_title,data[row_num[i]].num_voted_users,data[row_num[i]].cast_total_facebook_likes,data[row_num[i]].actor_3_name,data[row_num[i]].facenumber_in_poster,data[row_num[i]].plot_keywords,data[row_num[i]].movie_imdb_link,data[row_num[i]].num_user_for_reviews,data[row_num[i]].language,data[row_num[i]].country,data[row_num[i]].content_rating,data[row_num[i]].budget,data[row_num[i]].title_year,data[row_num[i]].actor_2_facebook_likes,data[row_num[i]].imdb_score,data[row_num[i]].aspect_ratio,data[row_num[i]].movie_facebook_likes);
     }
   }
   
   //free spaces
       for(i = 0; i < 1000; i++){
         free(input[i]);
         free(unsorted[i]);
       }
       free(row_num);
       free(unsorted_num);
       free(unsorted);
       free(data);
       free(input);
       return 0;
   }
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
