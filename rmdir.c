#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

int rmdire(char *pathname){

	if(rmdir(pathname) < 0){
		if(errno == ENOTEMPTY){
			fprintf(stderr, "rmdir:删除失败! 目录: %s 非空\n", pathname);
		}else if(errno == ENOTDIR)
			fprintf(stderr, "%s 不是一个目录\n", pathname);
		else if(errno == EACCES)
			fprintf(stderr, "目录不存在 %s \n", pathname);

		return -1;//error return
	}

	return 1;

}

int main(int argc, char* argv[]){
	
	if(argc < 1){
		fprintf(stderr, "命令错误\n");
		return -1;
	}

	if(strcmp(argv[0], "rmdir") == 0)
		rmdire(argv[1]);
	return 1;
}
