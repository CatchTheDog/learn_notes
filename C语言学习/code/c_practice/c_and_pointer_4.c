/*
** ����һ��ָ����NULL��β��ָ���б��ָ�룬���б��е��ַ����в���һ���ض����ַ�
*/

#include <stdio.h>
#include <assert.h>

#define		TRUE	 1
#define		FALSE	 0

int
find_char(char **strings,char value) {

	char* string;									/* ���ǵ�ǰ���ڲ��ҵ��ַ��� */

	/*
	** �����б��ÿ���ַ���
	*/
	while ((string = *strings++) != NULL) {
		/*
		** ����ַ����е�ÿ���ַ��Ƿ��ǲ��ҵ��ַ�
		*/
		while (*string != '\0') {
			if (*string++ == value)
				return TRUE;
		}
	}
	return FALSE;
}


int
find_char_2(char **strings,char value) {
	assert(strings != NULL);

	/*
	** �����б��ÿ���ַ���
	*/
	while (*strings != NULL) {
		/*
		** ����ַ����е�ÿ���ַ��Ƿ��ǲ��ҵ��ַ�
		*/
		while (**strings != '\0') {
			if (*(*strings)++ == value)
				return TRUE;
		}
		strings++;
	}
	return FALSE;
}