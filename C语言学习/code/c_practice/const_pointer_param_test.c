/*
** 此程序用于测试const关键字特性
**
** 如下三种形式的指针常量作为函数形参时，测试结果如下：
** 1.int const*p						可改变指针变量值，但不可改变指针变量指向的数组元素值
** 2.int* const p						可改变指针变量指向的数组元素值，但不可改变指针变量值
** 3.const int * p					可改变指针变量值，但不可改变指针变量指向的数组元素值
**
** 总结：const 在 * 之前，指针变量所指向的数组内元素值不可改变，指针变量值可以改变
**      const 在 * 之后，指针变量所指向的数组内元素值可改变，指针变量值不可改变
*/
#define _CRT_SECURE_NO_WARNINGS		/* 忽略内存泄露警告标识 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printArray(int* p, int len);
void test(int const* p);
void test_1(int const* p);
void test_2(int const* p);

/*
** 程序入口
*/
int main_3(int argc, char* argv[]) {
	int a[] = { 1,2,3,4,5 };

	test(a);						/* 测试 int const* p */
	test_1(a);						/* 测试 int* const p */
	test_2(a);						/* 测试 const int * p */
}

/*
** 测试 形参 int const* p 特性
*/
void test(int const* p) {
	int b[] = { 6,7,8,9,10 };

#if 0
	p[1] = 11;						/* 编译错误：左值指定const对象 */
#endif

	p = b;
	printArray(p, 5);
}

/*
** 测试 形参 int* const p 特性
*/
void test_1(int* const p) {
	int b[] = { 6,7,8,9,10 };
	p[1] = 11;

#if 0
	p = b;							/* 编译错误：左值指定const对象 */
#endif

	printArray(p, 5);
}

/*
** 测试 形参 const int* p 特性
*/
void test_2(const int* p) {
	int b[] = { 6,7,8,9,10 };

#if 0
	p[1] = 11;						/* 编译错误：左值指定const对象 */
#endif

	p = b;
	printArray(p, 5);
}

/*
** 打印数组内所有元素
*/
void printArray(int* p, int len) {
	for (int i = 0; i < len; i++) {
		printf("数组第%d个元素：%d\n", i, p[i]);
	}
}