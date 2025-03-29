#include "library_initializer.hpp"

namespace se {
LibraryInitializer::LibraryInitializer() : m_wsaData{}
{
#ifdef _WIN32
  if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
    throw std::runtime_error{"Failed to initialize Winsock"};
  }
#endif
}

LibraryInitializer::~LibraryInitializer()
{
#ifdef _WIN32
  WSACleanup();
#endif
}
} // namespace se
