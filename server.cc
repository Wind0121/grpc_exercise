#include "gen_code/Helloworld.grpc.pb.h"
#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/support/status.h>
#include <grpcpp/support/sync_stream.h>
#include <iostream>
#include <memory>
#include <string>

using namespace Helloworld;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReaderWriter;

class HelloworldImpl final : public HelloworldService::Service {
public:
  Status GetReply(ServerContext *context, const MsgReq *req,
                  MsgResp *resp) override {
    std::cout << "recv req from " << req->ip() << "\n";
    resp->set_reply("Hello " + req->name());
    return Status::OK;
  }

  Status GetStreamReply(ServerContext* context, ServerReaderWriter<MsgResp,MsgReq>* stream) override{
    MsgReq req;
    MsgResp resp;
    while(stream->Read(&req)){
      std::cout << "recv req from " << req.ip() << "\n";
      resp.set_reply("Hello " + req.name());
      stream->Write(resp);
    }
    return Status::OK;
  }
};

void RunServer() {
  std::string server_addr("0.0.0.0:50959");

  HelloworldImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_addr, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::shared_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_addr << "\n";

  server->Wait();
}

int main() {
  RunServer();
  return 0;
}