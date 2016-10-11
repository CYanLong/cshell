#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <sys/ioctl.h>
#include "cshell.h"

#define DIR_MAXLINE 1024

/* 关于文件和目录信息的结构
多个文件和子目录的信息使用链表连接. */
struct fileinfo{
	char *filename;
	struct stat *statbuf;
	struct fileinfo *next;
};

typedef struct fileinfo finfo;

/* 得到当前进程工作路径下的所有文件和目录 */
finfo* getfinfo(){
	DIR *dir;
	struct dirent *dirent; 
	finfo *fheader = NULL; /* finfo header */
	char *cwd = getcwd(NULL, 0); /* get processor current work driectory */
	printf("cwd %s\n", cwd);

	if((dir = opendir(cwd)) != NULL){ // 迭代cwd目录下所有文件和目录
		while((dirent = readdir(dir)) != NULL){
			struct stat *statbuf = malloc(sizeof(struct stat));
			if(lstat(dirent->d_name, statbuf) >= 0){
				if(fheader == NULL){//first
					fheader = malloc(sizeof(finfo));
					fheader->filename = dirent->d_name;
					fheader->statbuf = statbuf;
					fheader->next = NULL;
				}else{
					finfo *end = fheader;
					finfo *finfo = malloc(sizeof(finfo));

					while(end != NULL && end->next != NULL)
						end = end->next;
					finfo->filename = dirent->d_name;
					finfo->statbuf = statbuf;
					finfo->next = NULL;
					end->next = finfo;
				}
			}else{
				fprintf(stderr, "file %s stat error\n", dirent->d_name);
				exit(0);
			}
		}

	}else{
		fprintf(stderr, "opendir %s error", cwd);
		exit(0);
	}
	return fheader;
}

/* 解析文件的模式位 */
int parsemode(mode_t st_mode, char *buf){
	switch(st_mode & S_IFMT){
		case S_IFIFO: buf[0] = 'p'; break; 
		case S_IFCHR: buf[0] = 'c'; break;
		case S_IFDIR: buf[0] = 'd'; break;
		case S_IFREG: buf[0] = '-'; break;
		//case S_IFNWK: buf[0] = 'n'; break;
		case S_IFLNK: buf[0] = 'l'; break;
		default: buf[0] = 'u'; /* Unknow */
	}
	buf[1] = (st_mode & S_IRUSR) ? 'r' : '-';
	buf[2] = (st_mode & S_IWUSR) ? 'w' : '-';
	buf[3] = (st_mode & S_IXUSR) ? 'x' : '-';

	buf[4] = (st_mode & S_IRGRP) ? 'r' : '-';
	buf[5] = (st_mode & S_IWGRP) ? 'w' : '-';
	buf[6] = (st_mode & S_IXGRP) ? 'x' : '-';
	
	buf[7] = (st_mode & S_IROTH) ? 'r' : '-';
	buf[8] = (st_mode & S_IWOTH) ? 'w' : '-';
	buf[9] = (st_mode & S_IXOTH) ? 'x' : '-';
		
	buf[10] = '\0';
	return 1;
}

/* ls -l 命令 */
int list_l(){
	finfo *info;
	char mbuf[11];
	struct passwd *upwbufp;
	struct group *gbufp;
	struct tm *p;
	char tbuf[18];

	info = getfinfo();
	
	while(info != NULL){
		parsemode(info->statbuf->st_mode, mbuf);
		upwbufp = getpwuid(info->statbuf->st_uid);
		gbufp = getgrgid(info->statbuf->st_gid);
		
		p = localtime(&info->statbuf->st_ctime);
		strftime(tbuf, 18, "%F %R", p);

		printf("%-12s %-6d %-6s %-6s %-6ld %-18s", 
					mbuf, info->statbuf->st_nlink, upwbufp->pw_name, gbufp->gr_name,
					info->statbuf->st_size, tbuf);

		mode_t mode = info->statbuf->st_mode & S_IFMT;
		if(mode == S_IFDIR){
			printf(BLUE "%s\n" RESET, info->filename);
		}else if(mode == S_IFLNK){
			/* get the target path */
			char buf[1024];
			ssize_t len;
			if((len = readlink(info->filename, buf, sizeof(buf)-1)))
				buf[len] = '\0';
			printf(YELL "%s -> %s\n" RESET, info->filename, buf);
		}else{
			printf("%s\n", info->filename);
		}
		info = info->next;

	}
	return 1;
}

/* ls 命令 */
int list_simp(){
	finfo *info, *temp;
	long len = 0; /* 所有文件名的总长度 */
	int count = 0;	  /* 文件的个数 */
	info = getfinfo();
	temp = info;

	//得到所有文件名的总长度
	while(temp != NULL){
		len += strlen(temp->filename);
		temp = temp->next;
		count++;
	}
	
	count -= 2; /* 忽略 . 和 .. */

	/* 得到当前terminal的宽度 */
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	
	int avglen = len/count; /* 每个文件的平均长度 */
	int num = w.ws_col/avglen; /* 每行显示的个数 */
	printf("len %ld\n", len);
	printf("count: %d\n", count);
	printf("num: %d\n", num);

	int n = 1;
	while(info != NULL){
		if((strcmp(info->filename, ".") == 0)
				|| (strcmp(info->filename, "..") == 0)){
			info = info->next;
			continue;
		}
		printf("%-10s", info->filename);
		
		if((n % num) == 0)
			printf("\n");
		
		info = info->next;
		n++;
	}
	printf("\n");
	return 1;
}

int main(int argc, char* argv[]){
	printf("ls main\n");
	if((argc == 1) && (strcmp(argv[0], "ls") == 0))
		list_simp();
	if(argc == 2 && (strcmp(argv[0], "ls") == 0)
			&& (strcmp(argv[1], "-l") == 0))
		list_l();

	return 1;
}
