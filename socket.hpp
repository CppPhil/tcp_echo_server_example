#pragma once
#include <cstddef>

#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

namespace se {
using SocketType = SOCKET;
constexpr SocketType invalidSocketHandle{INVALID_SOCKET};
} // namespace se
#else

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace se {
using SocketType = int;
constexpr SocketType invalidSocketHandle{-1};
} // namespace se
#endif

namespace se {
class Socket {
public:
  Socket();

  // Delete copy constructor and copy assignment
  Socket(const Socket&)            = delete;
  Socket& operator=(const Socket&) = delete;

  // Move constructor
  Socket(Socket&& other) noexcept;

  // Move assignment operator
  Socket& operator=(Socket&& other) noexcept;

  ~Socket();

  bool isValid() const;

  SocketType getHandle() const;

  void bind(int port);

  void listen(int backlog);

  Socket accept();

  void write(const void* buffer, std::size_t length);

  std::string readLine();

private:
  void readImpl(void* buffer, std::size_t length);

  void close();

  SocketType m_handle;
};
} // namespace se
