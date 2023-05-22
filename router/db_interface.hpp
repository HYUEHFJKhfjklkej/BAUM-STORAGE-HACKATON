#ifndef FILE_HANDLER_H
#define FILE_HANDLER H

#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <iostream>
#include <sstream>

using boost::asio::ip::tcp;
class DbInterface{
public:

    DbInterface() = default;

    std::pair<int, std::vector<std::string>> GetFile(std::string hash){
        std::vector<std::string> data;
        int error_check = -1;
        for (int i = 0; i < disk_numbers; i++){
            data.push_back((i, hash));
            if (data[i] == "Error"){
                error_check = i;
                data.pop_back();
            }
        }
        if (error_check != -1){
            data.push_back(GetBlock(2, hash));
        }
        return std::make_pair(error_check, data);
        
    }

    std::string GetBlock(int num, std::string hash){
        try {
            boost::asio::io_service io_service;

            tcp::resolver resolver(io_service);
            tcp::resolver::query query(tcp::v4(), ip[num], port);
            tcp::resolver::iterator iterator = resolver.resolve(query);

            tcp::socket socket(io_service);
            boost::asio::connect(socket, iterator);
            std::string request = std::string("GET ") + hash;
            std::vector<char> buffer = str_to_vec(request);
            boost::asio::write(socket, boost::asio::buffer(buffer));

            boost::asio::streambuf buffer_r;
            std::stringstream ss;

            while (boost::asio::read(socket, buffer_r, boost::asio::transfer_at_least(1))) {
                ss << &buffer;
            }
            return ss.str();

        } catch (...) {
            return "Error";
        }
    }

    std::vector<char> str_to_vec(std::string& str){
        std::vector<char> res;
        for (size_t i = 0; i < str.size(); i++){
            res.push_back(str[i]);
        }
        return res;
    }

    void SetFile(std::string hash, std::vector<std::string> data){
        for (int i = 0; i < disk_numbers + 1; i++){
            std::string ret = SetBlock(hash, data[i], i);
        }
    }

    std::string SetBlock(std::string hash, std::string block, int index){
        try {
            boost::asio::io_service io_service;

            tcp::resolver resolver(io_service);
            tcp::resolver::query query(tcp::v4(), ip[index], port);
            tcp::resolver::iterator iterator = resolver.resolve(query);

            tcp::socket socket(io_service);
            boost::asio::connect(socket, iterator);
            std::string request = std::string("SET ") + hash + block;
            std::vector<char> buffer = str_to_vec(request);
            boost::asio::write(socket, boost::asio::buffer(buffer));

            boost::asio::streambuf buffer_r;
            std::stringstream ss;


        } catch (...) {
            return "Error";
        }
    }

    std::vector<std::string> ip = {"172.17.0.5", "172.17.0.6", "172.17.0.7"};
    std::string port = "8080";
    size_t disk_numbers = 2;
};

#endif