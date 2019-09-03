/*
** 此程序用于练习C语言字符串操作
**
** 字符串
*/

#define _CRT_SECURE_NO_WARNINGS				/* 忽略内存泄露警告标识 */
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
** 程序入口
*/
int main_4(int argc, char* argv[]) {

	char a[50] = "C program.";				/* 第一字符数组要有足够空间用于容纳复制/拼接的第二个字符串 */
	char b[  ] = "C & Pointer.";

	//stringcat(a, b);						/* 测试strcat(a,b) 函数 */
	//stringcpy(a, b);						/* 测试strcpy(a,b) 函数 */
	//stringncpy(a, b);						/* 测试strncpy(a,b,n) 函数 */
	//stringcmp(a, b);						/* 测试strcmp(a,b) 函数 */
	stringx(a);								/* 测试strlen,strlwr,strupr 函数 */
	stringchr(a,'p');
}

/*
** 测试strcat函数
*/
void stringcat(char a[],char b[]) {
	strcat(a,b);
	puts(a);
}

/*
** 测试strcpy函数
*/
void stringcpy(char * a,char * b) {
	strcpy(a,b);
	puts(a);
}

/*
** 测试strncpy函数
*/
void stringncpy(char a[],char b[]) {
	strncpy(a,b,3);							/* 不会在赋值后的字符串末尾添加 \0 字符 */
	puts(a);
}

/*
** 测试strcmp函数
*/
void stringcmp(char a[],char b[]) {
	int n = strcmp(a, b);
	printf("字符串a:%s与字符串b:%s比较结果：%d\n",a,b,n);
}

/*
** 字符串长度测试，转换为全大写/全小写 测试
*/
void stringx(char a[]) {

	printf("字符串a：%s 长度为：%d \n",a,strlen(a));

	/*
	** strlwr/strupr 已被注释
	*/
#if 0
	printf("字符串a: %s 转换为大写：%s \n",a,strupr(a));
	printf("字符串a: %s 转换为小写：%s \n", a, strlwr(a));
#endif

}

/*
** strchr函数测试
*/
void stringchr(char a[],char c) {
	char* p = strchr(a,c);
	printf("字符'%c'在字符串\"%s\"中的位置是：%p",c,a,p);
}