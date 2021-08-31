#ifndef LZ77_H
/**
 * 实现的lz77算法，level 1 速度更快,level 2压缩效果更好
 */
#define LZ77_H
#include <string>

namespace lz77 {
	int compress(const void*input,int length,void*output);
	std::string compress(const std::string& arr);
	int decompress(const void*input,int length,void*output);
	std::string decompress(const std::string&arr);
}

#endif