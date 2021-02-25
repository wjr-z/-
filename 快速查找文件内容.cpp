#include <io.h>
#include <mutex>
#include <cstdio>
#include <string>
#include <time.h>
#include <vector>
#include <thread>
#include <cstdlib>
#include <istream>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <windows.h>

#pragma execution_character_set("utf-8")
using namespace std;

mutex mutex_number;

inline void end_work();

struct my_file {//读入的文件
	int pos, siz, len;
	char* str;
	int find_file=0,sleep_count=2000,sleep_time=50;//优化CPU占用率，用于多线程防止CPU占用过大
	const int MAXN = 1 << 22;//优化磁盘读写，防止一次性读入过大文件
	my_file() {
		len = 1;
		str = (char*)malloc(sizeof(char*));
	}
	void _sleep() {
		if (++find_file >= sleep_count) {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
			find_file=0;
		}
	}
	bool read_FILE(string& s, int MAX_SIZE) {//读入一个文件
		FILE* fp;
		errno_t err;
		err = fopen_s(&fp, s.c_str(), "rb");
		if (err) {
			siz = 0, pos = 0;
			return false;
		}
		fseek(fp, 0, SEEK_END);
		siz = ftell(fp);
		if (siz >= MAX_SIZE) {//限制查找文件的大小
			fclose(fp);
			return false;
		}
		rewind(fp);
		if (len < siz) {//自制简易vector
			while (len < siz) {
				len <<= 1;
			}
			free(str);
			str = (char*)malloc(sizeof(char*) * len);
		}
		if(siz<MAXN)fread(str, 1, siz, fp);
		else {//磁盘读写优化
			int num,top=0;
			while ((num = fread(str+top, 1, MAXN, fp)))top+=num;
		}
		pos = 0;
		fclose(fp);
		return true;
	}void getline(string& s) {
		while (str[pos] != '\r' && str[pos] != '\n' && pos < siz)
			s .push_back( str[pos++] );
		if (str[pos] == '\r')++pos;
		if (str[pos] == '\n')++pos;
	}void getall(string& s) {
		while (pos < siz)s .push_back( str[pos++] );
	}void get_index(int& x) {
		x = 0;
		while (str[pos] != '\r' && str[pos] != '\n' && pos < siz)
			x = (x << 1) + (x << 3) + str[pos++] - '0';
		if (str[pos] == '\r')++pos;
		if (str[pos] == '\n')++pos;
	}
}a, b;


struct wjr {//设置值
	string FILE_PATH,root_path,FILE_name,FILE_content;
	int MAX_SIZE;
	int is_thread;
	wjr() {//默认值
		root_path="";
		FILE_PATH="in.txt";
		MAX_SIZE=30000000;//限制搜索文件大小
		is_thread=0;
	}
	void read_setting(my_file&x) {//读入配置文件
		if (!x.read_FILE(setting.FILE_PATH, setting.MAX_SIZE)) {
			end_work();
		}
		string a;
		int n;
		while (x.pos < x.siz) {
			a = "";
			x.getline(a);
			n = a.length();
			while (n && (a[n - 1] == ' ' || a[n - 1] == ':'))--n,a.pop_back();
			if (a == "path") {
				x.getline(setting.root_path);
			}
			if (a == "max_size") {
				x.get_index(setting.MAX_SIZE);
			}
			if (a == "thread") {
				x.get_index(setting.is_thread);
			}
			if (a == "content") {
				x.getall(setting.FILE_content);
			}
		}
		int len=root_path.length();
		if(!len)return;
		int L=0,R=len-1;
		while(L<len&&root_path[L]=='"')++L;
		while(R>=0&&root_path[R]=='"')--R;
		if(L<len)
			root_path=root_path.substr(L,R-L+1);
	}
}setting;

struct KMP {//KMP匹配
	string s;
	int* nxt, length;
	KMP() {
		length = 0;
	}
	void get_nxt() {
		nxt = (int*)malloc(sizeof(int*) * (length + 1));
		int i = 0, j = -1;
		nxt[0] = -1;
		while (i < length) {
			if (j == -1 || s[i] == s[j])
				nxt[++i] = ++j;
			else j = nxt[j];
		}
	}bool find(string& g, int n) {
		int i = 0, j = 0;
		while (i < n && j < length) {
			if(length+i>n+j)return false;
			if (g[i]==s[j]||j==-1)
				++i, ++j;
			else j = nxt[j];
		}
		if (j == length)return true;
		else return false;
	}
}_search;

inline void setting_work() {
	_search.s=setting.FILE_content;
	_search.length=_search.s.length();
	_search.get_nxt();
}


inline void init() {
	SetConsoleOutputCP(65001);
	b.find_file=b.sleep_count>>1;
}
int cnt,cnt_1,cnt_2;
inline void read_File_all(string&s, string& g,my_file&x) {
	g = "";
	if(!x.read_FILE(s,setting.MAX_SIZE))return;
	x.getall(g);
}
ofstream myout("out.txt");
inline void end_work() {
	free(a.str);
	free(b.str);
	free(_search.nxt);
	myout.close();
	system("pause");
	fclose(stdin);
	fclose(stdout);
	exit(0);
}
string path_extension(char*s) {
	static int len, i;
	len = strlen(s);
	i = 0;
	while(i<len&&s[i]!='.')++i;
	if (i == len )return "";
	string g = "";
	while (i < len)
		g .push_back( s[i++] );
	return g;
}
bool remove(string _extension) {
	if(_extension==".jpg"||_extension==".png"||_extension==".gif"||_extension==".mp4"||_extension==".exe"||_extension==".zip"||_extension==".dll"||_extension==".db")
		return true;
	return false;
}
void getFiles(string&path,my_file&x) {
	x._sleep();
	string Files,p=path+"\\*",ss;
	long hFile;
	struct _finddata_t fileinfo;
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {
		do {
			p = path + "\\" + fileinfo.name;
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0&&strcmp(fileinfo.name,"$RECYCLE.BIN")) {
					getFiles(p,x);
				}
			}
			else {
				if ((fileinfo.attrib& _A_SYSTEM)||remove(path_extension(fileinfo.name))) {
					continue;
				}
				read_File_all(p, ss,x);
				if (_search.find(ss, ss.length())) {
					mutex_number.lock();
					cout << p << '\n';
					myout << p << '\n';
					cnt++;
					mutex_number.unlock();
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void thread_get_Files_1() {
	string Files, p = setting.root_path + "\\*",ss;
	long hFile;
	struct _finddata_t fileinfo;
	int step=0;
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {
		do {
			++step;
			if (!(step & 1)) continue;
			p = setting.root_path + "\\" + fileinfo.name;
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, "$RECYCLE.BIN")) {
					getFiles(p,a);
				}
			}
			else {
				if ((fileinfo.attrib & _A_SYSTEM) || remove(path_extension(fileinfo.name))) {
					continue;
				}
				read_File_all(p, ss,a);
				if (_search.find(ss, ss.length())) {
					mutex_number.lock();
					cout << p << '\n';
					myout << p << '\n';
					cnt_1++;
					mutex_number.unlock();
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void thread_get_Files_2() {
	string Files, p = setting.root_path + "\\*",ss;
	long hFile;
	struct _finddata_t fileinfo;
	int step = 0;
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {
		do {
			++step;
			if (step&1) continue;
			p = setting.root_path + "\\" + fileinfo.name;
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0 && strcmp(fileinfo.name, "$RECYCLE.BIN")) {
					getFiles(p,b);
				}
			}
			else {
				if ((fileinfo.attrib & _A_SYSTEM) || remove(path_extension(fileinfo.name))) {
					continue;
				}
				read_File_all(p, ss,b);
				if (_search.find(ss, ss.length())) {
					mutex_number.lock();
					cout << p << '\n';
					myout << p << '\n';
					cnt_2++;
					mutex_number.unlock();
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

inline void thread_Files() {
	thread t1(thread_get_Files_1);
	thread t2(thread_get_Files_2);
	t1.join();
	t2.join();
}

inline void work() {
	int start=clock();
	init();
	setting.read_setting(a);
	setting_work();
	if(!setting.is_thread)getFiles(setting.root_path,a);
	else thread_Files();
	int end=clock();
	printf("在%dms内找到%d个文件\n",end-start,cnt+cnt_1+cnt_2);
	end_work();
}
int main() {
	work();
	return 0;
}