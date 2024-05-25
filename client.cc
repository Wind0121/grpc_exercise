#include "gen_code/Helloworld.grpc.pb.h"
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/support/status.h>
#include <iostream>
#include <memory>

using namespace Helloworld;
using grpc::ClientContext;
using grpc::Status;

class HelloworldClient {
public:
  HelloworldClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(HelloworldService::NewStub(channel)) {}

  void GetReply() {
    MsgReq req;
    req.set_name("world");
    req.set_ip("192.168.1.53");
    MsgResp resp;

    ClientContext context;
    Status status = stub_->GetReply(&context, req, &resp);

    if(status.ok()){
        std::cout << resp.reply() << "\n";
    }else{
        std::cout << "GetReply rpc failed\n";
    }
  }

private:
  std::shared_ptr<HelloworldService::Stub> stub_;
};

int main() {
  HelloworldClient client(grpc::CreateChannel(
      "192.168.1.53:50959", grpc::InsecureChannelCredentials()));
  client.GetReply();
  return 0;
}