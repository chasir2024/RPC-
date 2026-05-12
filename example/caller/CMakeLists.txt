#set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callfriendservice.cc ../friend.pb.cc)

add_executable(consume ${SRC_LIST})
target_link_libraries(consume mprpc protobuf)
