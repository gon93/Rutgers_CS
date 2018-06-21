/*
*********************************************
*  213 System Programming                   *
*  Seong Kon Kim                            *
*  Oct 2,2017                               *
*  sorter.h                                 *
*********************************************
*/


typedef struct csvfile{
    //char** array;
    char* color;
    char* num_critic_for_reviews; 
    char* duration; 
    char* director_facebook_likes; 
    char* actor_3_facebook_likes; 
    char* actor_1_facebook_likes; 
    char* gross; 
    char* num_voted_users; 
    char* cast_total_facebook_likes;
    char* facenumber_in_poster;
    char* budget; 
    char* title_year; 
    char* actor_2_facebook_likes; 
    char* imdb_score; 
    char* aspect_ratio; 
    char* movie_facebook_likes;    
    char* num_user_for_reviews;
    char* director_name;
    char* actor_2_name;  
    char* genres; 
    char* actor_1_name; 
    char* movie_title;  
    char* actor_3_name; 
    char* plot_keywords; 
    char* movie_imdb_link;
    char* language; 
    char* country; 
    char* content_rating;
    int row;
    int check;
    int quote;   
    
}CSVFILE;

typedef struct csvfile CSVFILE;



int first_token,i= 0;
struct CSVFILE *data = NULL;
char** unsorted;
int* unsorted_num;
float* unsorted_num_f;
char* parse_string(char*);
int prev_quote = 0;
int* row_num;
#define num 1000000

//Suggestion: define a struct that mirrors a record (row) of the data set


//Suggestion: prototype a mergesort function
