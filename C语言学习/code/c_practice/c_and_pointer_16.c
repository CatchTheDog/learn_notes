/*
** ����������ַ���
**     �����ַ���   char  *strcpy(char *dst, char const *src);
**	   �����ַ���   char *strcat(char *dst,char const *src);
*/

#define			_CRT_SECURE_NO_WARNINGS
#include		<string.h>
#include		<stdio.h>

char* my_strrstr(char const* s1, char const* s2);
void test_prefix();
void print_tokens();


int
main_16() {
	char a[100] = "C AND POINTER.";
	char* b = "It's Cool.";
	strcpy(a, b);
	printf("���ƺ��ַ���Ϊ��%s \n", a);
	strcat(a, b);
	printf("ƴ�Ӻ��ַ���Ϊ��%s \n", a);
	int c = strcmp(a, b);
	printf("�ַ����ȽϽ����%d \n", c);
	char* d = strchr(a, 'C');
	char* e = strrchr(a, 'C');
	printf("��һ��'C'����λ�ã�%d ,���һ��'C'����λ�ã�%d \n", d - a, e - a);
	char* f = strpbrk(a, b);
	printf("�ַ���a�е�һ�γ���b����һ�ַ���λ���ǣ�%d,���ַ��ǣ�%c \n", f - a, *f);
	char* g = strstr(a, "Co");
	printf("�ַ���a�е�һ�γ����Ӵ�'%s'��λ���ǣ�%d \n", "Co", g - a);
	char* h = my_strrstr(a, "Co");
	printf("�ַ���a�����һ�γ����Ӵ�'%s'��λ���ǣ�%d \n", "Co", h - a);

	test_prefix();

	print_tokens();
}

/*
** ���ַ���s1�в����ַ���s2���ҳ��ֵ�λ�ã�������һ��ָ���λ�õ�ָ��
** �Ժ��� char *strstr(char const *s1,char const *s2) Ϊ����ʵ�ּ���;
*/
char*
my_strrstr(char const* s1, char const* s2) {
	register char* last = NULL;								/* �ϴβ��ҵ��Ӵ���λ�� */
	register char* current = NULL;							/* ���β��ҵ��Ӵ���λ�� */

	if (*s2 != '\0') {
		current = strstr(s1, s2);
		while (current != NULL) {
			last = current;
			current = strstr(last + 1, s2);
		}
	}
	return last;
}



void
test_prefix() {
	int		len1, len2, len3, len4;
	char	buffer[] = "21,xyz,324,332,Smith,J,235-9032";

	len1 = strspn(buffer, "0123456789");
	len2 = strspn(buffer, "0123456789");
	len3 = strcspn(buffer, "0123456789");
	len4 = strcspn(buffer, ",0123456789");

	printf("len1:%d \n", len1);
	printf("len2:%d \n", len2);
	printf("len3:%d \n", len3);
	printf("len4:%d \n", len4);
}


/*
** debug��ʾ�������쳣��
**		�������쳣��д�����Ȩ�޳�ͻ��
**		**it** �� 0xF6CE94
*/
void
print_tokens() {
	char* line = "�����Ը�⣬�������ÿ�ε��� strtok ����ʱʹ�ò�ͬ�ķָ������ϡ� ��һ���ַ����Ĳ�ͬ��\r\n���ɲ�ͬ���ַ����Ϸָ���ʱ��������ɺܹ��á�\r\n";
	static char whitespace[] = " \t\f\r\v\n";
	char* token;
	for (token = strtok(line, whitespace); token != NULL; token = strtok(NULL, whitespace))
		printf("Next token is %s \n",token);
}