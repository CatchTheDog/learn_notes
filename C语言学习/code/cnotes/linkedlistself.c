#include <stdlib.h>
struct Node
{
	struct Node* pre; //ǰ���ڵ�ָ��
	struct Node* next; //��̽ڵ�ָ��
	int * data; //��̬��������ݿռ�
} head = { NULL,NULL,NULL }, tail = { NULL,NULL,NULL };
int n = 0;//������Ԫ�ظ���

/*�����ʼ��*/
void init(void) {
	head.next = &tail;
	tail.pre = &head;
}

void add(int a) {
	if (NULL == head.next)
		init();
	else {
		int* p = malloc(sizeof(int));
		if (NULL != p) {
			*p = a;
			struct Node node = { NULL,NULL,p };
			struct Node* pre = tail.pre;//���һ��Ԫ�ؽڵ��ָ��
			if (NULL != pre) {
				pre->next = &node;
				node.pre = pre;
				node.next = &tail;
				n++;
			}
		}
	}
}

void remove() {

}

void isEmpty() {

}

int size() {
	return n;
}
