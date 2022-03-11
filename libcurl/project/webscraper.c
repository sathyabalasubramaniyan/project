
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
#define MAX_WORDS 1000
#define MAXLEN 1000

//function declaration
int  webpage_to_text(char *url, char *filename);
static size_t file_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);
void readfile(char *path1);
void occurences(char *path);

//struct variable
struct fileptr
{
	const char *filename;
	FILE *stream;
};

int main(int argc, char **argv)
{
        char *path,*link;
        if(argc==2)
        {
          link=*++argv;
        }
        else
        {
         link="https://www.example.com/";
        }
        path="/home/user/Desktop/test13.html"; //path of the file to be downloaded
        webpage_to_text(link,path);
//"https://www.geeksforgeeks.org/",path);
//"https://www.restapitutorial.com/",path);
//"https://realpython.github.io/fake-jobs/",path);
//"https://www.tutorialspoint.com/c_standard_library/c_function_fputs.htm",path);
///https://www.tutorialspoint.com/c_standard_library/index.html",path);//https://www.example.com/",path);//address of the path
        // it downloads  html file   of webpage
        readfile(path);//it stores displayed words in website
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
        printf("\nHTML FILE OF THE WEBSITE IS DOWNLOADED in %s\n",filename);
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

void readfile(char *path1)
{
        char c,c1;
        char word[1000];
        char *path2="/home/user/Desktop/temp.txt";
        char *path3="/home/user/Desktop/result.txt";
        FILE *fptr1=fopen(path1,"r");
        FILE *fptr2=fopen(path2,"w");
        if(fptr1==NULL)
        printf("not able to open the file");
        if(fptr2==NULL)
        printf("not able to open the file");
        do
        {
        c=fgetc(fptr1);
        while(c=='<')//open tag
        {
         c=fgetc(fptr1);
         // printf("%c",c};
         if(isalpha(c))
         {
          fputc('\n',fptr2);
         }
         if(c=='!')
         {
           if((c=fgetc(fptr1))=='-')
           {
            if((c=fgetc(fptr1))=='-')
             {
                 c=fgetc(fptr1);
                 for(;c!='-';c=fgetc(fptr1))
                 {
                 //  printf("%c",c);
                  }
            //  printf("fun:");
             }
           }
         }
         if(c=='s')//for <script tag
         {
          if((c=fgetc(fptr1))=='c')
          {
           if((c=fgetc(fptr1))=='r')
             if((c=fgetc(fptr1))=='i')
             {
             // printf("%c",c);
              while(1)
              {
               while(c!='<')//pass until </script
               {
               // printf("%c",c);
                c=fgetc(fptr1);
               }
               if((c=fgetc(fptr1))=='/')
                if((c=fgetc(fptr1))=='s')
                 if((c=fgetc(fptr1))=='c')
                  break;
              }
           }
         }
         else//for <style tag
         {
          if(c=='t')
           if((c=fgetc(fptr1))=='y')
           {
            while(1)
            {
             while(c!='<')//pass until </style
             { 
              c=fgetc(fptr1);
             }
             if((c=fgetc(fptr1))=='/')
              if((c=fgetc(fptr1))=='s')
               if((c=fgetc(fptr1))=='t')
                break;
            }
          }
        }
        }
      while(c!='>')//find the close tag
       { c=fgetc(fptr1);}
       //printf("%c",c);
       c=fgetc(fptr1);//next character
      }
      if(c=='&')//for &lt; or &gt;
      {
     //  printf("\n");
       c=fgetc(fptr1);//next character
       if(c=='c')
       {
        while(c!='y')
        {
         c=fgetc(fptr1);
        }
        }
       else
       {
       while(c!=';'&&c!=' ')//break if it is &copy or one symbol(&)
       {
       // printf("%c",c);
        c=fgetc(fptr1);
       }
       }

      }

      else if(c=='-'||c==':'||c=='|'||c==';'||c=='/'||c=='('||c==')')//for invalid symbols
      {
      fputc('\n',fptr2);//print newline
      }
      else if(c==' ')//if string terminates
      {
      fputc('\n',fptr2);//print newline
      }
      else
      {
       //printf("%c",c);
       fputc(c,fptr2);//pass that character into newfile
      }
     }//do
     while(c!=EOF);
     fclose(fptr1);
     fclose(fptr2);
     printf("\nTHE DISPLAYABLE WORDS WITH SOME INVALID SYMBOLS ARE STORED in %s\n",path2);
     FILE *f=fopen(path2,"r");
     FILE *f1=fopen(path3,"w");
     if(f1==NULL)
      printf("not able to open the file");
     while(fscanf(f, "%s", word) != EOF)//for removing extra spaces 
     {
      if(strstr(word,"₹"))//for removing rupee symbol
      {
      }
      else if((!strcmp(word,"."))||(!strcmp(word,"00"))||(!strcmp(word,":"))||(!strcmp(word,"₹"))||(!strcmp(word,".."))||(!strcmp(word,"<"))||(!strcmp(word,">")))//removing invalid symbols
      {
      }
      else
      {
       fprintf(f1,"%s",word);//pass that word into new file
       fputc('\n',f1);//pass newline
      }
     }
     fclose(f);
     fclose(f1);
      printf("\nTHE DISPLAYABLE WORDS OF THE WEBSITE ARE STORED  in %s\n",path3);
     occurences(path3);//for finding occurences 
}
void occurences(char *path)
{
    FILE *fptr2,*fptr;
    int  i,len,index,flag,j,a,b,len2;
    char result[MAXLEN][50];//two dimensional array
    char word[50],word2[50];
    int  count[MAXLEN],arr[MAXLEN];
    char *path4="/home/user/Desktop/finalresult.txt";

    fptr2= fopen(path, "r");//open the file in read mode
    fptr=fopen(path4,"w");
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
   while (fscanf(fptr2, "%s", word2) != EOF)//get each line of a file in char array word
   {
        // printf("%s",word2);
        len=strlen(word2);
        len2=len;
        i=0;
        while(len--)
        {
        word[i]=tolower(word2[i]);//it converts into lowercase 
        i++;
        }
        word[i]='\0';
         if (ispunct(word[len2 - 1]))
            word[len2-1] = '\0';

        // printf("\t%s",word);
        //printf("\n");
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
//    fclose(fptr2);//closes the file
   //    for (i=0; i<index; i++)
   //  {
    //    printf("%-20s => %d\n", result[i], count[i]);
   // }
    for(i=0;i<index;i++)
     {
     arr[i]=i;//store the  index value into array 
     }
     for (i = 0;i<index;++i)
     {
        for (j=i+1;j<index;++j)
        {
          if (count[i]<count[j])//if the count of array is greater , swap the numbers and also their index values
          {
               a=count[i];
               b=arr[i];
               count[i]=count[j];
               count[j]=a;
               arr[i]=arr[j];//it swaps the index value
               arr[j]=b;
            }
            // for (int k = 0; k < n; ++k)
           //  {
            // printf("%d\n", arr[k]);
            // }
            // printf("\n");
         }
      }
      printf("\nTHE WORDS ARE RANKED  ACCORDING TO THE OCCURENCES\n");
      for (i = 0; i < index; ++i) //it prints the value as order in arr[i]
      {
           printf("%-20s==> %d\n",result[arr[i]],count[i]);
           fprintf(fptr,"%s",result[arr[i]]);
           fputc('\t',fptr);
           fprintf(fptr,"%d",count[i]);
           fputc('\n',fptr);
      }
      fclose(fptr);
      fclose(fptr2);
      printf("\nTHE WORDS AND THEIR COUNTS ARE SORTED AND STORED IN  %s\n",path4);
      return;
}
