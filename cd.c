#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>

/* cd 命令
 * 注:这是一个内建命令
 * */
int chcwd(char* pathname){
	
	/* 处理 ~ home 路径 */
	if(strcmp(pathname, "~") == 0){
		struct passwd *pw = getpwuid(getuid());
		pathname = pw->pw_dir;
	}
	if(chdir(pathname) < 0){
		perror("errno..\n");
	}
}

