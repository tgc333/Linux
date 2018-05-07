#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<dirent.h>

int count = 0;
char path[10][100];

void fcat(char *find);
void myselect(void);
void mycat(char p[]);

int main(int argc, void *argv[])
{	
	fcat(argv[1]);
	myselect();
	return 0;
}

void fcat(char *find)
{
	DIR *dirp;
	struct stat finfo;
	struct dirent *dentry;
	char buf[1024];
	if((dirp = opendir(".")) == NULL) exit(1);

	while(dentry = readdir(dirp))

		if(!strcmp(dentry->d_name,".") || !strcmp(dentry->d_name,"..")) {;}
		else{
			lstat(dentry->d_name,&finfo);
			
			if(!S_ISDIR(finfo.st_mode)){
				if(!strcmp(find,dentry->d_name)){
					getcwd(buf,sizeof(buf));
					strcat(buf,"/");
					strcat(buf,dentry->d_name);
					strcpy(path[count],buf);
					count++;
				}
			}
			
			else if(S_ISDIR(finfo.st_mode)){
				chdir(dentry->d_name);
				fcat(find);
				chdir("..");
			}
		}
	closedir(dirp);
}

void myselect()
{
	int sel,fd,size;
	char buf[1024];
	if(count==0)
		printf("There is no such file!\n");
	else if(count==1){
		printf("File found : %s\n",path[0]);
		printf("cat : %s\n",path[0]);
		fd=open(path[0],O_RDWR);
		size=read(fd,buf,sizeof(buf));
		buf[size]='\0';
		printf("%s",buf);
	}
	else if(count > 1){
		for(int i=0; i < count; i++){
			printf("[%d]   %s\n",i,path[i]);
		}
		printf("Which file do you want to read?\n");
		scanf("%d",&sel);
		printf("cat : %s\n",path[sel]);
		fd=open(path[sel],O_RDWR);
		size=read(fd,buf,sizeof(buf));
		buf[size]='\0';
		printf("%s",buf);
	}
}

void mycat(char p[])
{
	char buf[1024];
	int fd;
	fd = open(p,O_RDWR);
	read(fd,buf,sizeof(buf));
	printf("%s\n",buf);
}