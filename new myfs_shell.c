#include<string.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>

typedef struct super{
    char inode[512];
    char data[1024];
}super;
typedef struct single{
    char directblock[102];
}single;

typedef struct Double{
    single singleblock;
}Double;

typedef struct inode{
    char f_d;
    char time[20];
    int size;
    char direct;
    single singleblock;
    Double Doubleblock;
}inode;

typedef struct data{
    char minidata[8][16];
}data;

typedef struct filesys{
    char bootblock[2];
    super superblock;
    inode inodeblock[512];
    data datablock[1024];
}filesys;

char *timeString();
int superInodeCheck(filesys *fs);
int superDataCheck(filesys *fs);
void addSuperInode(filesys *fs,int count);
void addSuperData(filesys *fs,int count);
void mymkdir(filesys *fs);
void myflush();

int main()
{
    FILE *fp;
    char path[20]="/ ",command[40];
    int i;
    char *ptr[4],*word;
    filesys fs;

    ptr[0]=(char*)malloc(sizeof(char)*40);
    ptr[1]=(char*)malloc(sizeof(char)*40);
    ptr[2]=(char*)malloc(sizeof(char)*40);
    ptr[3]=(char*)malloc(sizeof(char)*40);

    if((fp=fopen("myfs","rb"))==NULL){
        printf("error : no myfs");
        return 0;
    }
    else
        fread(&fs,sizeof(filesys),1,fp);

    fclose(fp);

    //for(i=0;i<3;++i)
    //  printf("%c\n",fs.inodeblock[i].f_d);


    while(1){
        printf("[%s]$ ",path);
        scanf("%[^\n]",command);
    word=strtok(command," ");
    i=0;
    while(word!=NULL){
        ptr[i]=word;
        i++;
        word=strtok(NULL," ");
    }

    if(strcmp("mymkdir",ptr[0])==0){
        mymkdir(&fs);
    //for(i=0;i<3;++i)
    //  printf("%d\n",fs.inodeblock[i].size);
    }
    myflush();

        if(strcmp(command,"byebye")==0){
            fp=fopen("myfs","wb");
            //fseek(fp,0,SEEK_SET);
            fwrite(&fs,sizeof(filesys),1,fp);
            fclose(fp);
            break;
        }
        else{
            for(i=0;i<20;++i)
                command[i]='\0';
        }
    }
    return 0;
}

char *timeString(){
    struct tm today;
    time_t tt;
    char static tim[20];
    int ex[6];


    time(&tt);
    today = *localtime(&tt);

    ex[0]=today.tm_year + 1900;
    sprintf(tim,"%d/",ex[0]);
    ex[1]=today.tm_mon+1;
    sprintf(tim+5,"%d/",ex[1]);
    ex[2]=today.tm_mday;
    sprintf(tim+8,"%02d ",ex[2]);
    ex[3]=today.tm_hour;
    sprintf(tim+11,"%02d:",ex[3]);
    ex[4]=today.tm_min;
    sprintf(tim+14,"%02d:",ex[4]);
    ex[5]=today.tm_sec;
    sprintf(tim+17,"%02d",ex[5]);
    return tim;

}
int superInodeCheck(filesys *fs){
    int i,count;
    for(i=0;i<512;++i){
        if(fs->superblock.inode[i]=='0'){
            count=i;
            break;
        }
    }
    return count;
}
int superDataCheck(filesys *fs){
    int i,count;
    for(i=0;i<1024;++i){
        if(fs->superblock.data[i]=='0'){
            count=i;
            break;
        }
    }
    return count;
}
void addSuperInode(filesys *fs,int count){
    fs->superblock.inode[count]='1';
}
void addSuperData(filesys *fs,int count){
    fs->superblock.data[count]='1';
}
/*
   void moveSuperInode(filesys *fs,int count){
   fs->superblock.inode[count]='0';
   }
   void moveSuperData(filesys *fs,int count){
   fs->superblock.data[count]='0';
   }
   */

//int checkFileSize()



void mymkdir(filesys *fs){
    int static cnt=0;
    int super_inode_num,super_data_num;
    super_inode_num=superInodeCheck(fs);
    super_data_num=superDataCheck(fs);

    addSuperInode(fs,super_inode_num);
    addSuperData(fs,super_data_num);
    printf("%d\n",super_inode_num);
    printf("%d\n",super_data_num);

    fs->inodeblock[super_inode_num].f_d='d';
    strcpy(fs->inodeblock[super_inode_num].time,timeString());
    fs->inodeblock[super_inode_num].size=1;
    fs->inodeblock[super_inode_num].direct=(char)super_data_num;
    cnt++;
    sprintf(fs->datablock[super_data_num].minidata[cnt],". %d",super_inode_num);
}

void myflush()
{
        while(getchar()!='\n');
}
