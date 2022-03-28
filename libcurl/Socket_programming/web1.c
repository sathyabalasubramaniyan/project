//headerfiles
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>

//macros
#define MAX_WORDS 1000
#define MAXLEN 1000
#define MAX_LEN 10000

//function declaration
void readfile(char *path1);
void occurences(char *path);

int main(int argc, char **argv)
{
 char *path;
 path="/home/user/Desktop/newfile3.txt";        
 
 char buffer[BUFSIZ];
 char request[MAX_LEN]; //http request with domain name
 char *pt=buffer+1;
 int bytes_rece;
 
 //http://www.barc.gov.in/randd/artnp.html
 
 char HTTP_request[] = "GET / HTTP/1.0\r\nHost: %s\r\n\r\n"; //http request  without domain name
 
 int len;
 int socket_fd;
 int port = 80;//port no 
 
 char *hostname=NULL ;
 
 struct protoent *protoent;
 struct hostent *hostent;
 struct sockaddr_in sockaddr_in;
    
 in_addr_t in_addr;
 ssize_t nbytes_total , nbytes_last;
 
 if (argc > 1)
 {
        hostname = argv[1]; 
 }
 len = snprintf(request,MAX_LEN,HTTP_request,hostname);
 if (len >= MAX_LEN)
 {
        fprintf(stderr, "request_line size is exceeded \n");
        exit(EXIT_FAILURE);
 }
 printf("HTTP REQUEST:\n%s\n",request);
 protoent = getprotobyname("tcp");//returns a protoent structure  
 if (protoent == NULL) 
 {
        perror("getprotobyname error");
        exit(EXIT_FAILURE);
 }
 //printf("protocol : %d\n",protoent->p_proto);
 socket_fd = socket(AF_INET, SOCK_STREAM, protoent->p_proto);//create socket for tcp
 if (socket_fd== -1)
 {
        perror("socket error");
        exit(EXIT_FAILURE);
 }
 printf("TCP server socket created.\n");
 hostent = gethostbyname(hostname);//it returns a hostent structure 
 if (hostent == NULL) 
 {
         perror("gethostbyname error");
        exit(EXIT_FAILURE);
 }
 //printf("%s\n", *(hostent->h_addr_list));
 //printf("%s\n",inet_ntoa(*(struct in_addr*)*(hostent->h_addr_list)));
//printf("%d\n",inet_addr(inet_ntoa(*(struct in_addr*)*(hostent->h_addr_list))));
 in_addr = inet_addr(inet_ntoa(*(struct in_addr*)*(hostent->h_addr_list)));
 sockaddr_in.sin_addr.s_addr = in_addr;
 sockaddr_in.sin_family = AF_INET;
 sockaddr_in.sin_port = htons(port);
 
 if(connect(socket_fd,(struct sockaddr*)&sockaddr_in, sizeof(sockaddr_in)) == -1)
 {
        perror("connect error");
        exit(EXIT_FAILURE);
 }
 printf("Connected to the server.\n");
 nbytes_last = write(socket_fd, request,len);//write the request
 if (nbytes_last == -1) 
 {
            perror("write");
            exit(EXIT_FAILURE);
 }
 char *ptr=buffer+1;
 int bytes_received;
 FILE *fp;
 fp=fopen(path,"w");
 
//ignore http status line  and http request header
while(bytes_rece=recv(socket_fd, pt, 1, 0))   //Receiving response from host
{
          //    printf("%s",ptr);
       if(bytes_rece==-1)
       {
           perror("http response error");
           exit(1);
       }
       if((pt[-3]=='\r')  && (pt[-2]=='\n' ) && (pt[-1]=='\r')  && (*pt=='\n' ) )
       {  //HTTP headers are terminated by the sequence \r\n\r\n   
         break; 
       }                
       pt++; 
}

while ((nbytes_total = read(socket_fd, buffer, BUFSIZ)) !=0) //read the message body 
{
        fwrite(buffer,1,nbytes_total,fp);
       // write(STDOUT_FILENO, buffer, nbytes_total);
}
close(socket_fd);
fclose(fp);
printf("\nHTML FILE OF THE WEBSITE IS DOWNLOADED in %s\n",path);        
readfile(path);//it stores displayed words in website
return 0;      	

}
void readfile(char *path1)
{
        char c,c1;
        char word[1000];
        char *path2="/home/user/Desktop/temp3.txt";
        char *path3="/home/user/Desktop/result3.txt";
        FILE *fptr1=fopen(path1,"r");
        FILE *fptr2=fopen(path2,"w");
        if(fptr1==NULL)
        {
        printf("not able to open the file");
        }
        if(fptr2==NULL)
        {
        printf("not able to open the file");
        }
        do
        {
        c=fgetc(fptr1);
        while(c=='<')//open tag
        {
         c=fgetc(fptr1);
         if(isalpha(c))
         {
          fputc('\n',fptr2);
         }
         if(c=='!')//for <!--....--> tag
         {
           if((c=fgetc(fptr1))=='-')
           {
            if((c=fgetc(fptr1))=='-')
             {
                 c=fgetc(fptr1);
                 for(;c!='-';c=fgetc(fptr1))
                 {
                  }
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
              while(1)
              {
               while(c!='<')//pass until </script
               {
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
       { 
       c=fgetc(fptr1);
       }
       c=fgetc(fptr1);//next character
      }
      if(c=='&')//for &lt; or &gt; or &copy  or &
      {
       c=fgetc(fptr1);//next character
       if(c=='c')//&copy
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
     {
      printf("not able to open the file");
     }
     while(fscanf(f, "%s", word) != EOF)//for removing extra spaces
     {
      if(strstr(word,"₹"))//for removing rupee symbol
      {
      }
      else if((!strcmp(word,"."))||(!strcmp(word,"00"))||(!strcmp(word,":"))||(!strcmp(word,"-"))||(!strcmp(word,"_"))||(!strcmp(word,"₹"))||(!strcmp(word,".."))||(!strcmp(word,"<"))||(!strcmp(word,">")))//removing invalid symbols
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
    char *path4="/home/user/Desktop/finalresult3.txt";

    fptr2= fopen(path, "r");//open the file in read mode
    fptr=fopen(path4,"w");
    if (fptr2 == NULL)//error
    {
        printf("Unable to open file.\n");
        exit(1);
    }
    // Initialize  countarray  to 0
    for (i=0; i<MAX_WORDS; i++)
    {
        count[i] = 0;
    }
   index = 0;
   while (fscanf(fptr2, "%s", word2) != EOF)//get each line of a file in char array word
   {
        len=strlen(word2);
        len2=len;
        i=0;
        while(len--)
        {
        word[i]=tolower(word2[i]);//it converts into lowercase 
        i++;
        }
        word[i]='\0';
        if (ispunct(word[len2 - 1]))//check for punctuation mark

        {
            word[len2-1] = '\0';
        }
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
         }
      }
      printf("\nTHE WORDS ARE RANKED  ACCORDING TO THE OCCURRENCES\n");
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
