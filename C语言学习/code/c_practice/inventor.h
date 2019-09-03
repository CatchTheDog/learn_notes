/*
** 存货记录的声明
*/

/*
** 包含零件专用信息的结构
*/
typedef struct {
	int		cost;
	int		supplier;
	/* 其他信息 */
} Partinfo;

typedef struct{
	char	partno[10];
	short	quan;
} SUBASSYPART;

/*
** 存储装配件专用信息的结构
*/
typedef	struct {
	int			n_parts;
	SUBASSYPART	*part;
} Subassyinfo;


/*
** 存货记录结构——变体记录
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