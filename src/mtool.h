#ifndef MTOOL_H
#define MTOOL_H


#include <utility>
#include <chrono>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include "bint.h"

namespace Math {

	class timereference;

	timereference getTime();
	class timereference {
	private:
		typedef std::chrono::high_resolution_clock mclock;
		typedef std::chrono::microseconds mmis;

		mclock::time_point TimePoint;

		friend timereference getTime();
		static mclock::time_point NowTime();
		static double testTime(const mclock::time_point& start, const mclock::time_point& end);
	public:
		timereference(mclock::time_point TimePoint):TimePoint(TimePoint){}
		friend double operator-(const timereference& lhs, const timereference& rhs) {
			return testTime(rhs.TimePoint,lhs.TimePoint);
		}
	};

	template<typename Fn>
	double qtime(const Fn& ToBeTest) {
		auto start=getTime();
		ToBeTest();
		return getTime()-start;
	}

	template<typename Fn, typename... Args>
	double qtime(const Fn& ToBeTest, Args&&...List) {
		auto start=getTime();
		ToBeTest(std::forward<Args>(List)...);
		return getTime()-start;
	}

	template<typename Fn>
	double qavltime(const Fn& ToBeTest, int kth) {
		auto start=getTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest();
		return (getTime()-start) / kth;
	}

	template<typename Fn, typename... Args>
	double qavltime(const Fn& ToBeTest, int kth, Args&&...List) {
		auto start=getTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest(std::forward<Args>(List)...);
		return (getTime()-start) / kth;
	}

	template<typename Fn>
	double qcounttime(const Fn& ToBeTest, int kth) {
		auto start=getTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest();
		return getTime()-start;
	}

	template<typename Fn, typename... Args>
	double qcounttime(const Fn& ToBeTest, int kth, Args&&...List) {
		auto start=getTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest(std::forward<Args>(List)...);
		return getTime()-start;
	}

	//判断两个数组是否相同
	template<typename Ty>
	bool check(Ty* arr1,Ty*arr2,int n) {
		Ty*End1=arr1+n;
		while (arr1 < End1) {
			if(*arr1!=*arr2)return false;
			++arr1;
			++arr2;
		}return true;
	}

	template<typename Ty>
	bool check(const Ty&head,const Ty&nxt) {
		return head==nxt;
	}
	template<typename Ty,typename... Args>
	bool check(const Ty&head,const Ty&nxt,Args ...toBeTest) {
		if(head!=nxt)return false;
		return check(nxt,toBeTest...);
	}

	//避免数组内部交换时的拷贝
	template<typename Ty>
	void qswap(Ty* Start, Ty* End, int* rev) {
		int n = End - Start;

		bint2 vis;
		vis.relength(n);

		Ty* arr = Start;
		for (int i = 0; i < n; ++i) {
			if (vis[i] || i == rev[i])continue;
			if (rev[rev[i]] == i) {
				vis[i] = vis[rev[i]] = true;
				std::swap(arr[i], arr[rev[i]]);
				continue;
			}
			//环形交换
			Ty head = std::move(arr[i]);
			int j = i;
			vis[i] = true;
			while (!vis[rev[j]]) {
				arr[j] = std::move(arr[rev[j]]);
				j = rev[j];
				vis[j] = true;
			}
			arr[j] = std::move(head);
		}
	}
	template<typename Ty>
	void qswap(Ty* Start, Ty* End, std::initializer_list<int> rev) {
		int n = rev.size();
		if(n!=End-Start)return ;
		int*copyrev=new int[n],i=0;
		for(auto it=rev.begin(),End=rev.end();it!=End;++it)
			copyrev[i++]=*it;
		qswap(Start,End,copyrev);
	}

	//暂且只支持数值排序
	void bucketsort(uint32_t* Start, uint32_t* End);

	void getFiles(const std::string& path, std::vector<std::string>& filePath);

	std::vector<std::string> getFiles(const std::string& path);

	template<typename Ty>
	void printAll(Ty Start, Ty End) {
		while(Start<End)
			std::cout<<*Start<<std::endl,++Start;
	}

	template<typename Ty>
	void printAll(Ty inSTL) {
		printAll(inSTL.begin(),inSTL.end());
	}

}
#endif