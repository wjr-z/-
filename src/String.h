#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <string.h>

namespace Math {

	template<typename Char,typename Alloc = std::allocator<Char>>
	class String_Core {
	public:
		String_Core(){
			reset();
		}

		String_Core(const String_Core& other) {
			if (!other.category()) {
				copySmall(other);
			}
			else {
				copyMedium(other);
			}
		}

		String_Core(String_Core&& other)noexcept {
			_Ml = other._Ml;
			other.reset();
		}

		String_Core(const Char* _Data, const size_t s) {
			if(s<=maxSmallSize)
				initSmall(_Data,s);
			else initMedium(_Data,s);
		}

		String_Core& operator=(const String_Core&) = delete;

		~String_Core()noexcept {
			if (!category())return;
			getAl().deallocate(_Ml._Data,_Ml.capacity() + 1);
		}

		void swap(String_Core& other) {
			std::swap(_Ml,other._Ml);
		}

		Char* data() {
			return !category() ? _Small : _Ml._Data;
		}

		const Char* data()const {
			return !category() ? _Small : _Ml._Data;
		}

		const Char* c_str()const {
			return !category() ? _Small : _Ml._Data;
		}

		void shrink(const size_t delta) {
			if(!category())
				shrinkSmall(delta);
			else shrinkMedium(delta);
		}

		void reserve(const size_t s) {
			if(!category())
				reserveSmall(s);
			else reserveMedium(s);
		}

		Char* expandNoinit(const size_t delta, bool expGrowth = false) {
			size_t siz,newsiz;
			if (!category()) {
				siz = SmallSize();
				newsiz = siz + delta;
				if (newsiz <= maxSmallSize) {
					setSmallSize(newsiz);
					return _Small + siz;
				}
				reserveSmall(expGrowth?std::max(newsiz,2*maxSmallSize):newsiz);
			}
			else {
				siz = _Ml._Size;
				newsiz = siz + delta;
				const auto _Old_Capacity = _Ml.capacity();
				if (newsiz > _Old_Capacity) {
					reserveMedium(expGrowth ? std::max(newsiz, 1 + _Old_Capacity + _Old_Capacity / 2) : newsiz);
				}
			}
			setMediumSize(newsiz);
			return _Ml._Data + siz;
		}

		void push_back(char c){*expandNoinit(1,true)=c; }

		size_t size()const {
			return !category()?SmallSize():MediumSize();
		}

		size_t capacity()const {
			return !category()?maxSmallSize:_Ml.capacity();
		}

		static Alloc& getAl() {
			static Alloc _alloc;
			return _alloc;
		}

	private:
		Char* c_str() {
			return !category()?_Small:_Ml._Data;
		}

		struct _Medium {
			Char* _Data;
			size_t _Size;
			size_t _Capacity;
			size_t capacity()const {
				return _Capacity & capacityExtractMask;
			}
		};
		constexpr static size_t lastChar = sizeof(_Medium) - 1;
		constexpr static size_t maxSmallSize = lastChar / sizeof(Char);
		constexpr static uint8_t categoryExtractMask = 0x80;
		constexpr static size_t kShift = (sizeof(size_t)-1)*8;
		constexpr static size_t capacityExtractMask = ~((size_t)categoryExtractMask<<kShift);
		constexpr static size_t capacityOr = ~capacityExtractMask;

		union {
			uint8_t _Byte[sizeof(_Medium)];
			Char _Small[sizeof(_Medium)/sizeof(Char)];
			_Medium _Ml;
		};

		bool category()const {
			return (_Byte[lastChar] == categoryExtractMask);
		}

		void setCapacity(const size_t c) { 
			_Ml._Capacity = c | capacityOr;
		}

		void reset(){setSmallSize(0);}

		void setSmallSize(const size_t s) {
			_Small[maxSmallSize]=static_cast<uint8_t>(maxSmallSize-s);
			_Small[s]='\0';
		}

		void setMediumSize(const size_t s) {
			_Ml._Size = s;
			_Ml._Data[s]='\0';
		}

		size_t SmallSize()const {
			return maxSmallSize - _Small[maxSmallSize];
		}

		size_t MediumSize()const {
			return _Ml._Size;
		}

		void initSmall(const Char* _Data, const size_t s) {
			if ((reinterpret_cast<size_t>(_Data) & (sizeof(size_t) - 1)) == 0) {
				const size_t byteSize = s * sizeof(Char);
				constexpr size_t wordWidth = sizeof(size_t);
				switch ((byteSize + wordWidth - 1) / wordWidth) {
				case 3:
					_Ml._Capacity = reinterpret_cast<const size_t*>(_Data)[2];
				case 2:
					_Ml._Size = reinterpret_cast<const size_t*>(_Data)[1];
				case 1:
					_Ml._Data = *reinterpret_cast<Char**>(const_cast<Char*>(_Data));
				case 0:
					break;
				}
			}
			else {
				std::memcpy(_Small, _Data, sizeof(Char) * s);
			}
			setSmallSize(s);
		}

		void initMedium(const Char*_Data,const size_t s) {
			_Ml._Data = getAl().allocate(s+1);
			std::memcpy(_Ml._Data,_Data,sizeof(Char)*s);
			setMediumSize(s);
			setCapacity(s);
		}

		void copySmall(const String_Core& other) {
			_Ml = other._Ml;
		}

		void copyMedium(const String_Core& other) {
			const size_t s = other.size();
			_Ml._Data = getAl().allocate(s+1);
			std::memcpy(_Ml._Data,other._Ml._Data,sizeof(Char)*s);
			setMediumSize(s);
			setCapacity(s);
		}

		void shrinkSmall(const size_t delta) {
			setSmallSize(SmallSize()-delta);
		}

		void shrinkMedium(const size_t delta) {
			setMediumSize(MediumSize()-delta);
		}

		void reserveSmall(const size_t s) {
			if(s<=maxSmallSize)return ;
			const size_t _New_Capacity = s + 1;
			const auto _New_Data = getAl().allocate(_New_Capacity);
			const auto _Old_Size = SmallSize();
			std::memcpy(_New_Data,_Small,sizeof(Char)*_Old_Size);
			_Ml._Data=_New_Data;
			setMediumSize(_Old_Size);
			setCapacity(_New_Capacity);
		}

		void reserveMedium(const size_t s) {
			const auto _Old_Capacity = _Ml.capacity();
			if(s<= _Old_Capacity)
				return ;

			auto Al = getAl();
			const auto _New_Data = Al.allocate(s + 1);
			const auto _Old_Size = _Ml._Size;
			std::memcpy(_New_Data, _Ml._Data, sizeof(Char) * _Old_Size );
			Al.deallocate(_Ml._Data, _Old_Capacity + 1);
			_Ml._Data = _New_Data;
			setMediumSize(_Old_Size);
			setCapacity(s);
		}

	};

	template<typename Char,typename Alloc=std::allocator<Char>>
	class basic_String {
	public:

		using value_type      = Char;
		using reference       = Char&;
		using const_reference = const Char&;

		using size_type       = size_t;
		using difference_type = size_t;

		using pointer         = Char*;
		using const_pointer   = const Char*;

		using iterator        = Char*;
		using const_iterator  = const Char*;

		basic_String()noexcept{}

		basic_String(const basic_String& other)
			:_core(other._core) {

		}

		basic_String(basic_String&& other)
			:_core(std::move(other._core)) {

		}
		
		basic_String(const basic_String& other, const size_t pos, const size_t n) {
			assign(other,pos,n);
		}

		basic_String(const Char* s)
			:_core(s, std::strlen(s)) {

		}

		basic_String(const Char* s, const size_t n)
			:_core(s, n) {

		}

		basic_String(const size_t n, const Char c) {
			auto const pData = _core.expandNoinit(n);
			std::fill(pData,pData+n,c);
		}

		basic_String(const Char* _Begin, const Char* _End) 
			:_core(_Begin,_End-_Begin){

		}

		~basic_String()noexcept{}

		basic_String& operator=(const basic_String& other){
			if(this==&other)
				return *this;
			return assign(other.data(),other.size());
		}

		basic_String& operator=(basic_String&& other){
			_core.swap(other._core);
			other._core.reset();
			return *this;
		}

		basic_String& operator=(const Char c) {
			resize(1);
			*begin()=c;
			return *this;
		}

		iterator begin(){return _core.data(); }
		
		const_iterator begin()const{return _core.data();}

		const_iterator cbegin()const{return begin(); }

		iterator end(){return _core.data()+_core.size(); }

		const_iterator end()const{return _core.data()+_core.size(); }

		const_iterator cend()const{return end(); }

		const_reference front()const{return *begin(); }

		reference front(){return *begin(); }

		const_reference back()const{
			assert(!empty());
			return *(end()-1); 
		}

		reference back() {
			assert(!empty());
			return *(end()-1);
		}

		void pop_back() {
			assert(!empty());
			_core.shrink(1);
		}

		size_t size() const{
			return _core.size();
		}

		size_t length() const{
			return size();
		}

		void resize(const size_t n, const Char c = Char()) {
			const size_t sz = size();
			if (n <= sz) {
				_core.shrink(sz - n);
			}
			else {
				const auto delta = n - sz;
				auto pData = _core.expandNoinit(delta);
				std::fill(pData,pData+delta,c);
			}
			assert(size()==n);
		}

		size_t capacity() const {
			return _core.capacity();
		}

		void reserve(const size_t c) {
			_core.reserve(c);
		}

		void shrink_to_fit() {
			if (capacity() < size() * 3 / 2)
				return;
			basic_String(cbegin(), cend()).swap(*this);
		}

		void clear(){resize(0); }
		
		bool empty(){return size()==0; }

		Char& operator[](const size_t index){
			return *(begin()+index); 
		}

		const Char& operator[](const size_t index)const {
			return *(begin()+index);
		}

		basic_String& operator+=(const basic_String&other){return append(other); }

		basic_String& operator+=(const Char*s){return append(s); }

		basic_String& operator+=(const Char c){push_back(c);return *this; }

		basic_String& append(const basic_String& other) {
			return append(other.data(), other.size());
		}

		basic_String& append(
			const basic_String& other, const size_t pos, const size_t n) {
			return append(other.data()+pos,n);
		}

		basic_String& append(const Char* s, size_t n) {
			const auto oldSize = size();
			const auto oldData = data();
			const auto pData = _core.expandNoinit(n,true);
			if (oldData <= s && s < oldData + oldSize) {
				s = (pData - oldSize) + (s - oldData);
				std::memmove(pData,s,sizeof(Char)*n);
			}
			else {
				std::memcpy(pData,s,sizeof(Char)*n);
			}
			return *this;
		}

		basic_String& append(const Char* s) {
			return append(s,std::strlen(s));
		}

		basic_String& append(const size_t n, const Char c) {
			auto pData = _core.expandNoinit(n,true);
			std::fill(pData,pData+n,c);
			return *this; 
		}

		void push_back(const Char c) {
			_core.push_back(c);
		}

		basic_String& assign(const basic_String& other) {
			if(this==&other)
				return *this;
			assign(other.data(),other.size());
		}

		basic_String& assign(basic_String&& other) {
			*this=std::move(other);
			return *this;
		}

		basic_String& assign(const basic_String& other, const size_t pos, const size_t n) {
			return assign(other.data()+pos,n);
		}

		basic_String& assign(const Char* s, const size_t n) {
			if (!n) {
				resize(0);
			}
			else if (size() >= n) {
				std::memmove(_core.data(),s,sizeof(Char)*n);
				_core.shrink(size()-n);
			}
			else {
				resize(0);
				std::memcpy(_core.expandNoinit(n),s,sizeof(Char)*n);
			}
			return *this;
		}

		basic_String& assign(const Char* s) {
			assign(s,std::strlen(s));
			return *this;
		}

		basic_String& assign(const Char* _Begin, const Char* _End) {
			assign(_Begin,_End-_Begin);
			return *this;
		}

		void swap(basic_String&other){_core.swap(other._core); }

		const Char* c_str() const{return _core.c_str(); }

		const Char* data() const{return _core.data(); }

		Char* data(){return _core.data(); }

		basic_String substr(const size_t pos, const size_t n) {
			return basic_String(data()+pos,std::min(n,size()-pos));
		}

		friend std::ostream& operator<<(std::ostream& out, basic_String& x) {
			const auto str = x.data();
			const auto Length = x.length();
			for(size_t i=0;i<Length;++i)
				out<<str[i];
			return out;
		}

	private:
		String_Core<Char,Alloc> _core;
	};

	typedef basic_String<char> String;
}

#endif