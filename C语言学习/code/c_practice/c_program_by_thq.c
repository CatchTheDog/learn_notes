#include <stdio.h>
#include <math.h>

int main_1(int argc, char* argv[]) {
	/*extern void que_37();
	que_37();*/
	/*配置命令行参数方法：项目->属性->调试->命令参数*/
	if (argc > 0) {
		for (int i = 0; i < argc; i++) {
			printf("第%d个形参为：%s \n", i, argv[i]);
		}
	}
	else {
		puts("未传入形参，退出。");
		exit(0);
	}
}

void que_1() {
	int year_interval = 10; //时间间隔，单位：年
	float increase_rate = 0.09; //年增长率
	float r = pow((1.0 + increase_rate), year_interval);
	printf("假设年增长率稳定为：%5.5f,%d 年后我国经济总增长率为：%f \n", increase_rate, year_interval, r);
}

void que_2() {
	int a[10] = { 1 };
	int b[] = { 1,2,3 }; //数组大小为3
}

void que_3() {
	float pay[3][4] = { {1,2,3},{5,6},{9} }; //3行4列 
	float pay1[2][3] = { 1,2 };
	float pay2[2][3] = { {1} };
	float pay3[][3] = { 1,2,3,4,5,6 };//可省略第一维长度
	char a[] = "Hello!";
}

void que_4() {
	char x[100];
	char* p;
	p = gets(x);
	printf("输入字符串为：%s \n", p);
	//x = "不能用赋值语句将一个字符串常量或者字符数组直接赋给一个字符数组，而只能采用复制的方式";
	char y[100] = "Hello,老弟。";
	strcpy(x, y);
	printf("字符串x:%s,字符串y:%s \n", x, y);
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
	int* p = &a;// p指向变量a
	p = 1000; //修改p值(地址)为1000，而非 *p = 1000;所后续 *p 无法获取到变量a的值 
	printf("变量a:%d,指针变量p所指向的变量值：%d", a, *p);
}

void que_7() {
	int a = 123;
	int* p = &a;
	printf("a的值：%d,指针p指向的值：%d,指针变量p的值：%p \n", a, *p, p);
}

void que_9() {
	int* p1, * p2, * p, a, b;
	puts("请按如下格式输入两个整数：%d,%d \n");
	scanf_s("%d,%d", &a, &b);
	p1 = &a;
	p2 = &b;
	if (a < b)
		p = p1, p1 = p2, p2 = p; //交换p1,p2所指向的变量值 可以使用 p1=&b;p2=&a;
	printf("a=%d,b=%d \n", a, b);
	printf("max=%d,min=%d \n", *p1, *p2);
}

void que_10() {
	extern void swap(int* p1, int* p2);
	extern void swap_1(int* p1, int* p2);
	int* p1, * p2, a, b;
	puts("请按如下格式输入两个整数：%d,%d \n");
	scanf_s("%d,%d", &a, &b);
	p1 = &a, p2 = &b;
	if (a < b) swap(p1, p2);
	printf("a=%d,b=%d \n", a, b);
	printf("max=%d,min=%d \n", *p1, *p2);
}

/*因为函数调用过程中值传递的缘故，所以在函数内交换形参值，对实参不会产生影响*/
void swap_1(int* p1, int* p2) {
	int* temp = p1;
	p1 = p2;
	p2 = temp;
}

/*改变指针所指向的内存区域的值，则实参所指向的内存区域的值也会发生改变*/
void swap(int* p1, int* p2) {
	int temp = *p1;
	*p1 = *p2, * p2 = temp;
}

/*数组名代表数组中首元素的地址*/
void que_11() {
	int a[] = { 1,2,3,4 };
	int* p = &a[0]; //p指向a[0] 与 int* p = a; 等价
	// p+i 与 a+i 都是数组a序号为i的元素a[i]的地址，两者等价；意即 *(p+i) 与 *(a+i) 都是 a[i]，三者等价
	//在编译时 a[i] 按照 *(a+i) 进行处理
	//若指针变量p指向数组，则p[i]合法；在编译时，对下标的处理方法时转换为地址的，对p[i]处理为*(p+i)
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
	puts("-------------------------------采用下标法遍历数组开始----------------------------- \n");
	for (int i = 0; i < len; i++) {
		printf("数组第%d个元素值为：%d \n", i, a[i]);
	}
	puts("\n-------------------------------采用下标法遍历数组结束----------------------------- \n");
}

void printArray_1(int a[], int len) {
	puts("-------------------------------采用指标法遍历数组开始----------------------------- \n");
	int* p = a;
	for (int i = 0; i < len; i++, p++) {
		printf("数组第%d个元素值为：%d \n", i, *p);
	}
	puts("\n-------------------------------采用指标法遍历数组结束----------------------------- \n");
}

void printArray_2(int a[], int len) {
	puts("-------------------------------采用指标法2遍历数组开始----------------------------- \n");
	for (int i = 0; i < len; i++) {
		printf("数组第%d个元素值为：%d \n", i, *(a + i));
	}
	puts("\n-------------------------------采用指标法2遍历数组结束----------------------------- \n");
}

/*数组名作为函数参数，形参接收到的值为实参数组的首元素地址*/
void que_15() {
	extern void printArray_3(int*, int);
	int a[] = { 1,3,4,5 }, b = 100;
	int* p = a + 1;
	printArray_3(p, 3);
	//传入非数组指针,也可以运行，说明对于数组名作为函数参数，只会检查传入是否是对应类型指针，但是不会检测指针是否指向数组
	p = &b;
	printArray_3(p, 3);
}

void printArray_3(int a[], int len) {
	for (int i = 0; i < len; i++) {
		printf("数组第%d个剩余元素为：%d \n", i, *(a + i));
	}
}

/*指针指向多维数组*/
void que_16() {
	int a[][4] = { 1,2,3,4,5,6,7,8 }; // 2*4 二维数组  
	//二维数组可以看做是数组的数组，数组名a是数组首元素地址，但是二维数组的首元素是一个一维数组
	//a[0]是二维数组的第0行——一个一维"数组名"，则a[0]表示二维数组的第0行的第一列的元素的地址，所以a[0]等价于&a[0][0]
	//则a数组0行1列的元素的地址应表示为：a[0]+1
	//a[i]和*(a+i)等价,所以a[i]+j与*(a+i)+j等价，都是&a[i][j]
	// *(a[i]+j) 或 *(*(a+i)+j) 是 a[i][j] 的值
	//在二维数组中，a[i]只是一个地址，不代表某一元素的值。
	//二维数组名a指向行，是行指针，它每增加1，跳过一行。
	//一维数组名a[i]指向列，是列指针，它每增加1，跳过本行内的一个数组元素。
	//对行指针前面加*，就会转换为列指针；对列指针前面加&,就会转换为行指针。
	//二维数组中，a+i、a[i]、*(a+i)、&a[i]、&a[i][0]的值相等，即它们都代表同一地址。
}

void que_18() {
	extern double average(float*, int);
	extern void search(float*, int);
	extern double average_1(float*, int);
	extern void search_1(float*, int);
	extern void search_3(float*, int);
	float score[][4] = { 59,67,70,60,80,87,90,20,90,99,100,45 };
	printf("平均值：%.2f \n", average(score, 12));
	printf("平均值：%.2f \n", average_1(*score, 12));//传入*score,即列指针
	printf("平均值：%.2f \n", average_1(score, 12)); //传入数组名,即行指针
	// score 与 *score 的值都是score[0]的首地址(第零行第零个元素的地址)
	search(score, 2);
	search(*score, 2); //为什么会没有问题？
	search_1(score, 2); //为什么会没有问题？
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

//返回第n个学生的成绩
void search(float(*p)[4], int n) { //p是指向具有4个元素的一维数组的指针
	printf("第%d位学生的成绩如下: \n", n);
	for (int i = 0; i < 4; i++) {
		printf("%.2f ", *(*(p + n) + i)); //注意这个写法  这里将行指针转换为列指针
	}
	puts("\n");
}

void search_1(float* p, int n) { //p是一个float指针
	printf("第%d位学生的成绩如下: \n", n);
	for (int i = 0; i < 4; i++) {
		printf("%.2f ", *(p + (n * 4) + i)); //注意这个写法 这里按照列指针移动
	}
	puts("\n");
}

void search_3(float* p, int n) {
	puts("---------------------查找有一门及以上可能不及格学生开始------------------------\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 4; j++) {
			if (*(p + i * 4 + j) < 60) {
				printf("第%d位学生的第%d门课分数为：%.2f,不及格。\n", i, j, *(p + i * 4 + j));
			}
		}
	}
	puts("---------------------查找有一门及以上可能不及格学生结束------------------------\n");

}

void que_19() {
	extern void printArrayAd();
	printArrayAd();
}
void printArrayAd() {
	int a[][3] = { 1,2,3,4,5,6 };
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			printf("数组第(%d,%d)个元素 %d 的地址是：%p - %d \n", i, j, a[i][j], &a[i][j], &a[i][j]);
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
	char* string = "I love HongKong!"; //将字符串存入一个字符数组，然后将字符数组的首元素地址赋给指针变量；在C语言中只有字符变量，没有字符串变量
	printf("%s \n", string);//通过字符数组名或字符指针变量可以输出一个字符串，而对一个数值型数组，是不能企图用数组名输出它的全部元素的
	return 0;
}

void que_22() {
	char* string = "I love HongKong!";
	char b[20] = { '\0' }; //将数组所有元素初始化为'\0'
	for (int i = 0; i < strlen(string); i++) {
		//b[i] = string[i];
		*(b + i) = *(string + i);
	}
	//b[strlen(string)] = '\0'; //没有此句，就出现‘烫烫’
	puts(b);
}

void que_23() {
	//未初始化栈变量，系统默认赋值：0xcc;未初始化堆变量，系统默认赋值：0xcd
	printf("烫字的编码：%x \n", '烫');//输出 cccc
	printf("屯字的编码：%x \n", '屯');//输出 cdcd
}
/*需要在函数之间传递字符串，可以将字符数组名或者字符指针变量作为函数参数*/
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
	char* to_ad = to; //char * to = ""; 这样是不合法的，在复制时无法向目标字符数组写入字符，复制失败。
	que_24(from, to_ad); //在创建数组之后，将数组的地址赋给指针变量，然后将指针变量作为函数参数，是可以使用的。
	puts(to);
}

int que_27(int a, int b) {
	return a > b ? a : b;
}

void que_28() {
	extern int que_29(int, int);
	int a, b, mode;
	puts("请按照如下格式‘a:%d,b:%d’的格式输入a,b: \n");
	scanf_s("a:%d,b:%d", &a, &b);
	puts("请输入求值模式：1-max,2-min:");
	scanf_s("%d", &mode);
	int (*p)(int, int);
	p = que_27;//默认为求最大值
	if (mode == 2) {
		p = que_29; //可以改变函数指针所指向的函数，从而改变指向逻辑
	}
	printf("a:%d,b:%d 中 最值为：%d \n", a, b, (*p)(a, b)); //通过函数指针调用函数
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
	int score[][4] = { 12,13,14,15,16,17,18,19,20,21,22,23 };//3*4二维数组
	int num = 2;
	int* p = search_x(score, num);
	printf("第%d位学生的成绩如下:", num);
	for (int i = 0; i < 4; i++) {
		printf("%d ", *(p + i));
	}
}

int* search_x(int* p, int n) {
	return p + n;
}

/*用指针数组存储多个字符串*/
void que_35() {
	//int(*p)[4]; //指向一维数组的指针变量
	extern void sort_x(char* name[], int n);
	char* name[] = { "Follow me","BASIC","Great Wall","FORTRAN","Computer design" };  //含有四个元素的指针数组
	sort_x(name, 4);
	for (int i = 0; i < 4; i++) {
		printf("%s ", name[i]);
	}
}

/*使用选择排序对字符串进行排序*/
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

/*多重指针——指向指针的指针*/
void que_36() {
	char* name[] = { "A","B","C","D" };
	char** p = name; //指向指针的指针
	for (int i = 0; i < 4; i++) {
		printf("数组元素%d为：%s \n", i, *(p + i));
	}
}

void que_37() {
	int a[5] = { 1,3,5,7,9 };
	int* num[] = { &a[0],&a[1],&a[2],&a[3],&a[4] };
	int** p = num, i;
	for (i = 0; i < 5; i++, p++) {
		printf("%d ", **p); //多重指针
	}
}