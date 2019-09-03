/*
** ʹ�ö�ά����洢�ַ���
**
**
*/
#include	<stdio.h>


/*
** �ؼ����б�
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

#define		N_KEYWORD	(sizeof(keyword)/sizeof(keyword[0]))		/* �ؼ����б��С */
#define		NOT_FOUND	-1											/* δ�ҵ� */

static int lookup_keyword(char const* const desired_word, char const keyword[][9],int const size);

int
main_15_b() {
	char word[] = "while";
	int found_result = lookup_keyword(word,keyword, N_KEYWORD);
	printf("word:%s �ڹؼ����б��в��ҽ���ǣ�%d \n",word,found_result);
}


static int
lookup_keyword(char const * const desired_word,char const keyword[][9],int const size) {

	/*
	** ʹ�� char *p ��Ϊ����������ѯ���Ϊ 54 = 6 * 9
	** ʹ�� char(*p)[9] ��Ϊ����������ѯ���Ϊ 6
	*/
	for (char (*p)[9] = keyword;p < keyword + size;p++) {
		if (strcmp(desired_word , p) == 0) {
			return p - keyword;
		}
	}

	return -1;
}

