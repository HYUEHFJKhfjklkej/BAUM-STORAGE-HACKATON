#include <fstream>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <memory>
#include <utility>
#include <boost/asio/streambuf.hpp>


#include "file_handler.hpp"
#include "db_interface.hpp"

using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    do_read();
  }

  

public:
  void do_read()
  {
    auto self(shared_from_this());
    boost::asio::async_read_until(socket_, buffer_, "\0",
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
          if (!ec) {
            std::string data(boost::asio::buffers_begin(buffer_.data()),
                 boost::asio::buffers_end(buffer_.data()));

// Очистка буфера
            buffer_.consume(buffer_.size());
            data_ = data;
            // std::cout << data_;

// Использование полученной строки
            // std::cout << "Received data: " << data << std::endl;
            handler();
            // std::cout.write(
            //     boost::asio::buffer_cast<const char*>(buffer_.data()),
            //     buffer_.size());
            // buffer_.consume(buffer_.size());
            // do_read();
            
            
            
          }
        });
  }

  void handler()
  {
    // auto self(shared_from_this());
    std::vector<std::string> mess = split(data_);
    
    std::cout << data_;
    
    if (mess[0] == "get"){
      FileHandler fh(mess[1]);
      std::string hash = fh.GetHash();
      std::pair<int, std::vector<std::string>> file_blocks = db.GetFile(hash);
      std::string file;
      if (file_blocks.first == -1){
        file = fh.GetFullFile(file_blocks.second);
      } else {
        file = fh.GetFileParity(file_blocks.second, file_blocks.first);
      }
      socket_.write_some(boost::asio::buffer(db.str_to_vec(file)));
      // std::cout << "get" << mess[1];
    }else{
      FileHandler fh(mess[1], mess[2]);
      std::string hash = fh.GetHash();
      // std::cout << "set " << mess[2];
      std::vector<std::string> blocks = fh.GetBlocks();
      std::string parity = fh.GetParity();
      blocks.push_back(parity);
      db.SetFile(hash, blocks);
    }
    std::cout.flush();
  }

  std::vector<std::string> split(std::string str){
    std::vector<std::string> result(3);
    std::stringstream ss (str);

    std::string word;
    int i = 0;
    while (ss >> word){
      if (i == 0){
        result[i] = word;
        
      }
      if (i == 1){
        result[i] = word;
      }
      if (i > 1){
        result[2].append(word);
      }
      i++;
    }
    

    return result;
  }



  tcp::socket socket_;
  enum { max_length = 1024 };
  // char data_[max_length];
  DbInterface db;
  std::string data_;
  boost::asio::streambuf buffer_;
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            std::make_shared<session>(std::move(socket))->start();
          }

          do_accept();
        });
  }

  tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
  try
  {
    // if (argc != 2)
    // {
    //   std::cerr << "Usage: async_tcp_echo_server <port>\n";
    //   return 1;
    // }

    boost::asio::io_context io_context;

    server s(io_context, std::atoi("1488"));

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

