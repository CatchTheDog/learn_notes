/*
** �������һ���޷�������ֵ������ת��Ϊ�ַ�����ӡ����ǰ����ɾ��
**
** ע���ڽ��д�ӡʱ����ӡ�ַ��������������ͳ���������߳���Ŀ���ֲ�ԣ�
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
	putchar(value % 10 + '0');				/* ������ֵת��Ϊ�ַ���ֵ��� */
}

int
main_5(void) {
	binary_to_ascii(1234556);
	return 0;
}
