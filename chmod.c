#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>


/* 改变文件或目录的权限位 */
int changemod(char *path, mode_t mode){
	if(chmod(path, mode) < 0){
		if(errno == EACCES){
			perror("没有权限\n");
			exit(0);
		}else if(errno == ENOENT){
			fprintf(stderr, "不存在文件或目录: %s\n", path);
			exit(0);
		}
	}
	return 1;
}

int main(int argc, char** argv){
	
	if(argc != 3){
		perror("命令错误\n");
		return -1;
	}

	if(strcmp(argv[0], "chmod") == 0){
		mode_t mode = strtol(argv[1], NULL, 8); // 将数字字符串转换为八进制
		changemod(argv[2], mode);
	}
	return 1;

}
