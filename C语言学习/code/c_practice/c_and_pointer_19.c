/*
** ��ȡ������ʹ�ӡһ������ֵ
*/
#define			_CRT_SECURE_NO_WARNINGS
#include		<stdlib.h>
#include		<stdio.h>

int test_1();
int test_2();

/*
** �ú����ɡ�qsort�����ã����ڱȽ�����ֵ
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
** �����ڴ������ͷ�ʹ��
*/
static int
test_1() {
	int* array;						/* �����ڴ�ָ�� */
	int		n_values;					/* �洢ֵ���� */
	int		i;							/* ������ */

	/*
	** �۲칲�ж��ٸ�ֵ
	*/
	printf("How many values are there ?");
	if (scanf_s("%d", &n_values) != 1 || n_values <= 0) {
		printf("Illegal number of values. \n");
		exit(EXIT_FAILURE);
	}

	/*
	** �����ڴ棬���ڴ洢��Щֵ
	*/
	array = malloc(n_values * sizeof(int));
	if (array == NULL) {
		printf("Can't get memory for that many values. \n");
		exit(EXIT_FAILURE);
	}

	/*
	** ��ȡ��Щֵ
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
	** �������ֵ��������
	*/
	qsort(array, n_values, sizeof(int), compare_integers);

	/*
	** ��ӡ��Щֵ
	*/
	for (i = 0; i < n_values; i++)
		printf("%d \n", array[i]);

	/*
	** �ͷ��ڴ沢�˳�
	*/
	free(array);
	return EXIT_SUCCESS;
}

static int
test_2() {
	char		p[20];																/* �ַ�ָ�� */
	char* p1 = malloc(20 * sizeof(char));									/* ��̬�����ڴ�ָ�� */
	int			maxLen = 20;														/* ����ַ����� */
	char* quit = "quit";

	printf("�������ַ�����\n");
	while (scanf("%s", p) > 0 && strcmp(p, quit) != 0) {
		int len = strlen(p) + 1;
		if (len > maxLen) {
			maxLen = len;
			p1 = realloc(p1, maxLen);
		}
		if (p1 != NULL)
			strcpy(p1, p);
		else {
			printf("�����ڴ�ʧ�ܡ�");
			return EXIT_FAILURE;
		}
		printf("maxLen:%d,string:%s \n", maxLen, p1);
	}

	/*
	** �ͷ��ڴ�
	*/
	free(p1);
	return EXIT_SUCCESS;
}

char*
strdup(char const* string) {
	char* new_string;

	/*
	** �����㹻���ȵ��ڴ棬���ڴ洢�ַ��������Ľ�βNUL�ֽ�
	*/
	new_string = malloc(strlen(string) + 1);

	/*
	** ����õ��ڴ棬�͸����ַ���
	*/
	if (new_string != NULL)
		strcpy(new_string, string);

	return new_string;
}