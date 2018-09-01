#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char* readline()
{
    char* buf;
    size_t bufsize=0;
    getline(&buf,&bufsize,stdin);
    printf("%s\n",buf);
    return buf;

}

char **parse(char * input,char * delim)  
{
    int token_size=100;
    char **token_array=malloc(token_size * sizeof(char*));
    char *tmp;
    int count=0;
    tmp=strtok(input,delim);
    if(!token_array)
    {
        printf("allocation error\n");
        exit(EXIT_FAILURE);
    }
    while(tmp!=NULL)
    {
        token_array[count]=tmp;
        count++;

        if(count>=token_size)
        {
        token_size+=100;
        token_array=realloc(token_array, token_size *sizeof(char*));
        }
        if(!token_array)
        {
             printf("allocation error\n");
            exit(EXIT_FAILURE);
        }

    tmp=strtok(NULL,delim);
    }
    token_array[count]=NULL;
    return token_array;

}

