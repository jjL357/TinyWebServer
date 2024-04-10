#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <unordered_map>
#include <fcntl.h>       // fcntl()
#include <unistd.h>      // close()
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "epoller.h"
#include "../timer/heaptimer.h"

#include "../log/log.h"
#include "../pool/sqlconnpool.h"
#include "../pool/threadpool.h"

#include "../http/httpconn.h"

class WebServer {
public:
    WebServer(
        int port, int trigMode, int timeoutMS, bool OptLinger, 
        int sqlPort, const char* sqlUser, const  char* sqlPwd, 
        const char* dbName, int connPoolNum, int threadNum,
        bool openLog, int logLevel, int logQueSize); // 构造函数: 设置服务器参数　＋　初始化定时器／线程池／反应堆／连接队列


    ~WebServer(); //析构函数: 关闭listenFd_，　销毁　连接队列/定时器／线程池／反应堆
    void Start();
    /*
    创建端口，绑定端口，监听端口，　创建epoll反应堆，　将监听描述符加入反应堆

    等待事件就绪

    连接事件－－＞DealListen()

    写事件－－＞DealWrite()

    读事件－－＞DealRead()

    事件处理完毕，修改反应堆，再跳到２处循环执行
    */

private:
    bool InitSocket_();  // 初始化Socket连接
    void InitEventMode_(int trigMode); // 初始化事件模式
    void AddClient_(int fd, sockaddr_in addr); 
  
    void DealListen_(); // 新初始化一个ＨttpConnection对象
    void DealWrite_(HttpConn* client); // DealWrite：　对应连接对象进行处理－－＞若处理成功，则监听事件转换成　读　事件 
    void DealRead_(HttpConn* client); // DealRead：　 对应连接对象进行处理－－＞若处理成功，则监听事件转换成　写　事件

    void SendError_(int fd, const char*info);
    void ExtentTime_(HttpConn* client);
    void CloseConn_(HttpConn* client);

    void OnRead_(HttpConn* client);
    void OnWrite_(HttpConn* client);
    void OnProcess(HttpConn* client);

    static const int MAX_FD = 65536;

    static int SetFdNonblock(int fd);

    int port_; 
    bool openLinger_; // 优雅关闭选项
    int timeoutMS_;  /* 毫秒MS */  // 定时器的默认过期时间
    bool isClose_; // 服务启动标志
    int listenFd_; // 监听文件描述符
    char* srcDir_; // 需要获取的路径
    
    uint32_t listenEvent_;  // 监听事件 初始监听描述符监听设置
    uint32_t connEvent_;    // 连接事件 初始连接描述符监听设置
   
    std::unique_ptr<HeapTimer> timer_; // 定时器
    std::unique_ptr<ThreadPool> threadpool_; // 线程池
    std::unique_ptr<Epoller> epoller_; // 反应堆
    std::unordered_map<int, HttpConn> users_; //连接队列
};

#endif //WEBSERVER_H
