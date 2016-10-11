#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "cshell.h"

/* 默认未umask的权限位
 *	u=rw 
 *	g=rw
 *	o=rw
 * */
#define DMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

/* touch命令 
 * 创建一个新的空的文件 */
int touch(char *pathname){
	if(creat(pathname, DMODE & ~cumask) < 0){
		if(errno == EEXIST){
			fprintf(stderr, "touch: 失败! 文件已经存在: %s\n", pathname);
			exit(0);
		}
	}
	return 1;
}

int main(int argc, char** argv){
	
	if(argc < 2)
		perror("命令错误\n");
	if(strcmp(argv[0], "touch") == 0)
		touch(argv[1]);
	return 1;
}
