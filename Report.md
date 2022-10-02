#### 主机连接虚拟机

​	首先查看虚拟机地址：

​			虚拟机终端输入：	

```
					ifconfig
```

​	再打开主机的终端，

​								输入：	

```
					ssh [用户名]@+ip地址
```

​	再输入密码即可连接

#### 实现免密登录

​	生成公钥和私钥：

```
ssh -keygen
```

运行上述命令会生成两个文件 id_rsa（[私钥](https://so.csdn.net/so/search?q=私钥&spm=1001.2101.3001.7020)）、id_rsa.pub（公钥）

将公钥拷贝到要免密登录的机器上（虚拟机）的authorized_keys文件下：

​		这里采用的是直接复制的方式，还没搞清楚怎么解决 ssh-copy-id命令出错的问题

上述配置已经可以实现在本地终端免密登录，但是Vscode的Remote-ssh还需配置.ssh文件夹下的config文件

```
Host lxinyi
  HostName 192.168.91.128
  User lxinyi
  IdentityFile "C:\Users\lxiny\.ssh\id_rsa"
```

**这里犯了一个非常大的错误，把Host和HostName设置反了，浪费了一个小时都没连接上虚拟机**

IdentityFile就是本地密钥的地址

配置完上述文件后，直接打开Vscode的Remote-ssh即可连接到虚拟机了

#### 源代码到可执行文件的过程：

​	预处理：将.c文件变成.i文件或者.ii文件

​	编译：将预处理文件转换为汇编代码文件.s

​	汇编：得到.o机器语言文件

​	链接：将不同源文件产生的目标文件进行链接

#### gcc参数详解

​	编译参数			详细说明

​	-o						指定生成的目标文件，否则会生成a.out文件

​	-S						将预处理之后的文件编译成汇编代码							

​	-E						只执行预处理

​	-c  					  只激活预处理,编译,和汇编,也就是他只把程序做成机器语言文件

​	-I (大写的i)		 指定程序包含的头文件的路径，一般用于指定第三方库的文件

​	-L						编译时，用于指定程序第三方库的查找路径

​	-l (小写的L)		链接时，指定程序需要进行链接的库。注：一般库文件名是libxxx.so，-I指定							   xxx即可。如-Ixxx
​	-g    					打印程序的调试信息，如果需要使⽤gdb⼯具进⾏调试程序，程序编译的时								候，需要加上该参数。
​	-w    					不生成任何的警告信息。
​	-Wall    			   生成所有的警告信息。



#### 多程序编译

```
gcc test1.c test2.c -o test
```

#### 程序检错

```
gcc -Wall test.c -o test
或者
gcc -Werror test.c -o test
```

#### Echo Server 实现过程

##### 服务端（server）

​	1.socket()函数创建sockfd（socket描述符）

​	2.bind()函数将sockfd（套接字）bind到指定的IP和Port

​	3.listen()函数将sockfd从主动转为被动监听模式

​	4.accept()函数接收客户端的连接

​	5.recv()/send()接发收数据

​	6.close()函数关闭socket连接

##### 客户端（client）

​	1.socket()函数创建sockfd（socket描述符）

​	2.connect()函数向服务端发起连接请求

​	3.send()/recv()接发收数据

​	4.close()关闭socket连接

#### Linux基本操作命令

rm 删除文件 （rm -rf +目录 可强制递归删除该目录下所有文件）

ls 查看当前目录下的文件

cp 复制文件到指定路径，例如：

```
cp Readme.md ./test_weekly_reports
```

就是把Readme.md文件复制到当前路径的子文件夹test_weekly_reports中

mkdir 创建新的文件夹

rmdir 删除空的文件夹

mv 移动文件/目录或者重命名文件/目录，例如：

```
mv file_2.txt file_3.txt file_4.txt /home/office/
mv directory_1/ directory_2/
```

​	分别代表移动文件和重命名目录

touch 新建空文件（也可以使用vim a.txt建立，不过此时直接退出不保存不会创建空文件）

cat 显示文件内容

find/locate find可以在指定目录查找文件，locate功能更强大一些，可以找到某些文件存在什么地方

pwd 显示当前目录的绝对路径

#### Git基本操作

```
git init //初始化仓库，生成配置文件.git
git add  //track the file
git commit -m "additional message" //将文件添加到index区(即暂存区)
git clone  //克隆远程仓库
git push //用于从将本地的分支版本上传到远程并合并
git pull //更新本地仓库没有的但是远程仓库有的文件
git remote // 管理远端仓库列表
```

