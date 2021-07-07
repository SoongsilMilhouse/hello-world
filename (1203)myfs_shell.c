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
}filesys;                                    // 파일 시스템 구조체

typedef struct tree{                         //트리구조 자기참조 구조체
    char name[4];
    struct tree *child;
    struct tree *bro;
}tree;

//tree *head=NULL;
//tree *tail=NULL;
char *ptr[4],path[20]="/";
char *timeString();                        //시간측정함수
int superInodeCheck(filesys *fs);          //superblock 아이노드가 0인곳 찾기
int superDataCheck(filesys *fs);           //superblock 데이터가 0인곳 찾기
void addSuperInode(filesys *fs,int count); //미사용중인 수퍼블럭을 사용중으로 바꾸기
void addSuperData(filesys *fs,int count);  //   "
void mymkdir(filesys *fs,tree *);
void myflush();                            //버퍼 비우기
void mycat();
void myls(filesys *fs);
char *findNowD();                          //최종 디렉토리 찾기 ex) /etc/abc/  ----> abc를 리턴
void mypwd();
void mycd(tree *root);

int main()
{
    FILE *fp;
    char command[40];
    int i,j;
    char *word;
    filesys fs;
    tree *root=(tree*)malloc(sizeof(tree));

    strcpy(root->name,"/");
    root->child=NULL;
    root->bro=NULL;
//  tail=head=root;


    ptr[0]=(char*)malloc(sizeof(char)*20);
    ptr[1]=(char*)malloc(sizeof(char)*20);
    ptr[2]=(char*)malloc(sizeof(char)*20);
    ptr[3]=(char*)malloc(sizeof(char)*20);

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
        printf("[%s ]$ ",path);
        scanf("%[^\n]",command);
    word=strtok(command," ");
    i=0;
    while(word!=NULL){
        ptr[i]=word;
        i++;
        word=strtok(NULL," ");
    }

    if(strcmp("mymkdir",ptr[0])==0){
        mymkdir(&fs,root);
    //for(i=0;i<3;++i)
    //  printf("%d\n",fs.inodeblock[i].size);
    }
    else if(strcmp("mycat",ptr[0])==0){
        mycat(&ptr[1]);
    }
    else if(strcmp("myls",ptr[0])==0){
        myls(&fs);
    }
    else if(strcmp("mypwd",ptr[0])==0){
        mypwd();
    }
    else if(strcmp("mycd",ptr[0])==0){
        mycd(root);
    }
    myflush();

        if(strcmp(command,"byebye")==0){
            fp=fopen("myfs","wb");
            fwrite(&fs,sizeof(filesys),1,fp);
            fclose(fp);
            break;
        }
        else{
            for(i=0;i<20;++i)
                command[i]='\0';

            for(i=0;i<4;++i)
                for(j=0;j<20;++j)
                    ptr[i][j]='\0';
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



void mymkdir(filesys *fs,tree *root){
    int static cnt=0;
    int super_inode_num,super_data_num;
    char tmp[10];
    tree *new=(tree*)malloc(sizeof(tree));
    tree *head=root;
    tree *tail=NULL;

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

    strcpy(tmp,findNowD());
    if(strcmp(tmp,"/")==0){                                   //tree인데 구현하다 멈춤 (/___ 까지 구현)
        if(root->child==NULL){
            root->child=new;
            strcpy(new->name,ptr[1]);
    printf("%s\n",new->name);
            new->child=NULL;
            new->bro=NULL;
            head=tail=new;
        }
        else{
            tail=head=root->child;
            while(tail->bro)
                tail=tail->bro;
            tail->bro=new;
            strcpy(new->name,ptr[1]);
    printf("%s\n",new->name);
            new->child=NULL;
            new->bro=NULL;
        }
    }
    else{
        tail=root->child;
        while(1){
            if(strcmp(tail->name,tmp)==0)
                break;
            tail=tail->bro;
        }
        head=tail;
        if(head->child==NULL){
        head->child=new;
        strcpy(new->name,ptr[1]);
        new->child=NULL;
        new->bro=NULL;
        head=tail=new;
    }
        else{
            tail=tail->child;
            while(tail->bro)
                tail=tail->bro;
            tail->bro=new;
            strcpy(new->name,ptr[1]);
            new->child=NULL;
            new->bro=NULL;
        }
    }



}

void myflush()
{
        while(getchar()!='\n');
}
void mycat()
{
    int ch;
    FILE *fp;


    if((fp=fopen(ptr[1],"r"))==NULL){
        fprintf(stderr, "ERROR!\n");
        return;
    }

    while((ch=getc(fp))!=EOF){
        putchar(ch);
    }
    printf("\n");
    fclose(fp);
}

void myls(filesys *fs){
    int i,rootcnt=0,cnt;
    char tmp[10];
    for(i=0;i<20;++i){
        if(path[i]=='/')
            rootcnt=i;
        if(path[i]=='\0'){
            if(rootcnt==0)
                tmp[0]='/';
            strcpy(tmp,path+rootcnt+1);
            break;
        }
    }
    for(i=0;i<2;++i)
    printf("\n%s\n",fs->datablock[0].minidata[i]);

}



void mypwd(){
    printf("%s\n",path);
}

char *findNowD(){
    int i,rootcnt;
    char static tmp[10];

    for(i=0;i<20;++i){
        if(path[i]=='/')
            rootcnt=i;
        if(path[i]=='\0'){
            if(rootcnt==0)
                strcpy(tmp,"/");
            else
            strcpy(tmp,path+rootcnt+1);
            break;
        }
    }
    return tmp;
}

void mycd(tree *root){

    int i,cnt;
    if(strcmp(ptr[1],"\0")==0)
        strcpy(path,"/");
    else{
    for(i=0;i<20;++i)
        if(path[i]=='\0'){
            cnt=i;
            break;
        }

    strcat(path+cnt,ptr[1]);
    for(i=0;i<20;++i)
        if(path[i]=='\0'){
            cnt=i;
            break;
        }
    strcat(path+cnt,"/");
    }
}
