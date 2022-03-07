//wepscaper for www,example.com


//headerfiles
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
#include <ctype.h>

//macros
#define MAX_WORDS 100
#define MAX 30
#define MAXLEN 1000

//global variable
char *arr[MAX_WORDS];

//function declaration
int  webpage_to_text(char *url, char *filename);
static size_t file_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
void readfile(char *filename);
void wordsintofile(void);
void occurences(char *path);

//struct variable
struct fileptr
{
	const char *filename;
	FILE *stream;
};

int main(int argc, char **argv)
{
        char *path;
        path="/home/user/Desktop/test6.html"; //path of the file to be downloaded
	webpage_to_text("https://www.example.com/",path);//address of the path
        // it downloads  html file   of webpage
        readfile(path);//it stores displayed words in website
        wordsintofile();//it copies the stored word into a new file and count the words in new file
     	return 0;
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
               // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                //set the verbose information(infomation about  process) to display*
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
        printf("\nHTML FILE OF THE WEBSITE IS DOWNLOADED");
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
void readfile(char *filename)//to read the  saved data
{
    int h=0,body=0,j=0,l=0;
    FILE *fptr;
   // char *arr[MAX_WORDS];
    char a[MAX],*p;
    char *word = (char*)malloc(40);
    fptr = fopen(filename, "r");//open the file in read mode
    if (fptr == NULL)//error
    {
        printf("\nUnable to open file.\n");
    }
    else
    {
     printf("\n HTML FILE IS OPENED\n");
    while(1)//until EOF
    {
        if(fscanf(fptr, "%s", word) == EOF)
        {
        break;
        }
        if(body==1)//if body tag starts
        {
        if(strstr(word,"<")||strstr(word,">"))//if it has any tag name
        {
          //printf("\n word :%s",word);
          if(strstr(word,"<p>"))//if the line starts with para tag
          {
         // printf("\nparastart");
          for(j=0;word[j]!='>';j++);//find the close angle bracket for para tag
         // {
         // printf("num:%d\n",j);
         // }
          while(isalpha(word[++j]))//increment j and check the each character of word as alphabet
          {
          // printf("num:%d,%c\n",j,word[j]);
           a[l]=word[j];//if alphabet , store that character into char array
           l++;
           }
           a[l]='\0';//string terminator
           arr[h]=strdup(a);//duplicate the char array into one of the  pointer arrays
           h++;//increment the index of array of char pointers
          // printf("\na[]: %s , %d",a,l);
           for (int i=0; i<MAX;i++) //initialise 0 to char array for later use
           {
           a[i] = 0;
           }
           l=0;
         }
         else if(strstr(word,"</p>"))//if the line contains end para tag
         {
            // printf("\nparaend\t");
            p=strstr(word,"</p>");
            for(j=0;word[j]!=*p;j++)//store the characters into char array untill para end tag
            {
            a[j]=word[j];
            }
            a[j]='\0';//string terminator
            arr[h]=strdup(a);//duplicate the char array into one of the  pointer array
             h++;//increment the index of array of char pointers
          //  printf("\narr: %s , %d",a,j);
            for (int i=0; i<MAX;i++) //initialise 0 to char array for later use
            {
            a[i] = 0;
            }
            l=0;
        }
        else if(strstr(word,"<h"))//if the line contains h1,h2,h3,h4,h5,h6  tags
         {
          l=0;
          for(j=0;word[j]!='>';j++);//increment the characters of word until the close angle bracket of h tag approaches
          j++;
          while(isalpha(word[j]))//check the character as alphabet
          {
          // printf("\nl:%d,word:%c",l,word[j]);
           a[l]=word[j];//store the  all alphabetic characters into a char array
           l++;
           j++;
          }
          a[l]='\0';//string terminator
          arr[h]=strdup(a);//duplicate the char array into one of the  pointer array
          h++;//increment the index of array of char pointers
            // printf("\narr: %s,%d",a,l);
          for (int i=0; i<MAX;i++)
          {//initialise 0 to char array for later use
           a[i] = 0;
          }
          l=0;
         }
        else if(strstr(word,"</h")&&!strstr(word,"</html>"))//if the line contains heading tag end not html end tag
        {
        // printf("  h1end\t");
         p=strstr(word,"</h");
         for(l=0;word[l]!=*p;l++)//store the characters into a char array
         {
           a[l]=word[l];
         }
         a[l]='\0';//string terminator
         arr[h]=strdup(a);//duplicate the char array into one of the  pointer array
         h++;//increment the index of array of char pointers
         // printf("\narr: %s , %d",a,l);
         for (int i=0; i<MAX;i++)
         {
           a[i] = 0;//initialise 0 to char array for later use
         }
           l=0;
        }
       else if(strstr(word,"href"))//to extract the link word which shows the links
        {
        for(l=0;word[l]!='>';l++);//loop over the word until it matches with close angle brackets
        l++;
        j=0;
        while(isalpha(word[l]))//extract the link word
       {
         a[j]=word[l];//store into char array
        // printf("word:%c",word[l]);
        l++;
       j++;
       }
       a[j]='\0';//string terminator
        arr[h]=strdup(a);//duplicate the char array into one of the  pointer array
        h++;//increment the index of array of char pointers
        }
       }
       else//if the line doesnot has a tag in body tag
        {
         arr[h] = strdup(word);//duplicate the char array into one of the  pointer array
         //printf("\ncopy:%s",arr[h]);
         h++;//increment the index of array of char pointers
         }
        }
        if(strstr(word,"</head>"))//ignore the metadata
        {
          body=1;//if body tag starts
        }
    }//while end
   arr[h]=NULL;
   }
 printf("THE DISPLAYABLE WORDS ARE STORED ");
 fclose(fptr);
}


void wordsintofile(void)
{

    FILE *fptr1;
    char *charfile;
    charfile="/home/user/Desktop/newfile.txt";
    fptr1 = fopen(charfile, "w");//create a new file in that path in write mode
    if (fptr1 == NULL)//not able to open
    {
        printf("ERROR!");
        exit(1);
   }
   printf("\nNEW FILE IS CREATED\n");
   printf("THE WORDS THAT ARE WRIITEN INTO A NEW FILE :\n");
   for(int i=0;i<MAX_WORDS&&arr[i]!=NULL;i++)
   {
    fprintf(fptr1,"%s",arr[i]);//write the arr[i] into file
    fprintf(fptr1,"%s","\n");//write a newline character
    printf("\nword:%s",arr[i]);
   }
   fclose(fptr1);
   occurences(charfile);
}

void occurences(char *path)
{
    FILE *fptr2;
    int  i,len,index,flag,k,j;
    char result[MAXLEN][30];//two dimensional array
    char word[30],word2[30];
    int  count[MAXLEN];
    fptr2= fopen(path, "r");//open the file in read mode
    if (fptr2 == NULL)//error
    {
        printf("Unable to open file.\n");
        exit(1);
    }// Initialize  countarray  to 0
    for (i=0; i<MAX_WORDS; i++)
    {
        count[i] = 0;
    }
   index = 0;
   while (fscanf(fptr2, "%s", word) != EOF)//get each line of a file in char array word
   {
      // printf("word:%s",word2);
      // j=0;
      // k=0;
      // len = strlen(word2);//length of the word
      // printf("\tlen: %d",len);
       //while(len--)
       // {
    //   word[j++]=tolower(word2[k++]);}
      //  printf("len%d",len);
      //x  word[j]='\0';
       // printf("work :%s\n",word);
        flag=1;
        for (i=0;i<index &&(flag==1);i++)//loop until flag sets to 0 and i greater than index
        {
            if (strcmp(result[i], word) == 0)//it compares the word with  each element  in result array
            {
              flag=0;//if equals, set flag to 0;
            }
        }
        if (flag)//if the word comes first ,flag =1
        {
            strcpy(result[index], word);//copy the word into result  array
            count[index]++; //increment the count array
            index++;
        }
        else//flag =0 ,already has the word
        {
            count[i - 1]++;//increment the count of index of the result array
        }
    }
    fclose(fptr2);//closes the file
    printf("\n");
    for (i=0; i<index; i++)
    {
        printf("%-20s => %d\n", result[i], count[i]);
    }
 
   return;
}
