现有较为完善的大整数类，包括十进制和二进制两种，各种运算均进行了一定程度上的优化，乘法暂时使用的暴力和FFT，Karatsuba已实现但暂未使用。二进制大整数不仅可用作整数，还实现了部分bitset功能，支持快速异或、与、或等，并且支持动态扩展、缩小。支持随机大数，大素数，十进制和二进制快速转化等等。（相关文件Allocator.h、Array.h、Array.cpp、bint.h、bint.cpp、fftsg_h.c）  

实现了部分实用函数，包括log(log n)的向下取整log2和log10，O(1)求二进制1的个数，double对long long 的快速转换等等。（match_func.h、math_func.cpp)   

包括lz77算法，有两种模式，其中快速模式压缩率略小于Fast_Lz，压缩速度略高于Fast_Lz。（lz77.h、lz77.cpp）  

还有封装的Splay类，可以自定义pushup、pushdown函数，实现了迭代器。（Splay.h）  

实现了pollard-rho算法，可以快速分解10^20以内的数，以及Miller-Robin快速判断素数。（PollardRho.h、PollardRho.cpp）

---

暂时需要继续调整。

对于部分编译器，其vector实现可能不同，例如Qt的，此时需要在Array.h中找到：

```C++
#define USE_ALLOCATOR
```

并将其注释。

