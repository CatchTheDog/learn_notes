#include <stdio.h>
#include <math.h>

int main_1(int argc, char* argv[]) {
	/*extern void que_37();
	que_37();*/
	/*���������в�����������Ŀ->����->����->�������*/
	if (argc > 0) {
		for (int i = 0; i < argc; i++) {
			printf("��%d���β�Ϊ��%s \n", i, argv[i]);
		}
	}
	else {
		puts("δ�����βΣ��˳���");
		exit(0);
	}
}

void que_1() {
	int year_interval = 10; //ʱ��������λ����
	float increase_rate = 0.09; //��������
	float r = pow((1.0 + increase_rate), year_interval);
	printf("�������������ȶ�Ϊ��%5.5f,%d ����ҹ�������������Ϊ��%f \n", increase_rate, year_interval, r);
}

void que_2() {
	int a[10] = { 1 };
	int b[] = { 1,2,3 }; //�����СΪ3
}

void que_3() {
	float pay[3][4] = { {1,2,3},{5,6},{9} }; //3��4�� 
	float pay1[2][3] = { 1,2 };
	float pay2[2][3] = { {1} };
	float pay3[][3] = { 1,2,3,4,5,6 };//��ʡ�Ե�һά����
	char a[] = "Hello!";
}

void que_4() {
	char x[100];
	char* p;
	p = gets(x);
	printf("�����ַ���Ϊ��%s \n", p);
	//x = "�����ø�ֵ��佫һ���ַ������������ַ�����ֱ�Ӹ���һ���ַ����飬��ֻ�ܲ��ø��Ƶķ�ʽ";
	char y[100] = "Hello,�ϵܡ�";
	strcpy(x, y);
	printf("�ַ���x:%s,�ַ���y:%s \n", x, y);
}

void que_5() {
	int a = 100, b = 10;
	int* pointer_1, * pointer_2;
	pointer_1 = &a;
	pointer_2 = &b;
	printf("a=%d,b=%d \n", a, b);
	printf("* pointer_1=%d,*pointer_2=%d \n", *pointer_1, *pointer_2);
}

void que_6() {
	int a = 100;
	int* p = &a;// pָ�����a
	p = 1000; //�޸�pֵ(��ַ)Ϊ1000������ *p = 1000;������ *p �޷���ȡ������a��ֵ 
	printf("����a:%d,ָ�����p��ָ��ı���ֵ��%d", a, *p);
}

void que_7() {
	int a = 123;
	int* p = &a;
	printf("a��ֵ��%d,ָ��pָ���ֵ��%d,ָ�����p��ֵ��%p \n", a, *p, p);
}

void que_9() {
	int* p1, * p2, * p, a, b;
	puts("�밴���¸�ʽ��������������%d,%d \n");
	scanf_s("%d,%d", &a, &b);
	p1 = &a;
	p2 = &b;
	if (a < b)
		p = p1, p1 = p2, p2 = p; //����p1,p2��ָ��ı���ֵ ����ʹ�� p1=&b;p2=&a;
	printf("a=%d,b=%d \n", a, b);
	printf("max=%d,min=%d \n", *p1, *p2);
}

void que_10() {
	extern void swap(int* p1, int* p2);
	extern void swap_1(int* p1, int* p2);
	int* p1, * p2, a, b;
	puts("�밴���¸�ʽ��������������%d,%d \n");
	scanf_s("%d,%d", &a, &b);
	p1 = &a, p2 = &b;
	if (a < b) swap(p1, p2);
	printf("a=%d,b=%d \n", a, b);
	printf("max=%d,min=%d \n", *p1, *p2);
}

/*��Ϊ�������ù�����ֵ���ݵ�Ե�ʣ������ں����ڽ����β�ֵ����ʵ�β������Ӱ��*/
void swap_1(int* p1, int* p2) {
	int* temp = p1;
	p1 = p2;
	p2 = temp;
}

/*�ı�ָ����ָ����ڴ������ֵ����ʵ����ָ����ڴ������ֵҲ�ᷢ���ı�*/
void swap(int* p1, int* p2) {
	int temp = *p1;
	*p1 = *p2, * p2 = temp;
}

/*������������������Ԫ�صĵ�ַ*/
void que_11() {
	int a[] = { 1,2,3,4 };
	int* p = &a[0]; //pָ��a[0] �� int* p = a; �ȼ�
	// p+i �� a+i ��������a���Ϊi��Ԫ��a[i]�ĵ�ַ�����ߵȼۣ��⼴ *(p+i) �� *(a+i) ���� a[i]�����ߵȼ�
	//�ڱ���ʱ a[i] ���� *(a+i) ���д���
	//��ָ�����pָ�����飬��p[i]�Ϸ����ڱ���ʱ�����±�Ĵ�����ʱת��Ϊ��ַ�ģ���p[i]����Ϊ*(p+i)
}

void que_12() {
	extern void printArray(int*, int);
	extern void printArray_1(int*, int);
	extern void printArray_2(int*, int);
	int a[] = { 1,2,3,4,5,6,7,8,9,10 };
	int len = 10;
	printArray(a, len);
	printArray_1(a, len);
	printArray_2(a, len);
}

static void printArray(int a[], int len) {
	puts("-------------------------------�����±귨�������鿪ʼ----------------------------- \n");
	for (int i = 0; i < len; i++) {
		printf("�����%d��Ԫ��ֵΪ��%d \n", i, a[i]);
	}
	puts("\n-------------------------------�����±귨�����������----------------------------- \n");
}

void printArray_1(int a[], int len) {
	puts("-------------------------------����ָ�귨�������鿪ʼ----------------------------- \n");
	int* p = a;
	for (int i = 0; i < len; i++, p++) {
		printf("�����%d��Ԫ��ֵΪ��%d \n", i, *p);
	}
	puts("\n-------------------------------����ָ�귨�����������----------------------------- \n");
}

void printArray_2(int a[], int len) {
	puts("-------------------------------����ָ�귨2�������鿪ʼ----------------------------- \n");
	for (int i = 0; i < len; i++) {
		printf("�����%d��Ԫ��ֵΪ��%d \n", i, *(a + i));
	}
	puts("\n-------------------------------����ָ�귨2�����������----------------------------- \n");
}

/*��������Ϊ�����������βν��յ���ֵΪʵ���������Ԫ�ص�ַ*/
void que_15() {
	extern void printArray_3(int*, int);
	int a[] = { 1,3,4,5 }, b = 100;
	int* p = a + 1;
	printArray_3(p, 3);
	//���������ָ��,Ҳ�������У�˵��������������Ϊ����������ֻ���鴫���Ƿ��Ƕ�Ӧ����ָ�룬���ǲ�����ָ���Ƿ�ָ������
	p = &b;
	printArray_3(p, 3);
}

void printArray_3(int a[], int len) {
	for (int i = 0; i < len; i++) {
		printf("�����%d��ʣ��Ԫ��Ϊ��%d \n", i, *(a + i));
	}
}

/*ָ��ָ���ά����*/
void que_16() {
	int a[][4] = { 1,2,3,4,5,6,7,8 }; // 2*4 ��ά����  
	//��ά������Կ�������������飬������a��������Ԫ�ص�ַ�����Ƕ�ά�������Ԫ����һ��һά����
	//a[0]�Ƕ�ά����ĵ�0�С���һ��һά"������"����a[0]��ʾ��ά����ĵ�0�еĵ�һ�е�Ԫ�صĵ�ַ������a[0]�ȼ���&a[0][0]
	//��a����0��1�е�Ԫ�صĵ�ַӦ��ʾΪ��a[0]+1
	//a[i]��*(a+i)�ȼ�,����a[i]+j��*(a+i)+j�ȼۣ�����&a[i][j]
	// *(a[i]+j) �� *(*(a+i)+j) �� a[i][j] ��ֵ
	//�ڶ�ά�����У�a[i]ֻ��һ����ַ��������ĳһԪ�ص�ֵ��
	//��ά������aָ���У�����ָ�룬��ÿ����1������һ�С�
	//һά������a[i]ָ���У�����ָ�룬��ÿ����1�����������ڵ�һ������Ԫ�ء�
	//����ָ��ǰ���*���ͻ�ת��Ϊ��ָ�룻����ָ��ǰ���&,�ͻ�ת��Ϊ��ָ�롣
	//��ά�����У�a+i��a[i]��*(a+i)��&a[i]��&a[i][0]��ֵ��ȣ������Ƕ�����ͬһ��ַ��
}

void que_18() {
	extern double average(float*, int);
	extern void search(float*, int);
	extern double average_1(float*, int);
	extern void search_1(float*, int);
	extern void search_3(float*, int);
	float score[][4] = { 59,67,70,60,80,87,90,20,90,99,100,45 };
	printf("ƽ��ֵ��%.2f \n", average(score, 12));
	printf("ƽ��ֵ��%.2f \n", average_1(*score, 12));//����*score,����ָ��
	printf("ƽ��ֵ��%.2f \n", average_1(score, 12)); //����������,����ָ��
	// score �� *score ��ֵ����score[0]���׵�ַ(�����е����Ԫ�صĵ�ַ)
	search(score, 2);
	search(*score, 2); //Ϊʲô��û�����⣿
	search_1(score, 2); //Ϊʲô��û�����⣿
	search_1(*score, 2);
	search_3(score, 3);
}

double average(float* p, int n) {
	float sum = 0, avg;
	for (int i = 0; i < n; i++, p++) {
		sum += *p;
	}
	avg = sum / n;
	return avg;
}

double average_1(float* p, int n) {
	float* p_end, sum = 0, aver;
	p_end = p + n - 1;
	for (; p <= p_end; p++) {
		sum += *p;
	}
	aver = sum / n;
	return aver;
}

//���ص�n��ѧ���ĳɼ�
void search(float(*p)[4], int n) { //p��ָ�����4��Ԫ�ص�һά�����ָ��
	printf("��%dλѧ���ĳɼ�����: \n", n);
	for (int i = 0; i < 4; i++) {
		printf("%.2f ", *(*(p + n) + i)); //ע�����д��  ���ｫ��ָ��ת��Ϊ��ָ��
	}
	puts("\n");
}

void search_1(float* p, int n) { //p��һ��floatָ��
	printf("��%dλѧ���ĳɼ�����: \n", n);
	for (int i = 0; i < 4; i++) {
		printf("%.2f ", *(p + (n * 4) + i)); //ע�����д�� ���ﰴ����ָ���ƶ�
	}
	puts("\n");
}

void search_3(float* p, int n) {
	puts("---------------------������һ�ż����Ͽ��ܲ�����ѧ����ʼ------------------------\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 4; j++) {
			if (*(p + i * 4 + j) < 60) {
				printf("��%dλѧ���ĵ�%d�ſη���Ϊ��%.2f,������\n", i, j, *(p + i * 4 + j));
			}
		}
	}
	puts("---------------------������һ�ż����Ͽ��ܲ�����ѧ������------------------------\n");

}

void que_19() {
	extern void printArrayAd();
	printArrayAd();
}
void printArrayAd() {
	int a[][3] = { 1,2,3,4,5,6 };
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			printf("�����(%d,%d)��Ԫ�� %d �ĵ�ַ�ǣ�%p - %d \n", i, j, a[i][j], &a[i][j], &a[i][j]);
		}
	}
}

void  que_20() {
	char string[] = "I love HongKong!";
	printf("%s \n", string);
	printf("%c \n", string[7]);
	return 0;
}

void que_21() {
	char* string = "I love HongKong!"; //���ַ�������һ���ַ����飬Ȼ���ַ��������Ԫ�ص�ַ����ָ���������C������ֻ���ַ�������û���ַ�������
	printf("%s \n", string);//ͨ���ַ����������ַ�ָ������������һ���ַ���������һ����ֵ�����飬�ǲ�����ͼ���������������ȫ��Ԫ�ص�
	return 0;
}

void que_22() {
	char* string = "I love HongKong!";
	char b[20] = { '\0' }; //����������Ԫ�س�ʼ��Ϊ'\0'
	for (int i = 0; i < strlen(string); i++) {
		//b[i] = string[i];
		*(b + i) = *(string + i);
	}
	//b[strlen(string)] = '\0'; //û�д˾䣬�ͳ��֡����̡�
	puts(b);
}

void que_23() {
	//δ��ʼ��ջ������ϵͳĬ�ϸ�ֵ��0xcc;δ��ʼ���ѱ�����ϵͳĬ�ϸ�ֵ��0xcd
	printf("���ֵı��룺%x \n", '��');//��� cccc
	printf("���ֵı��룺%x \n", '��');//��� cdcd
}
/*��Ҫ�ں���֮�䴫���ַ��������Խ��ַ������������ַ�ָ�������Ϊ��������*/
void que_24(char from[], char to[]) {
	for (int i = 0; i < strlen(from); i++) {
		to[i] = from[i];
	}
}

void que_25() {
	char from[] = "I learn C.";
	char to[20] = { '\0' };
	que_24(from, to);
	puts(to);
}

void que_26() {
	char* from = "I learn C.";
	char to[20] = { '\0' };
	char* to_ad = to; //char * to = ""; �����ǲ��Ϸ��ģ��ڸ���ʱ�޷���Ŀ���ַ�����д���ַ�������ʧ�ܡ�
	que_24(from, to_ad); //�ڴ�������֮�󣬽�����ĵ�ַ����ָ�������Ȼ��ָ�������Ϊ�����������ǿ���ʹ�õġ�
	puts(to);
}

int que_27(int a, int b) {
	return a > b ? a : b;
}

void que_28() {
	extern int que_29(int, int);
	int a, b, mode;
	puts("�밴�����¸�ʽ��a:%d,b:%d���ĸ�ʽ����a,b: \n");
	scanf_s("a:%d,b:%d", &a, &b);
	puts("��������ֵģʽ��1-max,2-min:");
	scanf_s("%d", &mode);
	int (*p)(int, int);
	p = que_27;//Ĭ��Ϊ�����ֵ
	if (mode == 2) {
		p = que_29; //���Ըı亯��ָ����ָ��ĺ������Ӷ��ı�ָ���߼�
	}
	printf("a:%d,b:%d �� ��ֵΪ��%d \n", a, b, (*p)(a, b)); //ͨ������ָ����ú���
}

int que_29(int a, int b) {
	return a < b ? a : b;
}

int que_30(int (*p)(int, int), int (*q)(int, int)) {
	int a, b, i = 3, j = 5;
	a = (*p)(i, j);
	b = (*q)(j, i);
	printf("a:%d,b:%d \n", a, b);
}

int que_31(int a, int b) {
	return a + b;
}

int que_32(int a, int b) {
	return a - b;
}

void que_33() {
	int (*sum)(int, int) = que_31;
	int (*minus)(int, int) = que_32;
	que_30(sum, minus);
}

void que_34() {
	extern int* search_x(int*, int);
	int score[][4] = { 12,13,14,15,16,17,18,19,20,21,22,23 };//3*4��ά����
	int num = 2;
	int* p = search_x(score, num);
	printf("��%dλѧ���ĳɼ�����:", num);
	for (int i = 0; i < 4; i++) {
		printf("%d ", *(p + i));
	}
}

int* search_x(int* p, int n) {
	return p + n;
}

/*��ָ������洢����ַ���*/
void que_35() {
	//int(*p)[4]; //ָ��һά�����ָ�����
	extern void sort_x(char* name[], int n);
	char* name[] = { "Follow me","BASIC","Great Wall","FORTRAN","Computer design" };  //�����ĸ�Ԫ�ص�ָ������
	sort_x(name, 4);
	for (int i = 0; i < 4; i++) {
		printf("%s ", name[i]);
	}
}

/*ʹ��ѡ��������ַ�����������*/
void sort_x(char* name[], int n) {
	char* temp;
	int i, j, k;
	for (i = 0; i < n - 1; i++) {
		k = i;
		for (j = i + 1; j < n; j++) {
			if (strcmp(name[k], name[j]) > 0) k = j;
		}
		if (k != i) {
			temp = name[i];
			name[i] = name[k];
			name[k] = temp;
		}
	}
}

/*����ָ�롪��ָ��ָ���ָ��*/
void que_36() {
	char* name[] = { "A","B","C","D" };
	char** p = name; //ָ��ָ���ָ��
	for (int i = 0; i < 4; i++) {
		printf("����Ԫ��%dΪ��%s \n", i, *(p + i));
	}
}

void que_37() {
	int a[5] = { 1,3,5,7,9 };
	int* num[] = { &a[0],&a[1],&a[2],&a[3],&a[4] };
	int** p = num, i;
	for (i = 0; i < 5; i++, p++) {
		printf("%d ", **p); //����ָ��
	}
}