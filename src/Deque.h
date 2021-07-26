#ifndef DEQUE_H
#define DEQUE_H

#include <vector>

namespace Math {

	using std::vector;

	//双栈模拟双端队列
	//快于deque
	//暂未使用迭代器
	template<typename Ty>
	class Deque {
	private:
		std::vector<Ty>head;
		std::vector<Ty>tail;
	public:
		Deque() {

		}
		~Deque() {

		}
		void push_back(const Ty& _Val);
		void push_front(const Ty& _Val);
		void pop_back();
		void pop_front();
		Ty& operator[](const size_t& index) {
			return index < head.size() ? head[head.size() - index - 1] : tail[index - head.size()];
		}
		bool empty() { return !head.size() && !tail.size(); }
		size_t size() { return head.size() + tail.size(); }
		void clear() { head.clear(); tail.clear(); }
		void erase(const size_t& index) {
			if (index < head.size())
				head.erase(head.end() - (index + 1));
			else
				tail.erase(tail.begin() + (index - head.size()));
		}
		void insert(const size_t& index, const Ty& _Val) {
			if (index < head.size())
				head.insert(head.begin() + (head.size() - index), _Val);
			else
				tail.insert(tail.begin() + (index - head.size()), _Val);
		}
	};

	template<typename _Ty>
	void Deque<_Ty>::push_back(const _Ty& _Val) {
		tail.push_back(_Val);
	}
	template<typename _Ty>
	void Deque<_Ty>::push_front(const _Ty& _Val) {
		head.push_back(_Val);
	}
	template<typename _Ty>
	void Deque<_Ty>::pop_back() {
		if (!tail.empty()) {
			tail.pop_back();
			return;
		}
		size_t MidSize = (head.size() + 1) >> 1;
		tail.assign(head.begin(), head.begin() + MidSize);
		head.assign(head.begin() + MidSize, head.end());
		std::reverse(tail.begin(), tail.end());
		tail.pop_back();
	}
	template<typename _Ty>
	void Deque<_Ty>::pop_front() {
		if (!head.empty()) {
			head.pop_back();
			return;
		}
		size_t MidSize = (tail.size() + 1) >> 1;
		head.assign(tail.begin(), tail.begin() + MidSize);
		tail.assign(tail.begin() + MidSize, tail.end());
		std::reverse(head.begin(), head.end());
		head.pop_back();
	}
}

#endif DEQUE_H