/*
** 求多个数的和
**		测试函数不定长参数使用方法
**
*/
#include	<stdarg.h>
#include	<assert.h>

static float
average(int n_values, ...) {

	va_list		var_arg;
	int			count;
	float		sum		 =		 0;

	/*
	** 准备访问可变参数
	*/
	va_start(var_arg, n_values);

	/*
	** 添加取自可变参数列表的值
	*/
	for (count = 0; count < n_values; count++)
		sum += va_arg(var_arg,int);

	/*
	** 完成处理可变参数
	*/
	va_end(var_arg);

	return sum / n_values;
}

