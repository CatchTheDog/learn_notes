#include <stdio.h>
#include "sum.h"
#include "max.h"
#include "algorithm.h"
#define PI 3.141592653
#include <math.h>
#include "exercisechapter3.h"
#include <stdbool.h>

const int const_a = 3;

void sayHello(void);
void sovleQEOU();
void printfFormat();
void scanfFormat();
void charIO();
void sort_3();
void boolType();
void loopTest();
void arrayTest();
float average(float array[10]);
void storageType();
void test_pointer();
void exchange_num(float a, float b);
void exchange_num_1(float* p1, float* p2);
void array_pointer();
void array_pointer_1();

int main() {
	//sayHello();
	//printf("This is a C program.\n");
	//int a, b, result, maxNum;
	//printf("please input num_a and num_b(format:num_a,num_b):");
	//scanf_s("%d,%d",&a,&b);
	//result = sum(a,b);
	//maxNum = max(a,b);
	//printf("sum result is : %d \n",result);
	//printf("max number is : %d \n",maxNum);
	//int cmr = cumulativeMultiple(2, 1, 10);
	//printf("累乘结果：%d \n", cmr);
	//printf("圆周率PI:%.10f \n",PI);
	//printf("常变量constant_a: %d \n",const_a);
	//char c = '?';
	//printf("字符型变量实质是一个字节的整型变量：%c,%d \n",c,c);
	//signed char c_0 = -23;
	//printf("ASCII字符编码为正整数，如果赋值为负数，则不会输出字符：%c,%d \n",c_0,c_0);
	//unsigned char c_1 = 128; //扩展字符 128-255
	//printf("检测是否实现了扩展字符：%c,%d \n",c_1,c_1);
	//sovleQEOU();
	//printfFormat();
	//scanfFormat();
	//charIO();
	//printf("假如GDP年增长率为9%，则十年后GDP增长率为：%-10.6f。",q_1(0.09, 10));
	//sort_3();
	//boolType();
	//loopTest();
	//arrayTest();
	//float array[10] = {1.1,2.3,3.3,5.4,6.7,6.6,8.9,0.3,1.2,3.4};
	//printf("avg:%10.3f \n",average(array));
	//for (int i = 0; i < 3;i++) 
	//	storageType();
	//test_pointer();
	//exchange_num(1.2,3.1);
	//float a = 1.2, b = 3.1;
	//exchange_num_1(&a,&b);
	//array_pointer();
	array_pointer_1();
	return 0;
}

void sayHello(void) {
	printf("Hello.\n");
}

void sovleQEOU() {
	float a, b, c, p, q, x1, x2;
	printf("求解一元二次方程开始，请按照格式：a,b,c 分别输入二次系数、一次系数和常数：");
	scanf_s("%f,%f,%f", &a, &b, &c);
	if (pow(b, 2) - 4 * a * c > 0) {
		p = -(b / (2 * a));
		q = sqrt(pow(b, 2) - 4 * a * c) / (2 * a);
		x1 = p + q;
		x2 = p - q;
		printf("二元一次方程：%1.0fx^2+%1.0fx+%1.0f=0 的根为：x1=%7.2f,x2=%7.2f \n", a, b, c, x1, x2);
	}
	else {
		printf("b^2-4ac<0,方程无解，请修改输入参数。");
	}

}

void printfFormat() {
	printf("%5d\n%5d\n", 12, -345);
	printf("%-5c\n", 123);
	printf("%5c\n", 377);//对大于127的数，使用低位对应的字符作为输出
	printf("%6s\n", "CHINA");
	printf("%20.16f\n", 1.0 / 3);
	printf("%20e\n", 11213.456);
	printf("%-20.3e\n", 11234.34334);
	printf("%cf,%%,%Dd", 123); //输出‘特殊’字符
}

void scanfFormat() {
	float a, b, c;
	char x, y, z;
	printf("请按照如下格式输入a,b,c:a=%%f,b=%%f,c=%%f:\n");
	scanf_s("a=%f,b=%f,c=%f", &a, &b, &c);
	printf("a=%10f,b=%10f,c=%10f", a, b, c);
	//fflush(stdin); //清空缓冲区，在多个scanf连用时后续scanf会读取前一次输入时的字符\n 此种解决方式无效
	getchar();//读取缓冲区回车符号 可以解决问题
	printf("请按照如下格式输入x,y,z:x,y,z:\n");
	scanf_s("%c,%c,%c", &x, sizeof(x), &y, sizeof(y), &z, sizeof(z));
	printf("x=%c,y=%c,z=%c", x, y, z);
}

void charIO() {
	char a = 'B', b = 'O', c = 'Y';
	putchar(a);
	putchar(b);
	putchar(89); //字母Y的ASCII编码是89，将0-127之间的数字传入putchar()，也可输出字符
	putchar('\n');
	a = getchar(); //getchar()可以获取屏幕无法显式的字符，比如一些控制字符
	b = getchar();
	c = getchar();
	putchar(a);
	putchar(b);
	putchar(c);
	putchar('\n');
}

void sort_3() {
	double a, b, c, t;
	printf("请按照如下格式(%%f %%f %%f)输入a,b,c:\n");
	scanf_s("%lf %lf %lf", &a, &b, &c);
	if (a > b) {
		t = a;
		a = b;
		b = t;
	}
	if (c < a) {
		// a=c,b=a,c=b
		t = a;
		a = c;
		c = t;
		t = b;
		b = c;
		c = t;
	}
	else if (c < b) {
		t = b;
		b = c;
		c = t;
	}
	printf("排序后顺序：%5.2lf<=%5.2lf<=%5.2lf\n", a, b, c);
}

void boolType() {
	/*float score;
	scanf_s("%f",&score);
	bool a, b;
	a = score >= 60;
	b = score <= 69;
	if (a && b) printf("The grade is C\n");
	else printf("The grade can't be judged\n");*/
	printf("随机打印一下：%d\n",false ? 1 : 2);
	printf("随机打印一下：%d\n", 1 ? 1 : 2);
	printf("随机打印一下：%d\n", 0 ? 1 : 2);
	printf("随机打印一下：%d\n", '1' ? 1 : 2);
	printf("随机打印一下：%d\n", " " ? 1 : 2);
	printf("随机打印一下：%d\n", "" ? 1 : 2);
}

void loopTest() {
	int i = 0, sum = 0;
	while (++i <= 100) {
		sum += i;
	}
	printf("结果为：%d\n",sum);
	int j;
	for ( j = 0,printf("循环语句开始"); j < 10; j++, printf("循环语句结束哟\n"));
}

void arrayTest() {
	int a[10] = {0,1,2,3,4,5,6}; //可以只给一部分元素进行初始化赋值
	int b[] = {7,8,9};
	float pay[3][6]; //二维数组 3*6
	char c[11] = { "I am happy" }; //系统自动存入\0作为结束符，所以长度为11.
	int d[11] = { "I am happy" }; //因为字符型数据时以整数形式存放的，因此也可以用整型数组来存放字符数据。但是存放字符串有问题。
	printf("%s\n",c);
}

void strTest() {
	char c[] = "测试字符串处理函数。";

}

/*
  使用数组名作为函数实参
*/
float average(float array[10]) {
	int i ;
	float sum;
	for (i = 0, sum = 0.0; i < 10; i++)
		sum += array[i];
	return sum / 10;
}
/*
	变量存储类型测试
*/
void storageType() {
	auto int a = 1;
	static int b = 2;
	printf("自动局部变量 a ：%d \n",++a);
	printf("静态局部变量 b : %d \n",b++);
}
/*
	指针测试
*/
void test_pointer() {
	int i;
	int * i_pointer = &i; //将变量i的地址赋给指针变量i_pointer并规定该指针变量指向整型变量，此处*表示该变量是指针变量
	int i_1 = &i; //将变量i的地址赋给普通变量i_1,对存有变量地址的普通变量操作不会影响变量
	i = 456;
	printf("直接访问变量i:%d \n",i);
	*i_pointer = 789; //通过指针变量操作指针变量所指向的变量,*代表指针变量所指向的变量,如果没有*号，则只改变指针变量的值，而不会改变指针变量所指向的变量的值
	i_1 = 1000;
	printf("间接访问变量i:%d \n",* i_pointer); //通过指针变量获取变量值
	printf("指针为：%d \n",i_pointer); //获取指针变量值，即变量i的地址值
	printf("指针2为：%d \n", i_1);
	printf("直接访问变量i：%d \n",i);
}

void exchange_num(float a,float b) {
	float* p1 = &a,*p2 = &b,*p;
	if (a < b) {
		p = p1; //交换指针变量值(变量地址)，使指针变量指向的变量发生交换，而a,b的值并未发生交换
		p1 = p2; 
		p2 = p;
	}
	printf("由大到小输出：%10.5f>=%10.5f\n",a,b);//采用此种方式读取的值没有变化
	printf("由大到小输出：%10.5f>=%10.5f\n",*p1,*p2);//采用指针方式(间接访问)读取的值发生了改变
}

void exchange_num_1(float *p1,float *p2) {
	float c;
	if (*p1<*p2) {
		c = *p1;
		*p1 = *p2;
		*p2 = c;
	}
	printf("由大到小输出：%10.5f>=%10.5f\n", *p1, *p2);
}

void array_pointer() {
	int a [] = {1,2,3};
	int* p1 = &a[0],*p2=a,*p3=&a; //经测试，三种方式都可以把数组首元素地址赋给指针变量
	printf("指针p1:%d, p2:%d, p3:%d",p1,p2,p3);
	for (int i = 0; i < 3;i++) {
		printf("数组第%d个元素：%d \n",i,*p1);
		p1 ++;//地址从数组首元素地址偏移，获取其他元素,仅加1即可，毋须写成 p1 += sizeof(int); 指针会自动根据数据类型计算偏移量。
	}
	printf("另一种获取元素的方式：%d \n",*(a+2));
	printf("指向同一数组内元素的指针可以做求差值运算：%d \n",p1-p2);
}

void array_pointer_1(void) {
	int* p, i, a[10] = {1,2,3,4,5,6,7,8,9,10};
	p = a;
	for (i = 0; i < 10; i++)
		printf("第%d个元素：%d \n",i,p[i]);
}