## Project Overview

This project is a microservices RPC framework based on Muduo, Protobuf, and ZooKeeper. 
Service providers can register RPC nodes and publish services through the Mprpc framework.
Clients can discover these services via the ZooKeeper service configuration center and make remote method calls.

In industrial scenarios, for example, if the server is not networked and you need an RPC service to simplify the development process, 
you can opt not to use ZooKeeper.

## Project Build

Install Dependencies

- Protobuf
```bash
$ sudo apt-get install autoconf automake libtool curl make g++
$ git clone git@github.com:protocolbuffers/protobuf.git
$ cd protobuf
$ ./autogen.sh               
$ ./configure               
$ make -j32                 
$ sudo make install     	 
$ sudo ldconfig              
```
- ZooKeeper
```bash
 $ sudo apt-get install default-jre
$ git clone git@github.com:apache/zookeeper.git
$ cd zookeeper/conf
$ cp zoo_sample.cfg zoo.cfg  
$ vim zoo.cfg                # chance dataDir=[]
$ cd ../bin
$ ./zkServer.sh start        # activate zookeeper service

# 编译 c++ API
$ cd zookeeper/zookeeper-client/zookeeper-client-c
$ sudo ./configure
$ sudo make -j32
$ sudo make install
```
- Muduo
```bash
git clone https://github.com/chenshuo/muduo.git
cd muduo
mkdir build && cd build
cmake ..
make
sudo make install
```

After installing the dependencies, clone the project repository and run the build script:The generated header files and dynamic libraries will be placed in the lib directory.

```bash
git clone git@github.com:xiaohuarun/mprpc.git
./autobuild.sh
 ```
** tip**
**If you can't link to zookeeper's dynamic libraries, go to /usr/local/lib, check the name of the library that reported the error, and then add it to your system environment variables.**

# Framework Design
**Design Concept**
Protobuf already provides a code framework for RPC service method invocation, eliminating the need to implement a service interface description. The focus is on RPC service management and the RPC communication process. Additionally, Protobuf uses binary serialization for more efficient communication. Therefore, the initial framework design based on Protobuf is as follows: the blue color within the RPC service indicates the components already provided by Protobuf, and the RPC framework is built upon these components.

![RPC Framework Design](https://github.com/xiaohuarun/mprpc/blob/main/image/rpc.jpg)

**Service Mapping Table:** Records the local service names and service objects published by service providers. When a client initiates a call, it inputs the RPC method name, searches this table, retrieves the service object, and invokes it.

**Service Registration Middleware:** Service providers register published services. Clients obtain the URLs of services and methods from the service registration middleware to locate communication targets.

**Network Middleware:** Sending RPC requests and returning service invocation responses requires network middleware to facilitate communication between clients and servers.

**Communication Protocol:** When clients invoke remote RPC services, they need to provide the service name, method name, and method input parameters. Therefore, clients need to package these parameters in advance according to the agreed communication protocol.

![RPC process](https://github.com/xiaohuarun/mprpc/blob/main/image/RPC_process.jpg)


**Service Mapping Table:** Implemented by the provider class publicService, which publishes local services as RPC services.

**Service Registration Middleware:** Utilizes ZooKeeper's service registration and discovery features. It offers two advantages: 1) Decouples clients from servers, eliminating the need for manual modification of service URL configuration files by clients. 2) Implements heartbeat detection and service coordination between providers and the service center, ensuring real-time monitoring and updating of service URLs.

**Network Middleware:** Uses the muduo network library for communication. The muduo library adopts a design of one event loop per thread, enabling concurrent event loops across multiple threads. Each event loop utilizes a non-blocking + epoll I/O model for high performance.

**Communication Protocol:** Uses a design of head_size + head_str + request_str to avoid TCP encapsulation issues. head_str includes request size information to mitigate TCP encapsulation problems.

**Here is the exact process：**
1.Begin the service and register it with ZooKeeper.
Client Initiates Call:

2.The client initiates an RPC call.
Establish Connection:

3.Establish a connection between the client and the server.
Server Receives Data:

4.The server receives the incoming data.
Server Unpacks Data:

5.Unpack the received data on the server side.
Server Binds Callback:

6.Bind a callback function to handle the request.
Server Calls Method:

7.Execute the corresponding method on the server.
Server Returns Response:

8.Provide a response back to the client.

## Example Usage

**Detailed examples are in the project examples**

```c++
syntax = "proto3";

package math;

option cc_generic_services = true;

message AddNum {
    int32 num_first = 1;
    int32 num_second = 2;
}

message resultNum {
    int32 num_result = 1;
}

service addition {
    rpc GetAdd(AddNum) returns (resultNum);
}
```
Publish Rpc service:mathservice.cpp

```c++

#include "mathemat.pb.h"
#include "mprpcapplication.h"
#include "mprpcprovider.h"
#include "logger.h"

class Maths : public math::addition{
public:
    uint32_t add(uint32_t num1,uint32_t num2){
        return (num1+num2);
    }
 void GetAdd(::google::protobuf::RpcController *controller,
                       const ::math::AddNum* request,
                       ::math::resultNum* response,
                       ::google::protobuf::Closure* done)
                    {   

                        uint32_t num1 = request->num_first();
                        uint32_t num2 = request->num_sencond();

                        uint32_t result = add(num1, num2);

                        response->set_num_result(result);

                        done->Run();

                    }
};

int main(int argc, char **argv){

    LOG_INFO("Start execution of the computational addition RPC service");

    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象
    RpcProvider provider;
    provider.NotifyService(new Maths());

    // 启动一个rpc服务发布节点  Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();
    return 0;
}
```

Publishing client calls to Rpc services:mathclient.cpp
```c++
#include "mathemat.pb.h"
#include "mprpcapplication.h"
#include "logger.h"

int main(int argc, char **argv) {

  LOG_INFO("this is a action to add ");


  MprpcApplication::Init(argc, argv);

  // 调用远程发布的 rpc 方法，必须传入一个 ::google::protobuf::RpcChannel* 来构造代理对象
  math::addition_Stub stub(new MprpcChannel());

  math::AddNum request;
  request.set_num_first(1);
  request.set_num_sencond(1);

  math::resultNum response;
  
  // 控制信息对象
  MprpcController controller;
  //  rpc 调用过程
  stub.GetAdd(&controller, &request, &response, nullptr);

  if (controller.Failed()) {
    std::cout << controller.ErrorText() << '\n';
  } else {
    // 一次 rpc 调用完成，读取 response 结果，返回值的意义由用户设定
    std::cout<< "rpc add success! \n";
    std::cout << "Result = " << response.num_result() << std::endl;
    return 0;
  }
  return 0;
}
```

Here is the formatted Markdown text for your content:

Mathematical Journal
You can use 4 types of log messages. Log File Naming Format:
```c++
#include "logger.h"

int a = 2024;
std::string log = "hello, world!";

LOG_DEBUG(" log debug: %d, %s", a, log);
LOG_INFO(" log info: %d, %s", a, log);
LOG_ERROR(" log error: %d, %s", a, log);
LOG_FATAL(" log fatal: %d, %s", a, log);
```
The format of the configuration file, with five configuration parameters, is as follows
```txt
# rpc node ip address
rpcserverip=127.0.0.1        
# port port number of the rpc node
rpcserverport = 8001
# ip address of the zk
zookeeperip = 127.0.0.1  
#port port number of the zk
zookeeperport = 2181
#Worker thread for moduo
workerthread =4
```
测试
```c++
$ cd bin
# rpc 服务提供方
$ ./rpcserver -i example.conf
# rpc 服务调用方
$ ./client -i example.conf
```

