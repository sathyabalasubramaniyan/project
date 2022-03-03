#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/statvfs.h>
#include <sys/mount.h>
#include <openssl/md5.h>
#include <curl/curl.h>
#include<ctype.h>

#define MAX_WORDS 1000

//function declaration
int webpage_to_text(char *url, char *filename);
static size_t file_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
void readfile(char *filename);

//struct variable
struct fileptr 
{
	const char *filename;
	FILE *stream;
};

int main(int argc, char **argv) 
{
        
        char *path;
        path="/home/user/Desktop/curlex/test4.html"; //path of the file to be downloaded
	webpage_to_text("https://www.example.com/",path);//address of the path
        // it downloads  html file   of webpage
        readfile(path);     
     	return 0;
}
void readfile(char *filename)//to read the  saved data 
{
    int body=0,tag=1;
    FILE *fptr;
    char arr[MAX_WORDS],arr2[MAX_WORDS];
    char *p;
    int i=0,j=0,l=0;
    char *word = (char*)malloc(40);
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("\nUnable to open file.\n");
    }
    else
    {
     printf("\nFILE IS OPENED\n");
    for (int i=0; i<MAX_WORDS;i++) 
    {
        arr[i] = 0;
    }
    while(1)
    {   
        if(fscanf(fptr, "%s", word) == EOF)
        { 
        break;
        }
        if(body==1)
       {
        if(strstr(word,"<"))
        {
          if(strstr(word,"</p>"))
           {
            //printf("  paraend\t");
            p=strstr(word,"</p>");
            for(i=0;word[i]!=*p;i++)
            {
            arr2[i]=word[i];
            }
            arr2[i]='\0';
            printf("\narr: %s , %d",arr2,i);
           // p=strstr(word,"</p>");           //  printf("%s\t",p);
           for (int i=0; i<MAX_WORDS;i++) 
           {
           arr2[i] = 0;
          }
        }
        else if(strstr(word,"<p>"))
         {
          printf("parenstart");
          p=strstr(word,"<p>");
          printf("\n%s\n",word);
          for( j=0;word[j]!='>';j++)
          {
          printf("num:%d\n",j);
          }
          printf("num:%d,%c\n",j,word[j]);
          while(isalpha(word[++j]))
          {
           printf("num:%d,%c\n",j,word[j]);
           arr2[l]=word[j];
           l++;
           }
         arr2[l]='\0';
         printf("\narr: %s , %d",arr2,l);
          for (int i=0; i<MAX_WORDS;i++) 
          {
           arr2[i] = 0;
          }
         }
        else if(strstr(word,"</h"))
        {
        // printf("  h1end\t");
         p=strstr(word,"</h");
         for(i=0;word[i]!=*p;i++)
         {
           arr2[i]=word[i];
         }
         arr2[i]='\0';
          printf("\narr: %s , %d",arr2,i);
         // p=strstr(word,"</p>");
        //  printf("%s\t",p);
           for (int i=0; i<MAX_WORDS;i++)
            {
           arr2[i] = 0;
           }
           i=0;
        }
         else if(strstr(word,"<h"))
         {
 //        printf("parenstart");
   //      p=strstr(word,"<h");
     //    printf("%s\t",p);
         for(j=0;word[j]!='>';j++);
          while(word[++j]!='\n')
          {
           arr2[l]=word[j];
           l++;
           }
         arr2[l]='\0';
         printf("\narr: %s %d",arr2,i);
         for (int i=0; i<MAX_WORDS;i++) 
          {
           arr2[i] = 0;
          }
         }
       }//tag
      else{
       printf("%s\n",word);
       }
       }//body

        if(strstr(word,"</head>"))//ignore the metadata
        { 
          body=1;
                }
   }//while
  }//elseend
} 

int webpage_to_text(char *url, char *filename) 
{
        CURL *curl = NULL;//
        CURLcode res; //
        struct fileptr file; // struct variable
        file.filename = filename; //initialise filepath into filename
        file.stream = NULL; //stream as null
        curl_global_init(CURL_GLOBAL_DEFAULT);
        //sets up the global variables ,curl_global_default initialise both ssl and win32
        curl = curl_easy_init();
        //it returns a pointer to easy interface
        if(curl) //non-zero value
        {
                printf("\n url is %s, filename is %s\n", url, filename);
                curl_easy_setopt(curl, CURLOPT_URL, url); //it sets the url address
                // it sets the callback function as file_fwrite
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, file_fwrite); //This callback function(file_fwrite) gets called by libcurl as soon as there is data received that needs to be saved. 
                /* Set a pointer to our struct to pass to the callback */
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file); 
                // set address of struct file as write data in callback function and pass into callback function in 4th argument by typecasted to void pointer
                /*curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                //set the verbose information(infomation about  process) to display*/               
                res = curl_easy_perform(curl);//it perform the   operation and returns CURLE_OK if all done 
                curl_easy_cleanup(curl);//erases memory used by function interface
                if(CURLE_OK != res)  //error
                {
                        fprintf(stderr, "\n error: %d\n", res);
                        if(file.stream)//if file is  opened
                                fclose(file.stream); 
                        return -1;
                }
        }

        if(file.stream) //if file is  opened
        {
              fclose(file.stream); //close the file
        }
        curl_global_cleanup();
        //clean the initialse made by curl_global_init()
        printf("\nhtml content is downloaded ");       
        return 0;
}


static size_t file_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
 {
        struct fileptr *out=(struct fileptr *)stream; // convert void pointer into struct pointer and assign into other struct pointer 
        if(out && !out->stream)  //if stream=nil 
         {      
                out->stream = fopen(out->filename, "wb"); //fopen returns a pointer to path of the  file to write and store that pointer in stream
               // printf("out :%p",out->stream);
                if(!out->stream) //if is null (not able to open the file)
                {
                return -1;      
                }
         }

         return fwrite(buffer, size, nmemb, out->stream); //if stream is not empty 
         //fwrite writes the array of  elements pointed by buffer of size (size(bytes)*nmen(no of elements))  in location pointed by out->stream
} 
