/*
** ���ڴ���SUBASSEMBLY(װ���)�����¼�ĺ���
*/
#include	<stdlib.h>
#include	<stdio.h>
#include	"inventor.h"

Inverc *
create_subassy_record(int n_parts) {
	Inverc* new_rec;

	/*
	** ��ͼΪInverc �����ڴ�
	*/
	new_rec = malloc(sizeof(Inverc));
	if (new_rec != NULL) {
		/*
		** �ڴ����ɹ������ڴ洢SUBASSYINFO����
		*/
		new_rec->info.subassy = malloc(sizeof(Subassyinfo));
		if (new_rec->info.subassy != NULL) {
			/*
			** Ϊ�����ȡһ���㹻�������
			*/
			new_rec->info.subassy->part = malloc(n_parts * sizeof(SUBASSYPART));
			if (new_rec->info.subassy->part != NULL) {
				/*
				** ��ȡ���ڴ棬�����֪�ֶ�ֵ
				*/
				new_rec->type = SUBASSY;
				new_rec->info.subassy->n_parts = n_parts;
				return new_rec;
			}
			/*
			** �ڴ���ʹ���꣬�ͷ��ڴ�
			*/
			free(new_rec->info.subassy);
		}
		free(new_rec);
	}
	return NULL;
}