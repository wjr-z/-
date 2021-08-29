主要用于学习C++的相关知识，例如if else的分治预测，右值引用和左值引用，参数包，初始化列表，迭代器，封装，运算符重载，效率优化和测试等等。代码在VS上可以很好地运行，在其他的环境则不能保证，可以尝试在math_func.h注释掉

```
#define TEST
```

### bint.h、bint.cpp  

现有较为完善的大整数类，支持10进制和2进制两种，可通过中括号访问每一位，并且封装了vector进行动态扩展（超过现有长度自动扩展），可以直接使用int或者long long赋值等等，乘法使用了暴力、Karatsuba、TOOM-COOK、FFT，但Karatsuba和TOOM-COOK只进行了学习，并保留但未使用。除法暂时只用了我自己想到的两种分治除法和knuth除法。实现了大整数随机数，大整数随机质数，二进制和十进制的快速转化（分治）。  

upd 2021.8.18 : 修复pollard-rho以及高精度和低精度比较的部分bug，现在pollard-rho可以很快跑出绝大部分10^30以内的数，支持int、long long 和大整数。

```C++
常用运算符：+（单目/双目）、- (单目/双目)、* 、/ 等，二进制有 ^ 、| 、& 等，基本能像int那样用。

只能显示构造！
正确的初始化：
bint a(7);
bint a("124");
bint a(1),b(a),c(a,false);
bint a("-123");
...
错误的初始化：
bint a=7;
bint a="124";
bint a="-123";
...
    
赋值和int基本类似

toint():转化为int
toll():转化为long long
tostr():转换为字符串

length():数字长度，不包括符号
relength():改变长度，不足补0
[]:十进制/二进制下的第几位的数字
at()、save_at():压位后第几位的数字

isprime()
randprime()
pollarrho()
randbint()
randbint2()
...
```



### math_func.h、math_func.cpp  

只实现了一些简单函数

log(log n)的向下取整 log2和log10，思想很好理解。  

int和double串的合法性判断

等等

### mtool.h、mtool.cpp

```C++
timereference getTime();//获取当前时间，精度更高
double operator-(const timereference& lhs, const timereference& rhs);//两个时间的间隔，单位为毫秒

//例子：

auto start=getTime();
...//待测函数
auto end=getTime();
cout<<end-start<<endl;
```

```C++
template<typename Fn>
double qtime(const Fn& ToBeTest)//返回无参数函数运行一次的耗时

template<typename Fn, typename... Args>
double qtime(const Fn& ToBeTest, Args&&...List)//返回带参函数运行一次的耗时

template<typename Fn>
double qavltime(const Fn& ToBeTest, int kth)//返回无参函数运行k次的平均耗时

template<typename Fn, typename... Args>
double qavltime(const Fn& ToBeTest, int kth, Args&&...List)//返回带参函数运行k次的平均耗时

template<typename Fn>
double qcounttime(const Fn& ToBeTest, int kth)//返回无参函数运行k次的总耗时

template<typename Fn, typename... Args>
double qcounttime(const Fn& ToBeTest, int kth, Args&&...List)//返回带参函数运行k次的总耗时
    
//例子：
void print(int x){
    cout<<x<<endl;
}
cout<<qtime(print,3)<<endl;
cout<<qtime(
	[](int&x){
        cout<<x<<endl;
    },3
)<<endl;

//其余的基本同理，只是多了一个k参数
```

```C++
template<typename Ty>
bool check(Ty* arr1,Ty*arr2,int n)//比较前n个元素是否相同

template<typename Ty,typename... Args>
bool check(const Ty&head,const Ty&nxt,Args ...toBeTest)//比较所有参数是否相同（同类型）
```

```C++
template<typename Ty>
void qswap(Ty* Start, Ty* End, int* rev)
//设数组为a,让所有的a[i]=a[rev[i]]
//进行了内存优化，使用了自己的bint2即二进制大整数，之后也许会单独写一个动态长度bitset
//只使用了sizeof(int)*(n/8)的内存，并且避免了拷贝，主要思想是进行若干个环形移动

//例子：

int aa[3]={6,3,7};
qswap(aa, aa + 3, { 1,0,2 });
cout<<aa[0]<<endl<<aa[1]<<endl<<aa[2]<<endl;
```

```C++
void bucketsort(uint32_t* Start, uint32_t* End);
//unsigned int桶排序
//设n为元素个数
//n<=500时为sort。
//500<n<=32768时为桶的个数为256的桶排序。
//32768<n时为桶的个数为65536的桶排序。
//当数据范围很大时，并且数据接近随机时快于sort 2~4倍。
//复杂度之和n有关，和内部元素顺序无关，因此在较为有序或者完全有序时慢于sort。
```

```C++
std::vector<std::string> getFiles(const std::string& path);
//获取一个文件夹下的所有文件路径，如果该路径是文件，则只获取该路径
//若路径不合法，则返回空的vector
```

```C++
template<typename Ty>
void thread_sort(const Ty First, const Ty Last);
template<typename Ty, typename _Pr>
void thread_sort(const Ty First, const Ty Last, const _Pr _Pred = std::less<>());
//使用多线程进行sort，对于大小在8000以内的使用单线程，8000~10000使用双线程，10000以上使用四线程
```

### slist.h

顺序插入、访问O(1)，随机插入、访问O(sqrt(n))

实现有iterator迭代器

与list用法相似，部分功能尚未实现

与list不同的是可以快速访问所有位置，均摊O(sqrt(n))

例如：

```C++
slist<int>test;
for(int i=0;i<1000;++i)
    test.push_back(i);
//因为只有push_back，因此最初只有一块，块大小是1000
*(test.begin()+256);//256
test.debug();//调试用，输出每块的大小
//输出：
//34 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23 23
//块大小在sqrt(2*n)/2~sqrt(2*n)之间
```

使用maintain对所有元素进行一次维护  

合适的位置使用一次maintain可以降低后续操作的复杂度  

当前的动态维护暂不能保证常数最优化  

### Deque.h

于2021.8.29进行了大幅重构  

与vector不同之处在于在头部也预留了部分空间，头尾预留相同大小的空间

也许会有部分bug。  

### splay.h 

splay的封装，可以通过洛谷上的平衡树题，但暂且接口易用性不高，以后也许可能会进行简化。

平衡树，动态插入节点或者区间，自定义区间维护，区间翻转等。

### Allocator.h

网上找到的内存池类。

### lz77.h、lz77.cpp

自己实现的lz77算法

接口非常简单，具体见代码

```C++
#define LZLEVEL 2 //2:体积最小,1：速度最快
```





