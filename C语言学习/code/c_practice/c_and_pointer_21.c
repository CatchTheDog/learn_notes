/*
** 用于创建SUBASSEMBLY(装配件)存货记录的函数
*/
#include	<stdlib.h>
#include	<stdio.h>
#include	"inventor.h"

Inverc *
create_subassy_record(int n_parts) {
	Inverc* new_rec;

	/*
	** 试图为Inverc 分配内存
	*/
	new_rec = malloc(sizeof(Inverc));
	if (new_rec != NULL) {
		/*
		** 内存分配成功，现在存储SUBASSYINFO部分
		*/
		new_rec->info.subassy = malloc(sizeof(Subassyinfo));
		if (new_rec->info.subassy != NULL) {
			/*
			** 为零件获取一个足够大的数组
			*/
			new_rec->info.subassy->part = malloc(n_parts * sizeof(SUBASSYPART));
			if (new_rec->info.subassy->part != NULL) {
				/*
				** 获取到内存，填充已知字段值
				*/
				new_rec->type = SUBASSY;
				new_rec->info.subassy->n_parts = n_parts;
				return new_rec;
			}
			/*
			** 内存已使用完，释放内存
			*/
			free(new_rec->info.subassy);
		}
		free(new_rec);
	}
	return NULL;
}