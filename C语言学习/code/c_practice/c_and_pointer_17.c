/*
** ���Դ�����Ϣ��ʾ��
**		char *strerror(int error_number);
*/

#define		_CRT_SECURE_NO_WARNINGS
#include	<ctype.h>			/* ��ͷ�ļ����������ڲ����������ַ��ĺ��� */
#include	<string.h>

int
main_17() {
	printf("�ַ� '%c' ת��Ϊ��дΪ��'%c',ת��ΪСдΪ��'%c' \n",'A',toupper('A'),tolower('A'));
	char* a = strerror(101);
	printf("�������101��������Ϣ��%s \n",a);
}