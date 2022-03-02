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
        path="/home/user/Desktop/curlex/test3.html"; //path of the file to be downloaded
	webpage_to_text("https://www.example.com/",path);//address of the path
        // it downloads  html file   of webpage
        readfile(path);     
     	return 0;
}
void readfile(char *filename)//to read the  saved data 
{
    int flag=0;
    FILE *fptr;
    char arr[MAX_WORDS];
    char *word = (char*)malloc(40);
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("\nUnable to open file.\n");
    }
    else
    {
     printf("\nok");
    for (int i=0; i<MAX_WORDS;i++) //initialise the  char array to store the occurences of the word
    {
        arr[i] = 0;
    }
    while(1)
    {
        if(fscanf(fptr, "%s", word) == EOF)
        { 
        break;
        }
        if(flag==1)
        {
        printf("%s",word); //print only data in body tag
        }

       if(strstr(word,"</head>"))//ignore the metadata
        {
          printf("\n\n\t");
          flag=1;
        }
    }
    printf("\n");
}
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
