#pragma once
#include "rpccontroller.h"
#include "google/protobuf/service.h"

class RpcController: public google::protobuf::RpcController
{

public:
    RpcController();
    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void StartCancel();
    void SetFailed(const std::string& reason);
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback);
private:
    bool m_failed = false;
    std::string m_errorText;

};


// class PROTOBUF_EXPORT RpcController {
//  public:
//   inline RpcController() {}
//   virtual ~RpcController();
//   virtual void Reset() = 0;
//   virtual bool Failed() const = 0;
//   virtual std::string ErrorText() const = 0;
//   virtual void StartCancel() = 0;
//   virtual void SetFailed(const std::string& reason) = 0;

//   // If true, indicates that the client canceled the RPC, so the server may
//   // as well give up on replying to it.  The server should still call the
//   // final "done" callback.
//   virtual bool IsCanceled() const = 0;
//   virtual void NotifyOnCancel(Closure* callback) = 0;

// };
