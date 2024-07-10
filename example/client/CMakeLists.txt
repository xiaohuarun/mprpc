set(SRC mathclient.cpp ../mathemat.pb.cc)

add_executable(client ${SRC})
target_link_libraries(client mprpc protobuf)