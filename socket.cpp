#include "socket.hpp"

namespace se {
Socket::Socket() : m_handle{::socket(AF_INET, SOCK_STREAM, 0)}
{
  if (m_handle == invalidSocketHandle) {
    throw std::runtime_error{"Failed to create socket"};
  }
}

Socket::Socket(Socket&& other) noexcept : m_handle{other.m_handle}
{
  other.m_handle = invalidSocketHandle;
}

Socket& Socket::operator=(Socket&& other) noexcept
{
  SocketType temp{other.m_handle};
  other.m_handle = invalidSocketHandle;
  close();
  m_handle       = temp;
  return *this;
}

Socket::~Socket()
{
  close();
}

bool Socket::isValid() const
{
  return m_handle != invalidSocketHandle;
}

SocketType Socket::getHandle() const
{
  return m_handle;
}

void Socket::bind(int port)
{
  sockaddr_in addr{};
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (::bind(m_handle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    throw std::runtime_error{"Failed to bind socket"};
  }
}

void Socket::listen(int backlog)
{
  if (::listen(m_handle, backlog) < 0) {
    throw std::runtime_error{"Failed to listen on socket"};
  }
}

Socket Socket::accept()
{
  sockaddr_in clientAddress{};
  socklen_t   clientAddressLength{sizeof(clientAddress)};
  Socket      clientSocket{};

  clientSocket.m_handle = ::accept(
    m_handle,
    reinterpret_cast<sockaddr*>(&clientAddress),
    &clientAddressLength);
  if (clientSocket.m_handle == invalidSocketHandle) {
    throw std::runtime_error("Failed to accept connection");
  }

  return clientSocket;
}

void Socket::write(const void* buffer, std::size_t length)
{
  std::size_t totalBytesWritten{0};

  while (totalBytesWritten < length) {
    std::size_t bytesWritten{static_cast<std::size_t>(::send(
      m_handle,
      static_cast<const char*>(buffer) + totalBytesWritten,
      static_cast<int>(length - totalBytesWritten),
      0))};

    if (bytesWritten <= 0) {
      throw std::runtime_error{"Failed to send data"};
    }

    totalBytesWritten += bytesWritten;
  }
}

std::string Socket::readLine()
{
  std::string line{};
  char        c{0};

  while (c != '\n') {
    readImpl(&c, sizeof(c));
    line += c;
  }

  return line;
}

void Socket::readImpl(void* buffer, std::size_t length)
{
  size_t totalBytesRead{0};

  while (totalBytesRead < length) {
    std::size_t bytesRead{static_cast<std::size_t>(::recv(
      m_handle,
      static_cast<char*>(buffer) + totalBytesRead,
      static_cast<int>(length - totalBytesRead),
      0))};

    if (bytesRead <= 0) {
      throw std::runtime_error{"Failed to receive data"};
    }

    totalBytesRead += bytesRead;
  }
}

void Socket::close()
{
  if (isValid()) {
#ifdef _WIN32
    ::closesocket(m_handle);
#else
    ::close(m_handle);
#endif

    m_handle = invalidSocketHandle;
  }
}
} // namespace se
