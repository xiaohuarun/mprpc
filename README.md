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
$ vim zoo.cfg                # 更改 dataDir=[你的存储路径]
$ cd ../bin
$ ./zkServer.sh start        # 启动 zookeeper 服务器

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
### tip:If you can't link to zookeeper's dynamic libraries, go to /usr/local/lib, check the name of the library that reported the error, and then add it to your system environment variables.

