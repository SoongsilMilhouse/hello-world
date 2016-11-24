#include<stdio.h>
typedef struct super{
        char inode[64];
        char data[128];
}super;                                         //super block
typedef struct inode{
        char _d;
        char time[20];
        int size;
        int direct;
        int single;
        int _double;
}inode;                                         //inode list

typedef struct data{
        char dd[128];
}data;
                                                //data block
typedef struct filesys{
        char boot[2];
        super sb;
        inode in[512];
        data db[1024];
}filesys;
int main()
{
        FILE *fp;
        filesys fs;
        int i;


        fp=fopen("myfs","wb");
        fs.boot[0]=fs.boot[1]=0;
        for(i=0;i<64;++i)
                fs.sb.inode[i]=0;
        for(i=0;i<128;++i)
                fs.sb.data[i]=0;
        fs.sb.inode[0]=0;
        fwrite(&fs,sizeof(filesys),1,fp);
        //fread(&fs,sizeof(filesys),1,fp);
        fclose(fp);
        return 0;
}
