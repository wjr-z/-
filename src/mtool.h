#ifndef MTOOL_H
#define MTOOL_H

#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include <thread>

#include "bint.h"

namespace Math {

	class timereference;

	timereference GetTime();
	double operator-(const timereference& lhs, const timereference& rhs);
	class timereference {
	private:
		typedef std::chrono::high_resolution_clock mclock;
		typedef std::chrono::microseconds mmis;

		mclock::time_point TimePoint;

		friend timereference GetTime();
		static mclock::time_point NowTime();
		static double testTime(const mclock::time_point& start, const mclock::time_point& end);
	public:
		timereference(mclock::time_point TimePoint);
		friend double operator-(const timereference& lhs, const timereference& rhs);
	};

	template<typename Fn>
	double qtime(const Fn& ToBeTest) {
		auto start=GetTime();
		ToBeTest();
		return GetTime()-start;
	}

	template<typename Fn, typename... Args>
	double qtime(const Fn& ToBeTest, Args&&...List) {
		auto start=GetTime();
		ToBeTest(std::forward<Args>(List)...);
		return GetTime()-start;
	}

	template<typename Fn>
	double qavltime(const Fn& ToBeTest, int kth) {
		auto start=GetTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest();
		return (GetTime()-start) / kth;
	}

	template<typename Fn, typename... Args>
	double qavltime(const Fn& ToBeTest, int kth, Args&&...List) {
		auto start=GetTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest(std::forward<Args>(List)...);
		return (GetTime()-start) / kth;
	}

	template<typename Fn>
	double qcounttime(const Fn& ToBeTest, int kth) {
		auto start=GetTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest();
		return GetTime()-start;
	}

	template<typename Fn, typename... Args>
	double qcounttime(const Fn& ToBeTest, int kth, Args&&...List) {
		auto start=GetTime();
		for (int i = 0; i < kth; ++i)
			ToBeTest(std::forward<Args>(List)...);
		return GetTime()-start;
	}

	template<typename Fn>
	class con {
	public:
		static std::map<Fn, std::vector<Fn>>head;
		static void connect(Fn fx1, Fn fx2) {
			head[fx1].push_back(fx2);
		}
	};

	template<typename Fn>
	std::map<Fn, std::vector<Fn>> con<Fn>::head;

	template<typename Fn>
	void connect(Fn fx1, Fn fx2) {
		con<Fn>::connect(fx1, fx2);
	}

	//函数调用树
	template<typename Fn, typename... Args>
	void crun(Fn fx, Args&&... List) {
		fx(std::forward<Args>(List)...);
		for (auto i : con<Fn>::head[fx])
			crun(i, std::forward<Args>(List)...);
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

	template<typename iter1,typename iter2>
	bool check(iter1 left_begin,iter1 left_end,iter2 right_begin,iter2 right_end) {
		while(left_begin!=left_end&&right_begin!=right_end) {
			if(*left_begin!=*right_begin)return false;
			++left_begin;
			++right_begin;
		}
		if(left_begin!=left_end||right_begin!=right_end)return false;
		return true;
	}

	template<typename iter1, typename iter2>
	std::string scheck(iter1 left_begin, iter1 left_end, iter2 right_begin, iter2 right_end,int ShowLimit=32) {
		std::string ERROR;
		int ERROR_SIZE=0;
		bool find_error=false;
		int pos=0;
		int Show=0;
		while (left_begin != left_end && right_begin != right_end) {
			if (*left_begin != *right_begin) {
				if(!find_error) {
					ERROR+="错误： 元素不等\n";
					ERROR+="元素不等位置\n";
				}
				if (Show < ShowLimit) {
					if (Show) {
						if ((Show & 15))ERROR += " ";
						else ERROR += '\n';
					}
					++Show;
					ERROR += std::to_string(pos);

				}
				find_error = true, ++ERROR_SIZE;
			}
			++left_begin;
			++right_begin;
			++pos;
		}
		if (find_error) {
			if(ERROR_SIZE>Show) {
				ERROR+="\n共有"+std::to_string(ERROR_SIZE)+"处错误，"+
					"已列举出前"+std::to_string(Show)+"处不等位置";
			}
			ERROR += '\n';
		}
		if (left_begin != left_end || right_begin != right_end) {
			if(!find_error)ERROR+="错误：";
			ERROR += "长度不等";
		}
		if(ERROR.empty())return "正确";
		
		return ERROR;
	}

	template<typename Ty>
	bool check(const Ty&head,const Ty&nxt) {
		return head==nxt;
	}
	template<typename Ty,typename... Args>
	bool check(const Ty&head,const Ty&nxt,Args...toBeTest) {
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
		for(auto it=rev.begin(),end=rev.end();it!=end;++it)
			copyrev[i++]=*it;
		qswap(Start,End,copyrev);
		delete copyrev;
	}

	//暂且只支持数值排序
	void bucketsort(uint32_t* Start, uint32_t* End);

	void getFiles(const std::string& path, std::vector<std::string>& filePath);

	std::vector<std::string> getFiles(const std::string& path);

	std::string readFiles(const std::string&filename);

	void writeFiles(const std::string&filename,const std::string&str);

	template<typename Ty>
	void thread_sort(const Ty First, const Ty Last) {
		//使用多线程进行排序
		auto head = First, tail = Last;
		size_t m_size = 0;
		while (head < tail) {
			++head;
			--tail;
			++m_size;
		}
		if (m_size <= 4000)
			return std::sort(First, Last);

		if (m_size <= 5000) {
			auto Mid = head;
			std::thread
				t1(std::sort<const Ty>, First, Mid),
				t2(std::sort<const Ty>, Mid, Last);

			t1.join();
			t2.join();
			std::inplace_merge(First, Mid, Last);
		}else {
			auto Mid=head;
			auto Mid1=Mid,Mid2=Mid;
			m_size>>=1;
			while(m_size--) {
				--Mid1;
				++Mid2;
			}
			std::thread
				t1(std::sort<const Ty>,First,Mid1),
				t2(std::sort<const Ty>,Mid1,Mid),
				t3(std::sort<const Ty>,Mid,Mid2),
				t4(std::sort<const Ty>,Mid2,Last);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			std::inplace_merge(First,Mid1,Mid);
			std::inplace_merge(Mid,Mid2,Last);
			std::inplace_merge(First,Mid,Last);
		}
	}

	template<typename Ty, typename _Pr>
	void thread_sort(const Ty First, const Ty Last, const _Pr _Pred = std::less<>()) {
		auto head = First, tail = Last;
		size_t m_size = 0;
		while (head < tail) {
			++head;
			--tail;
			++m_size;
		}
		if (m_size <= 10000)
			return std::sort(First, Last, _Pred);

		if (m_size <= 1000000) {
			auto Mid = head;
			std::thread
				t1(std::sort<const Ty, _Pr>, First, Mid, _Pred),
				t2(std::sort<const Ty, _Pr>, Mid, Last, _Pred);

			t1.join();
			t2.join();
			std::inplace_merge(First, Mid, Last);
		}
		else {
			auto Mid = head;
			auto Mid1 = Mid, Mid2 = Mid;
			m_size >>= 1;
			while (m_size--) {
				--Mid1;
				++Mid2;
			}
			std::thread
				t1(std::sort<const Ty, _Pr>, First, Mid1, _Pred),
				t2(std::sort<const Ty, _Pr>, Mid1, Mid, _Pred),
				t3(std::sort<const Ty, _Pr>, Mid, Mid2, _Pred),
				t4(std::sort<const Ty, _Pr>, Mid2, Last, _Pred);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			std::inplace_merge(First, Mid1, Mid);
			std::inplace_merge(Mid, Mid2, Last);
			std::inplace_merge(First, Mid, Last);
		}
	}
}
#endif