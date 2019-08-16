/*
** �˳������ڲ���const�ؼ�������
**
** ����������ʽ��ָ�볣����Ϊ�����β�ʱ�����Խ�����£�
** 1.int const*p						�ɸı�ָ�����ֵ�������ɸı�ָ�����ָ�������Ԫ��ֵ
** 2.int* const p						�ɸı�ָ�����ָ�������Ԫ��ֵ�������ɸı�ָ�����ֵ
** 3.const int * p					�ɸı�ָ�����ֵ�������ɸı�ָ�����ָ�������Ԫ��ֵ
**
** �ܽ᣺const �� * ֮ǰ��ָ�������ָ���������Ԫ��ֵ���ɸı䣬ָ�����ֵ���Ըı�
**      const �� * ֮��ָ�������ָ���������Ԫ��ֵ�ɸı䣬ָ�����ֵ���ɸı�
*/
#define _CRT_SECURE_NO_WARNINGS		/* �����ڴ�й¶�����ʶ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printArray(int* p, int len);
void test(int const* p);
void test_1(int const* p);
void test_2(int const* p);

/*
** �������
*/
int main_3(int argc, char* argv[]) {
	int a[] = { 1,2,3,4,5 };

	test(a);						/* ���� int const* p */
	test_1(a);						/* ���� int* const p */
	test_2(a);						/* ���� const int * p */
}

/*
** ���� �β� int const* p ����
*/
void test(int const* p) {
	int b[] = { 6,7,8,9,10 };

#if 0
	p[1] = 11;						/* ���������ֵָ��const���� */
#endif

	p = b;
	printArray(p, 5);
}

/*
** ���� �β� int* const p ����
*/
void test_1(int* const p) {
	int b[] = { 6,7,8,9,10 };
	p[1] = 11;

#if 0
	p = b;							/* ���������ֵָ��const���� */
#endif

	printArray(p, 5);
}

/*
** ���� �β� const int* p ����
*/
void test_2(const int* p) {
	int b[] = { 6,7,8,9,10 };

#if 0
	p[1] = 11;						/* ���������ֵָ��const���� */
#endif

	p = b;
	printArray(p, 5);
}

/*
** ��ӡ����������Ԫ��
*/
void printArray(int* p, int len) {
	for (int i = 0; i < len; i++) {
		printf("�����%d��Ԫ�أ�%d\n", i, p[i]);
	}
}