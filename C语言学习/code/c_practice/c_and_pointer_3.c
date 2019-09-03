/*
** 计算一个字符串的长度
*/


#include <stdlib.h>

size_t
strLen(char *string) {

	int		length = 0;		/* 字符串长度 */

	/*
	** 依次访问字符串的内容，计数字符数，直到遇到NUL终止符
	*/
	while (*string++ != '\0')
		length++;

	return length;
}