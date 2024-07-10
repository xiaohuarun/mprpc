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