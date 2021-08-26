#ifndef DEQUE_H
/**
 * 双栈模拟双端队列
 * 使用头尾vector实现deque
 */
#define DEQUE_H

#include <vector>
#include "Allocator.h"

namespace Math {

	//双栈模拟双端队列
	//快于deque

	template<typename Ty, typename Alloc = std::allocator<Ty>>
	class Deque;

	template<typename Ty,typename Alloc=std::allocator<Ty>>
	class Deque_iterator
		: public std::iterator<std::random_access_iterator_tag, int> {
		template<typename Ty,typename Alloc>
		friend class Deque;
	public:
		using value_type = Ty;
		using reference = Ty&;
		using const_reference = const Ty&;
		using size_type = size_t;
		
	private:
		using v_iterator = typename std::vector<Ty, Alloc>::iterator;
		
		std::vector<Ty,Alloc>* Get() {
			auto address=&*Pos;
			if(!point->head.empty()&&(address >= &point->head.front() && address <= &point->head.back()))
				return &point->head;
			return &point->tail;
		}
	
	public:
		
		Deque_iterator(Deque<Ty,Alloc>*point,v_iterator Pos)
			: point(point),Pos(Pos) {
			
		}
		
		Deque_iterator(const Deque_iterator&other)
			:point(other.point),Pos(other.Pos) {
			
		}
		
		Deque_iterator& operator=(const Deque_iterator&other) {
			point=other.point;
			Pos=other.Pos;
			return *this;
		}

		bool operator==(const Deque_iterator&other) {
			return Pos==other.Pos&&point==other.point;
		}

		bool operator!=(const Deque_iterator&other) {
			return Pos!=other.Pos||point!=other.point;
		}
		
		Deque_iterator& operator++() {
			auto G = Get();
			if (G == &point->head) {
				if (Pos != point->head.begin())
					--Pos;
				else Pos = point->tail.begin();
			}
			else ++Pos;
			return *this;
		}
		
		Deque_iterator operator++(int) {
			Deque_iterator wjr(*this);
			++(*this);
			return wjr;
		}
		
		Deque_iterator& operator--() {
			auto G = Get();
			if (G == &point->tail) {
				if (Pos != point->tail.begin())
					--Pos;
				else Pos = point->head.begin();
			}
			else ++Pos;
			return *this;
		}

		reference operator*() {
			return *Pos;
		}
	
	private:
		
		Deque<Ty,Alloc>*point;
		v_iterator Pos;
	};

	template<typename Ty, typename Alloc = std::allocator<Ty>>
	class Deque_const_iterator
		: public std::iterator<std::random_access_iterator_tag, int> {
		template<typename Ty, typename Alloc>
		friend class Deque;
	public:
		using value_type = Ty;
		using reference = Ty&;
		using const_reference = const Ty&;
		using size_type = size_t;

	private:
		using v_iterator = typename std::vector<Ty, Alloc>::const_iterator;

		const std::vector<Ty, Alloc>* Get()const {
			auto address = &*Pos;
			if (!point->head.empty() && (address >= &point->head.front() && address <= &point->head.back()))
				return &point->head;
			return &point->tail;
		}

	public:

		Deque_const_iterator(const Deque<Ty, Alloc>* point, v_iterator Pos)
			: point(point), Pos(Pos) {

		}

		Deque_const_iterator(const Deque_const_iterator& other)
			:point(other.point), Pos(other.Pos) {

		}

		Deque_const_iterator& operator=(const Deque_const_iterator& other) {
			point = other.point;
			Pos = other.Pos;
			return *this;
		}

		bool operator==(const Deque_const_iterator& other)const {
			return Pos == other.Pos && point == other.point;
		}

		bool operator!=(const Deque_const_iterator& other)const {
			return Pos != other.Pos || point != other.point;
		}

		Deque_const_iterator& operator++() {
			auto G = Get();
			if (G == &point->head) {
				if (Pos != point->head.begin())
					--Pos;
				else Pos = point->tail.begin();
			}
			else ++Pos;
			return *this;
		}

		Deque_const_iterator operator++(int) {
			Deque_const_iterator wjr(*this);
			++(*this);
			return wjr;
		}

		Deque_const_iterator& operator--() {
			auto G = Get();
			if (G == &point->tail) {
				if (Pos != point->tail.begin())
					--Pos;
				else Pos = point->head.begin();
			}
			else ++Pos;
			return *this;
		}

		const_reference operator*()const {
			return *Pos;
		}

	private:

		const Deque<Ty, Alloc>* point;
		v_iterator Pos;
	};
	
	template <typename Ty,typename Alloc>
	class Deque {
		template<typename Ty,typename Alloc>
		friend class Deque_iterator;
		template<typename Ty, typename Alloc>
		friend class Deque_const_iterator;
	public:
		
		using value_type      = Ty;
		using reference       = value_type&;
		using const_reference = const value_type&;
		using size_type       = std::size_t;
		using iterator        = Deque_iterator<Ty,Alloc>;
		using const_iterator  = Deque_const_iterator<Ty,Alloc>;
	
	private:

		std::vector<Ty, Alloc> head;
		std::vector<Ty, Alloc> tail;

		template<typename iter>
		void _Construct(iter _First,iter _Last) { 
			for(;_First!=_Last;++_First)
				push_back(*_First);
		}
	
	public:
		
		Deque()noexcept = default;
		~Deque() noexcept = default;

		Deque(std::initializer_list<Ty> v_list) {
			_Construct(v_list.begin(),v_list.end());
		}

		Deque(const Deque&other) {
			for(auto i :other)
				push_back(i);
		}

		template<typename iter>
		void assign(iter _First,iter _Last) {
			clear();
			for(;_First!=_Last;++_First)
				push_back(*_First);
		}

		void push_back(const Ty& val) {
			tail.push_back(val);
		}
		
		void push_front(const Ty& val) {
			head.push_back(val);
		}
		
		void pop_back() {
			if (!tail.empty()) {
				tail.pop_back();
				return;
			}
			const size_type MidSize = (head.size() + 1) >> 1;
			tail.assign(head.begin() + 1, head.begin() + MidSize);
			head.assign(head.begin() + MidSize, head.end());
			std::reverse(tail.begin(), tail.end());
		}
		
		void pop_front() {
			if (!head.empty()) {
				head.pop_back();
				return;
			}
			const size_type MidSize = (tail.size() + 1) >> 1;
			head.assign(tail.begin() + 1, tail.begin() + MidSize);
			tail.assign(tail.begin() + MidSize, tail.end());
			std::reverse(head.begin(), head.end());
		}

		reference operator[](size_type index) {
			return index < head.size() ? head[head.size() - index - 1] : tail[index - head.size()];
		}

		reference front() {
			return !head.empty()?head.back():tail.front();
		}

		reference back() {
			return !tail.empty()?tail.back():head.front();
		}

		iterator begin() {
			return iterator(this,head.empty()?tail.begin():--head.end());
		}

		const_iterator begin()const noexcept{
			return const_iterator(this, head.empty() ? tail.begin() : --head.end());
		}

		iterator end() {
			return iterator(this,tail.end());
		}

		const_iterator end() const noexcept{
			return const_iterator(this, tail.end());
		}

		bool empty() const{ return tail.empty() && head.empty(); }
		
		size_type size() const{ return head.size() + tail.size(); }

		void clear() {
			head.clear();
			tail.clear();
		}

		void erase(size_type index) {
			
			if (index < head.size())
				head.erase(head.end() - (index + 1));
			else
				tail.erase(tail.begin() + (index - head.size()));
		}

		void insert(size_type index, const Ty& val) {
			
			if (index < head.size())
				head.insert(head.begin() + (head.size() - index), val);
			else
				tail.insert(tail.begin() + (index - head.size()), val);
		}
	};

}

#endif
