/*
** 指针数组测试
**
** 技巧：
**		1.使用指针数组存储字符串常量；
**		2.函数参数使用适当的const修饰保证参数不会在函数内部被修改而影响到实参
**       3.使用指向同一数组的指针求差集来获取当前元素在数组中的位置
*/
#include	<stdio.h>

/*
** 关键字列表
*/
static	char	const	*keyword[] = {
		"do",
		"for",
		"if",
		"register",
		"return",
		"switch",
		"while"
};

#define		N_KEYWORD	(sizeof(keyword)/sizeof(keyword[0]))		/* 关键字表包含元素个数 */
#define		NOT_FOUND	-1											/* 单词未找到          */

int lookup_keyword(char const *desired_word,char const *keyword_table[],int const size);

int
main_15() {
	printf("关键字数量是：%d \n", N_KEYWORD);
}

/*
** 判断参数是否与一个关键字列表中的任何单词匹配，并返回匹配的索引值。如果未找到，返回 -1
*/
static int
lookup_keyword(char const * const desired_word,char const *keyword_table[],int const size) {
	char const** kwp;

	/*
	** 对关键字表中每个词进行判断
	*/
	for (kwp = keyword_table; kwp < keyword + size;kwp++) {
		if (strcmp(desired_word, *kwp) == 0)
			/*
			** 如果关键字与所查找单词匹配，则返回它在关键字表中位置
			*/
			return kwp - keyword_table;
	}

	/*
	** 未找到
	*/
	return -1;
}