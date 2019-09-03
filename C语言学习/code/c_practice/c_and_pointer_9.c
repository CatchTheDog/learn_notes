/*
** 提供一个数，函数计算这个树的阶乘。
**
** 展示尾递归和迭代
**
*/


/*
** 采用递归(尾递归)方式计算阶乘
*/
long
factorial(unsigned int n) {
	if (n <= 0)
		return 1;
	else
		return n * factorial(n-1);
}


/*
** 采用迭代方式计算阶乘
** 迭代实现比递归要高效
*/
long
factorial_1(unsigned int n){
	int		result = 1;
	while (n > 1) 
		result *= n--;

	return result;
}


/*
** 用递归方法计算第n个斐波那契数的值
*/
unsigned long
fibonacci(unsigned int n) {
	if (n <= 2)
		return 1;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

/*
** 采用迭代方式计算第n个斐波那契数的值
*/
unsigned long
fibonacci_1(unsigned int n) {

	unsigned long	result;					/* 第n个斐波那契数的值 */
	unsigned long	previous_result;		/* 第n-1个斐波那契数的值 */
	unsigned long	next_older_result;		/* 第n-2个斐波那契数的值 */

	result = previous_result = 1;	/* n<=2 时，斐波那契数均为1 */
	
	while (n > 2) {
		n--;
		next_older_result = previous_result;
		previous_result = result;
		result = previous_result + next_older_result;
	}

	return result;
}
