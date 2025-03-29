#pragma once
#include <stdexcept>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

namespace se {
class LibraryInitializer {
public:
  LibraryInitializer();

  // Delete copy constructor and copy assignment
  LibraryInitializer(const LibraryInitializer&)            = delete;
  LibraryInitializer& operator=(const LibraryInitializer&) = delete;

  ~LibraryInitializer();

private:
#ifdef _WIN32
  WSADATA m_wsaData;
#endif
};
} // namespace se
