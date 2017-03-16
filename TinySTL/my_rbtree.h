#ifndef _MY_RBTREE_H_
#define _MY_RBTREE_H_

#include<iostream>
using namespace std;


//������ڵ���ɫ
typedef bool rb_tree_color_type;
const rb_tree_color_type rb_red = false;  //��ɫΪ0
const rb_tree_color_type rb_black = true;  //��ɫΪ1

//������ڵ�
template<class T>
class rb_tree_node{
	typedef rb_tree_color_type node_color;
	typedef rb_tree_node<T> node_type;
public:
	node_color color;  //�ڵ���ɫ
	node_type *parent;  //���ڵ�
	node_type *left;  //���ӽڵ�
	node_type *right; //���ӽڵ�
	T value;  //ֵ

	rb_tree_node(T &val);  //���캯��
	inline bool on_left();
	inline bool on_right();
};

//�����
template<class T>
class rb_tree{
public:
	typedef rb_tree_node<T> node_type;
	rb_tree();
	~rb_tree();
	void clear();
	void insert(T &val); //��ӽڵ�
	bool insert_unique(T &val); //���Ψһ�ڵ�
	node_type *find(T &val);  //��ѯ�ڵ�
	bool remove(T &val);  //ɾ���ڵ�
	inline node_type* maximum(); //���ֵ
	inline node_type* minimum(); //��Сֵ
	inline node_type* next(node_type *node); //��һ���ڵ�
	inline node_type *prev(node_type *node);  //��һ���ڵ�
	void print1();  //���
	void print2();  //���
	int height();  //�߶�
	unsigned int count();  //�ڵ���
	bool validate();  //��֤
	unsigned int get_rotate_times();  //��ȡ��ת����
private:
	node_type *root;  //����
	unsigned int rotate_times;  //��ת����
	unsigned int node_count;   //�ڵ���
	void _clear(node_type *sub_root); //�������
	void _insert(node_type *node); //�ڲ��ڵ���뺯��
	node_type *_find(node_type *sub_root, T &val);  //��ѯ
	inline node_type *_maximum(node_type *sub_root);  //���ֵ
	inline node_type *_minimum(node_type *sub_root);  //��Сֵ
	void _rebalance(node_type *node);  //�²���ڵ����ƽ��
	void _fix(node_type *node, node_type* parent); //ɾ���ڵ����ƽ��
	void _rotate_left(node_type *node);  //����
	void _rotate_right(node_type *node); //����
	void _print2(node_type *sub_root);  //���
	void _print1(node_type *sub_root);  //���
	int _height(node_type *sub_root); //�߶�
	bool _validate(node_type *&sub_root, int &count); //��֤������ĺϷ���
	void _rb_transplant(node_type *u, node_type *v);
};

template<class T>
rb_tree_node<T>::rb_tree_node(T &val)
	:value(val), parent(NULL), left(NULL), right(NULL), color(rb_red)
{}

template<class T>
inline bool rb_tree_node<T>::on_left(){
	return parent && this == parent->left;
}

template<class T>
inline bool rb_tree_node<T>::on_right(){
	return parent && this == parent->right;
}

template<class T>
rb_tree<T>::rb_tree()
	: root(NULL), rotate_times(0), node_count(0)
{}


template<class T>
unsigned int rb_tree<T>::get_rotate_times(){
	return rotate_times;
}

template<class T>
unsigned int rb_tree<T>::count(){
	return node_count;
}

template<class T>
rb_tree<T>::~rb_tree(){
	clear(); 
}

template<class T>
void rb_tree<T>::clear(){
	_clear(root);
	root = NULL;
	rotate_times = 0;
	node_count = 0;
}

template<class T>
void rb_tree<T>::_clear(node_type *sub_root){
	if (!sub_root) return;
	_clear(sub_root->left);
	_clear(sub_root->right);
	delete sub_root;
	sub_root = NULL;
}

template<class T>
void rb_tree<T>::insert(T &val){
	node_type *newNode = new node_type(val);  //�����ڵ�
	
	if (!root) {
		//��һ���ڵ�
		root = newNode;
		root->color = rb_black;
	}
	else{
		_insert(newNode);
	}

	node_count++;
}

template<class T>
bool rb_tree<T>::insert_unique(T &val){
	if (find(val))
		return false;  //�Ѿ����ڽڵ�
	insert(val);
	return true;
}

template<class T>
typename rb_tree<T>::node_type* rb_tree<T>::find(T &val){
	return _find(root, val);
}

template<class T>
typename rb_tree<T>::node_type* rb_tree<T>::_find(node_type *sub_root, T &val){
	if (!sub_root) return NULL;

	if (val > sub_root->value){
		//����������
		return _find(sub_root->right, val);
	}
	else if (val < sub_root->value){
		//����������
		return _find(sub_root->left, val);
	}
	else return sub_root;  //�ҵ����
}

template <class T>
void rb_tree<T>::_insert(node_type *node){
	node_type *y = NULL;
	node_type *x = root;

	while (x){
		y = x;
		if (node->value < x->value)
			x = x->left;
		else x = x->right;
	}

	node->parent = y;
	if (y == NULL){
		root = node;
	}
	else if (node->value < y->value)
		y->left = node;
	else y->right = node;

	_rebalance(node);
}

template<class T>
void rb_tree<T>::_rotate_left(node_type *x){
	node_type *y = x->right;  //��¼x�����ӽڵ㣬Ϊ��ת��ĸ��ڵ�
	x->right = y->left;     //���ӽڵ�����ӽڵ�ӵ�pivot���Ҳ�
	if (y->left)
		y->left->parent = x;    //�޸����ӽڵ�����ӽڵ�ĸ��ڵ�
	y->parent = x->parent;   //�޸����ӽڵ�ĸ��ڵ�
	if (x->parent == NULL){   //�ж��Ƿ�Ϊ���ڵ�
		root = y;
	}
	else if (x->parent->left == x)  //���游�ڵ�ĽǶ������Ӻ���
		x->parent->left = y;
	else x->parent->right = y;

	y->left = x;    //�޸�x��y�ĸ��ӹ�ϵ
	x->parent = y;
	rotate_times++;
}

template<class T>
void rb_tree<T>::_rotate_right(node_type *x){
	node_type *y = x->left;
	x->left = y->right;
	if (y->right)
		y->right->parent = x;
	y->parent = x->parent;

	if (x->parent == NULL){
		root = y;
	}
	else if (x->parent->left == x)
		x->parent->left = y;
	else x->parent->right = y;

	y->right = x;
	x->parent = y;
	rotate_times++;
}

template<class T>
void rb_tree<T>::_rebalance(node_type *node){
	if (node->parent && node->parent->color == rb_black)
		return;

	while (node != root && node->parent->color == rb_red){
		if (node->parent == node->parent->parent->left){
			node_type *uncle = node->parent->parent->right;
			if (uncle && uncle->color == rb_red){
				node->parent->color = rb_black;
				uncle->color = rb_black;
				node->parent->parent->color = rb_red;
				node = node->parent->parent;
			} 
			else
			{
				if (node == node->parent->right){
					node = node->parent;
					_rotate_left(node);
				}

				node->parent->color = rb_black;
				node->parent->parent->color = rb_red;
				_rotate_right(node->parent->parent);
			}
		}
		else{
			node_type *uncle = node->parent->parent->left;
			if (uncle && uncle->color == rb_red){
				node->parent->color = rb_black;
				uncle->color = rb_black;
				node->parent->parent->color = rb_red;
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->left){
					node = node->parent;
					_rotate_right(node);
				}

				node->parent->color = rb_black;
				node->parent->parent->color = rb_red;
				_rotate_left(node->parent->parent);
			}
		}
	}
	root->color = rb_black;
}

template<class T>
void rb_tree<T>::_rb_transplant(node_type *u, node_type* v){
	if (u->parent == NULL)
		root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else u->parent->right = v;

	if (v)
		v->parent = u->parent;
}

template<class T>
bool rb_tree<T>::remove(T &val){
	//���ҽڵ�
	node_type *node = find(val);
	node_type *x = NULL, *y = NULL, *x_parent = NULL;
	if (!node) return false;

	y = node;
	rb_tree_color_type ycolor = y->color; //��¼yԭ������ɫ
	if (node->left == NULL){
		//ֻ���Ҷ���
		x = node->right;
		_rb_transplant(node, node->right);
		x_parent = node->parent;
	}
	else if (node->right == NULL){
		x = node->left;
		_rb_transplant(node, node->left);
		x_parent = node->parent;
	}
	else{
		//���Ҷ��Ӷ���
		y = _minimum(node->right);  //����node�ĺ��
		ycolor = y->color;
		x = y->right;
		if (y->parent == node){
			x_parent = y;
			if (x){
				x->parent = y;
			}
		}
		else{
			x_parent = y->parent;
			_rb_transplant(y, y->right);
			y->right = node->right;
			if (y->right){
				y->right->parent = y;
			}
		}
		_rb_transplant(node, y);
		y->left = node->left;
		if (y->left)
			y->left->parent = y;
		y->color = node->color;
	}

	if (!y->parent){
		root = x;
		if (x) x->parent = NULL;
		x_parent = NULL;
	}
	//���yԭ������ɫ�Ǻ�ɫ����ô����ζ����һ����ɫ�ڵ㱻������
	//����������ʾ��п��ܱ��ƻ�
	//cout << "node: " << node->value << " " << node->color << "   y: " << y->value  << " " << y->color<< endl;
	if (ycolor == rb_black){
		_fix(x, x_parent);
	}
	
	delete node;
	node_count--;
	return true;
}

template<class T>
void rb_tree<T>::_fix(node_type *x, node_type *x_parent){
	
	if (x && x->color == rb_red){
		x->color = rb_black;
		return;
	}
	else if (x && x == root){
		x->color = rb_black;
		return;
	}

	while (!x || x->color == rb_black){
		if (x) x_parent = x->parent;
		if (!x_parent) break;

		if (x == x_parent->left){
			//xΪ���ڵ������
			node_type* w = x_parent->right;

			//case 1
			if (w && w->color == rb_red){
				w->color = rb_black;
				x_parent->color = rb_red;
				_rotate_left(x_parent);
				w = x_parent->right;
			}

			//case 2
			if (!w || (!w->left || w->left->color == rb_black) &&(!w->right || w->right->color == rb_black)){
				if (w)
					w->color = rb_red;
				x = x_parent;
			}
			else{
				//case 3
				if (!w->right || w->right->color == rb_black){
					w->color = rb_red;
					w->left->color = rb_black;
					_rotate_right(w);
					w = x_parent->right;
				}

				//case 4
				w->color = x_parent->color;
				x_parent->color = rb_black;
				if (w->right)
					w->right->color = rb_black;
				
				_rotate_left(x_parent);
				break;
			}
		}
		else{
			//����x�Ǹ��ڵ���Ҷ��ӵ����
			node_type* w = x_parent->left;
			if (w && w->color == rb_red){
				w->color = rb_black;
				x_parent->color = rb_red;
				_rotate_right(x_parent);
				w = x_parent->left;
			}
			if (!w || (!w->left || w->left->color == rb_black) && (!w->right || w->right->color == rb_black)){
				if (w)
					w->color = rb_red;
				x = x_parent;
			}
			else {
				if (!w->left || w->left->color == rb_black){
					w->right->color = rb_black;
					w->color = rb_red;
					_rotate_left(w);
					w = x_parent->left;
				}

				
				w->color = x_parent->color;
				x_parent->color = rb_black;
				if (w->left)
					w->left->color = rb_black;
		
				_rotate_right(x_parent);
				break;
			}
		}
	}

	if (x)
		x->color = rb_black;
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::maximum(){
	return _maximum(root);
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::minimum(){
	return _minimum(root);
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::_maximum(node_type* sub_root){
	while (sub_root && sub_root->right)
		sub_root = sub_root->right;

	return sub_root;
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::_minimum(node_type* sub_root){
	while (sub_root && sub_root->left)
		sub_root = sub_root->left;

	return sub_root;
}

template<class T>
inline typename rb_tree<T>::node_type *rb_tree<T>::next(node_type* node){
	if (!node) return NULL;

	if (node->right)
		return _minimum(node->right);   //���ҽڵ�����Сֵ
	else{
		while (node->on_right())
			node = node->parent;    //���ϻ���
		if (node == root)
			return NULL;
		else
			return node->parent;
	}
}

template<class T>
inline typename rb_tree<T>::node_type* rb_tree<T>::prev(node_type *node){
	if (!node) return NULL;
	if (node->left)
		return _maximum(node->left);    
	else{
		while (node->on_left())
			node = node->parent;
		if (node == root)
			return NULL;
		else
			return node->parent;
	}
}

template<class T>
void rb_tree<T>::print1(){
	_print1(root);
	cout << endl;
}
template<class T>
void rb_tree<T>::print2(){
	_print2(root);
	cout << endl;
}

template<class T>
void rb_tree<T>::_print1(node_type *sub_root){
	if (!sub_root) return;
	cout << sub_root->value << " ";
	_print1(sub_root->left);
	_print1(sub_root->right);
}

template<class T>
void rb_tree<T>::_print2(node_type *sub_root){
	if (!sub_root) return;
	_print2(sub_root->left);
	cout << sub_root->value << " ";
	_print2(sub_root->right);
}
template<class T>
int rb_tree<T>::height(){
	return _height(root);
}

template<class T>
int rb_tree<T>::_height(node_type* sub_root){
	if (!sub_root) return 0;
	int lh = _height(sub_root->left);
	int rh = _height(sub_root->right);
	return lh > rh ? lh + 1 : rh + 1;
}

template<class T>
bool rb_tree<T>::validate(){
	if (root && root->color == rb_red)
		return false;

	int count = 0;
	return _validate(root, count);
}

template<class T>
bool rb_tree<T>::_validate(node_type*& sub_root, int &count){
	if (!sub_root){
		count = 0;  //��ɫ�ڵ����
		return true;
	}

	if (sub_root->color == rb_red &&
		((sub_root->left && sub_root->left->color == rb_red)
		|| (sub_root->right && sub_root->right->color == rb_red))){
	cout << "���ڵ�Ϊ�죬�ӽڵ���ֺ�ɫ������" << endl;
		return false;
	}

	int lc = 0, rc = 0;
	bool lf = _validate(sub_root->left, lc);
	bool rf = _validate(sub_root->right, rc);

	if (lf && rf){
		if (sub_root->left && sub_root->right && rc != lc){
			//�������������ڣ��űȽϸ߶�
			cout << "�����ڸ߲�ͬ������" << endl;
			return false;
		}
		//��֤ͨ��
		count = lc > rc ? lc : rc;  //���ֵ��Ϊ�ڸ�
		if (sub_root->color == rb_black)
			count++;

		return true;
	}
	else return false;
}
#endif