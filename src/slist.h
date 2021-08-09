#ifndef SLIST_H
#define SLIST_H

#include <list>
#include <algorithm>

//顺序和首尾插入复杂度为O(1)
//随机访问和插入复杂度是O(sqrt(n))

template<typename Ty>
class slist;

template<typename Ty>
class slist_iterator 
	: public std::iterator<std::random_access_iterator_tag, int> {
	template<typename Ty>
	friend class slist;
private:
	typedef typename std::list<std::list<Ty>>::iterator iterator1;
	typedef typename std::list<Ty>::iterator iterator2;
public:
	using value_type = Ty;
	using reference = value_type&;
private:
	slist<Ty>*Point;
	iterator1 Pos1;
	iterator2 Pos2;
public:
	slist_iterator(slist<Ty>*Point,const iterator1&Pos1,const iterator2&Pos2)noexcept
		:Point(Point),Pos1(Pos1),Pos2(Pos2){ }

	slist_iterator(slist<Ty>*Point,iterator1&&Pos1,iterator2&&Pos2)noexcept
		:Point(Point),Pos1(std::move(Pos1)),Pos2(std::move(Pos2)){ }

	slist_iterator(const slist_iterator&Pos)noexcept
		:Point(Pos.Point),Pos1(Pos.Pos1),Pos2(Pos.Pos2){ }

	slist_iterator(slist_iterator&&Pos)noexcept
		:Point(std::move(Pos.Point)),Pos1(std::move(Pos.Pos1)),Pos2(std::move(Pos.Pos2)){ }

	~slist_iterator(){}

	bool operator==(const slist_iterator& other)const {
		return Pos2==other.Pos2;
	}

	bool operator!=(const slist_iterator& other)const {
		return Pos2!=other.Pos2;
	}

	slist_iterator& operator=(const slist_iterator& other) {
		Point=other.Point;
		Pos1=other.Pos1;
		Pos2=other.Pos2;
		return *this;
	}

	slist_iterator& operator++() {
		++Pos2;
		if (Pos2 == Pos1->end()) {
			++Pos1;
			if (Pos1 != Point->List.end())
				Pos2=Pos1->begin();
			else --Pos1;
		}
		return *this;
	}

	slist_iterator operator++(int) {
		slist_iterator it(*this);
		++(*this);
		return it;
	}

	slist_iterator& operator--() {
		if (Pos2 == Pos1->begin()) {
			--Pos1;
			Pos2=Pos1->end();
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
		if(!delta)return *this;

		Point->psplit(Pos1);

		++Pos2;
		while (delta&&Pos2!=Pos1->end()) {
			++Pos2;
			--delta;
		}
		--Pos2;
		if (!delta) return *this;

		--delta;
		++Pos1;

		for (;Pos1!=Point->List.end(); ++Pos1) {
			Point->psplit(Pos1);
			Point->pmerge(Pos1);
			if (Pos1->size() > delta) {
				Pos2 = Pos1->begin();
				while (delta)
					++Pos2,--delta;
				return *this;
			}
			delta -= Pos1->size();
		}

		--Pos1;
		Pos2=Pos1->end();
		return *this;
	}

	slist_iterator operator+(size_t delta) {
		slist_iterator it(*this);
		it+=delta;
		return it;
	}

	slist_iterator& operator-=(size_t delta) {
		if(!delta)return *this;

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
				Pos2=Pos1->end();
				while(delta)
					--Pos2,--delta;
				break;
			}
			delta-=Pos1->size();
			--Pos1;
		}while(Pos1!=Point->List.begin());
		return *this;
	}

	slist_iterator operator-(size_t delta) {
		slist_iterator it(*this);
		it-=delta;
		return it;
	}

	reference operator*() {
		return *Pos2;
	}
};

template<typename Ty>
class slist {
	template<typename Ty>
	friend class slist_iterator;
public:
	using iterator = slist_iterator<Ty> ;
private:
	typedef typename std::list<std::list<Ty>>::iterator iterator1;
	typedef typename std::list<Ty>::iterator iterator2;

	using value_type = Ty ;
	using referece = value_type& ;

	std::list<std::list<Ty>>List;
	size_t Size;

	void psplit(iterator1 pos) {
		//大于等于2*sqrt(Size)的进行分裂
		size_t NowSize = pos->size();
		if (NowSize * NowSize < 2 * Size) return;

		int BlockSize=sqrt(2*Size)/2+1;
		int BloCnt=NowSize/BlockSize;

		while (BloCnt > 1) {
			std::list<Ty>*nxt=new std::list<Ty>;
			iterator2 First=pos->end(),Last=First;
			size_t mid=0;
			while(mid<BlockSize)--First,++mid;
			nxt->splice(nxt->end(),*pos,First,Last);
			--BloCnt;
			iterator1 PosNxt=pos;
			++PosNxt;
			List.emplace(PosNxt,*nxt);
		}

	}

	void nsplit(const iterator1& pos) {
		//大于等于2*sqrt(Size)的进行分裂
		std::list<Ty>* point = &*pos;
		size_t NowSize = point->size();
		if (NowSize * NowSize < 2 * Size) return;

		int BlockSize = sqrt(2 * Size)/ 2+1;
		int BloCnt = NowSize / BlockSize;

		std::list<Ty>* pre;

		while (BloCnt > 1) {
			pre=new std::list<Ty>;
			iterator2 First = point->begin(), Last = First;
			size_t mid = 0;
			while (mid<BlockSize)++Last, ++mid;
			pre->splice(pre->end(), *point, First, Last);
			--BloCnt;
			List.emplace(pos, *pre);
		}
	}

	void pmerge(const iterator1& pos) {
		//当前块和后一个块均小于sqrt(Size)/2即可合并
		iterator1 npos = pos;
		++npos;
		if (npos == List.end())return;
		std::list<Ty>* point = &*pos, * nxt = &*npos;
		size_t NowSize = point->size(), NxtSize = nxt->size();
		if (2 * NowSize * NowSize > Size || 2 * NxtSize * NxtSize > Size)return;

		point->splice(point->end(), *nxt);

		List.erase(npos);
		pmerge(pos);
	}

	void nmerge(const iterator1& pos) {
		//当前块和前一个块均小于sqrt(Size)/2即可合并
		iterator1 ppos = pos;
		if (pos == List.begin())return;
		--ppos;
		std::list<Ty>* point = &*pos, * pre = &*ppos;
		size_t NowSize = point->size(), NxtSize = pre->size();
		if (2 * NowSize * NowSize > Size || 2 * NxtSize * NxtSize > Size)return;

		point->splice(point->end(), *pre);

		List.erase(ppos);
		nmerge(pos);
	}

	void initial() {
		Size=0;
		std::list<Ty>* Extend = new std::list<Ty>;
		List.emplace_back(*Extend);
		iterator1 it = List.end();
		--it;
	}

	void ierase(const iterator1& Pos) {
		if(!Pos->empty()||!Size)return ;
		List.erase(Pos);
	}
public:

	slist() {
		initial();
	}

	slist(size_t Count) {
		initial();
		while(Count)push_back(Ty()),--Count;
		maintain();
	}

	slist(size_t Count, const Ty& Val) {
		initial();
		while(Count)push_back(Val),--Count;
		maintain();
	}

	slist(const std::initializer_list<Ty>&ini) {
		initial();
		for(auto i : ini)
			push_back(i);
		maintain();
	}

	slist(slist& other) {
		initial();
		for(auto i : other)
			push_back(i);
		maintain();
	}

	slist& operator = (slist& other) {
		if(this==&other)return *this;
		List.clear();
		initial();
		for(auto i : other)
			push_back(i);
		maintain();
		return *this;
	}

	void push_back(const Ty& Val) {
		insert(end(),Val);
	}

	void push_front(const Ty& Val) {
		insert(begin(),Val);
	}

	void pop_back() {
		erase(--end());
	}

	void pop_front() {
		erase(begin());
	}

	void insert(iterator Pos, const Ty& Val) {
		Pos.Pos1->insert(Pos.Pos2,Val);
		++Size;
	}

	void erase(iterator Pos) {
		Pos.Pos1->erase(Pos.Pos2);
		--Size;
		ierase(Pos.Pos1);
	}

	iterator begin() {
		iterator1 Pos=List.begin();
		return iterator(this,Pos,Pos->begin());
	}

	iterator end() {
		iterator1 Pos=List.end();
		--Pos;
		return iterator(this,Pos,Pos->end());
	}

	referece back() {
		return *--end();
	}

	referece front() {
		return *begin();
	}

	size_t size(){return Size;}

	void maintain() {
		std::list<Ty>*Now=new std::list<Ty>;
		size_t bloSize=std::sqrt(Size),NowSize=0;
		std::list<std::list<Ty>>*NewList=new std::list<std::list<Ty>>;
		
		iterator1 Pos1=List.begin();
		iterator2 Pos2=Pos1->begin();

		for (size_t i = 0; i < Size; ++i) {
			Now->emplace_back(*Pos2);
			++NowSize;
			if (NowSize == bloSize) {
				NewList->emplace_back(*Now);
				Now=new std::list<Ty>;
				NowSize=0;
			}
			if (++Pos2 == Pos1->end()) {
				++Pos1;
				if(i<Size-1)Pos2=Pos1->begin();
			}
		}
		if (NowSize) {
			NewList->emplace_back(*Now);
			NowSize = 0;
		}
		List.swap(*NewList);
		NewList->clear();
	}

	void debug() {
		for(auto i :List)
			printf("%d ",i.size());
		printf("\n");
	}

};

#endif