/*
** 对值进行偶校验
*/

int
even_parity(int value,int n_bits) {
	int parity = 0;

	/*
	** 计数值中值为1的为的个数
	*/
	while (n_bits > 0) {
		/*
		** value与000000001按位求与，获取value最后一位值；将value最后一位值累加在parity中；
		** 然后将value右移1位。循环对value进行上述操作，直至所有位的值都累加在parity中。
		*/
		parity += value & 1;
		value >>= 1;
		n_bits -= 1;
	}

	/*
	** 如果计数器的最低位是0，返回TRUE(表示1的位数为偶数个)
	*/
	return (parity % 2) == 0;
}