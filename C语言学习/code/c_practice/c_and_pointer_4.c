/*
** 给定一个指向以NULL结尾的指针列表的指针，在列表中的字符串中查找一个特定的字符
*/

#include <stdio.h>
#include <assert.h>

#define		TRUE	 1
#define		FALSE	 0

int
find_char(char **strings,char value) {

	char* string;									/* 我们当前正在查找的字符串 */

	/*
	** 处理列表的每个字符串
	*/
	while ((string = *strings++) != NULL) {
		/*
		** 检测字符串中的每个字符是否是查找的字符
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
	** 处理列表的每个字符串
	*/
	while (*strings != NULL) {
		/*
		** 检测字符串中的每个字符是否是查找的字符
		*/
		while (**strings != '\0') {
			if (*(*strings)++ == value)
				return TRUE;
		}
		strings++;
	}
	return FALSE;
}