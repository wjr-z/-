#include "Huffman.h"
#include <iostream>

//#define DEBUG
using namespace std;
namespace Huffman {
	void makeroot(HuffmanTreeNode*&treenode, uint32_t dep,
		uint32_t Val, bitval*wjr) {
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

	HuffmanTreeNode* rt = nullptr;
	bitval _hash[256];

	void treebuild(uint32_t*sum) {
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

	void build(uint32_t* sum) {
		treebuild(sum);
		for (int i = 0; i < 256; ++i)
			_hash[i] = { 0,0 };
		makeroot(rt,0,0,_hash);
	}


	bitval getcode(uint8_t ch) {
		return _hash[ch];
	}


	void treecode(std::string&str,uint32_t* sum) {//·µ»ØÊ÷±àÂë

		str.resize(7);

		str[0]='w';
		str[1]='j';
		str[2]='r';
		str[3]='o';
		str[4]='r';
		str[5]='z';

		for (int i = 0; i < 256; ++i) {
			if (sum[i]) {
				str.push_back(i);
				str.push_back((sum[i]>>24));
				str.push_back((sum[i]>>16)&255);
				str.push_back((sum[i]>>8)&255);
				str.push_back((sum[i]&255));
			}
		}
		str .append( "tql" );//Î²
	}

	std::string compress(const std::string& arr) {

		static uint32_t sum[256];

		uint8_t* ql = (uint8_t*)(arr.c_str());
		uint32_t Length = arr.length();
		if (!Length)return "";
		
		memset(sum, 0, sizeof(int)<<8);
		for (int i = 0; i < Length; ++i)
			++sum[ql[i]];

		build(sum);

		std::string str;

		treecode(str,sum);

		uint32_t V = 0;
		uint32_t valLen = 0;

		for (int i = 0; i < Length; ++i) {
			auto it = _hash[ql[i]];
			uint32_t v = it.second;
			uint32_t Len = it.first;

			V = (V << Len) | v;
			valLen += Len;

			while (valLen >= 8) {
				str.push_back((V >> (valLen - 8)));
				V &= valLen != 8 ? ((1 << valLen - 8) - 1) : 0;
				valLen -= 8;
			}
		}
		char LastLength = valLen ? valLen : 8;

		if (valLen)
			str.push_back(V << (8 - valLen));

		str[6]=LastLength;

		return str;
	}

	int build(const std::string&tree,HuffmanTreeNode*&rt,char&LastLength,uint32_t*sum) {
		if (tree.substr(0, 6) != "wjrorz")
			return -1;
		int Length = tree.size();
		memset(sum, 0, sizeof(int)<<8);
		uint8_t* ql = (uint8_t*)tree.c_str();
		LastLength = ql[6];
		int i;
		for (i = 7; i < Length; i += 5) {
			if (ql[i] == 't' && ql[i + 1] == 'q' && ql[i + 2] == 'l')
				break;
			uint8_t X = ql[i];
			uint32_t _Sum = (ql[i + 1] << 24) | (ql[i + 2] << 16) | (ql[i + 3] << 8) | ql[i + 4];
			sum[X] = _Sum;
		}
		if (i >= Length)return -1;
		treebuild(sum);
		return i + 3;
	}

	std::string decompress(const std::string& str) {

		static char LastLength;
		static uint32_t sum[256];
		int Length = str.length();
		int head = build(str,rt,LastLength,sum);
		if (head == -1)return "";

		HuffmanTreeNode* x = rt;
		std::string ans;
		for (int i = head; i < Length - 1; ++i) {
			uint8_t _ch = str[i];
			for (int j = 7; ~j; --j) {
				bool cho = (_ch >> j) & 1;
				x = x->child[cho];
				if (x->isleaf()) {
					ans.push_back(x->Val);
					x = rt;
				}
			}
		}
		uint8_t _ch = str[Length - 1];
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