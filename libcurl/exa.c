#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX_WORDS 100
#define MAX 30

void readfile(char *filename);

int main()
{
        char *path;
        path="/home/user/Desktop/curlex/6_1ex.c.save";
        readfile(path);
        return 0;
}

void readfile(char *filename)//to read the  saved data 
{  
    int h=0,body=0,j=0,l=0;
    FILE *fptr;
    char *arr[MAX_WORDS];
    char a[MAX],*p;
    char *word = (char*)malloc(40);
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("\nUnable to open file.\n");
    }
    else
    {
     printf("\nFILE IS OPENED\n");
    while(1)
    {   
        if(fscanf(fptr, "%s", word) == EOF)
        { 
        break;
        }
        if(body==1)
        {
        if(strstr(word,"<")||strstr(word,">"))
        { 
          printf("\ntag");
          printf("\n word :%s",word);
          if(strstr(word,"<p>"))
          {
//          printf("\nparastart");
          for(j=0;word[j]!='>';j++)
          {
         // printf("num:%d\n",j);
          }
         // printf("num:%d,%c\n",j,word[j]);
          while(isalpha(word[++j]))
          {
          // printf("num:%d,%c\n",j,word[j]);
           a[l]=word[j];
           l++;
           }
           a[l]='\0';
           arr[h]=strdup(a);
           h++;
           printf("\na[]: %s , %d",a,l);
           for (int i=0; i<MAX;i++) 
           {
           a[i] = 0;
           }
           l=0;
         }//p tag
         else if(strstr(word,"</p>"))
         { 

            printf("\nparaend\t");
            p=strstr(word,"</p>");
            for(j=0;word[j]!=*p;j++)
            {
            a[j]=word[j];
            }
            a[j]='\0';
            arr[h]=strdup(a);
            h++;
            printf("\narr: %s , %d",a,j);
            for (int i=0; i<MAX;i++) 
            {
            a[i] = 0;
            }
            l=0;
        }//pendtag
       else if(strstr(word,"<h"))
         {
          l=0;
          for(j=0;word[j]!='>';j++);
          j++;
          while(isalpha(word[j]))
          {
           printf("\nl:%d,word:%c",l,word[j]);
           a[l]=word[j];
           l++;
           j++;

           }
          a[l]='\0';
          arr[h]=strdup(a);
         h++;
             printf("\narr: %s,%d",a,l);
          for (int i=0; i<MAX;i++) 
          {
           a[i] = 0;
          }
         l=0;
         }
        else if(strstr(word,"</h")&&!strstr(word,"</html>"))
        {
         printf("  h1end\t");
         p=strstr(word,"</h");
         for(l=0;word[l]!=*p;l++)
         {
           a[l]=word[l];
         }
         a[l]='\0';
        arr[h]=strdup(a);
       h++;
          printf("\narr: %s , %d",a,l);
         // p=strstr(word,"</p>");
        //  printf("%s\t",p);
           for (int i=0; i<MAX;i++)
            {
           a[i] = 0;
           }
           l=0;
        }//hiend
       else if(strstr(word,"href"))
        {
        for(l=0;word[l]!='>';l++);
        l++;
        j=0;
        while(isalpha(word[l]))
       {
         a[j]=word[l];
         printf("word:%c",word[l]);
        l++;
       j++;
       }       
       a[j]='\0';
       arr[h]=strdup(a);
      h++;
        }//href tag

        }//if tag
        else
        {
//         printf("\n word :%s",word);
         arr[h] = strdup(word);
  //       printf("\ncopy:%s",arr[h]); 
         h++;
    //     printf("\narr[0]:%s",arr[0]);
        }
        }//body 
        if(strstr(word,"</head>"))//ignore the metadata
        { 
          body=1;
        }//meta data
    }//while end
   arr[h]=NULL;
   printf("\n\nprint");
   for(int i=0;i<MAX_WORDS&&arr[i]!=NULL;i++)
   {
   printf("\nwor:%s",arr[i]);
   }
   }//else end
}//function end
