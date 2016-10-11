#ifndef CSHELL_H
#define CSHELL_H

#include <sys/stat.h>
#define BUFSIZE 64

/* 关于输入命令的结构体封装 */
struct Argument{
	char *argv[BUFSIZE];
	int count;
};

typedef struct Argument argu;

/* 定义输入最长命令大小 */
#define COMM_LINESIZE 1024

/* umask全局变量,cumask是为了避免与系统库冲突 */
#ifdef DEFINE_VARIABLES
#define EXTERN
#else
#define EXTERN extern
#endif
EXTERN mode_t cumask;

/* 输出颜色定义 */
#define RESET "\x1b[0m"
#define BLUE "\x1b[34m"
#define YELL "\x1b[33m"

#endif
