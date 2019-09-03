/*
** �˳���������ϰC�����ַ�������
**
** �ַ���
*/

#define _CRT_SECURE_NO_WARNINGS				/* �����ڴ�й¶�����ʶ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stringcat(char a[],char b[]);
void stringcpy(char* a,char* b);
void stringncpy(char a[], char b[]);
void stringcmp(char a[], char b[]);
void stringx(char a[]);
void stringchr(char a[], char c);

/*
** �������
*/
int main_4(int argc, char* argv[]) {

	char a[50] = "C program.";				/* ��һ�ַ�����Ҫ���㹻�ռ��������ɸ���/ƴ�ӵĵڶ����ַ��� */
	char b[  ] = "C & Pointer.";

	//stringcat(a, b);						/* ����strcat(a,b) ���� */
	//stringcpy(a, b);						/* ����strcpy(a,b) ���� */
	//stringncpy(a, b);						/* ����strncpy(a,b,n) ���� */
	//stringcmp(a, b);						/* ����strcmp(a,b) ���� */
	stringx(a);								/* ����strlen,strlwr,strupr ���� */
	stringchr(a,'p');
}

/*
** ����strcat����
*/
void stringcat(char a[],char b[]) {
	strcat(a,b);
	puts(a);
}

/*
** ����strcpy����
*/
void stringcpy(char * a,char * b) {
	strcpy(a,b);
	puts(a);
}

/*
** ����strncpy����
*/
void stringncpy(char a[],char b[]) {
	strncpy(a,b,3);							/* �����ڸ�ֵ����ַ���ĩβ��� \0 �ַ� */
	puts(a);
}

/*
** ����strcmp����
*/
void stringcmp(char a[],char b[]) {
	int n = strcmp(a, b);
	printf("�ַ���a:%s���ַ���b:%s�ȽϽ����%d\n",a,b,n);
}

/*
** �ַ������Ȳ��ԣ�ת��Ϊȫ��д/ȫСд ����
*/
void stringx(char a[]) {

	printf("�ַ���a��%s ����Ϊ��%d \n",a,strlen(a));

	/*
	** strlwr/strupr �ѱ�ע��
	*/
#if 0
	printf("�ַ���a: %s ת��Ϊ��д��%s \n",a,strupr(a));
	printf("�ַ���a: %s ת��ΪСд��%s \n", a, strlwr(a));
#endif

}

/*
** strchr��������
*/
void stringchr(char a[],char c) {
	char* p = strchr(a,c);
	printf("�ַ�'%c'���ַ���\"%s\"�е�λ���ǣ�%p",c,a,p);
}