/*
** 测试数组名与指针变量区别
**
*/

#include		<stdio.h>

int
main_11() {
	int		a[5] = { 1,2,3,4,5 }, b = 10;
	int		*c = &a;
	int		*d = &b;
	printf("数组a的第二个元素是:%d \n",*(++c));
#if 0
	printf("数组a的第二个元素是:%d \n", *(++a));
#endif
	printf("指针变量d使用下标访问：%d \n", d[0]);
	printf("指针变量d使用下标访问：%d \n", d[1]);
}