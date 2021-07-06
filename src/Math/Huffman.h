#ifndef HUFFMAN_H
#define HUFFMAXN_H

#include <string>
#include <queue>
#include "../BigInteger/Allocator.h"

struct HuffmanTreeNode {
	static Math::Allocator<HuffmanTreeNode>Alloc;
	HuffmanTreeNode* child[2];
	size_t Size;
	unsigned char Val;
	HuffmanTreeNode() :child{ nullptr,nullptr }, Size(0),Val(0) {}
	HuffmanTreeNode(const size_t& _Size,unsigned char _Val=0) : child{nullptr,nullptr}, Size(_Size),Val(_Val) {
	}
	~HuffmanTreeNode() {
		if(child[0]!=nullptr)
			Alloc.destroy(child[0]),Alloc.deallocate(child[0]);
		if(child[1]!=nullptr)
			Alloc.destroy(child[1]),Alloc.deallocate(child[1]);
	}
	bool isleaf(){return child[0]==nullptr; }
	bool operator<(const HuffmanTreeNode&other)const {
		return Size>other.Size;
	}
};

namespace Huffman {
	typedef std::pair<unsigned char, unsigned int> bitval;
	std::string encode(const std::string&);
	std::string decode(const std::string&);
	std::string encode(const char*,unsigned int = 0);
	std::string decode(const char*,unsigned int = 0);
}


#endif 