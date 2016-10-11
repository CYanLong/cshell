#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cshell.h"

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
			printf("comm:%s\n", buf);
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
				printf("ls:\n");
				if((cid = fork()) == 0){
					printf("child process\n");
					execv("./ls", arg->argv);
					exit(1);
				}else if(cid > 0){
					printf("parent\n");
					waitpid(cid, NULL, 0);
				}else{
					perror("fork error\n");
				}
			}

			if(strcmp(arg->argv[0], "rm") ==  0){
				if((cid = fork()) == 0){
					execv("./rm", arg->argv);
					exit(1);
				}
				else if(cid > 0){
					waitpid(cid, NULL, 0);
					continue;
				}
			}
			if(strcmp(arg->argv[0], "rmdir") == 0){
				if((cid = fork()) == 0){
					execv("./rmdir", arg->argv);
				}else if(cid > 0){
					waitpid(cid, NULL, 0);
				}else{
					perror("fork error\n");
				}
				continue;
			}
				
			
			if(strcmp(arg->argv[0], "touch") == 0){
				if((cid = fork()) == 0){
					execv("./touch", arg->argv);
				}else if(cid > 0){
					waitpid(cid, NULL, 0);
				}else{
					perror("fork error\n");
				}
				continue;
			}

			if(strcmp(arg->argv[0], "mkdir") == 0){
				
				if((cid = fork()) == 0){
					execv("./mkdir", arg->argv);
				}else if(cid > 0){
					waitpid(cid, NULL, 0);
				}else{
					perror("fork error");
				}
				continue;
			}
			
			if(strcmp(arg->argv[0], "cd") == 0){
				printf("cd comm\n");
				chcwd(arg->argv[1]);
				continue;
			}
			
			if(strcmp(arg->argv[0], "mv") == 0){
				if((cid = fork()) == 0){
					printf("mv mv\n");
					execv("./mv", arg->argv);
				}else{
					waitpid(cid, NULL, 0);
				}
				continue;
			}

			if(strcmp(arg->argv[0], "chmod") == 0){
				if((cid = fork()) == 0){
					execv("./chmod", arg->argv);
				}else if(cid > 0){
					waitpid(cid, NULL, 0);
				}
				continue;
			}

			if(strcmp(arg->argv[0], "cp") == 0){
				if((cid = fork()) == 0){
					execv("./cp", arg->argv);
				}else if(cid > 0){
					waitpid(cid, NULL, 0);
				}
				continue;
			}
			if(strncmp(arg->argv[0], "./", 2) == 0){ //执行程序
				if((cid = fork()) == 0){
					execv(arg->argv[0], &arg->argv[1]);
				}else if(cid > 0){
					waitpid(cid, NULL, 0);
				}
				continue;
			}
			if(strcmp(arg->argv[0], "ln") == 0){
				printf("lalala\n");
				if((cid = fork()) == 0){
					execv("./ln", arg->argv);
				}else if(cid > 0){
					waitpid(cid, NULL, 0);
				}
				continue;
			}
		}else{
			perror("read term error");
			return 1;
		}
		printf("\n");
	}
	return 1;
}

