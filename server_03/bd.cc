#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/container/flat_map.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

boost::container::flat_map<std::string, std::string> db;

class session : public std::enable_shared_from_this<session> {
 public:
  explicit session(tcp::socket socket) : socket_(std::move(socket)) {}

  void start() { read_request(); }

 private:
  tcp::socket socket_;
  beast::flat_buffer buffer_;
  http::request<http::string_body> request_;

  void read_request() {
    auto self = shared_from_this();
    http::async_read(
        socket_, buffer_, request_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
          boost::ignore_unused(bytes_transferred);
          if (!ec) self->process_request();
        });
  }

  void process_request() {
    std::stringstream ss(request_.body());
    std::string command, hash, file;
    ss >> command >> hash;

    if (command == "get")
      send_response(handle_get(hash));
    else if (command == "set") {
      ss >> file;
      handle_post(hash, file);
    } else
      send_response(http::response<http::string_body>(http::status::bad_request,
                                                      request_.version()));
  }

  http::response<http::string_body> handle_get(const std::string& hash) {
    auto it = db.find(hash);
    if (it == db.end())
      return http::response<http::string_body>(http::status::not_found,
                                               request_.version());

    http::response<http::string_body> res;
    res.body() = it->second;
    res.prepare_payload();
    return res;
  }

  void handle_post(const std::string& hash, const std::string& file) {
    db[hash] = file;
    send_response(http::response<http::string_body>(http::status::ok,
                                                    request_.version()));
  }

  void send_response(http::response<http::string_body> response) {
    auto self = shared_from_this();
    http::async_write(
        socket_, response,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
          boost::ignore_unused(bytes_transferred);
          self->socket_.shutdown(tcp::socket::shutdown_send, ec);
        });
  }
};

int main() {
  try {
    auto const address = net::ip::make_address("0.0.0.0");
    auto const port = static_cast<unsigned short>(std::atoi("8080"));

    net::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};
    for (;;) {
      tcp::socket socket{ioc};
      acceptor.accept(socket);
      std::make_shared<session>(std::move(socket))->start();
    }
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
