#include <stdio.h>
#include "algorithm.h"

/**
*累乘通用算法，如果可以传入运算符，则更通用
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