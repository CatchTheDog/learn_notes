/*
** 读取、排序和打印一列整型值
*/
#define			_CRT_SECURE_NO_WARNINGS
#include		<stdlib.h>
#include		<stdio.h>

int test_1();
int test_2();

/*
** 该函数由‘qsort’调用，用于比较整型值
*/
int
compare_integers(void const* a, void const* b) {
	register int	const* pa = a;
	register int	const* pb = b;
	return *pa > * pb ? 1 : *pa < *pb ? -1 : 0;
}


int
main_19() {
	//test_1();

	test_2();
}

/*
** 测试内存分配和释放使用
*/
static int
test_1() {
	int* array;						/* 分配内存指针 */
	int		n_values;					/* 存储值个数 */
	int		i;							/* 计数器 */

	/*
	** 观察共有多少个值
	*/
	printf("How many values are there ?");
	if (scanf_s("%d", &n_values) != 1 || n_values <= 0) {
		printf("Illegal number of values. \n");
		exit(EXIT_FAILURE);
	}

	/*
	** 分配内存，用于存储这些值
	*/
	array = malloc(n_values * sizeof(int));
	if (array == NULL) {
		printf("Can't get memory for that many values. \n");
		exit(EXIT_FAILURE);
	}

	/*
	** 读取这些值
	*/
	for (i = 0; i < n_values; i++) {
		printf("? ");
		if (scanf_s("%d", array + i) != 1) {
			printf("Error reading value #%d \n", i);
			free(array);
			exit(EXIT_FAILURE);
		}
	}

	/*
	** 对输入的值进行排序
	*/
	qsort(array, n_values, sizeof(int), compare_integers);

	/*
	** 打印这些值
	*/
	for (i = 0; i < n_values; i++)
		printf("%d \n", array[i]);

	/*
	** 释放内存并退出
	*/
	free(array);
	return EXIT_SUCCESS;
}

static int
test_2() {
	char		p[20];																/* 字符指针 */
	char* p1 = malloc(20 * sizeof(char));									/* 动态分配内存指针 */
	int			maxLen = 20;														/* 最大字符长度 */
	char* quit = "quit";

	printf("请输入字符串：\n");
	while (scanf("%s", p) > 0 && strcmp(p, quit) != 0) {
		int len = strlen(p) + 1;
		if (len > maxLen) {
			maxLen = len;
			p1 = realloc(p1, maxLen);
		}
		if (p1 != NULL)
			strcpy(p1, p);
		else {
			printf("分配内存失败。");
			return EXIT_FAILURE;
		}
		printf("maxLen:%d,string:%s \n", maxLen, p1);
	}

	/*
	** 释放内存
	*/
	free(p1);
	return EXIT_SUCCESS;
}

char*
strdup(char const* string) {
	char* new_string;

	/*
	** 请求足够长度的内存，用于存储字符串和它的结尾NUL字节
	*/
	new_string = malloc(strlen(string) + 1);

	/*
	** 如果得到内存，就复制字符串
	*/
	if (new_string != NULL)
		strcpy(new_string, string);

	return new_string;
}