/*
** ������������ָ���������
**
*/

#include		<stdio.h>

int
main_11() {
	int		a[5] = { 1,2,3,4,5 }, b = 10;
	int		*c = &a;
	int		*d = &b;
	printf("����a�ĵڶ���Ԫ����:%d \n",*(++c));
#if 0
	printf("����a�ĵڶ���Ԫ����:%d \n", *(++a));
#endif
	printf("ָ�����dʹ���±���ʣ�%d \n", d[0]);
	printf("ָ�����dʹ���±���ʣ�%d \n", d[1]);
}