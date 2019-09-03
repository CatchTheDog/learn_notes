/*
** �ṹ�����
**
**
*/

#include	<string.h>
#include	<stdio.h>
#include	<stddef.h>
#define		PRODUCT_SIZE	100

struct DISD_REGISTER_FORMAT {
	unsigned	command : 5;		/* ���� */
	unsigned	secotr : 5;			/* ���� */
	unsigned	track : 9;			/* �ŵ� */
	unsigned	error_code : 8;		/* ������� */
	unsigned	head_loaded : 1;	/* Head Loaded */
	unsigned	write_protect : 1;	/* д���� */
	unsigned	disk_spinning : 1;	/* Disk Spinning */
	unsigned	error_occured : 1;	/* ���ִ��� */
	unsigned	ready : 1;			/* ���� */
};


#define		DISK_REGISTER ((struct DISK_REGESTER_FORMAT *) 0xc0200142)


void student_info_test();
void struct_p_test();
void margin_alignment();
void struct_as_params();
void union_test();
void testlloc();

int
main_18() {
	student_info_test();

	struct_p_test();

	margin_alignment();

	struct_as_params();

	union_test();

	testlloc();
}


void
student_info_test() {
	/*
	** ���ڽṹ�嶨�壬���ɣ�ʹ��typedef
	*/
	typedef struct {
		unsigned int	year;					/* ��			*/
		unsigned int	month;					/* ��			*/
		unsigned int	day;					/* ��			*/
		unsigned int	hour;					/* ʱ			*/
		unsigned int	minute;					/* ��			*/
		unsigned int	second;					/* ��			*/
	} DATE;

	/*
	** ѧ����Ϣ�ṹ�嶨��
	*/
	struct STUDENT {
		unsigned int		num;				/* ѧ�����		*/
		char* name;				/* ����			*/
		unsigned int		age;				/* ����			*/
		DATE				birthday;			/* ��������		*/
		char* hobby[10];			/* ����			*/
	} s = {										/* �ṹ���ʼ��    */
				1,
				"·�˼�",
				21,
				{
					1990,
					2,
					18,
					12,
					0,
					0
				},
				{
					"LOL",
					"���"
				}
	};										/* ���ڶ���֮������������� */

	struct STUDENT x, y[10], * z;				/* Ҳ�ɵ�������������		*/
	DATE		   date1;						/* ʹ��typedef��			*/

	printf("ѧ����Ϣ:\n\t��ţ�%d,\n\t������%s,\n\t���䣺%d,\n\t�������ڣ�%d-%d-%d %d:%d:%d,\n\t���ã�%s,%s \n"
		, s.num, s.name, s.age, s.birthday.year, s.birthday.month, s.birthday.day
		, s.birthday.hour, s.birthday.minute, s.birthday.second, s.hobby[0], s.hobby[1]);
}

/*
** �ṹ��ָ��ͳ�Ա����
*/
void
struct_p_test() {
	typedef struct {
		int		a;
		short	b[2];
	} Ex2;

	typedef struct EX {
		int		a;
		char	b[3];
		Ex2		c;
		struct  EX* d;
	} Ex;

	Ex	x = { 10,"Hi",{5,{-1,25}},NULL };
	Ex  y = { 11,"Ok",{6,{0,26}},NULL };
	x.d = &y;
	Ex* px = &x;

	printf("ָ�����:%d \n", px->a);
	printf("ָ�����2��%d \n", px->c.a);
	printf("ָ�����3��%d \n",px->d->a);
}


/*
** �ڴ�߽�������
*/
void
margin_alignment() {
	struct ALIGN {
		char	a;
		int		b;
		char	c;
	};

	struct ALIGN_1 {
		int		b;
		char	a;
		char	c;
	};
	
	printf("�ṹ��ALIGNռ���ڴ��СΪ��%d \n",sizeof(struct ALIGN));
	printf("�ṹ���Աa����ڽṹ��ƫ�Ƶ�ַ��%d \n",offsetof(struct ALIGN,a));
	printf("�ṹ���Աb����ڽṹ��ƫ�Ƶ�ַ��%d \n", offsetof(struct ALIGN, b));
	printf("�ṹ���Աc����ڽṹ��ƫ�Ƶ�ַ��%d \n", offsetof(struct ALIGN, c));


	printf("�ṹ��ALIGN_1ռ���ڴ��СΪ��%d \n", sizeof(struct ALIGN_1));
	printf("�ṹ���Աa����ڽṹ��ƫ�Ƶ�ַ��%d \n", offsetof(struct ALIGN_1, a));
	printf("�ṹ���Աb����ڽṹ��ƫ�Ƶ�ַ��%d \n", offsetof(struct ALIGN_1, b));
	printf("�ṹ���Աc����ڽṹ��ƫ�Ƶ�ַ��%d \n", offsetof(struct ALIGN_1, c));
}

/*
** �ṹ�嶨��
*/
typedef struct {
	char	product[PRODUCT_SIZE];
	int		quantity;
	float	unit_price;
	float	total_amount;
} Transaction;


/*
** ��ӡ�վ�
*/
void
print_receipt(Transaction trans) {
	printf("%s \n",trans.product);
	printf("%d @ %.2f total %.2f \n",trans.quantity,trans.unit_price,trans.total_amount);
}

/*
** ʹ��register const ���νṹ��ָ�����
*/
void print_receipt_1(register Transaction const *p) {
	printf("%s \n", p -> product);
	printf("%d @ %.2f total %.2f \n", p->quantity, p->unit_price, p->total_amount);
}

/*
** �ṹ����Ϊ��������
*/
void
struct_as_params() {
	Transaction current_trans = { "����Ʒ",12,10.1,121.2 };
	print_receipt(current_trans);

	print_receipt_1(&current_trans);
}

///*
//** λ�β���
//*/
//void
//bit_field_test() {
//	/*
//	** ���߿��������ĸ������ŵ���ʼ��ȡ
//	*/
//	DISK_REGISTER->sector = 12;
//	DISK_REGISTER->track = new_track;
//	DISK_REGISTER->command = READ;
//
//	/*
//	** �ȴ���ֱ���������
//	*/
//	while (!DISK_REGISTER->ready)
//		;
//
//	/*
//	** ������
//	*/
//	if (DISK_REGISTER->error_occurred)
//		switch (DISK_REGISTER->error->occurred) {
//
//		}
//}


/*
** ���������
*/
void
union_test() {
	union A {
		float	f;
		int		i;
	} a;

	union A b = { 5 };
	
	a.f = 1.2;
	printf("%f \n",a.f);
	printf("%f \n",b.f);

	/*
	** BASIC�����ڱ���������ͺ�ֵ��Ϣ�Ľṹ
	** ��Ϊ���κ�һ��������ֻ����һ��ֵ���������ֽṹ���˷�δʹ�õ������ֶεĿռ�
	*/
	struct VARIABLE {
		enum { INT,FLOAT,STRING } type;
		int		int_value;
		float	float_value;
		char	*string_value;
	};

	/*
	** �Ľ����BASIC�����ڱ���������ͺ�ֵ��Ϣ�Ľṹ
	*/
	struct VARIABLE_1 {
		//enum { INT,FLOAT,STRING } type;
		union {
			int		i;
			float	f;
			char	*s;
		} value;
	};
}

/*
** ��̬�ڴ�������
*/
void testlloc() {
	int		*pi = malloc(25*sizeof(int));				/* ʹ��sizeof(int) �ǿ���ֲ�ķ�ʽ */
	if (pi == NULL) {
		printf("Out of memory! \n");
		exit(1);
	}
	/*
	** �������ڴ��ʼ��Ϊ0
	*/
	for (int i = 0; i < 25; i++,pi++) {
		*pi = 0;
		printf("\t��ʼ�� %d \n",i);
	}
		
	
}