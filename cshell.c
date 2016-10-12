#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cshell.h"

#define FORK_AND_EXEC(path, arg) \
	if((cid = fork()) == 0){ \
		execv(path, arg); \
	}else if(cid > 0){ \
		waitpid(cid, NULL, 0);\
	}else{ \
		perror("fork error"); \
	} \
	continue \

int main(int argc, char** argv){
	char command[COMM_LINESIZE], *cwd;
	ssize_t n;
	int i;
	argu *arg = NULL;
	pid_t cid;
	for(;;){
		cwd = getcwd(NULL, 0);
		printf("[ %s ]cshell >> ", cwd);
		fflush(stdout);
		if((n = read(STDIN_FILENO, command, sizeof(command))) > 0){
			char buf[COMM_LINESIZE];

			strncpy(buf, command, n-1);
			buf[n-1] = '\0';
			arg = pargu(buf);
			
			printf("arg count: %d\n", arg->count);
			for(i = 0; i < arg->count; i++){
				printf("arg[%d]: %s\n", i, arg->argv[i]);
			}
			
			if(arg->count < 1){
				printf("参数太少\n");
				continue;
			}
				
			if(strcmp(arg->argv[0], "ls") == 0){
				FORK_AND_EXEC("./ls", arg->argv);
			}else if(strcmp(arg->argv[0], "rm") ==  0){
				FORK_AND_EXEC("./rm", arg->argv);
			}else if(strcmp(arg->argv[0], "rmdir") == 0){
				FORK_AND_EXEC("./rmdir", arg->argv);
			}else if(strcmp(arg->argv[0], "touch") == 0){
				FORK_AND_EXEC("./touch", arg->argv);
			}else if(strcmp(arg->argv[0], "mkdir") == 0){
				FORK_AND_EXEC("./mkdir", arg->argv);
			}else if(strcmp(arg->argv[0], "cd") == 0){
				chcwd(arg->argv[1]);
			}else if(strcmp(arg->argv[0], "mv") == 0){
				FORK_AND_EXEC("./mv", arg->argv);
			}else if(strcmp(arg->argv[0], "chmod") == 0){
				FORK_AND_EXEC("./chmod", arg->argv);
			}else if(strcmp(arg->argv[0], "cp") == 0){
				FORK_AND_EXEC("./cp", arg->argv);
			}else if(strncmp(arg->argv[0], "./", 2) == 0){ //执行程序
				FORK_AND_EXEC(arg->argv[0], &arg->argv[1]);
			}else if(strcmp(arg->argv[0], "ln") == 0){
				FORK_AND_EXEC("./ln", arg->argv);
			}else if(strcmp(arg->argv[0], "ping") == 0){
				FORK_AND_EXEC("./ping", arg->argv);
			}
		}else{
			perror("read term error");
			return 1;
		}
		printf("\n");
	}
	return 1;
}

