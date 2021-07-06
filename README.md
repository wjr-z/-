### 集合了一些算法和数据结构

暂时有十进制、二进制大整数类，高精度浮点数类（效率勉强还够用，默认精度32位十进制位），Splay类（但感觉很少能用到）。

二进制与十进制的$O(nlog^2n)$转化（n为转化位数），质数的快速判断(Miller Robin算法)，质因数分解(Pollard-rho算法)。

---

## bint类：
大整数类，以 10^8 为进制。
以下均用"第index位"表示原数字(10^1 进制)的第index位，而用"第index个元素"表示压位后数字(10^8 进制)的第index位。  
均使用 n 代表较大的元素个数，m 代表较小的元素个数。  

请使用显示转化。

运算符重载：

```c++
+/+= : 大数加法，复杂度均摊O(m)，支持符号不同的相加，支持直接与 int 的相加。   
-/-= : 大数减法，复杂度均摊O(m)，支持符号不同的相减，支持直接与 int 的相减。   
* / *= : 大数乘法，复杂度与数据范围相关，具体详见后面算法栏，同样支持不同符号相乘，位数上限为int 最大值。支持直接与 int 的相乘。   
\\/\\= : 大数除法，最坏复杂度为O(nlog^2 n) ，最好复杂度很小，尤其是当 n 和 m 相近时，会很快，当 m 接近 0.5n 时最慢。当除数为 0 时，返回被除数。   
\%/\%= : 大数取模，复杂度取决于除法。   
++ : 支持前后自增，前自增均摊复杂度O(1)，后自增由于拷贝，复杂度O(n)。   
-- : 支持前后自减，前自减均摊复杂度O(1)，后自减复杂度O(n)。   
<</<<= : 乘以2^k，复杂度与乘法类似   
\>>/>>= : 除以2^k，复杂度与除法类似   
单目运算符+ : 返回自己   
单目运算符- : 返回负数   
判断运算符 : 基本齐全，但注意两个相近的数比较复杂度是O(n)。   
istream>> : 输入
ostream<< : 输出
```

相关函数：

### 成员函数：
```c++
void assign(const bint& other, const int& L, const int& R) : 从头拷贝另一个 bint 的第 L 个元素到第R-1个元素。   
bool iszero() : 判断是否为 0 。   
bool ispositive() :  true代表是正数，false代表是负数。   
uint length() : 返回数字有几位。   
void relength(const int&) : 设置位数，如果小于当前位数，自动舍去高位。   
void reverse(int Length) : 将低位开始的 Length位翻转，默认 Length=length() 。   
uint at(const size_t& index) : 返回第 index 位的数字。   
void set(const size_t&index,const uint&val) : 设置第 index 位为 val 。   
void quick_mul_10() : 快速乘以 10 ，比直接乘以 10 会更快，因为使用的是整体移位，复杂度是O(n) 。   
bint& quick_mul_10k(const int&k) : 快速乘以 10^k ，复杂度是 O(n+k) 。   
bint& quick_divide_10k(const int&k) : 快速除以 10^k ，复杂度是O(n) 。   
void abs() : 变为绝对值   
int toint() : 返回 int ，注意，存储的数大于 int 最大值会爆int。   
long long toll() : 返回 long long ，存储的数大于long long 最大值会爆 long long。   
string tostr() : 返回 string 。   
bint2 to2bit() : 返会 bint2 ，bint2 是 2^32 进制的 bint 类（不完善）。复杂度是O(nlogn)，但常数略大
void randdata(const bint&other) : 随机一个[0,other]的数，使用mt19937
```



### 友元函数：
```c++
bint qpow(bint ,int ) / qpow(int ,bint ) / qpow(bint ,bint ) : 使用快速幂    
bint abs(bint) : 返回绝对值  
bint randdata(const bint&L,const bint&R) : 生成一个[L,R]的随机数
bint randdata(const int k) : 生成一个k位随机数
```

 

### Math.h中函数：
```c++
bint sqrt(bint) : 返回开方，取了相近的迭代初值，均摊复杂度较小   
int log2(const bint&) : 返回 log2 ，原理是先转化为 bint2,返回 bint2 的位数即答案。
```



---

## bint2 类
略快于bint类，不是很完善

## bfloat 类
尾数使用 bint ，指数使用 int，精度为10^(-floatlim)，floatlim初始为64，可以自行设置（设置是非线程安全的，因此需要在多线程外设置精度，建议在main开始就设置，且仅设置一次）   
大致同 bint    

特有函数：   

### 成员函数：
```c++
void setbase(const bint&) : 设置尾数   
void setexp(const int&) : 设置指数   
string tostr() :  返回以科学计数法表示的string   
```



### 友元函数：
```c++
bfloat fabs(const bfloat&) : 返回绝对值   
bool approximate(const bfloat&,const bfloat&) : 判断两个数是否相似，误差在5\*10^(-floatlim)之内返回true   
void setfloatlim(const int&) : 设置精度，即尾数最大位数，在main开始时就应该设置，且最好仅设置一次，或者不在多线程内设置即可。   
bfloat qpow(bfloat ,int ) : 快速幂   
bfloat qpow(bfloat ,bint ) : 快速幂   
```



### Math.h中函数：
```c++
bfloat sqrt(const bfloat&) : 开根   
bfloat sqrtk(const bfloat&,const int&k) : 1/k 次方   
bfloat sqrtk(const bfloat&,const bint&k) :  1/k 次方   
bfloat sqrtpq(const bfloat&,const bfloat&p,const bfloat&q) : p/q 次方   
bfloat pow(const bfloat&a,const  bfloat&b) : a^b，暂且不支持b为负数    
bfloat ln(const bfloat&) : 求 ln x，但暂时精度可能会有些许问题，floatlim=64时大概能保证32位的精度吧   
bfloat exp(const bfloat&) : 求 e^x，精度问题同上。   
```



---

## Splay类

当前版本主要用于维护序列，支持懒惰标记、区间操作、简单的迭代器操作等。

可用

```C++
Splay<int> test;
```

进行构造

可以自定义类进行构造，例如：

```C++
class TESTSplayNode{
private:
	int val;
	long long sum;
	int tag,size;
	friend int getSize(TESTSplayNode* x) {
		return (x!=nullptr)?x->size:0;
	}
	friend long long getSum(TESTSplayNode* x) {
		return (x!=nullptr)?x->sum:0;
	}
public:
	TESTSplayNode(const int& _val = 0) :val(_val),sum(_val),tag(0),size(1) {

	}
	int getval(){return val;}
	long long getsum(){return sum;}
	void pushup(TESTSplayNode* l, TESTSplayNode* r) {
		size=getSize(l)+getSize(r)+1;
		sum=getSum(l)+getSum(r)+val;
	}
	void pushtag(int S) {
		sum+=S*1ll*size;
		val+=S;
		tag+=S;
	}
	void pushdown(TESTSplayNode* l, TESTSplayNode* r) {
		if (tag) {
			if(l!=nullptr)
				l->pushtag(tag);
			if(r!=nullptr)
				r->pushtag(tag);
			tag=0;
		}
	}
};

Splay<int,TESTSplayNode>test;
```

其中自定义类至少要有 getval函数、pushup函数、pushrev函数、pushdown函数。

介绍如下：

```C++
getval函数用于获取该节点的val值
pushup函数用于从左右儿子节点更新当前点信息，例如 size 应为左右儿子节点size总和 + 1，sum应为左右耳子节点sum总和+1，但注意可能出现nullptr即空节点。
pushrev函数用于Splay内部进行旋转左右儿子后对当前点存储信息的更新。例如求最大子序列时，若翻转一个区间，则该区间维护信息会发生改变，而为了封装性，不能使得自定义类进行旋转类内部节点，因此需要此函数单独用于旋转。
pushdown函数用于懒惰标记的下放，最简单的例子参见上面，上面实现的是序列支持区间加，区间求和。
```

食用方式如下，以无自定义类为例，自定义类类似：

```
Splay<int>test;
test.insert(0,4);
test.insert(test.begin(),4);//等价于上面

test.insert(test.size()-1,7);//test.size()=2
test.insert(test.end(),7);//等价于上面

test.DEBUG();//调试信息，输出所有的元素
for(auto it = test.begin();it!=test.end();++it)
	std::cout<<it->getval()<<' ';//若为自定义类，可以同样用it->来调用自定义类接口
std::cout<<std::endl;

auto it = test.begin();
Splay<int>::iterator it = test.begin();

auto it = test.kthelement(3);//返回第三个元素的迭代器
auto it = test.begin()+2;

auto it = test.split(1,3);//返回指向第[1,3]个元素的迭代器，此时该迭代器指向这个区间子树的根节点，但是如果之后进行了其他操作可能并不是指向该区间根节点，因此split出一个区间后即时修改值或者获取值。

test.reverse(1,4);//将[1,4]元素翻转

//test.erase(1);
test.erase(test.begin());//等价于上面

test.erase(test.begin(),test.end());

Splay<int>test2;

test2.swap(test);//O(1)交换容器

test.insert(test.begin(),test2.begin(),test2.end());

test=test2;
```

若有自定义类，则可以对数列进行更多维护。

---

## Matrix类

还没开始

---

## math_func.h
有一些相关函数，例如quicklog2、quicklog10等，对于一些操作进行了效率优化。   

```C++
isprime(int) : 判断一个injt是否是素数，O(logn)复杂度
isprime(bint): 判断一个大整数是否是素数，O(log^2n)复杂度，因为大整数还有位数相乘的复杂度，且常数较大（vector构造）
暂无 isprime(long long)，主要是懒
```

---

## fftsg_h : 
使用的 ooura_FFT ，据说很快，也确实很快！   


---

下面是用到的一些算法及分析：   

### bint乘法：
对于数据范围采用三种算法，一种是乘以int，一种是暴力，一种是FFT。   
因为这个FFT常数真的是小，所以Karatsuba没用到qwq   

- 当 m = 1 时使用低精度乘以高精度   
- 当 m < 32 时 ， m <= 2*log2(n) + 8 时使用暴力，否则使用 FFT 。   
- 当 m >= 32 时， m <= log2(n) + 4 时使用暴力，否则使用 FFT 。   

为啥我没用TOOM-COOK？因为太懒了   

### bint除法：
自研的两种算法+knuth除法   
详见代码。   

### 二进制转十进制 以及 十进制转二进制
采用分治实现，具体实现很简单，见代码   

### sqrt
采用牛顿迭代    
设一个迭代初值X，每次让X=(X+A/X)/2即可，当相邻两次迭代值相差较小时即可退出，例如整数为1，浮点数我设置的是5\*10\^(-floatlim)。

### pow(a,b)
设 C 为 b 整数部分，先用快速幂求出，然后对于 b 的小数部分，化为 sqrtpq来求即可，但这样误差可能较大，以后改进

### ln x
使用的泰勒展开，先将 x 缩小到 2 以内(小于1的可以转化为大于1的再做)，这个可以用 ln a^b =b ln a 来实现，然后用泰勒展开实现。

### exp x 
将 x 缩小到 1 以内再泰勒展开。

---

效率方面

计算1000次 1234^5678 的平方，python用时0.67s，bint用时0.45s。   
乘法在大范围内远快于python，中等范围接近，小范围内慢于python，而且会慢很多，因为常数较大，之后使用TOOM-COOK或者优化Karatsuba常数可能会有所改观。
除法在中等范围内时间接近，大范围会快很多吧，小范围慢于python。   
现在主要是常数问题，不得不说有的时候一点点的改动就会使常数减少很多呢。
