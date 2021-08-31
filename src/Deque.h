#ifndef DEQUE_H
/**
 * 暂未使用循环数组
 * 而是将数组尽可能置于中间，前后预留空间
 * 可能会占用更多的内存，但效率会更高
 */
#define DEQUE_H

#include <algorithm>
#include <iterator>
#include <memory>

namespace Math {

	//双栈模拟双端队列
	//快于deque

	template<typename Ty,typename Alloc=std::allocator<Ty>>
	class Deque;

	template<typename Ty, typename Alloc = std::allocator<Ty>>
	class Deque_const_iterator
		: public std::_Iterator_base0 {
	
	public:
		using value_type	  = Ty;
		using reference		  = Ty&;
		using const_reference = const Ty&;
		using const_pointer   = const Ty*;
		using difference_type = size_t;

		Deque_const_iterator(const_pointer Pos)
			:Pos(Pos) {

		}
		Deque_const_iterator(const Deque_const_iterator& other)
			:Pos(other.Pos) {

		}

		~Deque_const_iterator() = default;

		bool operator==(const Deque_const_iterator& other) {
			return Pos == other.Pos;
		}

		bool operator!=(const Deque_const_iterator& other) {
			return Pos != other.Pos;
		}

		Deque_const_iterator& operator=(const Deque_const_iterator& other) {
			Pos = other.Pos;
			return *this;
		}

		Deque_const_iterator& operator+=(const difference_type index) {
			Pos += index;
			return *this;
		}

		Deque_const_iterator& operator-=(const difference_type index) {
			Pos -= index;
			return *this;
		}

		Deque_const_iterator operator+(const difference_type index) {
			return Deque_const_iterator(Pos + index);
		}

		Deque_const_iterator operator-(const difference_type index) {
			return Deque_const_iterator(Pos - index);
		}

		Deque_const_iterator& operator++() {
			++Pos;
			return *this;
		}

		Deque_const_iterator& operator--() {
			--Pos;
			return *this;
		}

		Deque_const_iterator operator++(int) {
			Deque_const_iterator ans(*this);
			++(*this);
			return ans;
		}

		Deque_const_iterator operator--(int) {
			Deque_const_iterator ans(*this);
			--(*this);
			return ans;
		}

		difference_type operator-(const Deque_const_iterator& other) {
			return Pos - other.Pos;
		}

		const_reference operator*() const {
			return *Pos;
		}

		const_pointer operator->()const {
			return Pos;
		}

		const_pointer Pos;
	};

	template<typename Ty, typename Alloc = std::allocator<Ty>>
	class Deque_iterator
		: public std::_Iterator_base0 {
	public:
		
		using value_type	  = Ty;
		using reference		  = Ty&;
		using const_reference = const Ty&;
		using pointer		  = Ty*;
		using difference_type = size_t;

		Deque_iterator(pointer Pos)
			:Pos(Pos) {
			
		}
		Deque_iterator(const Deque_iterator&other)
			:Pos(other.Pos) {
			
		}

		Deque_iterator(const Deque_const_iterator<Ty>&other)
			:Pos(const_cast<pointer>(other.Pos)) {
			
		}
		
		~Deque_iterator()=default;

		bool operator==(const Deque_iterator&other) {
			return Pos==other.Pos;
		}

		bool operator!=(const Deque_iterator&other) {
			return Pos!=other.Pos;
		}

		Deque_iterator& operator=(const Deque_iterator&other) {
			Pos=other.Pos;
			return *this;
		}

		Deque_iterator& operator+=(const difference_type index) {
			Pos+=index;
			return *this;
		}

		Deque_iterator& operator-=(const difference_type index) {
			Pos-=index;
			return *this;
		}

		Deque_iterator operator+(const difference_type index) {
			return Deque_iterator(Pos+index);
		}

		Deque_iterator operator-(const difference_type index) {
			return Deque_iterator(Pos-index);
		}

		Deque_iterator& operator++() {
			++Pos;
			return *this;
		}

		Deque_iterator& operator--() {
			--Pos;
			return *this;
		}

		Deque_iterator operator++(int) {
			Deque_iterator ans(*this);
			++(*this);
			return ans;
		}

		Deque_iterator operator--(int) {
			Deque_iterator ans(*this);
			--(*this);
			return ans;
		}

		difference_type operator-(const Deque_iterator&other) {
			return Pos-other.Pos;
		}

		reference operator*() {
			return *Pos;
		}

		pointer operator->() {
			return Pos;
		}
		
		pointer Pos;
	};

	template <typename Ty,typename Alloc>
	class Deque {
		template<typename ty,typename alloc>
		friend class Deque_iterator;
	public:
		
		using value_type      = Ty;
		using reference       = value_type&;
		using const_reference = const value_type&;
		using pointer		  = Ty*;
		using size_type       = std::size_t;
		using iterator        = Deque_iterator<Ty,Alloc>;
		using const_iterator  = Deque_const_iterator<Ty,Alloc>;
	
	private:
		
		pointer _Begin,_End;
		pointer head,tail;

		void _Destroy(pointer _First, pointer _Last) {
			std::_Destroy_range(_First, _Last, Getal());
		}

		void tidy() {
			_Destroy(head, tail);
			Getal().deallocate(_Begin, capacity());
		}

		void _Ufill(pointer _Where,const size_type _Count,const Ty&Val) {
			std::_Uninitialized_fill_n(_Where,_Count,Val,Getal());
		}

		template<typename iter>
		void _Ucopy(iter _First,iter _Last,pointer _Dest) {
			std::_Uninitialized_copy(_First,_Last,_Dest,Getal());
		}

		template<typename iter>
		void _Umove(iter _First, iter _Last, pointer _Dest) {
			std::_Uninitialized_move(_First,_Last,_Dest,Getal());
		}

		void _Umove(pointer _First,pointer _Last,pointer _Dest) {
			if(_Dest<_First||_Dest>_Last) {
				std::_Uninitialized_move(_First,_Last,_Dest,Getal());
				return ;
			}
			std::_Uninitialized_move(_Dest, _Last, _Dest + (_Dest - _First), Getal());
			std::_Uninitialized_move(_First, _Dest, _Dest, Getal());
		}

		size_type _Calcalute_Growth(const size_type _Newsize) {
			const size_type _Oldcapacity = capacity();
			const size_type _Geometric = _Oldcapacity << 1; 
			if (_Geometric < _Newsize)
				return _Newsize;
			return _Geometric;
		}

		void _Resize_Reallocate(const size_type _Newsize,const bool type = true) {

			//不修改实际size，预留_Newsize，将其尽可能置于中间

			//type 为 true 代表后面新扩展 _Newsize-size()个元素
			//否则代表前面扩展_Newsize-size()个元素

			const size_type _Oldcapacity = capacity();
			auto al = Getal();
			const size_type _Size = size();

			//当前的size()小于一半容量时原地拷贝
			//下次次原地拷贝需要至少capacity()/2次操作，均摊O(1)
			if((_Size<<1)<_Oldcapacity&&_Newsize < _Oldcapacity) {
				pointer _Copyhead;
				if(type) {
					_Copyhead = _Begin + ((_Oldcapacity - _Newsize) >> 1);
				}else {
					_Copyhead = _Begin + ((_Oldcapacity - _Newsize) >> 1) + _Newsize - _Size;
				}
				const pointer _Copytail = _Copyhead + _Size;

				_Umove(head, tail, _Copyhead);
				_Destroy(head,tail);
				
				head = _Copyhead;
				tail = _Copytail;
				
				return ;
			}
			
			const size_type _Geometric = _Calcalute_Growth(_Newsize);
			
			const pointer _Newstk = al.allocate(_Geometric);

			pointer _Newhead;
			if (type) {
				_Newhead = _Newstk + ((_Geometric - _Newsize) >> 1);
			}
			else {
				_Newhead = _Newstk + ((_Geometric - _Newsize) >> 1) + _Newsize - _Size;
			}
			const pointer _Newtail = _Newhead + _Size;
			
			_Umove(head,tail,_Newhead);
			tidy();

			_Begin = _Newstk;
			_End = _Newstk + _Geometric;
			head = _Newhead;
			tail = _Newtail;
		}

		void _Reserve(const size_type _Newsize) {

			//预留空间，将原数组置于中间而非新数组

			if(_Newsize<=capacity())return ;

			const size_type _Geometric = _Calcalute_Growth(_Newsize);

			auto al = Getal();

			const pointer _Newstk = al.allocate(_Geometric);

			const size_type _Size = size();
			const pointer _Newhead = _Newstk + ((_Geometric - _Size) >> 1);
			const pointer _Newtail = _Newhead + _Size;

			_Umove(head, tail, _Newhead);
			tidy();

			_Begin = _Newstk;
			_End = _Newstk + _Geometric;
			head = _Newhead;
			tail = _Newtail;
		}

		void _Append_Move(pointer _First,pointer _Last,pointer _Dest) {
			_Umove(_First,_Last,_Dest);
			if(_Dest>tail) {
				_Ufill(tail,_Dest-tail,Ty());
			}
			const pointer _Dest_End=_Dest+(_Last-_First);
			if(_Dest_End<head) {
				_Ufill(_Dest_End,head-_Dest_End,Ty());
			}
		}

	public:
		
		Deque()
			:_Begin(nullptr),_End(nullptr),head(nullptr),tail(nullptr) {
			
		}

		~Deque() {
			tidy();
		}

		Deque(std::initializer_list<Ty> list)
			:_Begin(nullptr), _End(nullptr), head(nullptr), tail(nullptr) {
			assign(list.begin(),list.end());
		}

		Deque(const Deque&other)
			:_Begin(nullptr), _End(nullptr), head(nullptr), tail(nullptr) {
			assign(other.head,other.tail);
		}

		Deque(Deque&&other)noexcept
			:_Begin(other._Begin),_End(other._End),
			head(other.head),tail(other.tail) {
			other._Begin=other._End=other.head=other.tail=nullptr;
		}

		Deque(const size_type _Count,const Ty&Val=Ty())
			:_Begin(nullptr), _End(nullptr), head(nullptr), tail(nullptr) {
			insert(end(),_Count,Val);
		}

		Deque& operator=(const Deque&other) {
			if(this==&other)return *this;
			assign(other.head,other.tail);
			return *this;
		}

		Deque& operator=(Deque&&other) noexcept{
			_Begin=other._Begin;
			_End=other._End;
			head=other.head;
			tail=other.tail;
			other._Begin = other._End = other.head = other.tail = nullptr;
			return *this;
		}

		template<typename iter>
		void assign(iter _First,iter _Last) {
			size_type _Count = static_cast<size_type>(std::distance(_First,_Last));
			clear();
			if (capacity() < _Count)
				_Resize_Reallocate(_Count);

			head = _Begin + ((capacity() - _Count) >> 1);
			tail = head + _Count;
			_Ucopy(_First,_Last,head);
		}

		Alloc& Getal() {
			static Alloc Al;
			return Al;
		}

		size_type size()const noexcept{
			return tail-head;
		}

		size_type capacity()const {
			return _End-_Begin;
		}

		bool empty()const noexcept {
			return head==tail;
		}

		void clear() {
			_Destroy(head,tail);
			head = tail = _Begin + (capacity() >> 1);
		}

		void pop_front() {
			Getal().destroy(head);
			++head;
		}

		void pop_back() {
			--tail;
			Getal().destroy(tail);
		}

		void push_front(const Ty&Val) {
			if(head==_Begin)_Resize_Reallocate(size()+1,false);
			--head;
			Getal().construct(head,Val);
		}

		void push_back(const Ty&Val) {
			if(tail==_End)_Resize_Reallocate(size()+1);
			Getal().construct(tail,Val);
			++tail;
		}

		void resize(const size_type index) {
			const size_type _Size = size();
			if(index<_Size) {
				_Destroy(head+index,tail);
				tail = head + index;
			}
			if (index > _Size) {
				const size_type _Delta = index - size();
				if (tail + _Delta <= _End) {
					_Ufill(tail, _Delta, Ty());
					return;
				}
				_Resize_Reallocate(index);
				_Ufill(tail, _Delta, Ty());
				tail += _Delta;
			}
		}

		void reserve(const size_type index) {
			_Reserve(index);
		}

		void back_reserve(const size_type index) {
			if(head +index <= _End)
				return ;
			const size_type _Newsize = size() + index - (_End-head);
			_Resize_Reallocate(_Newsize);
		}

		void front_reserve(const size_type index) {
			if(tail>=_Begin+index)
				return ;
			const size_type _Newsize = size() + index - (tail-_Begin);
			_Resize_Reallocate(_Newsize,false);
		}

		reference operator[](const size_type index) {
			return *(head+index);
		}

		const_reference operator[](const size_type index)const {
			return *(head+index);
		}

		reference front() noexcept{
			return *head;
		}

		const_reference front() const noexcept {
			return *head;
		}

		reference back() {
			return *tail;
		}

		const_reference back()const noexcept {
			return *tail;
		}

		iterator begin() {
			return iterator(head);
		}

		const_iterator begin()const {
			return const_iterator(head);
		}

		iterator end() {
			return iterator(tail);
		}

		const_iterator end()const {
			return const_iterator(tail);
		}

		void swap(Deque&other) noexcept {
			std::swap(_Begin,other._Begin);
			std::swap(_End,other._End);
			std::swap(head,other.head);
			std::swap(tail,other.tail);
		}

		template<typename iter, std::enable_if_t<std::_Is_iterator_v<iter>, int> = 0>
		void insert(iterator _Where,iter _First,iter _Last) {
			auto _Count = static_cast<size_type>(std::distance(_First,_Last));
			const size_type _Possize=_Where.Pos-head;

			const auto _Move_Presize = _Possize;
			const auto _Move_Sufsize = size() - _Move_Presize;

			const auto _Oldcapacity = capacity();

			if ((_Begin + _Count > head || ((_Move_Presize << 1) > _Oldcapacity)) &&
				(tail + _Count > _End || ((_Move_Sufsize << 1) > _Oldcapacity))) {
				_Resize_Reallocate(size() + _Count);
			}

			const pointer _Wpos = head + _Possize;
			const auto _Presize = head - _Begin;
			const auto _Sufsize = _End - tail;
			if (_Move_Presize < _Move_Sufsize && _Presize >= _Count) {
				_Append_Move(head,_Wpos,head-_Count);
				_Ucopy( _First, _Last, _Wpos - _Count);
				head -= _Count;
			}
			else {
				_Append_Move(_Wpos,tail,_Wpos+_Count);
				_Ucopy(_First, _Last,_Wpos);
				tail += _Count;
			}
		}

		void insert(iterator _Where, const Ty& Val) {
			const size_type _Count = 1;
			const size_type _Possize = _Where.Pos - head;

			const auto _Move_Presize = _Possize;
			const auto _Move_Sufsize = size() - _Move_Presize;

			const auto _Oldcapacity = capacity();

			if ((_Begin + _Count > head || ((_Move_Presize << 1) > _Oldcapacity)) &&
				(tail + _Count > _End || ((_Move_Sufsize << 1) > _Oldcapacity))) {
				_Resize_Reallocate(size() + _Count);
			}

			const pointer _Wpos = head + _Possize;
			const auto _Presize = head - _Begin;
			const auto _Sufsize = _End - tail;
			if (_Move_Presize < _Move_Sufsize && _Presize >= _Count) {
				_Append_Move(head, _Wpos, head - _Count);
				*(_Wpos - _Count) = Val;
				head -= _Count;
			}
			else {
				_Append_Move(_Wpos, tail, _Wpos + _Count);
				*_Wpos = Val;
				tail += _Count;
			}
		}

		void insert(iterator _Where, Ty&& Val) {
			const size_type _Count = 1;
			const size_type _Possize = _Where.Pos - head;

			const auto _Move_Presize = _Possize;
			const auto _Move_Sufsize = size() - _Move_Presize;

			const auto _Oldcapacity = capacity();

			if ((_Begin + _Count > head || ((_Move_Presize << 1) > _Oldcapacity)) &&
				(tail + _Count > _End || ((_Move_Sufsize << 1) > _Oldcapacity))) {
				_Resize_Reallocate(size() + _Count);
			}

			const pointer _Wpos = head + _Possize;
			const auto _Presize = head - _Begin;
			const auto _Sufsize = _End - tail;
			if (_Move_Presize < _Move_Sufsize && _Presize >= _Count) {
				_Append_Move(head, _Wpos, head - _Count);
				*(_Wpos - _Count) = std::move(Val);
				head -= _Count;
			}
			else {
				_Append_Move(_Wpos, tail, _Wpos + _Count);
				*_Wpos = std::move(Val);
				tail += _Count;
			}
		}

		void insert(iterator _Where,const size_type _Count,const Ty&Val) {
			const size_type _Possize = _Where.Pos - head;

			const auto _Move_Presize = _Possize;
			const auto _Move_Sufsize = size() - _Move_Presize;

			const auto _Oldcapacity = capacity();

			if ((_Begin + _Count > head || ((_Move_Presize << 1) > _Oldcapacity)) &&
				(tail + _Count > _End || ((_Move_Sufsize << 1) > _Oldcapacity))) {
				_Resize_Reallocate(size() + _Count);
			}

			const pointer _Wpos = head + _Possize;
			const auto _Presize = head - _Begin;
			const auto _Sufsize = _End - tail;
			if (_Move_Presize < _Move_Sufsize && _Presize >= _Count) {
				_Append_Move(head, _Wpos, head - _Count);
				_Ufill(_Wpos-_Count,_Count,Val);
				head -= _Count;
			}
			else {
				_Append_Move(_Wpos, tail, _Wpos + _Count);
				_Ufill(_Wpos,_Count,Val);
				tail += _Count;
			}
		}

		void erase(iterator _First,iterator _Last) {
			const size_type _Count = _Last - _First;
			const pointer _Pos1 = _First.Pos;
			const pointer _Pos2 = _Last.Pos;
			const auto _Move_Presize = _Pos1 - head;
			const auto _Move_Sufsize = tail - _Pos2;
			if(_Move_Presize<=_Move_Sufsize) {
				_Umove(head,_Pos1,head+_Count);
				_Destroy(head,head+_Count);
				head+=_Count;
			}else {
				_Umove(_Pos2,tail,_Pos1);
				_Destroy(tail-_Count,tail);
				tail-=_Count;
			}
		}

		iterator erase(iterator _Pos) {
			iterator _Nxt=_Pos;
			++_Nxt;
			const size_type _Count = 1;
			const pointer _Pos1 = _Pos.Pos;
			const pointer _Pos2 = _Pos1 + 1;
			const auto _Move_Presize = _Pos1 - head;
			const auto _Move_Sufsize = tail - _Pos2;
			if (_Move_Presize <= _Move_Sufsize) {
				_Umove(head, _Pos1, head + _Count);
				_Destroy(head, head + _Count);
				head += _Count;
			}
			else {
				_Umove(_Pos2, tail, _Pos1);
				_Destroy(tail - _Count, tail);
				tail -= _Count;
			}
			return _Nxt;
		}

	};

}
#endif