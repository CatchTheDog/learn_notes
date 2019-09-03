/*
** 指针运算测试：指针 +/- 整数
*/

#include <stdlib.h>
#define		N_VALUES				5							/* 数组长度 */

float		values[N_VALUES];									/* 数组 */

/*
** 将数组各个元素都初始化为0
*/
void
initArray() {
	for (float *vp = &values[0]; vp < &values[N_VALUES]; )
		*vp++ = 0;
}
