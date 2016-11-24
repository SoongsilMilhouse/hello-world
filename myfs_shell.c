#include<stdio.h>
#include<string.h>
int main()
{
        FILE *fp;
        char path[20]="/ ",command[20];
        int i;

        if((fp=fopen("myfs","r"))==NULL){
                printf("error : no myfs");
                return 0;
        }
        while(1){
                printf("[%s]$ ",path);
                scanf("%s",command);
                if(strcmp(command,"byebye")==0)
                        break;
                else{
                        for(i=0;i<20;++i)
                                command[i]='\0';
                }
        }
        return 0;
}
