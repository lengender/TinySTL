#include"my_rbtree.h"
#include<time.h>
#include<windows.h>

int main()
{
	srand((unsigned)GetCurrentTime());
	int times = 10, len = 30;

	while (times--){
		rb_tree<int> tree;
		for (int i = 0; i < len; ++i){
			int num = rand() % 30;
			tree.insert_unique(num);
			if (!tree.validate())
				cout << "����ʱʧȥƽ��" << endl;
		}
		tree.print1();
		tree.print2();
		cout << "���� ";
		for (rb_tree<int>::node_type *node = tree.minimum();
			node; node = tree.next(node)){
			cout << node->value << " ";
		}
		cout << "\n��ת����-�ڸ�-�ڵ�����" << tree.get_rotate_times()
			<< " " << tree.height() << " " << tree.count() << endl;

		cout << "ɾ���� " << endl;
		for (int i = 0; i < len; ++i){
			int num = rand() % len;
			if (tree.remove(num)) cout << num << " ";
			//tree.print1();
			//tree.print2();
			//cout << "--------" << endl;
			if (!tree.validate())
			{
				cout << "ɾ��ʱʧȥƽ��" << endl;
				return 0;
			}
		}
		cout << endl;

		cout << "����";
		for (rb_tree<int>::node_type*node = tree.maximum(); node; node = tree.prev(node))
		{
			cout << node->value << " ";
		}
		cout << "\n��ת����-�ڸ�-�ڵ�����" << tree.get_rotate_times()
			<< " " << tree.height() << " " << tree.count() << endl;
		cout << "________________________________________________________________________________" << endl;
	}
}