/*
** 结构体测试
**
**
*/

#include	<string.h>
#include	<stdio.h>
#include	<stddef.h>
#define		PRODUCT_SIZE	100

struct DISD_REGISTER_FORMAT {
	unsigned	command : 5;		/* 命令 */
	unsigned	secotr : 5;			/* 扇区 */
	unsigned	track : 9;			/* 磁道 */
	unsigned	error_code : 8;		/* 错误代码 */
	unsigned	head_loaded : 1;	/* Head Loaded */
	unsigned	write_protect : 1;	/* 写保护 */
	unsigned	disk_spinning : 1;	/* Disk Spinning */
	unsigned	error_occured : 1;	/* 出现错误 */
	unsigned	ready : 1;			/* 就绪 */
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
	** 日期结构体定义，技巧：使用typedef
	*/
	typedef struct {
		unsigned int	year;					/* 年			*/
		unsigned int	month;					/* 月			*/
		unsigned int	day;					/* 日			*/
		unsigned int	hour;					/* 时			*/
		unsigned int	minute;					/* 分			*/
		unsigned int	second;					/* 秒			*/
	} DATE;

	/*
	** 学生信息结构体定义
	*/
	struct STUDENT {
		unsigned int		num;				/* 学生编号		*/
		char* name;				/* 姓名			*/
		unsigned int		age;				/* 年龄			*/
		DATE				birthday;			/* 出生日期		*/
		char* hobby[10];			/* 爱好			*/
	} s = {										/* 结构体初始化    */
				1,
				"路人甲",
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
					"逛街"
				}
	};										/* 可在定义之后紧跟变量声明 */

	struct STUDENT x, y[10], * z;				/* 也可单独做变量声明		*/
	DATE		   date1;						/* 使用typedef简化			*/

	printf("学生信息:\n\t编号：%d,\n\t姓名：%s,\n\t年龄：%d,\n\t出生日期：%d-%d-%d %d:%d:%d,\n\t爱好：%s,%s \n"
		, s.num, s.name, s.age, s.birthday.year, s.birthday.month, s.birthday.day
		, s.birthday.hour, s.birthday.minute, s.birthday.second, s.hobby[0], s.hobby[1]);
}

/*
** 结构体指针和成员测试
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

	printf("指针测试:%d \n", px->a);
	printf("指针测试2：%d \n", px->c.a);
	printf("指针测试3：%d \n",px->d->a);
}


/*
** 内存边界对齐测试
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
	
	printf("结构体ALIGN占用内存大小为：%d \n",sizeof(struct ALIGN));
	printf("结构体成员a相对于结构体偏移地址：%d \n",offsetof(struct ALIGN,a));
	printf("结构体成员b相对于结构体偏移地址：%d \n", offsetof(struct ALIGN, b));
	printf("结构体成员c相对于结构体偏移地址：%d \n", offsetof(struct ALIGN, c));


	printf("结构体ALIGN_1占用内存大小为：%d \n", sizeof(struct ALIGN_1));
	printf("结构体成员a相对于结构体偏移地址：%d \n", offsetof(struct ALIGN_1, a));
	printf("结构体成员b相对于结构体偏移地址：%d \n", offsetof(struct ALIGN_1, b));
	printf("结构体成员c相对于结构体偏移地址：%d \n", offsetof(struct ALIGN_1, c));
}

/*
** 结构体定义
*/
typedef struct {
	char	product[PRODUCT_SIZE];
	int		quantity;
	float	unit_price;
	float	total_amount;
} Transaction;


/*
** 打印收据
*/
void
print_receipt(Transaction trans) {
	printf("%s \n",trans.product);
	printf("%d @ %.2f total %.2f \n",trans.quantity,trans.unit_price,trans.total_amount);
}

/*
** 使用register const 修饰结构体指针参数
*/
void print_receipt_1(register Transaction const *p) {
	printf("%s \n", p -> product);
	printf("%d @ %.2f total %.2f \n", p->quantity, p->unit_price, p->total_amount);
}

/*
** 结构体作为函数参数
*/
void
struct_as_params() {
	Transaction current_trans = { "测试品",12,10.1,121.2 };
	print_receipt(current_trans);

	print_receipt_1(&current_trans);
}

///*
//** 位段测试
//*/
//void
//bit_field_test() {
//	/*
//	** 告诉控制器从哪个扇区磁道开始读取
//	*/
//	DISK_REGISTER->sector = 12;
//	DISK_REGISTER->track = new_track;
//	DISK_REGISTER->command = READ;
//
//	/*
//	** 等待，直到操作完成
//	*/
//	while (!DISK_REGISTER->ready)
//		;
//
//	/*
//	** 检查错误
//	*/
//	if (DISK_REGISTER->error_occurred)
//		switch (DISK_REGISTER->error->occurred) {
//
//		}
//}


/*
** 联合体测试
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
	** BASIC中用于保存变量类型和值信息的结构
	** 因为对任何一个变量，只会有一个值，所以这种结构会浪费未使用的两个字段的空间
	*/
	struct VARIABLE {
		enum { INT,FLOAT,STRING } type;
		int		int_value;
		float	float_value;
		char	*string_value;
	};

	/*
	** 改进后的BASIC中用于保存变量类型和值信息的结构
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
** 动态内存分配测试
*/
void testlloc() {
	int		*pi = malloc(25*sizeof(int));				/* 使用sizeof(int) 是可移植的方式 */
	if (pi == NULL) {
		printf("Out of memory! \n");
		exit(1);
	}
	/*
	** 将所有内存初始化为0
	*/
	for (int i = 0; i < 25; i++,pi++) {
		*pi = 0;
		printf("\t初始化 %d \n",i);
	}
		
	
}