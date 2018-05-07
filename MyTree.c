#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<dirent.h>

int count=0;

void printMode(struct stat finfo)
{
	mode_t mode = finfo.st_mode;
	printf("[");
	if(S_ISREG(mode))	printf("-");
	if(S_ISBLK(mode))	printf("b");
	if(S_ISLNK(mode))	printf("l");
	if(S_ISCHR(mode))	printf("c");
	if(S_ISDIR(mode))	printf("d");
	if(S_ISFIFO(mode))	printf("p");
	if(S_ISSOCK(mode))	printf("s");
	
		if(S_IRUSR & mode)	printf("r"); 
		else	printf("-");
		if(S_IWUSR & mode)	printf("w"); 
		else	printf("-");
		if(S_IXUSR & mode)	printf("x"); 
		else	printf("-");
	
		if(S_IRGRP & mode)	printf("r"); 
		else	printf("-");
		if(S_IWGRP & mode)	printf("w"); 
		else	printf("-");
		if(S_IXGRP & mode)	printf("x"); 
		else	printf("-");
		
		
		if(S_IROTH & mode)	printf("r"); 
		else	printf("-");
		if(S_IWOTH & mode)	printf("w"); 
		else	printf("-");
		if(S_IXOTH & mode)	printf("x"); 
		else	printf("-");
		printf("]  ");
}

void myTree(void)
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
			if(S_ISLNK(finfo.st_mode)){
				int link = readlink(dentry->d_name,buf,sizeof(buf));
				buf[link] = '\0';
				for(int i=0; i<sizeof(buf); i++)
					if(buf[i]==' ')
						buf[i]='\0';
				for(int i=0; i<count; i++)
					printf("    ");
				printMode(finfo);
				printf("%s -> %s\n",dentry->d_name,buf);
			}
			else if(!S_ISDIR(finfo.st_mode)){
				for(int i=0; i<count; i++)
					printf("    ");
				printMode(finfo);
				printf("%s\n",dentry->d_name);
			}
			
			else if(S_ISDIR(finfo.st_mode)){
				for(int i=0; i<count; i++)
					printf("    ");
				printMode(finfo);
				printf("%s\n",dentry->d_name);
				chdir(dentry->d_name);
				count++;
				myTree();
				chdir("..");
			}
		}
		count--;
	closedir(dirp);
}

int main(void)
{	
	myTree();

	return 0;
}