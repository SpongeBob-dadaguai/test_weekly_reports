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

#### 本周遇到的问题以及解决方式(印象比较深的错误)

##### 1.读取字符串的时候，首先采用了scanf("%s",msg);的方式，但是发现遇到空白符号，例如空格，就是输入“who are you”的时候，scanf只会将who读到msg中，其他的字符则存储在缓冲区，这时我就打算使用gets进行读取，但是linux已经禁止这种危险的方式，最后我学习到了scanf("%[^n]", msg)的方式，可以选择结束符号，这种方式就可以解决读入的问题了，但是这个方式会将\n存储在缓冲区，需要使用getchar()将这个缓冲区字符读出。

##### 2.先贴一段代码：

```c
for(int i = 0; i < 5; i++) {
	printf("%d ", i);
	sleep(1);
}
```

###### 我认为这段代码执行结果应该是从0到4打印，每两个数之间的打印时间间隔为1s，总耗时5s，但是结果却是，等待了5s之后，所有数字一起打印，这时候我感觉很奇怪，感觉逻辑上没有问题，但是结果不对。我首先想到了缓冲区的问题，是不是字符全部在缓冲区，没有打印出来，因此我将上面的代码修改为：

```c
for(int i = 0; i < 5; i++) {
	printf("%d ", i);
	fflush(stdout);  //强行将缓冲区的字符打印到终端
	sleep(1);
}
```

###### 这时候的代码运行结果与预想的一样，间隔一秒打印一个数字。

###### 究其原因：发现linux系统的printf会将所有需要打印到终端的数据存入缓冲区，只有遇到换行符的时候才会打印到终端，但是上面的代码printf("%d ", i) 没有换行符，所以会一直等到程序结束才打印。

###### 那么解决方式就有：

```c
1.printf("%d\n", i);  //添加换行符

2.fflsuh(stdout); //刷新缓冲区

3.setvbuf(stdout, NULL, _IONBF, 0); //禁用缓冲区
```

#### 本周主要学习过程：

##### Linux系统下的多线程：

###### 	在我学习并实现echo server的时候，发现并不能实现多个客户端同时连接到一个服务器，而且客户端一旦连接到服务器，服务器就要一直等待客户端传输数据，无法进行其他操作，但是Linux的多线程处理就可以解决这个问题。

###### 	首先，我对于多线程的理解，就是将服务器或者本地资源分配到多个同时进行的操作，或者说，将每一个线程简单理解为一个函数，这些函数不再像单线程那样要一个一个执行，而是同时进行操作，最大限度地利用资源，减少时间。至于echo server的多线程实现方式，我采用的是链表的结构，每当有一个新的客户端链接进来，新增链表节点，并将这个客户端的线程分出去，main主程序线程不受客户端线程的影响，而且我将main线程的终止程序放到单独的线程——quit，让main线程等待quit线程结束之后再结束，就是说，可以随时输入终止命令结束main线程，下面是一些函数：

```c
pthread_t new_thread;  //创建一个线程结构体
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
//创建一个新的线程，第一个参数是类型为pthread_t的变量的地址，第二个参数是创建线程的属性，一般来说不用修改，设置为NULL即可，第三个参数是该线程要执行的函数，第四个参数就是函数的参数
int pthread_detach(pthread_t thread)  //将线程分离出来
int pthread_join(pthread_t thread, void **retval)  //将thread线程加入到当前线程，即当前线程需要等待thread线程结束之后才能运行thread之后的部分，第二个参数是thread结束后的返回值
int pthread_exit(void **retval) //结束当前进程
int pthread_equal(pthread_t t1, pthread_t t2);//判断两个线程是否相同
pthread_t pthread_self(void) //返回当前线程的ID
```

##### Linux I/O复用：

###### select

###### 优点：操作简单，对于小规模的用户连接较为方便；

###### 缺点：连接数量受限，一般是1024个，而且每次查找活跃的客户端时，都需要循环遍历，时间耗费非常大

```c
fd_set fds;  //创建文件描述符集合
FD_ZERO(fd_set *fdset);     // 清除fdset所有标志位   
FD_SET(int fd, fd_set fdset);       // 设置fdset标志位fd   
FD_CLR(int fd, fd_set fdset);       // 清除fdset标志位fd   
int FD_ISSET(int fd, fd_set *fdset);    // 判断fdset的位fd是否被设置
```
###### poll

###### poll的实现原理和select类似，不过不限制文件描述符的个数
```c
struct pollfd  
{  
    int fd;         // 文件描述符  
    short events;       // 注册的事件   
    short revents;      // 实际发生的事件，有内核填充  
};  
int poll(struct pollfd *fds, nfds_t nfds, int timeout); //返回活跃的文件描述符的个数
```

##### epoll
###### epoll与poll以及select有很大的区别，在客户端数量非常多的情况下，epoll的处理效率非常高，主要是因为，epoll只关心活跃的文件描述符

```c
int epoll_create(int size)
// epoll把用户关心的文件描述符上的事件放在内核上的一个事件表中，从而无须像select和poll那样每次调用都要重复传入文件描述符集合事件表。但epoll需要使用一个额外的文件描述符，来唯一标识内核中这个事件表，这个文件描述符使用如下epoll_create函数创建：

int epoll_ctl(int opfd, int op, int fd, struct epoll_event *event);
//对内核的事件表进行操作，其中op是操作方式，包括：
// EPOLL_CTL_ADD 添加文件描述符
// EPOLL_CTL_MOD 修改事件表上的事件，比如将输入EPOLLIN改为输出EPOLLOUT
// EPOLL_STL_DEL 删除事件表上的事件
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);  
//返回活跃的文件描述符个数，后面就只用关心这些文件描述符，从而节省遍历的时间