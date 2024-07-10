set(SRC mathservice.cpp ../mathemat.pb.cc)

add_executable(rpcserver ${SRC})
target_link_libraries(rpcserver mprpc protobuf)