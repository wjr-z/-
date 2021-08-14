#ifndef SLIST_H
#define SLIST_H

#include <algorithm>
#include <cmath>
#include <list>

namespace Math {

	//顺序和首尾插入复杂度为O(1)
	//随机访问和插入复杂度是O(sqrt(n))

	template<typename Ty>
	class slist;

	//slist节点
	template<typename Ty>
	struct slist_node {
	private:
		using value_type = Ty;
		using reference  = value_type&;
		using node       = slist_node<Ty>;
		using iterator1  = typename std::list<std::list<node>>::iterator;

	public:
		Ty Val;//节点权值
		iterator1 linked;//链接到块状链表的iterator
		slist_node() noexcept = default;
		slist_node(const Ty& Val, const iterator1& linked)
			:Val(Val), linked(linked) {
		}
		slist_node(const slist_node& other)noexcept
			:Val(other.Val), linked(other.linked) {
		}
		slist_node(slist_node&& other)noexcept
			:Val(std::move(other.Val)), linked(std::move(other.linked)) {
		}
		slist_node& operator=(const slist_node& other) {
			if (this == &other)return *this;
			Val = other.Val;
			linked = other.linked;
			return *this;
		}
		slist_node& operator=(slist_node&& other) noexcept {
			if (this == &other)return *this;
			Val = std::move(other.Val);
			linked = std::move(other.linked);
			return *this;
		}
		~slist_node() noexcept = default;
	};

	//slist迭代器
	template<typename Ty>
	class slist_iterator
		: public std::iterator<std::random_access_iterator_tag, int> {

		template<typename Ty>
		friend class slist;

	private:
		using node       = slist_node<Ty>;
		using iterator1  = typename std::list<std::list<node>>::iterator;
		using iterator2  = typename std::list<node>::iterator;

	public:
		using value_type = Ty;
		using reference  = value_type&;
		using pointer    = value_type*;

	private:
		//指向原容器
		slist<Ty>* Point;
		//第二层list迭代器
		iterator2 Pos2;

		//判断是否是容器尾部
		bool isEnd()const noexcept {
			return Pos2._Ptr == ((--Point->List.end())->end())._Ptr;
		}

		//获取第一层list的迭代器
		iterator1 GetIterator1()const noexcept {
			//当为容器尾部时需要返回--end()
			//其余时候返回节点指向的第一层list迭代器即可
			return isEnd() ? --Point->List.end() : Pos2->linked;
		}
	public:
		slist_iterator(slist<Ty>* Point, const iterator2& Pos2)noexcept
			:Point(Point), Pos2(Pos2) {
		}

		slist_iterator(slist<Ty>* Point, iterator2&& Pos2)noexcept
			:Point(Point), Pos2(std::move(Pos2)) {
		}

		slist_iterator(const slist_iterator& Pos)noexcept
			:Point(Pos.Point), Pos2(Pos.Pos2) {
		}

		slist_iterator(slist_iterator&& Pos)noexcept
			:Point(std::move(Pos.Point)), Pos2(std::move(Pos.Pos2)) {
		}

		~slist_iterator() {}

		bool operator==(const slist_iterator& other)const {
			return Pos2._Ptr == other.Pos2._ptr;
		}

		bool operator!=(const slist_iterator& other)const {
			return Pos2._Ptr != other.Pos2._Ptr;
		}

		slist_iterator& operator=(const slist_iterator& other) {
			if (this == &other)return *this;
			Point = other.Point;
			Pos2 = other.Pos2;
			return *this;
		}

		slist_iterator& operator=(slist_iterator&& other) noexcept {
			if (this == &other)return *this;
			Point = std::move(other.Point);
			Pos2 = std::move(other.Pos2);
			return *this;
		}

		reference operator*()const noexcept {
			return Pos2->Val;
		}

		pointer operator->()const noexcept {
			return std::pointer_traits<pointer>::pointer_to(**this);
		}

		slist_iterator& operator++() {
			iterator1 Pos1 = GetIterator1();
			++Pos2;
			if (isEnd()) return *this;
			if (Pos2 == Pos1->end()) {
				//到达当前块的尾部+1位置，应该跳到下一块
				Pos2 = (++Pos1)->begin();
			}
			return *this;
		}

		slist_iterator operator++(int) {
			slist_iterator it(*this);
			++(*this);
			return it;
		}

		slist_iterator& operator--() {
			iterator1 Pos1 = GetIterator1();
			if (Pos2 == Pos1->begin()) {
				Pos2 = (--Pos1)->end();
			}
			--Pos2;
			return *this;
		}

		slist_iterator operator--(int) {
			slist_iterator it(*this);
			--(*this);
			return *this;
		}

		slist_iterator& operator+=(size_t delta) {
			if (!delta)return *this;

			//首先获取第一层list的迭代器
			iterator1 Pos1 = GetIterator1();
			//如果改节点大小过大则分裂节点
			Point->psplit(Pos1);

			++Pos2;
			//逐元素遍历该块
			while (delta && Pos2 != Pos1->end()) {
				++Pos2;
				--delta;
			}
			--Pos2;

			//元素在该块，则可以直接返回
			if (!delta) return *this;
			//在下一块，则首先进行块的跳跃
			--delta;
			++Pos1;

			for (; Pos1 != Point->List.end(); ++Pos1) {
				Point->psplit(Pos1);
				Point->pmerge(Pos1);
				if (Pos1->size() > delta) {
					//在该块，对该块进行遍历
					Pos2 = Pos1->begin();
					while (delta)
						++Pos2, --delta;
					return *this;
				}
				delta -= Pos1->size();
			}

			//遍历到容器尾部
			--Pos1;
			Pos2 = Pos1->end();
			return *this;
		}

		slist_iterator operator+(size_t delta) {
			slist_iterator it(*this);
			it += delta;
			return it;
		}

		slist_iterator& operator-=(size_t delta) {
			if (!delta)return *this;
			//同+=
			iterator1 Pos1 = GetIterator1();
			Point->nsplit(Pos1);

			while (delta && Pos2 != Pos1->begin()) {
				--Pos2;
				--delta;
			}

			if (!delta)
				return *this;

			--Pos1;

			do {
				Point->nsplit(Pos1);
				Point->nmerge(Pos1);
				if (Pos1->size() >= delta) {
					Pos2 = Pos1->end();
					while (delta)
						--Pos2, --delta;
					break;
				}
				delta -= Pos1->size();
				--Pos1;
			} while (Pos1 != Point->List.begin());
			return *this;
		}

		slist_iterator operator-(size_t delta) {
			slist_iterator it(*this);
			it -= delta;
			return it;
		}

	};

	//slist const迭代器
	//不能进行+=,-=

	template<typename Ty>
	class slist_const_iterator
		: public std::iterator<std::random_access_iterator_tag, int> {

		template<typename Ty>
		friend class slist;

	private:
		using node       = slist_node<Ty>;
		using iterator1  = typename std::list<std::list<node>>::const_iterator;
		using iterator2  = typename std::list<node>::const_iterator;

	public:
		using value_type = Ty;
		using reference  = const value_type&;
		using pointer    = value_type*;

	private:
		//指向原容器
		slist<Ty>* Point;
		//第二层list迭代器
		iterator2 Pos2;

		//判断是否是容器尾部
		bool isEnd()const noexcept {
			return Pos2._Ptr == ((--Point->List.end())->end())._Ptr;
		}

		//获取第一层list的迭代器
		iterator1 GetIterator1()const noexcept {
			//当为容器尾部时需要返回--end()
			//其余时候返回节点指向的第一层list迭代器即可
			return isEnd() ? --Point->List.end() : Pos2->linked;
		}
	public:
		slist_const_iterator(const slist<Ty>* Point, const iterator2& Pos2)noexcept
			:Point(const_cast<slist<Ty>*>(Point)), Pos2(Pos2) {
		}

		slist_const_iterator(const slist<Ty>* Point, iterator2&& Pos2)noexcept
			:Point(const_cast<slist<Ty>*>(Point)), Pos2(std::move(Pos2)) {
		}

		slist_const_iterator(const slist_const_iterator& Pos)noexcept
			:Point(Pos.Point), Pos2(Pos.Pos2) {
		}

		slist_const_iterator(slist_const_iterator&& Pos)noexcept
			:Point(std::move(Pos.Point)), Pos2(std::move(Pos.Pos2)) {
		}

		~slist_const_iterator() {}

		bool operator==(const slist_const_iterator& other)const {
			return Pos2._Ptr == other.Pos2._ptr;
		}

		bool operator!=(const slist_const_iterator& other)const {
			return Pos2._Ptr != other.Pos2._Ptr;
		}

		slist_const_iterator& operator=(const slist_const_iterator& other) {
			if (this == &other)return *this;
			Point = other.Point;
			Pos2 = other.Pos2;
			return *this;
		}

		slist_const_iterator& operator=(slist_const_iterator&& other)noexcept {
			if (this == &other)return *this;
			Point = std::move(other.Point);
			Pos2 = std::move(other.Pos2);
			return *this;
		}

		reference operator*()const noexcept {
			return Pos2->Val;
		}

		pointer operator->()const noexcept {
			return std::pointer_traits<pointer>::pointer_to(**this);
		}

		slist_const_iterator& operator++() noexcept {
			++Pos2;
			if (isEnd()) return *this;
			iterator1 Pos1 = GetIterator1();
			if (Pos2 == Pos1->end()) {
				//到达当前块的尾部+1位置，应该跳到下一块
				Pos2 = (++Pos1)->begin();
			}
			return *this;
		}

		slist_const_iterator operator++(int) noexcept {
			slist_const_iterator it(*this);
			++(*this);
			return it;
		}

		slist_const_iterator& operator--() noexcept {
			iterator1 Pos1 = GetIterator1();
			if (Pos2 == Pos1->begin()) {
				Pos2 = (--Pos1)->end();
			}
			--Pos2;
			return *this;
		}

		slist_const_iterator operator--(int) noexcept {
			slist_const_iterator it(*this);
			--(*this);
			return *this;
		}

	};

	template<typename Ty>
	class slist {

		template<typename Ty>
		friend class slist_iterator;

		template<typename Ty>
		friend class slist_const_iterator;

	public:
		using iterator       = slist_iterator<Ty>;
		using const_iterator = slist_const_iterator<Ty>;

	private:
		using node           = slist_node<Ty>;
		using iterator1      = typename std::list<std::list<node>>::iterator;
		using iterator2      = typename std::list<node>::iterator;

		using value_type     = Ty;
		using referece       = value_type&;

		//第一层维护块，第二层维护块内节点
		std::list<std::list<node>>List;
		size_t Size;

		//将块内节点指向的第一层迭代器统一修改
		static void Linked(iterator2 _First, iterator2 _Last, iterator1 LinkedFa) {
			for (; _First != _Last; ++_First)
				_First->linked = LinkedFa;
		}

		//通过迭代器获取iterator1
		static iterator1 GetIterator1(const iterator& Pos) {
			return Pos.GetIterator1();
		}

		//前向操作,从前往后遍历时，尝试向后分裂
		void psplit(iterator1 pos) {
			const size_t NowSize = pos->size();
			//对于大于sqrt(2)*Size的块进行分裂
			if (NowSize * NowSize < 2 * Size) return;

			//分裂成大小在sqrt(2)/2 * Size ~ sqrt(2)*Size 的块
			const int BlockSize = std::sqrt(2 * Size) / 2 + 1;
			//分裂出的块大小
			int BloCnt = NowSize / BlockSize;

			while (BloCnt > 1) {
				//新建新的块链
				std::list<node> nxt;
				//分配当前节点到nxt块链
				iterator2 First = pos->end(), Last = First;
				//将nxt分配BlockSize个元素
				size_t mid = 0;
				while (mid < BlockSize)--First, ++mid;
				nxt.splice(nxt.end(), *pos, First, Last);
				--BloCnt;
				//将新的块链插入到List
				iterator1 PosNxt = pos;
				++PosNxt;
				List.emplace(PosNxt, nxt);
				--PosNxt;
				//修改节点指向的iterator1
				Linked(PosNxt->begin(), PosNxt->end(), PosNxt);
			}

		}

		//后向操作，尝试向前分裂
		void nsplit(iterator1 pos) {
			//同上
			const size_t NowSize = pos->size();
			if (NowSize * NowSize < 2 * Size) return;

			const int BlockSize = std::sqrt(2 * Size) / 2 + 1;
			int BloCnt = NowSize / BlockSize;

			while (BloCnt > 1) {
				//同上
				std::list<node> pre;
				iterator2 First = pos->begin(), Last = First;
				size_t mid = 0;
				while (mid < BlockSize)++Last, ++mid;
				pre.splice(pre.end(), *pos, First, Last);
				--BloCnt;
				List.emplace(pos, pre);
				iterator1 PrePos = pos;
				--PrePos;
				Linked(PrePos->begin(), PrePos->end(), PrePos);
			}
		}

		//试图和后面的块进行合并
		void pmerge(const iterator1& pos) {
			//当前块和后一个块均小于sqrt(Size)/2即可合并
			iterator1 npos = pos;
			++npos;
			if (npos == List.end())return;
			const size_t NowSize = pos->size(), NxtSize = npos->size();
			if (2 * NowSize * NowSize > Size || 2 * NxtSize * NxtSize > Size)return;

			Linked(npos->begin(), npos->end(), pos);
			pos->splice(pos->end(), *npos);

			List.erase(npos);
			pmerge(pos);
		}

		void nmerge(const iterator1& pos) {
			//当前块和前一个块均小于sqrt(Size)/2即可合并
			iterator1 ppos = pos;
			if (pos == List.begin())return;
			--ppos;
			const size_t NowSize = pos->size(), NxtSize = ppos->size();
			if (2 * NowSize * NowSize > Size || 2 * NxtSize * NxtSize > Size)return;

			Linked(ppos->begin(), ppos->end(), pos);
			pos->splice(pos->end(), *ppos);

			List.erase(ppos);
			nmerge(pos);
		}

		void initial() {
			Size = 0;
			List.emplace_back(std::list<node>());
		}

		void ierase(const iterator1& Pos) {
			if (!Pos->empty() || !Size)return;
			List.erase(Pos);
		}
	public:

		slist() {
			initial();
		}

		slist(size_t Count) {
			initial();
			while (Count)push_back(Ty()), --Count;
			maintain();
		}

		slist(size_t Count, const Ty& Val) {
			initial();
			while (Count)push_back(Val), --Count;
			maintain();
		}

		slist(const std::initializer_list<Ty>& ini) {
			initial();
			for (auto i : ini)
				push_back(i);
			maintain();
		}

		slist(const slist& other) {
			initial();
			for (auto it = other.begin(); it != other.end(); ++it)
				push_back(*it);
			maintain();
		}

		slist(slist&&other)noexcept : List(std::move(other.List)),Size(other.Size){
			
		}

		slist& operator = (const slist& other) {
			if (this == &other)return *this;
			List.clear();
			initial();
			for (auto i : other)
				push_back(i);
			maintain();
			return *this;
		}

		slist& operator= (slist&& other)noexcept {
			if(this==&other)return *this;
			List=std::move(other.List);
			Size=other.Size;
			return *this;
		}

		void push_back(const Ty& val) {
			insert(end(), val);
		}

		void push_front(const Ty& val) {
			insert(begin(), val);
		}

		void pop_back() {
			erase(--end());
		}

		void pop_front() {
			erase(begin());
		}

		void insert(iterator Pos, const Ty& Val) {
			auto Pos1 = GetIterator1(Pos);
			Pos1->emplace(Pos.Pos2, slist_node<Ty>(Val, Pos1));
			++Size;
		}

		iterator erase(iterator Pos) {
			iterator Nxt=Pos;
			++Nxt;
			iterator1 Pos1 = GetIterator1(Pos);

			Pos1->erase(Pos.Pos2);
			--Size;
			ierase(Pos1);
			return Nxt;
		}

		iterator begin() {
			return iterator(this, (List.begin())->begin());
		}

		const_iterator begin() const noexcept {
			return const_iterator(this, (List.begin())->begin());
		}

		iterator end() {
			return iterator(this, (--List.end())->end());
		}

		const_iterator end() const noexcept {
			return const_iterator(this, (--List.end())->end());
		}

		referece back() {
			return *--end();
		}

		referece front() {
			return *begin();
		}

		size_t size()const { return Size; }

		value_type& operator[](size_t index) {
			if((index<<1)>Size)return *(end()-(Size-index));
			return *(begin() + index);
		}

		void maintain() {
			std::list<node>* Now = new std::list<node>;
			const size_t bloSize = std::sqrt(Size);
			size_t NowSize = 0;
			std::list<std::list<node>>* NewList = new std::list<std::list<node>>;

			iterator1 Pos1 = List.begin();
			iterator2 Pos2 = Pos1->begin();

			for (size_t i = 0; i < Size; ++i) {
				Now->emplace_back(*Pos2);
				++NowSize;
				if (NowSize == bloSize) {
					NewList->emplace_back(*Now);
					iterator1 back = NewList->end();
					--back;
					iterator2 _First = back->begin(), _Last = back->end();
					Linked(_First, _Last, back);

					Now = new std::list<node>;
					NowSize = 0;
				}
				if (++Pos2 == Pos1->end()) {
					++Pos1;
					if (i < Size - 1)Pos2 = Pos1->begin();
				}
			}
			if (NowSize) {
				NewList->emplace_back(*Now);
				iterator1 back = NewList->end();
				--back;
				iterator2 _First = back->begin(), _Last = back->end();
				Linked(_First, _Last, back);
			}
			List.swap(*NewList);
			NewList->clear();
		}

		void debug(int mode = 0) {
			switch (mode) {
			case 0: {
				for (auto i = begin(); i != end(); ++i) {
					printf("%d ", *i);
				}
				break;
			}
			case 1: {
				for (auto i : List) {
					printf("%d ", i.size());
				}
				break;
			}
			default: ;
			}
			printf("\n");
		}


	};

}

#endif