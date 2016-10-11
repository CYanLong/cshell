#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>


/* 文件重命名 */
int mv(char *oldname, char *newname){
	if(rename(oldname, newname) < 0){
		if(errno == ENOENT){
			printf("cyl\n");
			fprintf(stderr, "%s is not exist\n", oldname);
			exit(0);
		}if(errno == EACCES){
			fprintf(stderr, "%s not write permission\n", oldname);
			exit(0);
		}
	}
	return 1;
}

int main(int argc, char** argv){
	if(argc < 3){
		perror("命令错误\n");
		exit(-1);
	}

	if(strcmp(argv[0], "mv") == 0){
		mv(argv[1], argv[2]);
	}
}
