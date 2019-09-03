/*
** 使用二维数组存储字符串
**
**
*/
#include	<stdio.h>


/*
** 关键字列表
*/
char const keyword[][9] = {
	"do",
	"for",
	"if",
	"register",
	"return",
	"switch",
	"while"
};

#define		N_KEYWORD	(sizeof(keyword)/sizeof(keyword[0]))		/* 关键字列表大小 */
#define		NOT_FOUND	-1											/* 未找到 */

static int lookup_keyword(char const* const desired_word, char const keyword[][9],int const size);

int
main_15_b() {
	char word[] = "while";
	int found_result = lookup_keyword(word,keyword, N_KEYWORD);
	printf("word:%s 在关键词列表中查找结果是：%d \n",word,found_result);
}


static int
lookup_keyword(char const * const desired_word,char const keyword[][9],int const size) {

	/*
	** 使用 char *p 作为计数器，查询结果为 54 = 6 * 9
	** 使用 char(*p)[9] 作为计数器，查询结果为 6
	*/
	for (char (*p)[9] = keyword;p < keyword + size;p++) {
		if (strcmp(desired_word , p) == 0) {
			return p - keyword;
		}
	}

	return -1;
}

