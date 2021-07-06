#include "Huffman.h"
#include <iostream>

//#define DEBUG
using namespace std;
namespace Huffman {
	void makeroot(HuffmanTreeNode*&treenode, unsigned int dep,
		unsigned int Val, bitval*wjr) {
		if (treenode == nullptr)return;
		if (treenode->isleaf()) {
			wjr[treenode->Val] = { dep,Val };
			return;
		}
		makeroot(treenode->child[0], dep + 1, Val << 1, wjr);
		makeroot(treenode->child[1], dep + 1, Val << 1 | 1, wjr);
	}

	struct cmp {
		bool operator()(HuffmanTreeNode* lhs, HuffmanTreeNode* rhs)const {
			return (*lhs) < (*rhs);
		}
	};

	void treebuild(HuffmanTreeNode*&rt,unsigned int*sum) {
		if (rt != nullptr) {
			HuffmanTreeNode::Alloc.destroy(rt);
			HuffmanTreeNode::Alloc.deallocate(rt);
			rt=nullptr;
		}

		std::priority_queue<HuffmanTreeNode*, vector<HuffmanTreeNode*>, cmp>que;

		for (int i = 0; i < 256; ++i) {
			if (!sum[i])continue;
			HuffmanTreeNode*x=HuffmanTreeNode::Alloc.allocate();
			x->child[0]=x->child[1]=nullptr;
			x->Size=sum[i];
			x->Val=i;
			que.push(x);
		}

		int InQueSize = que.size();
		while (InQueSize > 1) {
			HuffmanTreeNode* top1 = que.top();
			que.pop();
			HuffmanTreeNode* top2 = que.top();
			que.pop();
			--InQueSize;

			HuffmanTreeNode* fa = HuffmanTreeNode::Alloc.allocate();
			fa->Size=top1->Size+top2->Size;
			fa->child[0] = top1;
			fa->child[1] = top2;
			fa->Val=0;
			que.push(fa);
		}
		rt = que.top();
		que.pop();
		if (rt->isleaf()) {
			HuffmanTreeNode*X=new HuffmanTreeNode;
			X->child[0]=rt;
			rt=X;
		}
	}

	std::string treecode(char LastLength, unsigned int* sum);

	std::string encode(const std::string& arr) {

		static unsigned int sum[256];
		static HuffmanTreeNode*rt=nullptr;
		static bitval _hash[256];

		unsigned char* ql = (unsigned char*)(arr.c_str());
		unsigned int Length = arr.length();
		if (!Length)return "";
		
		memset(sum, 0, sizeof(int)<<8);
		for (int i = 0; i < Length; ++i)
			++sum[ql[i]];

		treebuild(rt,sum);

		for(int i=0;i<256;++i)
			_hash[i]={0,0};

		makeroot(rt, 0, 0, _hash);


		std::string _encode;
		unsigned int V = 0;
		unsigned int valLen = 0;

		for (int i = 0; i < Length; ++i) {
			auto it = _hash[ql[i]];
			unsigned int v = it.second;
			unsigned int Len = it.first;

			V = (V << Len) | v;
			valLen += Len;

			while (valLen >= 8) {
				_encode.push_back((V >> (valLen - 8)));
				V &= valLen != 8 ? ((1 << valLen - 8) - 1) : 0;
				valLen -= 8;
			}
		}
		char LastLength = valLen ? valLen : 8;

		if (valLen)
			_encode.push_back(V << (8 - valLen));

		return treecode(LastLength,sum).append(_encode);
	}

	std::string encode(const char*str,unsigned int Len) {

		static unsigned int sum[256];
		static HuffmanTreeNode* rt = nullptr;
		static bitval _hash[256];

		unsigned char* ql = (unsigned char*)(str);
		unsigned int Length = Len?Len:strlen(str);
		if (!Length)return "";

		memset(sum, 0, sizeof(int) << 8);
		for (int i = 0; i < Length; ++i)
			++sum[ql[i]];

		treebuild(rt, sum);

		for (int i = 0; i < 256; ++i)
			_hash[i] = { 0,0 };

		makeroot(rt, 0, 0, _hash);


		std::string _encode;
		unsigned int V = 0;
		unsigned int valLen = 0;

		for (int i = 0; i < Length; ++i) {
			auto it = _hash[ql[i]];
			unsigned int v = it.second;
			unsigned int Len = it.first;

			V = (V << Len) | v;
			valLen += Len;

			while (valLen >= 8) {
				_encode.push_back((V >> (valLen - 8)));
				V &= valLen != 8 ? ((1 << valLen - 8) - 1) : 0;
				valLen -= 8;
			}
		}
		char LastLength = valLen ? valLen : 8;

		if (valLen)
			_encode.push_back(V << (8 - valLen));

		return treecode(LastLength, sum).append(_encode);
	}


	inline void appendint(std::string& str, int Val) {
		str += (Val >> 24);
		str += (Val >> 16) & 255;
		str += (Val >> 8) & 255;
		str += Val & 255;
	}

	std::string treecode(char LastLength,unsigned int*sum) {//·µ»ØÊ÷±àÂë
		std::string str;
		str += "wjrorz";//Í·
		str += LastLength;
		for (int i = 0; i < 256; ++i) {
			if (sum[i]) {
				str += i;
				appendint(str, sum[i]);
			}
		}
		str += "tql";//Î²
		return str;
	}

	int build(const std::string&treecode,HuffmanTreeNode*&rt,char&LastLength,unsigned int*sum) {
		if (treecode.substr(0, 6) != "wjrorz")
			return -1;
		int Length = treecode.size();
		memset(sum, 0, sizeof(int)<<8);
		unsigned char* ql = (unsigned char*)treecode.c_str();
		LastLength = ql[6];
		int i;
		for (i = 7; i < Length; i += 5) {
			if (ql[i] == 't' && ql[i + 1] == 'q' && ql[i + 2] == 'l')
				break;
			unsigned char X = ql[i];
			unsigned int _Sum = (ql[i + 1] << 24) | (ql[i + 2] << 16) | (ql[i + 3] << 8) | ql[i + 4];
			sum[X] = _Sum;
		}
		if (i >= Length)return -1;
		treebuild(rt,sum);
		return i + 3;
	}

	std::string decode(const std::string& str) {
		static HuffmanTreeNode*rt=nullptr;
		static char LastLength;
		static unsigned int sum[256];
		int Length = str.length();
		int head = build(str,rt,LastLength,sum);
		if (head == -1)return "";

		HuffmanTreeNode* x = rt;
		std::string ans;
		for (int i = head; i < Length - 1; ++i) {
			unsigned char _ch = str[i];
			for (int j = 7; ~j; --j) {
				bool cho = (_ch >> j) & 1;
				x = x->child[cho];
				if (x->isleaf()) {
					ans.push_back(x->Val);
					x = rt;
				}
			}
		}
		unsigned char _ch = str[Length - 1];
		for (int j = 7; j >= 8 - LastLength; --j) {
			bool cho = (_ch >> j) & 1;
			x = x->child[cho];
			if (x->isleaf()) {
				ans.push_back(x->Val);
				x = rt;
			}
		}
		return ans;
	}

	int build(const char*treecode, unsigned int Len,HuffmanTreeNode*& rt, char& LastLength, unsigned int* sum) {
		if (treecode[0]!='w'||treecode[1]!='j'||treecode[2]!='r'
			||treecode[3]!='o'||treecode[4]!='r'||treecode[5]!='z')
			return -1;
		unsigned int Length = Len;
		memset(sum, 0, sizeof(int) << 8);
		unsigned char* ql = (unsigned char*)treecode;
		LastLength = ql[6];
		int i;
		for (i = 7; i < Length; i += 5) {
			if (ql[i] == 't' && ql[i + 1] == 'q' && ql[i + 2] == 'l')
				break;
			unsigned char X = ql[i];
			unsigned int _Sum = (ql[i + 1] << 24) | (ql[i + 2] << 16) | (ql[i + 3] << 8) | ql[i + 4];
			sum[X] = _Sum;
		}
		if (i >= Length)return -1;
		treebuild(rt, sum);
		return i + 3;
	}

	std::string decode(const char*str,unsigned int Len) {
		static HuffmanTreeNode* rt = nullptr;
		static char LastLength;
		static unsigned int sum[256];
		int Length = Len?Len:strlen(str);
		int head = build(str, Len ,rt, LastLength, sum);
		if (head == -1)return "";

		HuffmanTreeNode* x = rt;
		std::string ans;
		for (int i = head; i < Length - 1; ++i) {
			unsigned char _ch = str[i];
			for (int j = 7; ~j; --j) {
				bool cho = (_ch >> j) & 1;
				x = x->child[cho];
				if (x->isleaf()) {
					ans.push_back(x->Val);
					x = rt;
				}
			}
		}
		unsigned char _ch = str[Length - 1];
		for (int j = 7; j >= 8 - LastLength; --j) {
			bool cho = (_ch >> j) & 1;
			x = x->child[cho];
			if (x->isleaf()) {
				ans.push_back(x->Val);
				x = rt;
			}
		}
		return ans;
	}
}