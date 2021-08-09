#include <io.h>
#include "mtool.h"

namespace Math {

	timereference GetTime(){return timereference::NowTime(); }

	timereference::mclock::time_point timereference::NowTime() {
		static mclock::time_point nowtime;
		nowtime = mclock::now();
		return nowtime;
	}
	double timereference::testTime(const mclock::time_point& start, const mclock::time_point& end) {
		return std::chrono::duration_cast<mmis>(end - start).count() / 1000.0;
	}

	void bucketsort(uint32_t* Start, uint32_t* End) {
		static uint32_t tong[65536];
		uint32_t n = End - Start;
		if (n <= 500) {
			std::sort(Start, End);
			return;
		}
		uint32_t* copy = new uint32_t[n];
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
		intptr_t   hFile = 0;
		struct _finddata_t fileinfo;
		std::string p;
		if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1) {
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
			} while (_findnext(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}
	}

	std::vector<std::string> getFiles(const std::string& path) {
		std::vector<std::string>filePath;
		if (!_access(path.c_str(), 0))
			getFiles(path, filePath);
		struct _finddata_t fileinfo;
		intptr_t   hFile = 0;
		if ((hFile = _findfirst(path.c_str(), &fileinfo)) != -1) {
			if (!(fileinfo.attrib & _A_SUBDIR)) {
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					filePath.push_back(path);
				}
			}
		}
		return filePath;
	}

}