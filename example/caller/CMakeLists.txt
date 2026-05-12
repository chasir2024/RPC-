set(SRC_LIST calluserservice.cc ../user.pb.cc)

add_executable(consume ${SRC_LIST})
target_link_libraries(consume mprpc protobuf)
