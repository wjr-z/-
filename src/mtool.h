#ifndef MTOOL_H
/**
 * 工具类，包括高精度时间封装
 * 函数用时、容器元素比对
 * 文件输入、输出封装等
 */
#define MTOOL_H

#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include <thread>
#include <bitset>
#include <vector>

namespace Math {

	template<typename... _Type>
	class in_type;

	template<typename _Type1, typename _Type2>
	class in_type<_Type1, _Type2> {
	public:
		static constexpr bool value = std::is_same<_Type1, _Type2>::value;
	};

	template<typename _To_Be_Checked, typename _Val, typename..._Res>
	class in_type<_To_Be_Checked, _Val, _Res...> {
	public:
		using _Base = in_type<_To_Be_Checked, _Res...>;
		static constexpr bool value = std::is_same<_To_Be_Checked, _Val>::value || _Base::value;
	};


	template<typename _Type1, typename _Type2, typename... _Res>
	class out_type {
	public:
		static constexpr bool value = !in_type<_Type1, _Type2, _Res...>::value;
	};

	class time_ref;

	time_ref GetTime();
	double operator-(const time_ref& lhs, const time_ref& rhs);

	class time_ref {
	private:
		typedef std::chrono::high_resolution_clock m_clock;
		typedef std::chrono::microseconds m_mis;

		m_clock::time_point TimePoint;

		friend time_ref GetTime();
		static m_clock::time_point NowTime();
		static double testTime(const m_clock::time_point& start, const m_clock::time_point& end);
	public:
		time_ref(const m_clock::time_point&TimePoint);
		friend double operator-(const time_ref& lhs, const time_ref& rhs);
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
		size_t n = std::distance(left_begin,left_end);
		size_t m = std::distance(right_begin,right_end);
		if(n!=m)return false;
		while(left_begin!=left_end&&right_begin!=right_end) {
			if(*left_begin!=*right_begin)return false;
			++left_begin;
			++right_begin;
		}
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

		std::bitset<n> vis;
		//vis.relength(n);

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
}
#endif