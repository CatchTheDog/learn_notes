/*
** ����һ���ַ����ĳ���
*/


#include <stdlib.h>

size_t
strLen(char *string) {

	int		length = 0;		/* �ַ������� */

	/*
	** ���η����ַ��������ݣ������ַ�����ֱ������NUL��ֹ��
	*/
	while (*string++ != '\0')
		length++;

	return length;
}