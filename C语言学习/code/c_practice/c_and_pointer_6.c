/*
** ��ֵ����żУ��
*/

int
even_parity(int value,int n_bits) {
	int parity = 0;

	/*
	** ����ֵ��ֵΪ1��Ϊ�ĸ���
	*/
	while (n_bits > 0) {
		/*
		** value��000000001��λ���룬��ȡvalue���һλֵ����value���һλֵ�ۼ���parity�У�
		** Ȼ��value����1λ��ѭ����value��������������ֱ������λ��ֵ���ۼ���parity�С�
		*/
		parity += value & 1;
		value >>= 1;
		n_bits -= 1;
	}

	/*
	** ��������������λ��0������TRUE(��ʾ1��λ��Ϊż����)
	*/
	return (parity % 2) == 0;
}