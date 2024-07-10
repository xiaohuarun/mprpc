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
$ ./autogen.sh               # 自动生成 configure 配置文件
$ ./configure                # 配置编译环境
$ make -j32                  # 编译
$ sudo make install     	 # 拷贝头文件，安装库文件
$ sudo ldconfig              # 刷新系统可用动态库
```
- ZooKeeper
```bash
 $ sudo apt-get install default-jre
$ git clone git@github.com:apache/zookeeper.git
$ cd zookeeper/conf
$ cp zoo_sample.cfg zoo.cfg  # zookeeper 配置文件
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

    ```bash
    git clone git@github.com:xiaohuarun/mprpc.git
    ./autobuild.sh
    ```

