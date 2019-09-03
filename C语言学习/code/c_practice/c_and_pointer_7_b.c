/*
** 用于维护一个地址列表的抽象数据类型
**
** 演示C语言限制全局变量和函数的作用域对实现数据封装的功能
*/

#include	"c_and_pointer_7_a.h"
#include	<stdio.h>
#define		NOT_FOUND		-1;			/* 字符串未找到时返回此值 */

/*
** 每个地址的三个部分，分别保存于三个数组的对应元素中
*/
static		char		name[MAX_ADDRESSES][NAME_LENGTH];
static		char		address[MAX_ADDRESSES][ADDR_LENGTH];
static		char		phone[MAX_ADDRESSES][PHONE_LENGHT];


/*
** 在数组中查找一个名字并返回查找到的位置的下标
** 若这个名字在数组中不存在，则返回 -1 
*/
static int
find_entry(char const *name_to_find) {
	int entry;

	for (entry = 0; entry < MAX_ADDRESSES; entry++) {
		if (strcmp(name_to_find, name[entry]) == 0)
			return entry;
	}

	return NOT_FOUND;
}

/*
** 给定一个名字，查找并返回对应的地址。
** 若名字未找到，函数返回一个NULL指针
*/
char const *
lookup_address(char const *name) {
	int entry;

	entry = find_entry(name);
	if (entry == -1)
		return NULL;
	else
		return address[entry];
}

/*
** 给定一个名字，查找并返回对应的电话号码。
** 若名字未找到，函数返回一个NULL指针
*/
char const *
lookup_phone(char const *name) {
	int entry;

	entry = find_entry(name);
	if (entry == -1)
		return NULL;
	else
		return phone[entry];
}

