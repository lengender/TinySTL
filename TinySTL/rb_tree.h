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

	//底层节点
	struct _rb_tree_node_base{
		typedef _rb_tree_color_type			color_type; 
		typedef _rb_tree_node_base*			base_ptr;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		/*
			给定某个点的位置，找到最左边的节点
			如果给定根节点的位置，可以找到整棵红黑树最左边的节点
			返回节点位置
		*/
		static base_ptr minimum(base_ptr x){
			while (x->left != 0){
				x = x->left;
			}
			return x;
		}

		//找最右边节点
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
		//子类继承父类成员
		typedef _rb_tree_node<Value>* link_type;
		Value value_field;
	};

	//底层迭代器
	struct rb_tree_base_iterator{
		typedef typename _rb_tree_node_base::base_ptr base_ptr; 
		typedef typename TinySTL::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t					difference_type;
		base_ptr node;   //成员变量，指向父类节点的指针，这是联系迭代器和节点的纽带

		//迭代器的子类实现operator++时调用本函数
		void increment(){
			if (node->right != 0){
				node = node->right;
				while (node->left != 0){
					node = node->left;
				}
			}
			else{
				base_ptr y = node->parent;  //找出父节点
				while (node == y->right) {//如果现行节点本身就是右子节点
					node = y;  //就一直上溯，直到不为右子节点为止
					y = y->parent;
				} 
				if (node->right != y){  //若此时的右子节点不等于此时的父节点，此时父节点即为解答
					node = y;           //否则，此时的node为解答
				}
			}
		}

		//迭代器的子类实现operator--时调用本函数
		void decrement(){
			if (node->color == _rb_tree_red && node->parent->parent == node){
				//如果是红节点，其祖父节点等于自己，那么右节点即为解答
				//该情况发生于node为header时，注意header的右子节点（即mostring），
				//指向整棵树的max节点
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

	//迭代器
	template<class Value, class Ref, class Ptr>
	struct _rb_tree_iterator : public rb_tree_base_iterator{
			//注意正式迭代器没有成员变量，只是继承了基类迭代器的node变量
			//基层迭代器的node变量是红黑树节点与迭代器连接的纽带
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

		//解引用，返回节点值
		reference operator*() const{ return link_type(node)->value_field; }
		pointer operator->() const{ return *(operator*()); }

		//返回迭代器指向的节点的颜色
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

		//比较
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
		typedef rb_tree_node*							link_type;  //节点指针
		typedef size_t									size_type;
		typedef ptrdiff_t								difference_type;
	protected:
		size_type node_count;	//节点数量
		link_type	header;  //	哨兵节点，其parent指针指向根节点，实现上的一个技巧
		Compare key_compare;  //比较大小的函数

	public:
		typedef _rb_tree_iterator<value_type, reference, pointer> iterator;   //定义红黑树迭代器
	protected:
		//调用空间配置器申请一个节点
		link_type get_node() { return rb_tree_node_allocator::allocate(); }

		//调用空间配置器释放一个节点
		void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

		//申请并初始化节点
		link_type create_node(const value_type& x){
			//x是节点的值
			link_type tmp = get_node();
			construct(&tmp->value_field, x);   //调用全局构造函数初始化节点
			return tmp;
		}

		//克隆节点
		link_type clone_node(link_type x){
			link_type tmp = create_node(x->value_field);
			tmp->color = x->color;
			tmp->left = 0;
			tmp->right = 0;
			return tmp;
		}

		//释还节点
		void destroy_node(link_type p){
			destroy(&p->value_field);
			put_node(p);   //释放内存
		}

	private:
		//初始化红黑树
		void init(){
			header = get_node();  
			color(header) = _rb_tree_red;   //设置header节点为红色
			root() = 0;  //root()获得红黑树的根节点，header的parent指针指向根节点，初始化为null
			leftmost() = header;  //设置header节点的左子树指向自己
			rightmost() = header;  //设置hedaer节点的右子树指向自己
		}
	public:
		//构造函数
		rb_tree(const Compare& cmp = Compare()) :node_count(0), key_compare(cmp){ init(); }

		//析构函数
		~rb_tree(){
			put_node(header);
		}

	protected:
		//获得根节点(header是哨兵，其parent指向根节点)
		link_type &root() const{ return (link_type&)(header->parent); }

		//获得整棵树最左边的节点
		link_type& leftmost() const { return (link_type&)(header->left); }

		//获得整棵树最右边的节点
		link_type& rightmost() const{ return (link_type&)(header->right); }

		//返回节点的子节点
		static link_type& left(link_type x){ return (link_type&)(x->left); }
		static link_type& right(link_type x) { return (link_type&)(x->right); }
		//返回节点的父节点
		static link_type& parent(link_type x) { return (link_type&)(x->parent); }

		//返回节点的value
		static reference value(link_type x) { return x->value_field; }

		//返回节点的颜色
		static color_type& color(link_type x) { return (color_type&)(x->color); }

		//返回节点的key
		static const key_type& key(link_type x){ return KeyOfValue()(value(x)); }

		//返回最小值节点
		static link_type minimum(link_type x){
			return (link_type)_rb_tree_node_base::minimum(x);
		}
		//返回最大值节点
		static link_type msximum(link_type x){
			return (link_type)_rb_tree_node_base::maximum(x);
		}
	public:
		//获得根节点的值(header是哨兵，其parent指向根节点)
		value_type root_value() { return value((link_type)header->parent); }

		//返回比较大小的函数
		Compare key_comp() const { return key_compare; }

		//返回一个迭代器，指向红黑树最左边的节点
		iterator begin() { retuen leftmost(); }

		//返回一个迭代器，指向红黑树最右边的节点
		iterator end() { return header; }

		bool empty() const{ return node_count == 0; }
		size_type size() const{ return node_count; }
		size_type max_size() const{ return size_type(-1); }
	public:
		//插入新值，节点键值不能重复，如果重复，则插入无效
		//返回值是pair,pair的第一个元素是rb_tree迭代器，指向新节点，第二个元素表示成功与否
		std::pair<iterator, bool> insert_unique(const value_type& v){
			link_type y = header;   
			link_type x = root(); 
			bool comp = true;   
			while (x != 0){  //x不为null,说明我们找到新插入节点的位置，于是执行while循环内的语句，不停向下遍历
				y = x;   //y保存看x节点的父节点

				//如果待插入节点值小于x的值，comp为true，否则false,key_compare是比较大小函数，由模板指定
				comp = key_compare(KeyOfValue()(v), key(x));

				x = comp ? left(x) : right(x);
			}

			iterator j = iterator(y);   //令j指向插入点的父节点
			if (comp){ //如果插入点的值比父节点的值小，以为着我们要插入到父节点的左边，进入if
				if (begin() == j){
					return std::pair<iterator, bool>(_insert(x, y, v), true);
					//x为插入点，y为插入点的父节点，v为插入的值
				}
				else{
					j--;
				}
			}
			//新值不与既有节点重复，可以插入
			if (key_compare(key(j.node), KeyOfValue()(v))){
				return std::pair<iterator, bool>(_insert(x, y, v), true);
			}
			return std::pair<iterator, bool>(j, false);  
		}

		//插入新值，节点的键值允许重复
		//返回红黑树的迭代器，该迭代器指向新节点
		iterator insert_equal(const value_type& v){
			link_type y = header;
			link_type x = root();  //x指向根节点
			while (x != 0){
				y = x;
				x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
			}
			return _insert(x, y, v);
		}

		//寻找红黑树中是否有键值为k的节点
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
		//插入操作，x_是待插入节点，y_是插入节点的父节点，v插入的值
		iterator _insert(base_ptr x_, base_ptr y_, const value_type& v){
			//base_ptr实为_rb_tree_node_base*, link_type实为_rb_tree_node*
			//我们把_rb_tree_node_base*向下强转为_rb_tree_node*
			//_rb_tree_node比_rb_tree_node_base多了value_field，用于保存值
			link_type x = (link_type)x_;
			link_type y = (link_type)y_;
			link_type z;

			/*1.y == header, 插入点的父节点为header(注意header是哨兵，header不属于树的一部分,但是header的parent指向根节点)
				y == header说明插入点为根节点
			  2. x == 0, 说明插入点在叶子节点下方（叶子节点的左子树和右子树均为null）,也就是在叶子节点下挂新的节点
			     x != 0说明插入点在树的内部某个节点上
			  3. key_compare(KeyOfValue()(v), key(y)) 待插入节点的值要比父节点的值小(意味着我们要插入到父节点的左子树上)
 			*/
			if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))){
				z = create_node(v);  //创建新节点，令新节点的值(value_field)为v
				left(y) = z;   //令父节点的左子树为z,我们成功把新节点加入到树中
				if (y == header){ //y==header,插入点的父节点为header,说明根节点还没有被初始化，进入if就是初始化根节点
					root() = z;  //z 成了根节点
					rightmost() = z;  //令z为整棵树最右边的节点
				}
				else if (y == leftmost()){ //如果父节点是整棵树最左边节点
					leftmost() = z;  //我们把新节点作为整棵树最左边的节点
				}
			}
			else{
				z = create_node(v);  //创建新节点
				right(y) = z; //插入节点到父节点的右边
				if (y == rightmost()){ //如果父节点是整棵树最右边的节点
					rightmost() = z;
				}
			}
			
			parent(z) = y;  //令新节点的父节点为y
			left(z) = 0; 
			right(z) = 0;

			_rb_tree_rebalance(z, header->parent); //header是哨兵，一旦建立不能改变，header->parent指向根节点
			++node_count;
			return iterator(z);
		}

		//重新平衡红黑树(改变颜色和旋转树形)
		// x 为新增节点 ， root为根节点
		inline void _rb_tree_rebalance(_rb_tree_node_base* x, _rb_tree_node_base*& root){
			x->color = _rb_tree_red;  //新插入节点为红色
			//如果新增节点(x)不为根节点，且新增节点的父节点为红色，进入while循环
			while (x != root && x->parent->color == _rb_tree_red){
				if (x->parent == x->parent->parent->left){ //如果父节点是祖父节点的左子节点
					_rb_tree_node_base* y = x->parent->parent->right;  //令y为伯父节点
					if (y && y->color == _rb_tree_red){ //如果伯父节点存在且为红
						x->parent->color = _rb_tree_black;
						y->color = _rb_tree_black;
						x->parent->parent->color = _rb_tree_red;
						x = x->parent->parent;
					}
					else{ //如果伯父节点不存在，或者伯父节点为黑
						if (x == x->parent->right){ //如果新增节点是父节点的右子节点，说明插入点的方式是内插
							x = x->parent;   //父节点为旋转支点
							/*
								整理一下从while开始的条件判断分支，我们可以得出做左旋转的条件
								1.新增节点不是根节点
								2.新增节点的父节点是红色
								3.父节点是祖父节点的左子节点
								4.伯父节点不存在，或者伯父节点为黑
								5.新增节点为父节点的右子节点
							*/
							_rb_tree_rotate_left(x, roog);  //左旋转
						}
						x->parent->color = _rb_tree_black;  //修改颜色
						x->parent->parent->color = _rb_tree_red;
						_rb_tree_rotate_right(x->parent->parent, root); //左旋转完了接着右旋
					}
				}
				else{  //父节点是祖父节点的右子节点
					_rb_tree_node_base* y = x->parent->parent->left;  //令y为伯父节点
					if (y && y->color == _rb_tree_red){ //如果伯父节点存在且为红
						x->parent->color = _rb_tree_black;
						y->color = _rb_tree_black;
						x->parent->parent->color = _rb_tree_red;
						x = x->parent->parent;
					}
					else{ //如果伯父节点不存在，或者伯父节点为黑
						//如果新增节点为父节点的左子节点，说明插入点的方式为内插
						if (x == x->parent->left){
							x = x->parent;  //父节点为旋转支点
							/*
								整理一下从while开始的条件判断分支，我们可以判断出做右旋转的条件
								1.新增节点不是根节点
								2.新增节点的父节点是红色
								3.父节点是祖父节点的右子节点
								4.伯父节点不存在，或者伯父节点为黑
								5.新增节点是父节点的左子节点
							*/
							_rb_tree_rotate_right(x, root);  //右旋转
						}
						x->parent->color = _rb_tree_black;
						x->parent->parent->color = _rb_tree_red;
						_rb_tree_rotate_left(x->parent->parent, root);
					}
				}
			}
			root->color = _rb_tree_black; //根节点永远是黑
		}

		/*
			左旋转
			新节点必为红节点，如果插入节点的父节点也是红色，就违反了红黑树的规则，必须做选择
			x:左旋转的支点
			root: 红黑树的根
		*/
		inline void _rb_tree_rotate_left(_rb_tree_node_base* x, _rb_tree_node_base*& root){
			_rb_tree_node_base* y = x->right; //y 为旋转点的右子节点
			x->right = y->left; //旋转点为父，旋转点的右子节点为子，完成父子对换
			if (y->left != 0){
				y->left->parent = x;
			}
			y->parent = x->parent;

			if (x == root){
				root = y; //令y 为根节点
			}
			else if (x == x->parent->left){
				x->parent->left = y; //令旋转点的父节点的左子节点为y
			}
			else{
				x->parent->right = y; //令旋转点的父节点的右子节点为y
			}
			
			y->left = x;
			x->parent = y; //右子节点为旋转点的父节点
		}

		/*
			右旋转
			新节点必为红节点，如果插入点的父节点也是红色，就违反了红黑树的规则，此时必须做旋转
			x:右旋转的支点
			root:红黑树的根
		*/
		inline void _rb_tree_rotate_right(_rb_tree_node_base* x, _rb_tree_node_base*& root){
			_rb_tree_node_base *y = x->left; //令y为旋转点的左子节点
			x->left = y->right;   //令y的右子节点为旋转点的左子节点
			if (y->right != 0){
				y->right->parent = x;  //设定父节点
			}
			y->parent = x->parent;

			//令y完全顶替x的位置
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