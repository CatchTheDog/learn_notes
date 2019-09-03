/*
** 本程序测试字符串
**     复制字符串   char  *strcpy(char *dst, char const *src);
**	   连接字符串   char *strcat(char *dst,char const *src);
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
	printf("复制后字符串为：%s \n", a);
	strcat(a, b);
	printf("拼接后字符串为：%s \n", a);
	int c = strcmp(a, b);
	printf("字符串比较结果：%d \n", c);
	char* d = strchr(a, 'C');
	char* e = strrchr(a, 'C');
	printf("第一个'C'出现位置：%d ,最后一个'C'出现位置：%d \n", d - a, e - a);
	char* f = strpbrk(a, b);
	printf("字符串a中第一次出现b中任一字符的位置是：%d,此字符是：%c \n", f - a, *f);
	char* g = strstr(a, "Co");
	printf("字符串a中第一次出现子串'%s'的位置是：%d \n", "Co", g - a);
	char* h = my_strrstr(a, "Co");
	printf("字符串a中最后一次出现子串'%s'的位置是：%d \n", "Co", h - a);

	test_prefix();

	print_tokens();
}

/*
** 在字符串s1中查找字符串s2最右出现的位置，并返回一个指向该位置的指针
** 以函数 char *strstr(char const *s1,char const *s2) 为基础实现即可;
*/
char*
my_strrstr(char const* s1, char const* s2) {
	register char* last = NULL;								/* 上次查找到子串的位置 */
	register char* current = NULL;							/* 本次查找到子串的位置 */

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
** debug显示，程序异常：
**		引发了异常：写入访问权限冲突。
**		**it** 是 0xF6CE94
*/
void
print_tokens() {
	char* line = "如果你愿意，你可以在每次调用 strtok 函数时使用不同的分隔符集合。 当一个字符串的不同部\r\n分由不同的字符集合分隔的时候，这个技巧很管用。\r\n";
	static char whitespace[] = " \t\f\r\v\n";
	char* token;
	for (token = strtok(line, whitespace); token != NULL; token = strtok(NULL, whitespace))
		printf("Next token is %s \n",token);
}