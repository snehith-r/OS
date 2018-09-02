#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include "get_input.h"
#include "execute.h"
void getprompt()
{
    char s[200],*user;
    user=getenv("LOGNAME");
    gethostname(s,200);
    char *cwd,fi[100];
    cwd=getcwd(NULL,0);

    fprintf(stderr,"<%s%s@%s%s:%s%s%s>","\x1B[1;34m",user,s,"\x1B[0m","\x1B[1;32m",cwd,"\x1B[0m");
    return;

}

void shell_loop()
{
    while(1)
    {
        int stat;
        while(waitpid(-1, &stat, WNOHANG) > 0);

        char home[1002];
        getcwd(home,1000);
        int home_len=strlen(home);

        char pwd[10002];
        getcwd(pwd,1000);

        int pwd_len=strlen(pwd);

        char path[1002];
        if(!strcmp(home,pwd) && pwd_len<=home_len)
        for(int i=home_len;i<=pwd_len;i++)
        { 
        path[i-home_len+1]=pwd[i];
        path[0]='~';
        }
        else
        {
            for(int i=0;i<=pwd_len;i++) path[i]=pwd[i];
        }

        char *username=getenv("USER"); 
        char host[102];
        gethostname(host,100);
        printf(">%s%s@%s%s:%s%s%s","\x1B[1;34m",username,host,"\x1B[0m","\x1B[1;32m",path,"\x1B[0m");
        
        char* input=readline();
        char** arg_array=parse(input,";");
        for(int i=0;arg_array[i]!=NULL;++i)
        {
            if(strcmp(arg_array[i],"exit")==0)
            {
                exit(0);
            }   

            int p[2];
            pid_t proc;
            char** pipedargs=parse(arg_array[i],"|");
            if(pipedargs[1]!=NULL)
            {
                for (int j=0;pipedargs[j]!=NULL;++j)
                {
                    int count=0;
                    char **args=parse(pipedargs[j]," \t\n\a\r");
                    
                    for(int i=0;args[i]!=NULL;++i)
                    count++;
                    pipe(p);
                    proc=fork();
                    if(proc==0)
                    {
                        dup2(p[0],0);
                        if(pipedargs[j+1]!=NULL)dup2(p[1],1);
                        close(p[0]);
                        execute(args,count,home,home_len);
                        exit(2);
                    }
                    else
                    {
                        wait(NULL);
                        close(p[1]);
                        i++;
                    }                
                }
                
            }

            else
            {
                    int count=0;
                    char **args=parse(arg_array[i]," \t\n\a\r");
                    for(int i=0;args[i]!=NULL;++i)
                    count++;
                    execute(args,count,home,home_len);

            }


         }
    
    }
}

int main()
{
    shell_loop();
    return 0;
}