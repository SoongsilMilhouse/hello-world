#include<stdio.h>
#include<time.h>
#include<string.h>
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
void root(filesys *fs);
void timeString(filesys *fs);
int main()
{
    FILE *fp;
    filesys fs;
    int i;


    fp=fopen("myfs","wb");
    fs.bootblock[0]=fs.bootblock[1]='0';
    for(i=0;i<512;++i)
        fs.superblock.inode[i]='0';
    for(i=0;i<1024;++i)
        fs.superblock.data[i]='0';
    fs.superblock.inode[0]='1';
    fs.superblock.data[0]='1';
    root(&fs);
    fwrite(&fs,sizeof(filesys),1,fp);

    fclose(fp);
    return 0;
}
void root(filesys *fs){
    fs->inodeblock[0].f_d='d';
    fs->inodeblock[0].size=0;
    timeString(fs);
    fs->inodeblock[0].direct='0';
    sprintf(fs->datablock[0].minidata[0],". %d",1);

}
void timeString(filesys *fs){
    struct tm today;
    time_t tt;
    char tim[20];
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
    strcpy(fs->inodeblock[0].time,tim);
    printf("%s",fs->inodeblock[0].time);

}
