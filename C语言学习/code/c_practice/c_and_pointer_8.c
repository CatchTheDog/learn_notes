/*
** 程序接收一个无符号整型值，把它转换为字符并打印它，前导零删除
**
** 注：在进行打印时：打印字符常量而不是整型常量可以提高程序的可移植性：
**		'0' + 0 = '0'
**		'0' + 1 = '1'
**		'0' + 2 = '2'
*/

#include <stdio.h>


void
binary_to_ascii(unsigned int value) {
	unsigned int	quotient;

	quotient = value / 10;
	if (quotient != 0)
		binary_to_ascii(quotient);
	putchar(value % 10 + '0');				/* 将整型值转换为字符型值输出 */
}

int
main_5(void) {
	binary_to_ascii(1234556);
	return 0;
}
