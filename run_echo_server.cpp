#include <iostream>
#include <string>
#include <thread>

#include "library_initializer.hpp"
#include "run_echo_server.hpp"
#include "socket.hpp"

namespace se {
void runEchoServer(int port)
{
  try {
    LibraryInitializer libraryInitializer{};
    Socket             serverSocket{};
    serverSocket.bind(port);
    serverSocket.listen(5);

    std::cout << "TCP Echo server is listening on port " << port << '\n';

    for (;;) {
      Socket clientSocket{serverSocket.accept()};
      std::cout << "Client connected\n";

      std::thread thread([clientSocket = std::move(clientSocket)]() mutable {
        try {
          for (;;) {
            std::string buffer{clientSocket.readLine()};
            clientSocket.write(buffer.c_str(), buffer.size());
          }
        }
        catch ([[maybe_unused]] const std::runtime_error& exception) {
          return;
        }
      });
      thread.detach();
    }
  }
  catch (const std::runtime_error& exception) {
    std::cerr << "Exception: " << exception.what() << '\n';
  }
}
} // namespace se
