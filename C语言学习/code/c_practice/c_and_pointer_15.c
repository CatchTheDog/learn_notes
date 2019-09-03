/*
** ָ���������
**
** ���ɣ�
**		1.ʹ��ָ������洢�ַ���������
**		2.��������ʹ���ʵ���const���α�֤���������ں����ڲ����޸Ķ�Ӱ�쵽ʵ��
**       3.ʹ��ָ��ͬһ�����ָ��������ȡ��ǰԪ���������е�λ��
*/
#include	<stdio.h>

/*
** �ؼ����б�
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

#define		N_KEYWORD	(sizeof(keyword)/sizeof(keyword[0]))		/* �ؼ��ֱ����Ԫ�ظ��� */
#define		NOT_FOUND	-1											/* ����δ�ҵ�          */

int lookup_keyword(char const *desired_word,char const *keyword_table[],int const size);

int
main_15() {
	printf("�ؼ��������ǣ�%d \n", N_KEYWORD);
}

/*
** �жϲ����Ƿ���һ���ؼ����б��е��κε���ƥ�䣬������ƥ�������ֵ�����δ�ҵ������� -1
*/
static int
lookup_keyword(char const * const desired_word,char const *keyword_table[],int const size) {
	char const** kwp;

	/*
	** �Թؼ��ֱ���ÿ���ʽ����ж�
	*/
	for (kwp = keyword_table; kwp < keyword + size;kwp++) {
		if (strcmp(desired_word, *kwp) == 0)
			/*
			** ����ؼ����������ҵ���ƥ�䣬�򷵻����ڹؼ��ֱ���λ��
			*/
			return kwp - keyword_table;
	}

	/*
	** δ�ҵ�
	*/
	return -1;
}