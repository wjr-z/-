# Math

正在从最底层的大数开始，之后会不局限于数学方面，例如二进制转十进制，十进制转二进制等等都会逐步完善。

---

## bint类：
大整数类，以 10^8 为进制。
以下均用"第index位"表示原数字(10^1进制)的第index位，而用"第index个元素"表示压位后数字(10^8进制)的第index位。
均使用 n 代表较大的元素个数，m 代表较小的元素个数。

int / long long 不会隐式转化为 bint ，应尽量避开隐式转化，而使用显示转化！！！   
但可以用string 和 char 隐式转化为 bint ，因为 string/char 发生一些意料之外的隐式转化的概率很小。

运算符重载：

+/+= : 大数加法，复杂度均摊O(m)，支持符号不同的相加，支持直接与 int 的相加。   
-/-= : 大数减法，复杂度均摊O(m)，支持符号不同的相减，支持直接与 int 的相减。   
\*/\*= : 大数乘法，复杂度与数据范围相关，具体详见后面算法栏，同样支持不同符号相乘，位数上限为int 最大值。支持直接与 int 的相乘。   
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

相关函数：

### 成员函数：
assign(const bint& other, const int& L, const int& R) : 从头拷贝另一个 bint 的第 L 个元素到第R-1个元素。
iszero() : 判断是否为 0 。
ispositive() :  true代表是正数，false代表是负数。
length() : 返回数字有几位。
relength(const int&) : 设置位数，如果小于当前位数，自动舍去高位。
reverse(int Length) : 将低位开始的 Length位翻转，默认 Length=length() 。
at(const size_t& index) : 返回第 index 位的数字。
set(const size_t&index,const short&val) : 设置第 index 位为 val 。
quick_mul_10() : 快速乘以 10 ，比直接乘以 10 会更快，因为使用的是整体移位，复杂度是O(n) 。
quick_mul_10k(const int&k) : 快速乘以 10^k ，复杂度是 O(n+k) 。
quick_divide_10k(const int&k) : 快速除以 10^k ，复杂度是O(n) 。
abs() : 变为绝对值
toint() : 返回 int ，注意，存储的数大于 int 最大值会爆int。
toll() : 返回 long long ，存储的数大于long long 最大值会爆 long long。
tostr() : 返回 string 。
to2bit() : 返会 bint2 ，bint2 是 2^32 进制的 bint 类（不完善）。

### 友元函数：
qpow(bint ,int ) / qpow(int ,bint ) / qpow(bint ,bint ) : 使用快速幂

