#include <stdio.h>
#include "algorithm.h"

/**
*�۳�ͨ���㷨��������Դ�������������ͨ��
**/
int cumulativeMultiple(int step, int start, int end) {
	if (start < end && step > 0 && end-start>=step) {
		int pre, next;
		for (pre = start, next = start + step; next <= end;next+=step) {
			pre *= next;
		}
		return pre;
	}
	return 0;
}