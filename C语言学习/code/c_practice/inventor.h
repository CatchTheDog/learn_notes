/*
** �����¼������
*/

/*
** �������ר����Ϣ�Ľṹ
*/
typedef struct {
	int		cost;
	int		supplier;
	/* ������Ϣ */
} Partinfo;

typedef struct{
	char	partno[10];
	short	quan;
} SUBASSYPART;

/*
** �洢װ���ר����Ϣ�Ľṹ
*/
typedef	struct {
	int			n_parts;
	SUBASSYPART	*part;
} Subassyinfo;


/*
** �����¼�ṹ���������¼
*/
typedef		struct {
	char	partno[10];
	int		quan;
	enum {
		PART, SUBASSY
	} type;
	union {
		Partinfo* part;
		Subassyinfo* subassy;
	} info;
} Inverc;