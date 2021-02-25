# quick_search_file_content
## 快速查找文件内容

```
Create a new file "in.txt" in the same directory of the ".exe"

Enter the path at the first line of "in.txt"

Enter the file content that you want to search at the second line of "in.txt"(Support multiple lines)

Then click ".exe" and you will get a file named "out.txt" that lists file paths that you want
```

```
<<<<<<< HEAD
��.exeͬĿ¼���½�һ��in.txt
=======
在.exe同目录下新建一个in.txt
>>>>>>> ae3f9c61b50eb368fd3792aa50de7851689d4dd4

在"in.txt"第一行输入路径

在"in.txt"第二行输入你想查找的文件内容(支持多行)

<<<<<<< HEAD
Ȼ����".exe"�����õ�һ���о�������Ҫ���ļ�·����"out.txt"�ļ�
=======
然后点击".exe"，你会得到一个列举了你想要的文件路径的"out.txt"文件
>>>>>>> ae3f9c61b50eb368fd3792aa50de7851689d4dd4
```


### Something that you should know

You should note that it won't search for files larger than 47M 

Because I designed it just for searching files with the suffix ".txt",".html",".css",".js" and so on.

So I think 47M is big enough.

If you want to search bigger files,you coulde change MAX_SIZE in my code and then generate a new ".exe".

And it won't search files with the suffix ".jpg" ".png" ".gif" ".mp4" ".exe" ".zip" ".dll" ".db"

And it won't search the name of files beacuse I'm so tired that I forget to write this function.

### About the futrue

I spend one day writing this ugly code with few functions.

I spend much times on optimizing efficiency so that it can serach about 200M to 250M at 1 second now.

If I have time , I will use cache mechanism , hash , trie tree and Multithreading .

And I might beautify the code.

And maybe I will make it a software later.

If there is such a if or I won't have this if forever.
