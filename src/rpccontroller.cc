#include "rpccontroller.h"
#include "google/protobuf/service.h"

RpcController::RpcController() {
    m_failed = false;
    m_errorText = "";
}

void RpcController::Reset() {
    m_failed = false;
    m_errorText = "";
}

bool RpcController::Failed() const {
    return m_failed;
}

std::string RpcController::ErrorText() const {
    return m_errorText;
}

void RpcController::StartCancel() {
}

void RpcController::SetFailed(const std::string& reason) {
    m_failed = true;
    m_errorText = reason;
}

bool RpcController::IsCanceled() const {
    return false;
}

void RpcController::NotifyOnCancel(google::protobuf::Closure* callback) {
}
