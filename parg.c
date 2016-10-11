/* 解析输入的命令 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cshell.h"

/* 将input 命令字符串解析为argu结构 */
argu* pargu(char* input){
	char *p, *temp;
	char comm[COMM_LINESIZE];
	argu *arg;
	
	strcpy(comm, input);

	arg = malloc(sizeof(argu));
	arg->count = 0;
	p = strtok(comm, " "); //以空格分割
	
	int n = 0;
	while(p != NULL){
		temp = (char *)malloc(strlen(p) + 1);
		strcpy(temp, p);
		arg->count++;
		arg->argv[n++] = temp;
		p = strtok(NULL, " ");
	}
	arg->argv[arg->count] = NULL;
	return arg;
}

