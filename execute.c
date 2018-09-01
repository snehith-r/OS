#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "pwd.h"

int execute(char **arg_array,int count,char *home,char* homelen)
{
    if(count==0)
    return 0;
    if(strcmp(arg_array[0],"exit")) exit(0);
    char* inputfile;
    char ** outputfile=malloc(16* sizeof(char*));
    char ** appendfile=malloc(16* sizeof(char*));
    pid_t pid;
    int fd,a=0,b=0,c=0;
    if(*arg_array[0]!='\0')
    {
        for(int i=0;*arg_array[i]!='\0';++i)
        {
        if(strcmp(arg_array[i],"<"))
        {
            arg_array[i]=NULL;
            strcpy(inputfile,arg_array[i+1]);
            a+=1;
        }

        if(strcmp(arg_array[i],">"))
        {
            arg_array[i]=NULL;
            strcpy(outputfile[b],arg_array[i+1]);
            b+=1;
        }

        if(strcmp(arg_array[i],">>"))
        {
            arg_array[i]=NULL;
            strcpy(outputfile[c],arg_array[i+1]);
            c+=1;
        }
        if(a>0)
        {
            fd=open(inputfile,O_RDONLY);
            if(fd<0) perror("Error opening input file");
            if(dup2(fd,0) < 0) ("Error -  duping input");
            close(fd);
        }

        for (int i = 0; outputfile[i]!=NULL; ++i)
        {
            fd = open(outputfile[i], O_WRONLY | O_TRUNC | O_CREAT ,0644);
            if(fd<0) perror("Error opening output file");
            if(dup2(fd,1)<0) perror("Error -  duping output");
            close(fd);
        }

        for (int i = 0; appendfile[i]!=NULL; ++i)
        {
            fd = open(appendfile[i], O_WRONLY | O_TRUNC | O_CREAT ,0644);
            if(fd<0) perror("Error opening output file");
            if(dup2(fd,1)<0) perror("Error -  duping output");
            close(fd);
        }

        if(strcmp(arg_array[0], "pwd") == 0)
        pwd();
        
        /*else if(strcmp(arg_array[0], "cd") == 0)
        cd(arg_array,count,home);
        
        else if(strcmp(arg_array[0], "echo") == 0)
        echo(arg_array,count);*/


        }
    }


}