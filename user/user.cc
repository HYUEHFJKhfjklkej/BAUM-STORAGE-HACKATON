#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

void send_file(tcp::socket& socket, const std::string& filename) {
  std::ifstream input_file(filename, std::ios::binary | std::ios::ate);
  if (input_file) {
    std::streamsize size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (input_file.read(buffer.data(), size)) {
      boost::asio::write(socket, boost::asio::buffer(buffer));
    }
  } else {
    std::cerr << "Could not open file!\n";
  }
}

void receive_file(tcp::socket& socket, const std::string& filename) {
  std::ofstream output_file(filename, std::ios::binary | std::ios::trunc);
  boost::asio::streambuf buffer;

  while (boost::asio::read(socket, buffer, boost::asio::transfer_at_least(1))) {
    output_file << &buffer;
  }
}

void send_string(tcp::socket& socket, const std::string& str) {
  boost::asio::write(socket, boost::asio::buffer(str));
}

std::string read_string(tcp::socket& socket) {
  boost::asio::streambuf buffer;
  boost::asio::read_until(socket, buffer, '\n');
  std::istream input_stream(&buffer);
  std::string str;
  std::getline(input_stream, str);
  return str;
}

int main() {
  boost::asio::io_service io_service;

  tcp::resolver resolver(io_service);
  tcp::resolver::query query(tcp::v4(), "localhost", "12345");
  tcp::resolver::iterator iterator = resolver.resolve(query);

  tcp::socket socket(io_service);
  boost::asio::connect(socket, iterator);

  std::string command;
  std::getline(std::cin, command);

  if (command == "send") {
    std::cout << "Enter file path: ";
    std::string filename;
    std::getline(std::cin, filename);
    send_file(socket, filename);
  } else if (command == "receive") {
    std::cout << "Enter output file path: ";
    std::string filename;
    std::getline(std::cin, filename);
    receive_file(socket, filename);
  } else if (command.substr(0, 4) == "get ") {
    std::string name = command.substr(4);
    send_string(socket, name);
    std::cout << "Received: " << read_string(socket) << '\n';
  } else if (command.substr(0, 4) == "set ") {
    std::size_t pos = command.find(' ', 4);
    if (pos != std::string::npos) {
      std::string name = command.substr(4, pos - 4);
      std::string file = command.substr(pos + 1);
      send_string(socket, name + " " + file);
    } else {
      std::cerr << "Invalid command!\n";
    }
  } else {
    std::cerr << "Unknown command!\n";
  }

  return 0;
}
