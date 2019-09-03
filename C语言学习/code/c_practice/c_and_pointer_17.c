/*
** 测试错误信息提示：
**		char *strerror(int error_number);
*/

#define		_CRT_SECURE_NO_WARNINGS
#include	<ctype.h>			/* 该头文件包含了用于操作单独的字符的函数 */
#include	<string.h>

int
main_17() {
	printf("字符 '%c' 转换为大写为：'%c',转换为小写为：'%c' \n",'A',toupper('A'),tolower('A'));
	char* a = strerror(101);
	printf("错误代码101的描述信息：%s \n",a);
}