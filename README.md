# quick_search_file_content
## 快速查找文件内容

## Introduce
I spent two days on how to optimize the search
First,I used fopen_s , freopen and KMP ,then this can search about 60G at 4 minutes.
Then I think I can use multithreading to optimize it
After I use multithreading ,it can search about 60G at 2 minutes
But this will take up a lot of CPU and disk
So I set some limits to it to protect CPU and disk
**Now it can search 60G at about 2minutes30seconds.**
(There are many files that won't be searched such as ".dll")

## Usage
### Create a new file "in.txt" in the same directory of the ".exe"

### Then you need to configure "in.txt" like this:

```
path:
D:\BaiduNetdiskDownload
max_size:
3000000
thread:
1
content:
qwq
```

### Click ".exe" and you will get a file named "out.txt" that lists file paths that you want

```

## 使用
### 在".exe"同目录下新建一个"in.txt"

### 然后你需要像下面这样配置"in.txt"
```
path:
D:\BaiduNetdiskDownload
max_size:
3000000
thread:
1
content:
qwq
```
### 点击".exe"，你就会得到一个"out.txt"文件，其中列出了你想要的搜索的文件路径


### About(Something that you must know)

### It have four configuration option: 'path','thread','max_size' and 'content'.

Default configuration:
```
path:

max_size:
3000000
thread:
0
content:

```

path: The root path of the search you want

max_size: The max size of files that you want to search

thread: Set to a non-zero value to turn on multithreading

content: The content that you want to search(It can be multiple lines)

You must at least set 'path' and 'content'.

And the 'content' must be the bottom.

You can see examples like "in.txt"(read) and "out.txt"(write)

#### If it takes much CPU ,please set 'thread' to 0 and set 'max_size' smaller such as 10000000(about 10M)

#### Set 'thread' to non-zero can serach quicker(time->65% CPU->170% disk->150% Test when searching 60G files)

#### You can read my ugly ".cpp" to find more details or make changes 

#### My English is not good (hope that you can understand)

