
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