/*
** �ṩһ��������������������Ľ׳ˡ�
**
** չʾβ�ݹ�͵���
**
*/


/*
** ���õݹ�(β�ݹ�)��ʽ����׳�
*/
long
factorial(unsigned int n) {
	if (n <= 0)
		return 1;
	else
		return n * factorial(n-1);
}


/*
** ���õ�����ʽ����׳�
** ����ʵ�ֱȵݹ�Ҫ��Ч
*/
long
factorial_1(unsigned int n){
	int		result = 1;
	while (n > 1) 
		result *= n--;

	return result;
}


/*
** �õݹ鷽�������n��쳲���������ֵ
*/
unsigned long
fibonacci(unsigned int n) {
	if (n <= 2)
		return 1;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

/*
** ���õ�����ʽ�����n��쳲���������ֵ
*/
unsigned long
fibonacci_1(unsigned int n) {

	unsigned long	result;					/* ��n��쳲���������ֵ */
	unsigned long	previous_result;		/* ��n-1��쳲���������ֵ */
	unsigned long	next_older_result;		/* ��n-2��쳲���������ֵ */

	result = previous_result = 1;	/* n<=2 ʱ��쳲���������Ϊ1 */
	
	while (n > 2) {
		n--;
		next_older_result = previous_result;
		previous_result = result;
		result = previous_result + next_older_result;
	}

	return result;
}
