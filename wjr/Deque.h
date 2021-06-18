#pragma once
#ifndef DEQUE_H
#define DEQUE_H

#define _MATH_BEGIN namespace Math{
#define _MATH_END }

_MATH_BEGIN

//双栈模拟双端队列
//快于deque

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
	void push_back(const Ty& _Val) {
		tail.push_back(_Val);
	}
	void push_front(const Ty& _Val) {
		head.push_back(_Val);
	}
	void pop_back() {
		if (!tail.empty()) {
			tail.pop_back();
			return;
		}
		int MidSize = (head.size() + 1) >> 1;
		tail.assign(head.begin(), head.begin() + MidSize);
		head.assign(head.begin() + MidSize, head.end());
		std::reverse(tail.begin(), tail.end());
		tail.pop_back();
	}
	void pop_front() {
		if (!head.empty()) {
			head.pop_back();
			return;
		}
		int MidSize = (tail.size() + 1) >> 1;
		head.assign(tail.begin(), tail.begin() + MidSize);
		tail.assign(tail.begin() + MidSize, tail.end());
		std::reverse(head.begin(), head.end());
		head.pop_back();
	}
	Ty& operator[](const size_t& index) {
		return index < head.size() ? head[head.size() - index - 1] : tail[index - head.size()];
	}
	bool empty() { return !head.size() && !tail.size(); }
	size_t size() { return head.size() + tail.size(); }
	void clear() { head.clear(); tail.clear(); }
	void erase(const int& index) {
		if (index < head.size()) {
			head.erase(head.end() - (index + 1));
		}
		else {
			tail.erase(tail.begin() + (index - head.size()));
		}
	}
	void insert(const int& index, const Ty& _Val) {
		if (index < head.size()) {
			head.insert(head.begin() + (head.size() - index), _Val);
		}
		else {
			tail.insert(tail.begin() + (index - head.size()), _Val);
		}
	}
};

_MATH_END

#endif DEQUE_H