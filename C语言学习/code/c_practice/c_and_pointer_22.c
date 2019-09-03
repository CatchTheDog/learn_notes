/*
** 释放存货记录的函数
*/

#include	<stdlib.h>
#include	"inventor.h"


void
discard_inventory_record(Inverc * record) {
	/*
	** 删除记录中的变体部分
	*/

	switch (record->type) {
		case SUBASSY:
			free(record->info.subassy->part);
			free(record->info.subassy);
			break;
		case PART:
			free(record->info.part);
			break;
		default:
			break;
	}
}