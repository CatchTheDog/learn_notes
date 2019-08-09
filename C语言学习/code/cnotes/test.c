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
	//printf("�۳˽����%d \n", cmr);
	//printf("Բ����PI:%.10f \n",PI);
	//printf("������constant_a: %d \n",const_a);
	//char c = '?';
	//printf("�ַ��ͱ���ʵ����һ���ֽڵ����ͱ�����%c,%d \n",c,c);
	//signed char c_0 = -23;
	//printf("ASCII�ַ�����Ϊ�������������ֵΪ�������򲻻�����ַ���%c,%d \n",c_0,c_0);
	//unsigned char c_1 = 128; //��չ�ַ� 128-255
	//printf("����Ƿ�ʵ������չ�ַ���%c,%d \n",c_1,c_1);
	//sovleQEOU();
	//printfFormat();
	//scanfFormat();
	//charIO();
	//printf("����GDP��������Ϊ9%����ʮ���GDP������Ϊ��%-10.6f��",q_1(0.09, 10));
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
	printf("���һԪ���η��̿�ʼ���밴�ո�ʽ��a,b,c �ֱ��������ϵ����һ��ϵ���ͳ�����");
	scanf_s("%f,%f,%f", &a, &b, &c);
	if (pow(b, 2) - 4 * a * c > 0) {
		p = -(b / (2 * a));
		q = sqrt(pow(b, 2) - 4 * a * c) / (2 * a);
		x1 = p + q;
		x2 = p - q;
		printf("��Ԫһ�η��̣�%1.0fx^2+%1.0fx+%1.0f=0 �ĸ�Ϊ��x1=%7.2f,x2=%7.2f \n", a, b, c, x1, x2);
	}
	else {
		printf("b^2-4ac<0,�����޽⣬���޸����������");
	}

}

void printfFormat() {
	printf("%5d\n%5d\n", 12, -345);
	printf("%-5c\n", 123);
	printf("%5c\n", 377);//�Դ���127������ʹ�õ�λ��Ӧ���ַ���Ϊ���
	printf("%6s\n", "CHINA");
	printf("%20.16f\n", 1.0 / 3);
	printf("%20e\n", 11213.456);
	printf("%-20.3e\n", 11234.34334);
	printf("%cf,%%,%Dd", 123); //��������⡯�ַ�
}

void scanfFormat() {
	float a, b, c;
	char x, y, z;
	printf("�밴�����¸�ʽ����a,b,c:a=%%f,b=%%f,c=%%f:\n");
	scanf_s("a=%f,b=%f,c=%f", &a, &b, &c);
	printf("a=%10f,b=%10f,c=%10f", a, b, c);
	//fflush(stdin); //��ջ��������ڶ��scanf����ʱ����scanf���ȡǰһ������ʱ���ַ�\n ���ֽ����ʽ��Ч
	getchar();//��ȡ�������س����� ���Խ������
	printf("�밴�����¸�ʽ����x,y,z:x,y,z:\n");
	scanf_s("%c,%c,%c", &x, sizeof(x), &y, sizeof(y), &z, sizeof(z));
	printf("x=%c,y=%c,z=%c", x, y, z);
}

void charIO() {
	char a = 'B', b = 'O', c = 'Y';
	putchar(a);
	putchar(b);
	putchar(89); //��ĸY��ASCII������89����0-127֮������ִ���putchar()��Ҳ������ַ�
	putchar('\n');
	a = getchar(); //getchar()���Ի�ȡ��Ļ�޷���ʽ���ַ�������һЩ�����ַ�
	b = getchar();
	c = getchar();
	putchar(a);
	putchar(b);
	putchar(c);
	putchar('\n');
}

void sort_3() {
	double a, b, c, t;
	printf("�밴�����¸�ʽ(%%f %%f %%f)����a,b,c:\n");
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
	printf("�����˳��%5.2lf<=%5.2lf<=%5.2lf\n", a, b, c);
}

void boolType() {
	/*float score;
	scanf_s("%f",&score);
	bool a, b;
	a = score >= 60;
	b = score <= 69;
	if (a && b) printf("The grade is C\n");
	else printf("The grade can't be judged\n");*/
	printf("�����ӡһ�£�%d\n",false ? 1 : 2);
	printf("�����ӡһ�£�%d\n", 1 ? 1 : 2);
	printf("�����ӡһ�£�%d\n", 0 ? 1 : 2);
	printf("�����ӡһ�£�%d\n", '1' ? 1 : 2);
	printf("�����ӡһ�£�%d\n", " " ? 1 : 2);
	printf("�����ӡһ�£�%d\n", "" ? 1 : 2);
}

void loopTest() {
	int i = 0, sum = 0;
	while (++i <= 100) {
		sum += i;
	}
	printf("���Ϊ��%d\n",sum);
	int j;
	for ( j = 0,printf("ѭ����俪ʼ"); j < 10; j++, printf("ѭ��������Ӵ\n"));
}

void arrayTest() {
	int a[10] = {0,1,2,3,4,5,6}; //����ֻ��һ����Ԫ�ؽ��г�ʼ����ֵ
	int b[] = {7,8,9};
	float pay[3][6]; //��ά���� 3*6
	char c[11] = { "I am happy" }; //ϵͳ�Զ�����\0��Ϊ�����������Գ���Ϊ11.
	int d[11] = { "I am happy" }; //��Ϊ�ַ�������ʱ��������ʽ��ŵģ����Ҳ��������������������ַ����ݡ����Ǵ���ַ��������⡣
	printf("%s\n",c);
}

void strTest() {
	char c[] = "�����ַ�����������";

}

/*
  ʹ����������Ϊ����ʵ��
*/
float average(float array[10]) {
	int i ;
	float sum;
	for (i = 0, sum = 0.0; i < 10; i++)
		sum += array[i];
	return sum / 10;
}
/*
	�����洢���Ͳ���
*/
void storageType() {
	auto int a = 1;
	static int b = 2;
	printf("�Զ��ֲ����� a ��%d \n",++a);
	printf("��̬�ֲ����� b : %d \n",b++);
}
/*
	ָ�����
*/
void test_pointer() {
	int i;
	int * i_pointer = &i; //������i�ĵ�ַ����ָ�����i_pointer���涨��ָ�����ָ�����ͱ������˴�*��ʾ�ñ�����ָ�����
	int i_1 = &i; //������i�ĵ�ַ������ͨ����i_1,�Դ��б�����ַ����ͨ������������Ӱ�����
	i = 456;
	printf("ֱ�ӷ��ʱ���i:%d \n",i);
	*i_pointer = 789; //ͨ��ָ���������ָ�������ָ��ı���,*����ָ�������ָ��ı���,���û��*�ţ���ֻ�ı�ָ�������ֵ��������ı�ָ�������ָ��ı�����ֵ
	i_1 = 1000;
	printf("��ӷ��ʱ���i:%d \n",* i_pointer); //ͨ��ָ�������ȡ����ֵ
	printf("ָ��Ϊ��%d \n",i_pointer); //��ȡָ�����ֵ��������i�ĵ�ֵַ
	printf("ָ��2Ϊ��%d \n", i_1);
	printf("ֱ�ӷ��ʱ���i��%d \n",i);
}

void exchange_num(float a,float b) {
	float* p1 = &a,*p2 = &b,*p;
	if (a < b) {
		p = p1; //����ָ�����ֵ(������ַ)��ʹָ�����ָ��ı���������������a,b��ֵ��δ��������
		p1 = p2; 
		p2 = p;
	}
	printf("�ɴ�С�����%10.5f>=%10.5f\n",a,b);//���ô��ַ�ʽ��ȡ��ֵû�б仯
	printf("�ɴ�С�����%10.5f>=%10.5f\n",*p1,*p2);//����ָ�뷽ʽ(��ӷ���)��ȡ��ֵ�����˸ı�
}

void exchange_num_1(float *p1,float *p2) {
	float c;
	if (*p1<*p2) {
		c = *p1;
		*p1 = *p2;
		*p2 = c;
	}
	printf("�ɴ�С�����%10.5f>=%10.5f\n", *p1, *p2);
}

void array_pointer() {
	int a [] = {1,2,3};
	int* p1 = &a[0],*p2=a,*p3=&a; //�����ԣ����ַ�ʽ�����԰�������Ԫ�ص�ַ����ָ�����
	printf("ָ��p1:%d, p2:%d, p3:%d",p1,p2,p3);
	for (int i = 0; i < 3;i++) {
		printf("�����%d��Ԫ�أ�%d \n",i,*p1);
		p1 ++;//��ַ��������Ԫ�ص�ַƫ�ƣ���ȡ����Ԫ��,����1���ɣ�����д�� p1 += sizeof(int); ָ����Զ������������ͼ���ƫ������
	}
	printf("��һ�ֻ�ȡԪ�صķ�ʽ��%d \n",*(a+2));
	printf("ָ��ͬһ������Ԫ�ص�ָ����������ֵ���㣺%d \n",p1-p2);
}

void array_pointer_1(void) {
	int* p, i, a[10] = {1,2,3,4,5,6,7,8,9,10};
	p = a;
	for (i = 0; i < 10; i++)
		printf("��%d��Ԫ�أ�%d \n",i,p[i]);
}