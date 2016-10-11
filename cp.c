#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "cshell.h"

/* Lalaldddddddddddddddddddd */
/* 复制sfd文件到dfd  */
int copy(int dfd, int sfd){
	char buf[BUFSIZE];
	ssize_t n;
	//printf("read start\n");
	while((n = read(sfd, buf, sizeof(buf))) > 0){
		write(dfd, buf, n);
	}

	return 1;
}

/* cp 命令实现 */
int cp(char *destpath, char *sourpath){
	
	char buf[BUFSIZE];
	int sfd, dfd;	
	
	if((sfd = open(sourpath, O_RDONLY)) < 0){
		if(errno == ENOENT)
			fprintf(stderr, "命令失败! 不存在文件:%s\n", sourpath);
		else
			fprintf(stderr, "打开文件 %s 未知错误!\n", sourpath);
		return -1;
	}
	
	//得到源文件的modes属性.
	//当目标文件不存在新建文件时,会依据源文件权限来创建.
	struct stat stbuf;
	mode_t smode; //源文件的mode_t.
	if(stat(sourpath, &stbuf) == 0){
		smode = stbuf.st_mode;
	}else{
		fprintf(stderr, "stat 文件 %s 错误\n", sourpath);
		return -1;
	}

	/* 默认情况:若不存在目标文件,会直接一个新文件.
	 * 若存在, 覆盖.*/
	if((dfd = open(destpath, O_WRONLY | O_CREAT, smode & ~cumask)) < 0){
		fprintf(stderr, "create %s errno\n", destpath);
	}else{
		copy(dfd, sfd);	
	}
	close(sfd);
	close(dfd);
	return 1;
}

int main(int argc, char** argv){
	if(argc != 3){
		perror("命令行错误\n");
		exit(0);
	}

	if(strcmp(argv[0], "cp") == 0){
		cp(argv[2], argv[1]);
	}
	return 1;
}
