#include <stdlib.h>
struct Node
{
	struct Node* pre; //前驱节点指针
	struct Node* next; //后继节点指针
	int * data; //动态分配的数据空间
} head = { NULL,NULL,NULL }, tail = { NULL,NULL,NULL };
int n = 0;//链表内元素个数

/*链表初始化*/
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
			struct Node* pre = tail.pre;//最后一个元素节点的指针
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
