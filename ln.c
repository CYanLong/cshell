#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char** argv){
	
	if(argc < 3){
		perror("命令错误\n");
		return -1;
	}
	printf("argc: %d\n", argc);
	if((argc == 3) && (strcmp(argv[0], "ln") == 0)){// hard link
		printf(";a;a\n");
		if(link(argv[1], argv[2]) < 0){
			if(errno == EEXIST){
				fprintf(stderr, "ln 命令错误! %s已存在\n", argv[2]);
			}else if(errno == ENOENT){
				fprintf(stderr, "ln 创建硬链接失败! %s 不存在\n!", argv[2]);
			}
		}
	}
	/* symbolic link */
	if((argc == 4) && (strcmp(argv[0], "ln") == 0) && 
			(strcmp(argv[1], "-s") ==0)){
		
		if(symlink(argv[2], argv[3]) < 0){
			if(errno == EEXIST){
				fprintf(stderr, "%s 文件已经存在\n", argv[3]);
			}else{
				perror("未知错误\n");
			}
		}
	}
}
