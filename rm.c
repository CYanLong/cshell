#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

//删除文件
int rm(char *pathname){
	int i;
	if(unlink(pathname) < 0){
		if(errno == EISDIR){
			fprintf(stderr, "无法删除! %s 是一个目录\n", pathname);
		}else if(errno == ENOENT){
			fprintf(stderr, "rm: 无法删除! 没有那个文件: %s\n", pathname);
		}else{
			fprintf(stderr, "rm:删除失败! 未知错误");
		}
		exit(1);
	}
	return 1;
}

int main(int argc, char* argv[]){
	
	//printf("argv[1]", argv[1]);
	if(argc < 2)
		perror("输入命令错误");
	
	if(strcmp(argv[0], "rm") == 0)
		rm(argv[1]);
	
}

