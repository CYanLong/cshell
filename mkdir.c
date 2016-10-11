#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "cshell.h"

/* mkdir时未umask的权限位 */
#define DD_MODE ( S_IRUSR | S_IWUSR | S_IXUSR |\
				  S_IRGRP | S_IWGRP | S_IXGRP |\
				  S_IROTH | S_IWOTH | S_IXOTH)


/* mkdir 命令 */
int mkdire(char *pathname){
	if(mkdir(pathname, DD_MODE & ~cumask ) < 0){
		if(errno == EEXIST){
			fprintf(stderr, "dir %s already exist\n", pathname);
		}
	}
}

int main(int argc, char** argv){
	if(argc < 2){
		perror("命令错误\n");
		exit(0);
	}
	if(strcmp(argv[0], "mkdir") == 0)
		mkdire(argv[1]);

	return 1;
}

