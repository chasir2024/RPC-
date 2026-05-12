# aux_source_directory(. SRC_LIST)
set(SRC_LIST
    rpcprovider.cc
    mprpcapplication.cc
    mprpcchannel.cc
    rpcheader.pb.cc
    mprpcconfig.cc
    logger.cc
)
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base pthread)
