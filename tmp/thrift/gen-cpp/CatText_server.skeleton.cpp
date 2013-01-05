// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "CatText.h"
#include <protocol/TBinaryProtocol.h>
#include <server/TSimpleServer.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;

class CatTextHandler : virtual public CatTextIf {
 public:
  CatTextHandler() {
    // Your initialization goes here
  }

  void LoadResource() {
    // Your implementation goes here
    printf("LoadResource\n");
  }

  int32_t CatNumber(const std::string& doc) {
    // Your implementation goes here
    printf("CatNumber\n");
  }

  void CatName(std::string& _return, const std::string& doc) {
    // Your implementation goes here
    printf("CatName\n");
  }

  void TextFeature(std::string& _return, const std::string& doc) {
    // Your implementation goes here
    printf("TextFeature\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  shared_ptr<CatTextHandler> handler(new CatTextHandler());
  shared_ptr<TProcessor> processor(new CatTextProcessor(handler));
  shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

