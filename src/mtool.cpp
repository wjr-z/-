#include "mtool.h"

#include <fstream>
#include <io.h>
#include <sstream>

namespace Math {

	time_ref GetTime(){return time_ref::NowTime(); }

	time_ref::m_clock::time_point time_ref::NowTime() {
		static m_clock::time_point nowtime;
		nowtime = m_clock::now();
		return nowtime;
	}
	double time_ref::testTime(const m_clock::time_point& start, const m_clock::time_point& end) {
		return static_cast<double>(std::chrono::duration_cast<m_mis>(end - start).count()) *0.001;
	}

	time_ref::time_ref(m_clock::time_point TimePoint) :TimePoint(TimePoint) {}

	double operator-(const time_ref& lhs, const time_ref& rhs) {
		return time_ref::testTime(rhs.TimePoint, lhs.TimePoint);
	}

	void bucketsort(uint32_t* Start, uint32_t* End) {
		static uint32_t tong[65536];
		const auto n = static_cast<const uint32_t>(End - Start);
		if (n <= 500) {
			std::sort(Start, End);
			return;
		}
		const auto copy = new uint32_t[n];
		if (n <= 32768) {
			for (int i = 0; i < 4; ++i) {
				memset(tong, 0, sizeof(uint32_t) << 8);
				for (uint32_t* j = Start; j < End; ++j)
					++tong[((*j) >> (i << 3)) & 255];
				for (int j = 1; j < 256; ++j)
					tong[j] += tong[j - 1];
				for (uint32_t* j = End - 1; j >= Start; --j)
					copy[--tong[((*j) >> (i << 3)) & 255]] = *j;
				for (uint32_t* j = Start,*Copy=copy; j < End; ++j,++Copy)
					*j = *Copy;
			}
		}
		else {
			for (int i = 0; i < 2; ++i) {
				memset(tong, 0, sizeof(uint32_t) << 16);
				for (uint32_t* j = Start; j < End; ++j)
					++tong[((*j) >> (i << 4)) & 65535];
				for (int j = 1; j < 65536; ++j)
					tong[j] += tong[j - 1];
				for (uint32_t* j = End - 1; j >= Start; --j)
					copy[--tong[((*j) >> (i << 4)) & 65535]] = *j;
				for (uint32_t* j = Start,*Copy=copy; j < End; ++j,++Copy)
					*j = *Copy;
			}
		}
		delete[]copy;
	}

	void getFiles(const std::string& path, std::vector<std::string>& filePath) {
		intptr_t h_file;
		struct _finddata_t fileinfo{};
		std::string p;
		if ((h_file = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {
			do {
				if ((fileinfo.attrib & _A_SUBDIR))//如果是文件夹
				{
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
						getFiles(p.assign(path).append("\\").append(fileinfo.name), filePath);
				}
				else//如果是文件
				{
					filePath.emplace_back(p.assign(path).append("\\").append(fileinfo.name));
				}
			} while (_findnext(h_file, &fileinfo) == 0);
			_findclose(h_file);
		}
	}

	std::vector<std::string> getFiles(const std::string& path) {
		std::vector<std::string>filePath;
		if (!_access(path.c_str(), 0))
			getFiles(path, filePath);
		struct _finddata_t fileinfo{};
		if ((_findfirst(path.c_str(), &fileinfo)) != -1) {
			if (!(fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					filePath.push_back(path);
				}
			}
		}
		return filePath;
	}

	std::string readFiles(const std::string&filename) {
		const std::ifstream in(filename, std::ios::binary);
		std::ostringstream tmp;
		tmp<<in.rdbuf();
		return tmp.str();
	}

	void writeFiles(const std::string& filename, const std::string& str) {
		std::ofstream out(filename,std::ios::binary);
		out.write(str.c_str(),static_cast<std::streamsize>(str.length()));
	}


}
