
#ifndef QASIOSOCKET_MYASIOTcp_H
#define QASIOSOCKET_MYASIOTcp_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/asio/basic_socket_acceptor.hpp>
#include <boost/asio/basic_socket_iostream.hpp>
#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/detail/socket_option.hpp>
#include <boost/asio/detail/socket_types.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/basic_resolver.hpp>
#include <boost/asio/ip/basic_resolver_iterator.hpp>
#include <boost/asio/ip/basic_resolver_query.hpp>

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace ip {

class Tcp
{
public:
  /// The type of a Tcp endpoint.
  typedef basic_endpoint<Tcp> endpoint;

  /// Construct to represent the IPv4 Tcp protocol.
  static Tcp v4()
  {
    return Tcp(BOOST_ASIO_OS_DEF(AF_INET));
  }

  /// Construct to represent the IPv6 Tcp protocol.
  static Tcp v6()
  {
    return Tcp(BOOST_ASIO_OS_DEF(AF_INET6));
  }

  static Tcp all()
   {
     return Tcp(BOOST_ASIO_OS_DEF(AF_UNSPEC));
   }

  /// Obtain an identifier for the type of the protocol.
  int type() const
  {
    return BOOST_ASIO_OS_DEF(SOCK_STREAM);
  }

  /// Obtain an identifier for the protocol.
  int protocol() const
  {
    return BOOST_ASIO_OS_DEF(IPPROTO_TCP);
  }

  /// Obtain an identifier for the protocol family.
  int family() const
  {
    return family_;
  }

  /// The Tcp socket type.
  typedef basic_stream_socket<Tcp> socket;

  /// The Tcp acceptor type.
  typedef basic_socket_acceptor<Tcp> acceptor;

  /// The Tcp resolver type.
  typedef basic_resolver<Tcp> resolver;

#if !defined(BOOST_ASIO_NO_IOSTREAM)
  /// The Tcp iostream type.
  typedef basic_socket_iostream<Tcp> iostream;
#endif // !defined(BOOST_ASIO_NO_IOSTREAM)

  /// Socket option for disabling the Nagle algorithm.
  /**
   * Implements the IPPROTO_Tcp/Tcp_NODELAY socket option.
   *
   * @par Examples
   * Setting the option:
   * @code
   * boost::asio::ip::Tcp::socket socket(io_service);
   * ...
   * boost::asio::ip::Tcp::no_delay option(true);
   * socket.set_option(option);
   * @endcode
   *
   * @par
   * Getting the current option value:
   * @code
   * boost::asio::ip::Tcp::socket socket(io_service);
   * ...
   * boost::asio::ip::Tcp::no_delay option;
   * socket.get_option(option);
   * bool is_set = option.value();
   * @endcode
   *
   * @par Concepts:
   * Socket_Option, Boolean_Socket_Option.
   */
#if defined(GENERATING_DOCUMENTATION)
  typedef implementation_defined no_delay;
#else
  typedef boost::asio::detail::socket_option::boolean<
    BOOST_ASIO_OS_DEF(IPPROTO_TCP), BOOST_ASIO_OS_DEF(TCP_NODELAY)> no_delay;
#endif

  /// Compare two protocols for equality.
  friend bool operator==(const Tcp& p1, const Tcp& p2)
  {
    return p1.family_ == p2.family_;
  }

  /// Compare two protocols for inequality.
  friend bool operator!=(const Tcp& p1, const Tcp& p2)
  {
    return p1.family_ != p2.family_;
  }

private:
  // Construct with a specific family.
  explicit Tcp(int protocol_family)
    : family_(protocol_family)
  {
  }

  int family_;
};

} // namespace ip
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_IP_Tcp_HPP
