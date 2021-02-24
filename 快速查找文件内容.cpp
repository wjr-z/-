#pragma GCC optimize(3,"Ofast","inline")
#include <cstdio>
#include <cstdlib>
#include <istream>
#include <iostream>
#include <string>
#include <io.h>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <time.h>
#pragma execution_character_set("utf-8")
using namespace std;
inline void end_work();
char* str;
int len = 1, pos, siz;
int open_count;
string FILE_path;
const int MAX_SIZE=50000000;
inline bool init(string&s) {
	FILE* fp;
	errno_t err;
	err=fopen_s(&fp,s.c_str(),"rb");
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
	if (len < siz) {
		while (len < siz) {
			len <<= 1;
		}
		free(str);
		str = (char*)malloc(sizeof(char*) * len);
	}
	fread(str, 1, siz, fp);
	pos = 0;
	fclose(fp);
	return true;
}
inline void getline(string& s) {
	while (str[pos] != '\r' && str[pos] != '\n' && pos < siz)
		s += str[pos++];
	if (str[pos] == '\r')pos+=2;//对于\r\n换行吃掉\r\n
}
inline void get_all(string& s) {
	while (pos < siz)s += str[pos++];
}
struct KMP {
	string s;
	int* nxt, length;
	KMP() {
		length = 0;
	}
	inline void get_nxt() {
		nxt = (int*)malloc(sizeof(int*) * (length + 1));
		int i = 0, j = -1;
		nxt[0] = -1;
		while (i < length) {
			if (j == -1 || s[i] == s[j])
				nxt[++i] = ++j;
			else j = nxt[j];
		}
	}inline bool find(string& g, int n) {
		int i = 0, j = 0;
		while (i < n && j < length) {
			if (length +i > n + j)return false;//主串长度没有匹配串剩余长度长
			if (j == -1 || g[i] == s[j])
				++i, ++j;
			else j = nxt[j];
		}
		if (j == length)return true;
		else return false;
	}
}a;
string root_path;
inline void maintain() {
	int n = root_path.length();
	if (!n) {
		cout << "请在.exe同目录下新建in.txt文件，第一行为查找根目录的路径(直接用复制路径即可)，第二行为查找的内容" << '\n';
		end_work();
	}
	int L = 0, R = n - 1;
	while (root_path[L] == '"')L++;
	while (root_path[R] == '"')R--;
	string t(root_path.substr(L, R - L + 1));
	root_path = t;
	n = root_path.length();
	if (!n) {
		cout << "请检查输入的路径格式" << endl;
		end_work();
	}
}
inline void read() {
	str = (char*)malloc(sizeof(char*));
	SetConsoleOutputCP(65001);
	//std::ios::sync_with_stdio(false);
	if(!init((FILE_path="in.txt")))end_work();
	getline(root_path);
	get_all(a.s);
	maintain();
	a.length = a.s.length();
}
int cnt;
inline void read_File_all(string&s, string& g) {
	g = "";
	if(!init(s))return;
	get_all(g);
}
ofstream myout("out.txt");
inline void end_work() {
	free(str);
	free(a.nxt);
	myout.close();
	system("pause");
	fclose(stdin);
	fclose(stdout);
	exit(0);
}
inline string path_extension(char*s) {
	static int len, i;
	len = strlen(s);
	i = 0;
	while(i<len&&s[i]!='.')++i;
	if (i == len )return "";
	string g = "";
	while (i < len)
		g += s[i++];
	return g;
}
string ss;
inline bool remove(string _extension) {
	if(_extension==".jpg"||_extension==".png"||_extension==".gif"||_extension==".mp4"||_extension==".exe"||_extension==".zip"||_extension==".dll"||_extension==".db")
		return true;
	return false;
}
inline void getFiles(string path) {
	string Files;
	long hFile;
	struct _finddata_t fileinfo;
	string p = path + "\\*";
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1) {
		do {
			string h = path + "\\" + fileinfo.name;
			if ((fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0&&strcmp(fileinfo.name,"$RECYCLE.BIN")) {
					getFiles(h);
				}
			}
			else {
				if ((fileinfo.attrib& _A_SYSTEM)||remove(path_extension(fileinfo.name))) {
					continue;
				}
				read_File_all(h, ss);
				if (a.find(ss, ss.length())) {
					cout<<h<<'\n';
					myout << h << '\n';
					cnt++;
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}inline void work() {
	int start=clock();
	read();
	a.get_nxt();
	getFiles(root_path);
	int end=clock();
	printf("在%dms内找到%d个文件\n",end-start,cnt);
	end_work();
}
int main() {
	work();
	return 0;
}

