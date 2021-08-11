#ifndef SPLAY_H
#define SPLAY_H

#include <algorithm>

namespace Math {
	//注意，非线程安全

	/*---SplayNode---*/
	/*---splay节点存储的值，标记等---*/
	/*---默认Splay节点，可以自定义---*/
	/*---需要至少有一个Ty参数构造函数---*/
	/*---pushup、pushrev和pushdown函数---*/

	template<typename Ty>
	class SplayNode {
	private:
		Ty val;
	public:
		SplayNode(const Ty& _val = Ty()) :val(_val) {
		}
		Ty getval() { return val; }//获取val
		void pushup(SplayNode* l, SplayNode* r) {//向上更新
		}
		void pushrev(SplayNode* l, SplayNode* r) {//reverse后向上更新
		}
		void pushdown(SplayNode* l, SplayNode* r) {//向下更新标记
		}
		bool operator<(const SplayNode<Ty>& other)const {
			return val < other.val;
		}
	};

	template<typename Ty, typename Pr>
	struct TreeNode {
	public:
		Pr* val;
		TreeNode* ch[2], * fa;
		size_t size;
	private:
		bool rev;//是否reverse
		bool is;//是否是前后哨兵节点
	public:
		TreeNode(const Ty& _val = Ty(), const int& _is = false)
			:val(new Pr(_val)), ch{ nullptr,nullptr }, fa(nullptr), size(1), is(_is), rev(false) {
		}
		~TreeNode() {
			delete val;
			if (ch[0] != nullptr)
				delete ch[0];
			if (ch[1] != nullptr)
				delete ch[1];
		}
		Pr* getl() { return ch[0] != nullptr ? ch[0]->val : nullptr; }
		Pr* getr() { return ch[1] != nullptr ? ch[1]->val : nullptr; }
	private:
		bool whichchild() { return fa->ch[1] == this; }
		void rotate() {
			TreeNode* y = fa, * z = y->fa;
			int k = !whichchild();
			if (z != nullptr)
				z->ch[y->whichchild()] = this;
			fa = z;
			y->ch[!k] = ch[k];
			if (ch[k] != nullptr)
				ch[k]->fa = y;
			ch[k] = y;
			y->fa = this;
			y->pushup();
		}
		void dfspushdown(TreeNode* goal) {
			if (fa != nullptr)
				fa->dfspushdown(goal);
			pushdown();
		}
	public:
		void pushup() {
			size = (ch[0] != nullptr ? ch[0]->size : 0) + (ch[1] != nullptr ? ch[1]->size : 0) + 1;
			val->pushup(getl(), getr());
		}
		void pushrev() {
			std::swap(ch[0], ch[1]);
			rev ^= 1;
			val->pushrev(getl(), getr());
			//在reverse后因为左右儿子改变可能需要更新val
		}
		void pushdown() {
			if (rev) {
				if (ch[0] != nullptr)
					ch[0]->pushrev();
				if (ch[1] != nullptr)
					ch[1]->pushrev();
				rev = false;
			}
			val->pushdown(getl(), getr());
			//懒惰标记等的下放
		}
		void splay(TreeNode*& rt, TreeNode* goal = nullptr) {
			TreeNode* y, * z;
			dfspushdown(goal);
			while (fa != goal) {
				y = fa, z = y->fa;
				if (z != goal)
					whichchild() ^ y->whichchild() ? rotate() : y->rotate();
				rotate();
			}
			pushup();
			if (goal == nullptr)rt = this;
		}
	};

	template<typename Ty, typename Pr = SplayNode<Ty>>
	class Splay;

	template<typename Ty, typename Pr>
	class MyIterator : public std::iterator<std::input_iterator_tag, Ty> {
		template<typename _Ty, typename _Pr>
		friend class Splay;

	public:
		MyIterator(Ty* p = nullptr, Ty** _rt = nullptr) :_ptr(p), rt(_rt) {
		}
		//赋值
		MyIterator& operator = (const MyIterator& iter) {
			if(this==&iter)return *this;
			_ptr = iter._ptr;
			rt = iter.rt;
			return*this;
		}
		//不等于
		bool operator != (const MyIterator& iter) {
			return _ptr != iter._ptr;
		}
		//等于
		bool operator == (const MyIterator& iter) {
			return _ptr == iter._ptr;
		}
		//前缀自加
		MyIterator& operator ++ () {
			_ptr->splay(*rt);
			Ty* Nxt = _ptr;
			Nxt = Nxt->ch[1];
			while (Nxt->ch[0] != nullptr)
				Nxt = Nxt->ch[0];
			_ptr = Nxt;
			_ptr->splay(*rt);
			return *this;
		}
		//后缀自加
		MyIterator operator ++ (int) {
			MyIterator tmp = *this;
			++(*this);
			return tmp;
		}
		//前缀自减
		MyIterator& operator--() {
			_ptr->splay(*rt);
			Ty* Nxt = _ptr;
			Nxt = Nxt->ch[0];
			while (Nxt->ch[1] != nullptr)
				Nxt = Nxt->ch[1];
			_ptr = Nxt;
			_ptr->splay(*rt);
			return *this;
		}
		//后缀自减
		MyIterator operator--(int) {
			MyIterator tmp = *this;
			--(*this);
			return tmp;
		}

		MyIterator operator+(int kth) {
			if (!kth)return *this;
			if (kth < 0)return (*this) - (-kth);
			_ptr->splay(*rt);
			Ty* Nxt = _ptr;
			Nxt = Nxt->ch[1];
			while (kth) {
				if (Nxt->ch[0] == nullptr) {
					--kth;
					if (!kth) {
						Nxt->splay(*rt);
						return MyIterator(Nxt, rt);
					}
					Nxt = Nxt->ch[1];
				}
				else {
					if (kth <= Nxt->ch[0]->size)
						Nxt = Nxt->ch[0];
					else {
						kth -= Nxt->ch[0]->size + 1;
						if (!kth) {
							Nxt->splay(*rt);
							return MyIterator(Nxt, rt);
						}
						Nxt = Nxt->ch[1];
					}
				}
			}
			return *this;
		}
		MyIterator operator-(int kth) {
			if (!kth)return *this;
			if (kth < 0)return (*this) + (-kth);
			_ptr->splay(*rt);
			Ty* Nxt = _ptr;
			Nxt = Nxt->ch[0];
			while (true) {
				if (Nxt->ch[1] == nullptr) {
					--kth;
					if (!kth) {
						Nxt->splay(*rt);
						return MyIterator(Nxt, rt);
					}
					Nxt = Nxt->ch[0];
				}
				else {
					if (kth <= Nxt->ch[1]->size)
						Nxt = Nxt->ch[1];
					else {
						kth -= Nxt->ch[1]->size + 1;
						if (!kth) {
							Nxt->splay(*rt);
							return MyIterator(Nxt, rt);
						}
						Nxt = Nxt->ch[0];
					}
				}
			}
		}

		int operator-(const MyIterator& other) {
			if (*this == other)return 0;
			Ty* Nxt = _ptr, * Pre = other._ptr;
			Pre->splay(*rt);
			Nxt->splay(*rt, Pre);
			if (Pre->ch[1] == Nxt)
				return (Nxt->ch[0] != nullptr ? Nxt->ch[0]->size : 0) + 1;
			else return -((Nxt->ch[1] != nullptr ? Nxt->ch[1]->size : 0) + 1);
		}

		Pr& operator * () {
			return *(_ptr->val);//返回树节点
		}
		Pr* operator->()const {
			return _ptr->val;
		}
	private:
		Ty* _ptr;//实际的内容指针，通过该指针跟容器连接
		Ty** rt;//指向原容器的根节点，因为需要splay到根来确保复杂度。
	};

	//非线程安全，请勿在多个线程使用一个Splay实例，或者自行实现锁或原子操作

	template<typename Ty, typename Pr>
	class Splay {//维护序列
	public:
		typedef TreeNode<Ty, Pr> tree_node;
		typedef MyIterator<tree_node, Pr> iterator;
	private:
		tree_node* rt;
		tree_node* End;
		void splay(tree_node* x, tree_node* goal = nullptr) {
			x->splay(rt, goal);
		}
		tree_node* findkth(int kth) {
			++kth;
			tree_node* x = rt;
			while (true) {
				x->pushdown();
				if (x->ch[0] == nullptr) {
					kth -= 1;
					if (!kth)return x;
					x = x->ch[1];
				}
				else {
					if (kth <= x->ch[0]->size)
						x = x->ch[0];
					else {
						kth -= x->ch[0]->size + 1;
						if (!kth)return x;
						x = x->ch[1];
					}
				}
			}
		}
		tree_node* build(const Ty* valarr, const int& L, const int& R) {
			if (L > R)return nullptr;
			int mid = L + R >> 1;
			tree_node* now = new tree_node(valarr[mid]);
			now->ch[0] = build(valarr, L, mid - 1);
			if (now->ch[0] != nullptr)
				now->ch[0]->fa = now;
			now->ch[1] = build(valarr, mid + 1, R);
			if (now->ch[1] != nullptr)
				now->ch[1]->fa = now;
			now->pushup();
			return now;
		}

		void initial() {
			rt = new tree_node(0, true);
			End = rt->ch[1] = new tree_node(0, true);
			rt->ch[1]->fa = rt;
			rt->size = 2;
		}
		tree_node* Split(const int& L, const int& R) {
			tree_node* x = findkth(L - 1);
			splay(x);
			tree_node* y = findkth(R + 1);
			splay(y, x);
			return y->ch[0];
		}
	public:
		Splay() {
			initial();
		}
		Splay(const Ty* valarr, const int& n) {
			initial();
			insert(0, valarr, n);
		}
		Splay(Splay& other) {
			initial();
			insert(begin(), other.begin(), other.end());
		}
		~Splay() {
			delete rt;
		}
		Splay& operator=(Splay& other) {
			if(this==&other)return *this;
			delete rt;
			initial();
			insert(begin(), other.begin(), other.end());
			return *this;
		}
		size_t size() { return rt->size - 2; }
		void insert(const int& kth, const Ty& val) {//第k个元素后加入val
			tree_node* x = findkth(kth);
			splay(x);
			tree_node* y = findkth(kth + 1);
			splay(y, x);
			tree_node* ins = new tree_node(val);
			y->ch[0] = ins;
			ins->fa = y;
			splay(ins);
		}
		void insert(iterator pos, const Ty& val) {
			//在_pos迭代器位置加入val
			//insert(begin(),val)等价于insert(0,val)
			tree_node* x = (pos - 1)._ptr, * y = pos._ptr;
			splay(x);
			splay(y, x);
			tree_node* ins = new tree_node(val);
			y->ch[0] = ins;
			ins->fa = y;
			splay(ins);
		}
		void insert(const int& kth, const Ty* valarr, const int& n) {//在第k个元素后加入valarr前n个元素
			tree_node* x = findkth(kth);
			splay(x);
			tree_node* y = findkth(kth + 1);
			splay(y, x);
			tree_node* z = build(valarr, 0, n - 1);
			y->ch[0] = z;
			z->fa = y;
			splay(z);
		}
		void insert(iterator pos, const Ty* valarr, const int& n) {
			tree_node* x = (pos - 1)._ptr;
			splay(x);
			tree_node* y = pos._ptr;
			splay(y, x);
			tree_node* z = build(valarr, 0, n - 1);
			y->ch[0] = z;
			z->fa = y;
			splay(z);
		}
		void insert(iterator pos, iterator Val) {
			Val._ptr->splay(rt);
			insert(pos, Val->getval());
		}
		void insert(iterator _Ins, iterator _Begin, iterator _End) {
			if (_Begin == _End)return;
			const int Length = _End - _Begin;
			Ty* valarr = new Ty[Length];
			int i = 0;
			for (iterator it = _Begin; it != _End; ++it, ++i)
				valarr[i] = it->getval();
			insert(_Ins, valarr, Length);
		}
		void swap(Splay<Ty, Pr>& other) noexcept {
			//O(1)交换两个Splay
			std::swap(rt, other.rt);
			std::swap(End, other.End);
		}
		void insert(iterator _Ins, Splay<Ty, Pr>& other) {
			if ((size() << 1) < other.size()) {
				Splay<Ty, Pr>mid = other;
				mid.insert(mid.begin(), begin(), _Ins);
				mid.insert(mid.end(), _Ins, end());
				*this = mid;
			}
			else {
				insert(_Ins, other.begin(), other.end());
			}
		}

		void erase(const int& kth) {
			tree_node* x = Split(kth, kth);
			x->fa->ch[0] = nullptr;
			splay(x->fa);
			delete x;
		}
		void erase(iterator pos) {
			iterator Pre, Nxt;
			Pre = Nxt = pos;
			--Pre;
			++Nxt;
			tree_node* x = Pre._ptr, * y = Nxt._ptr;
			x->splay(rt);
			y->splay(rt, x);
			delete y->ch[0];
			y->ch[0] = nullptr;
			y->splay(rt);
		}
		void erase(const int& L, const int& R) {
			tree_node* x = Split(L, R);
			x->fa->ch[0] = nullptr;
			splay(x->fa);
			delete x;
		}
		void erase(iterator pos_l, iterator pos_r) {
			iterator Pre, Nxt;
			Pre = pos_l;
			Nxt = pos_r;
			--Pre;
			tree_node* x = Pre._ptr, * y = Nxt._ptr;
			x->splay(rt);
			y->splay(rt, x);
			delete y->ch[0];
			y->ch[0] = nullptr;
			y->splay(rt);
		}
		void reverse(const int& L, const int& R) {
			if (L > R)return;
			Split(L, R)->pushrev();
		}
		iterator kthelement(int kth) {
			tree_node* x = findkth(kth);
			splay(x);
			return iterator(x, &rt);
		}
		iterator split(const int& L, const int& R) {
			return iterator(Split(L, R), &rt);
		}
		iterator begin() {
			return kthelement(1);
		}
		iterator end() {
			return iterator(End, &rt);
		}
	};

}

#endif SPLAY_H