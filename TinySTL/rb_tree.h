#ifndef _RB_TREE_H_
#define _RB_TREE_H_

#include<memory>
#include"iterator.h"
#include"alloc.h"
#include"construct.h"
namespace TinySTL{
	typedef bool				_rb_tree_color_type;
	const _rb_tree_color_type _rb_tree_red = false;
	const _rb_tree_color_type _rb_tree_black = true;

	//�ײ�ڵ�
	struct _rb_tree_node_base{
		typedef _rb_tree_color_type			color_type; 
		typedef _rb_tree_node_base*			base_ptr;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		/*
			����ĳ�����λ�ã��ҵ�����ߵĽڵ�
			����������ڵ��λ�ã������ҵ����ú��������ߵĽڵ�
			���ؽڵ�λ��
		*/
		static base_ptr minimum(base_ptr x){
			while (x->left != 0){
				x = x->left;
			}
			return x;
		}

		//�����ұ߽ڵ�
		static base_ptr maximum(base_ptr x){
			while (x->right != 0){
				x = x->right;
			}
			return x;
		}
	};

	//rb_tree node
	template<class Value>
	struct _rb_tree_node : public _rb_tree_node_base{
		//����̳и����Ա
		typedef _rb_tree_node<Value>* link_type;
		Value value_field;
	};

	//�ײ������
	struct rb_tree_base_iterator{
		typedef typename _rb_tree_node_base::base_ptr base_ptr; 
		typedef typename TinySTL::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t					difference_type;
		base_ptr node;   //��Ա������ָ����ڵ��ָ�룬������ϵ�������ͽڵ��Ŧ��

		//������������ʵ��operator++ʱ���ñ�����
		void increment(){
			if (node->right != 0){
				node = node->right;
				while (node->left != 0){
					node = node->left;
				}
			}
			else{
				base_ptr y = node->parent;  //�ҳ����ڵ�
				while (node == y->right) {//������нڵ㱾��������ӽڵ�
					node = y;  //��һֱ���ݣ�ֱ����Ϊ���ӽڵ�Ϊֹ
					y = y->parent;
				} 
				if (node->right != y){  //����ʱ�����ӽڵ㲻���ڴ�ʱ�ĸ��ڵ㣬��ʱ���ڵ㼴Ϊ���
					node = y;           //���򣬴�ʱ��nodeΪ���
				}
			}
		}

		//������������ʵ��operator--ʱ���ñ�����
		void decrement(){
			if (node->color == _rb_tree_red && node->parent->parent == node){
				//����Ǻ�ڵ㣬���游�ڵ�����Լ�����ô�ҽڵ㼴Ϊ���
				//�����������nodeΪheaderʱ��ע��header�����ӽڵ㣨��mostring����
				//ָ����������max�ڵ�
				node = node->right;
			}
			else if (node->left != 0){
				base_ptr y = node->left;
				while (y->right != 0){
					y = y->right;
				}
				node = y;
			}
			else{
				base_ptr y = node->parent;
				while (node == y->left){
					node = y;
					y = y->parent;
				}
				node = y;
			}
		}
	};

	//������
	template<class Value, class Ref, class Ptr>
	struct _rb_tree_iterator : public rb_tree_base_iterator{
			//ע����ʽ������û�г�Ա������ֻ�Ǽ̳��˻����������node����
			//�����������node�����Ǻ�����ڵ�����������ӵ�Ŧ��
		typedef Value					value_type;
		typedef Ref						reference;
		typedef Ptr						pointer;
		typedef _rb_tree_iterator<Value, Value&, Value*>		iterator;
		typedef _rb_tree_iterator<Value, Ref, Ptr>				self;
		typedef _rb_tree_node<Value>*							link_type;
		typedef size_t											size_type;

		_rb_tree_iterator(){}
		_rb_tree_iterator(link_type x) { node = x; }
		_rb_tree_iterator(const iterator& it){ node = it.node; }

		//�����ã����ؽڵ�ֵ
		reference operator*() const{ return link_type(node)->value_field; }
		pointer operator->() const{ return *(operator*()); }

		//���ص�����ָ��Ľڵ����ɫ
		_rb_tree_color_type color() { return node->color == _rb_tree_red ? 0 : 1; }

		self& operator++() const{ increment(); return *this; }
		self operator++(int){ 
			self tmp = *this;
			increment();
			return tmp;
		}

		self& operator--() const { decrement(); return *this; }
		self operator--(int){
			self tmp = *this;
			decrement();
			return tmp;
		}

		//�Ƚ�
		bool operator==(const self& x) const{ return x.node == node; }
		bool operator!=(const self& x) const{ return x.node != node; }
	};

	//class of rb_tree
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = TinySTL::alloc>
	class rb_tree{
	protected:
		typedef void*												void_pointer;
		typedef _rb_tree_node_base*									base_ptr;
		typedef _rb_tree_node<Value>								rb_tree_node;
		typedef Simple_Alloc<rb_tree_node, Alloc>					rb_tree_node_allocator;
		typedef _rb_tree_color_type									color_type;
	public:
		typedef Key										key_type;
		typedef Value									value_type;
		typedef value_type*								pointer;
		typedef const value_type*						const_pointer;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef rb_tree_node*							link_type;  //�ڵ�ָ��
		typedef size_t									size_type;
		typedef ptrdiff_t								difference_type;
	protected:
		size_type node_count;	//�ڵ�����
		link_type	header;  //	�ڱ��ڵ㣬��parentָ��ָ����ڵ㣬ʵ���ϵ�һ������
		Compare key_compare;  //�Ƚϴ�С�ĺ���

	public:
		typedef _rb_tree_iterator<value_type, reference, pointer> iterator;   //��������������
	protected:
		//���ÿռ�����������һ���ڵ�
		link_type get_node() { return rb_tree_node_allocator::allocate(); }

		//���ÿռ��������ͷ�һ���ڵ�
		void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

		//���벢��ʼ���ڵ�
		link_type create_node(const value_type& x){
			//x�ǽڵ��ֵ
			link_type tmp = get_node();
			construct(&tmp->value_field, x);   //����ȫ�ֹ��캯����ʼ���ڵ�
			return tmp;
		}

		//��¡�ڵ�
		link_type clone_node(link_type x){
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = 0;
			tmp->right = 0;
			return tmp;
		}

		//�ͻ��ڵ�
		void destroy_node(link_type p){
			destroy(&p->value_field);
			put_node(p);   //�ͷ��ڴ�
		}

	private:
		//��ʼ�������
		void init(){
			header = get_node();  
			color(header) = _rb_tree_red;   //����header�ڵ�Ϊ��ɫ
			root() = 0;  //root()��ú�����ĸ��ڵ㣬header��parentָ��ָ����ڵ㣬��ʼ��Ϊnull
			leftmost() = header;  //����header�ڵ��������ָ���Լ�
			rightmost() = header;  //����hedaer�ڵ��������ָ���Լ�
		}
	public:
		//���캯��
		rb_tree(const Compare& cmp = Compare()) :node_count(0), key_compare(cmp){ init(); }

		//��������
		~rb_tree(){
			put_node(header);
		}

	protected:
		//��ø��ڵ�(header���ڱ�����parentָ����ڵ�)
		link_type &root() const{ return (link_type&)(header->parent); }

		//�������������ߵĽڵ�
		link_type& leftmost() const { return (link_type&)(header->left); }

		//������������ұߵĽڵ�
		link_type& rightmost() const{ return (link_type&)(header->right); }

		//���ؽڵ���ӽڵ�
		static link_type& left(link_type x){ return (link_type&)(x->left); }
		static link_type& right(link_type x) { return (link_type&)(x->right); }
		//���ؽڵ�ĸ��ڵ�
		static link_type& parent(link_type x) { return (link_type&)(x->parent); }

		//���ؽڵ��value
		static reference value(link_type x) { return x->value_field; }

		//���ؽڵ����ɫ
		static color_type& color(link_type x) { return (color_type&)(x->color); }

		//���ؽڵ��key
		static const key_type& key(link_type x){ return KeyOfValue()(value(x)); }

		//������Сֵ�ڵ�
		static link_type minimum(link_type x){
			return (link_type)_rb_tree_node_base::minimum(x);
		}
		//�������ֵ�ڵ�
		static link_type msximum(link_type x){
			return (link_type)_rb_tree_node_base::maximum(x);
		}
	public:
		//��ø��ڵ��ֵ(header���ڱ�����parentָ����ڵ�)
		value_type root_value() { return value((link_type)header->parent); }

		//���رȽϴ�С�ĺ���
		Compare key_comp() const { return key_compare; }

		//����һ����������ָ����������ߵĽڵ�
		iterator begin() { retuen leftmost(); }

		//����һ����������ָ���������ұߵĽڵ�
		iterator end() { return header; }

		bool empty() const{ return node_count == 0; }
		size_type size() const{ return node_count; }
		size_type max_size() const{ return size_type(-1); }
	public:
		//������ֵ���ڵ��ֵ�����ظ�������ظ����������Ч
		//����ֵ��pair,pair�ĵ�һ��Ԫ����rb_tree��������ָ���½ڵ㣬�ڶ���Ԫ�ر�ʾ�ɹ����
		std::pair<iterator, bool> insert_unique(const value_type& v){
			link_type y = header;   
			link_type x = root(); 
			bool comp = true;   
			while (x != 0){  //x��Ϊnull,˵�������ҵ��²���ڵ��λ�ã�����ִ��whileѭ���ڵ���䣬��ͣ���±���
				y = x;   //y���濴x�ڵ�ĸ��ڵ�

				//���������ڵ�ֵС��x��ֵ��compΪtrue������false,key_compare�ǱȽϴ�С��������ģ��ָ��
				comp = key_compare(KeyOfValue()(v), key(x));

				x = comp ? left(x) : right(x);
			}

			iterator j = iterator(y);   //��jָ������ĸ��ڵ�
			if (comp){ //���������ֵ�ȸ��ڵ��ֵС����Ϊ������Ҫ���뵽���ڵ����ߣ�����if
				if (begin() == j){
					return std::pair<iterator, bool>(_insert(x, y, v), true);
					//xΪ����㣬yΪ�����ĸ��ڵ㣬vΪ�����ֵ
				}
				else{
					j--;
				}
			}
			//��ֵ������нڵ��ظ������Բ���
			if (key_compare(key(j.node), KeyOfValue()(v))){
				return std::pair<iterator, bool>(_insert(x, y, v), true);
			}
			return std::pair<iterator, bool>(j, false);  
		}

		//������ֵ���ڵ�ļ�ֵ�����ظ�
		//���غ�����ĵ��������õ�����ָ���½ڵ�
		iterator insert_equal(const value_type& v){
			link_type y = header;
			link_type x = root();  //xָ����ڵ�
			while (x != 0){
				y = x;
				x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
			}
			return _insert(x, y, v);
		}

		//Ѱ�Һ�������Ƿ��м�ֵΪk�Ľڵ�
		iterator find(const key_type& k){
			link_type y = header;
			link_type x = root();

			while (x != 0){
				if (!key_compare(key(x), k)){
					y = x;
					x = left(x);
				}
				else{
					x = right(x);
				}
			}
			iterator j = iterator(y);
			return (j == end() || key_compare(k, key(j.node)) ? end() : j);
		}
	private:
		//���������x_�Ǵ�����ڵ㣬y_�ǲ���ڵ�ĸ��ڵ㣬v�����ֵ
		iterator _insert(base_ptr x_, base_ptr y_, const value_type& v){
			//base_ptrʵΪ_rb_tree_node_base*, link_typeʵΪ_rb_tree_node*
			//���ǰ�_rb_tree_node_base*����ǿתΪ_rb_tree_node*
			//_rb_tree_node��_rb_tree_node_base����value_field�����ڱ���ֵ
			link_type x = (link_type)x_;
			link_type y = (link_type)y_;
			link_type z;

			/*1.y == header, �����ĸ��ڵ�Ϊheader(ע��header���ڱ���header����������һ����,����header��parentָ����ڵ�)
				y == header˵�������Ϊ���ڵ�
			  2. x == 0, ˵���������Ҷ�ӽڵ��·���Ҷ�ӽڵ������������������Ϊnull��,Ҳ������Ҷ�ӽڵ��¹��µĽڵ�
			     x != 0˵��������������ڲ�ĳ���ڵ���
			  3. key_compare(KeyOfValue()(v), key(y)) ������ڵ��ֵҪ�ȸ��ڵ��ֵС(��ζ������Ҫ���뵽���ڵ����������)
 			*/
			if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))){
				z = create_node(v);  //�����½ڵ㣬���½ڵ��ֵ(value_field)Ϊv
				left(y) = z;   //��ڵ��������Ϊz,���ǳɹ����½ڵ���뵽����
				if (y == header){ //y==header,�����ĸ��ڵ�Ϊheader,˵�����ڵ㻹û�б���ʼ��������if���ǳ�ʼ�����ڵ�
					root() = z;  //z ���˸��ڵ�
					rightmost() = z;  //��zΪ���������ұߵĽڵ�
				}
				else if (y == leftmost()){ //������ڵ�������������߽ڵ�
					leftmost() = z;  //���ǰ��½ڵ���Ϊ����������ߵĽڵ�
				}
			}
			else{
				z = create_node(v);  //�����½ڵ�
				right(y) = z; //����ڵ㵽���ڵ���ұ�
				if (y == rightmost()){ //������ڵ������������ұߵĽڵ�
					rightmost() = z;
				}
			}
			
			parent(z) = y;  //���½ڵ�ĸ��ڵ�Ϊy
			left(z) = 0; 
			right(z) = 0;

			_rb_tree_rebalance(z, header->parent); //header���ڱ���һ���������ܸı䣬header->parentָ����ڵ�
			++node_count;
			return iterator(z);
		}

		//����ƽ������(�ı���ɫ����ת����)
		// x Ϊ�����ڵ� �� rootΪ���ڵ�
		inline void _rb_tree_rebalance(_rb_tree_node_base* x, _rb_tree_node_base*& root){
			x->color = _rb_tree_red;  //�²���ڵ�Ϊ��ɫ
			//��������ڵ�(x)��Ϊ���ڵ㣬�������ڵ�ĸ��ڵ�Ϊ��ɫ������whileѭ��
			while (x != root && x->parent->color == _rb_tree_red){
				if (x->parent == x->parent->parent->left){ //������ڵ����游�ڵ�����ӽڵ�
					_rb_tree_node_base* y = x->parent->parent->right;  //��yΪ�����ڵ�
					if (y && y->color == _rb_tree_red){ //��������ڵ������Ϊ��
						x->parent->color = _rb_tree_black;
						y->color = _rb_tree_black;
						x->parent->parent->color = _rb_tree_red;
						x = x->parent->parent;
					}
					else{ //��������ڵ㲻���ڣ����߲����ڵ�Ϊ��
						if (x == x->parent->right){ //��������ڵ��Ǹ��ڵ�����ӽڵ㣬˵�������ķ�ʽ���ڲ�
							x = x->parent;   //���ڵ�Ϊ��ת֧��
							/*
								����һ�´�while��ʼ�������жϷ�֧�����ǿ��Եó�������ת������
								1.�����ڵ㲻�Ǹ��ڵ�
								2.�����ڵ�ĸ��ڵ��Ǻ�ɫ
								3.���ڵ����游�ڵ�����ӽڵ�
								4.�����ڵ㲻���ڣ����߲����ڵ�Ϊ��
								5.�����ڵ�Ϊ���ڵ�����ӽڵ�
							*/
							_rb_tree_rotate_left(x, roog);  //����ת
						}
						x->parent->color = _rb_tree_black;  //�޸���ɫ
						x->parent->parent->color = _rb_tree_red;
						_rb_tree_rotate_right(x->parent->parent, root); //����ת���˽�������
					}
				}
				else{  //���ڵ����游�ڵ�����ӽڵ�
					_rb_tree_node_base* y = x->parent->parent->left;  //��yΪ�����ڵ�
					if (y && y->color == _rb_tree_red){ //��������ڵ������Ϊ��
						x->parent->color = _rb_tree_black;
						y->color = _rb_tree_black;
						x->parent->parent->color = _rb_tree_red;
						x = x->parent->parent;
					}
					else{ //��������ڵ㲻���ڣ����߲����ڵ�Ϊ��
						//��������ڵ�Ϊ���ڵ�����ӽڵ㣬˵�������ķ�ʽΪ�ڲ�
						if (x == x->parent->left){
							x = x->parent;  //���ڵ�Ϊ��ת֧��
							/*
								����һ�´�while��ʼ�������жϷ�֧�����ǿ����жϳ�������ת������
								1.�����ڵ㲻�Ǹ��ڵ�
								2.�����ڵ�ĸ��ڵ��Ǻ�ɫ
								3.���ڵ����游�ڵ�����ӽڵ�
								4.�����ڵ㲻���ڣ����߲����ڵ�Ϊ��
								5.�����ڵ��Ǹ��ڵ�����ӽڵ�
							*/
							_rb_tree_rotate_right(x, root);  //����ת
						}
						x->parent->color = _rb_tree_black;
						x->parent->parent->color = _rb_tree_red;
						_rb_tree_rotate_left(x->parent->parent, root);
					}
				}
			}
			root->color = _rb_tree_black; //���ڵ���Զ�Ǻ�
		}

		/*
			����ת
			�½ڵ��Ϊ��ڵ㣬�������ڵ�ĸ��ڵ�Ҳ�Ǻ�ɫ����Υ���˺�����Ĺ��򣬱�����ѡ��
			x:����ת��֧��
			root: ������ĸ�
		*/
		inline void _rb_tree_rotate_left(_rb_tree_node_base* x, _rb_tree_node_base*& root){
			_rb_tree_node_base* y = x->right; //y Ϊ��ת������ӽڵ�
			x->right = y->left; //��ת��Ϊ������ת������ӽڵ�Ϊ�ӣ���ɸ��ӶԻ�
			if (y->left != 0){
				y->left->parent = x;
			}
			y->parent = x->parent;

			if (x == root){
				root = y; //��y Ϊ���ڵ�
			}
			else if (x == x->parent->left){
				x->parent->left = y; //����ת��ĸ��ڵ�����ӽڵ�Ϊy
			}
			else{
				x->parent->right = y; //����ת��ĸ��ڵ�����ӽڵ�Ϊy
			}
			
			y->left = x;
			x->parent = y; //���ӽڵ�Ϊ��ת��ĸ��ڵ�
		}

		/*
			����ת
			�½ڵ��Ϊ��ڵ㣬��������ĸ��ڵ�Ҳ�Ǻ�ɫ����Υ���˺�����Ĺ��򣬴�ʱ��������ת
			x:����ת��֧��
			root:������ĸ�
		*/
		inline void _rb_tree_rotate_right(_rb_tree_node_base* x, _rb_tree_node_base*& root){
			_rb_tree_node_base *y = x->left; //��yΪ��ת������ӽڵ�
			x->left = y->right;   //��y�����ӽڵ�Ϊ��ת������ӽڵ�
			if (y->right != 0){
				y->right->parent = x;  //�趨���ڵ�
			}
			y->parent = x->parent;

			//��y��ȫ����x��λ��
			if (x == root){
				root = y;
			}
			else if (x == x->parent->right){
				x->parent->right = y; 
			}
			else{
				x->parent->left = y;
			}
			y->right = x;
			x->parent = y;
		}

	};


}

#endif