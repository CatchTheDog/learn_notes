/*
** ָ��������ԣ�ָ�� +/- ����
*/

#include <stdlib.h>
#define		N_VALUES				5							/* ���鳤�� */

float		values[N_VALUES];									/* ���� */

/*
** ���������Ԫ�ض���ʼ��Ϊ0
*/
void
initArray() {
	for (float *vp = &values[0]; vp < &values[N_VALUES]; )
		*vp++ = 0;
}
