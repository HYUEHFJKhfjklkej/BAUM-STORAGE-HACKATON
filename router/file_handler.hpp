#ifndef FILE_HANDLER_H
#define FILE_HANDLER H

#include <string>
#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

#include "lib/Recover.hpp"

class FileHandler{
public:
    FileHandler(std::string filename, std::string data) : filename(filename), data_(data){}
    FileHandler(std::string filename) : filename(filename){}

    std::string GetFileParity(std::vector<std::string> par_block, int index){


        storage::utils::FileRecover tst(par_block[0], index, !index, StrToByte(par_block[1]));
        return tst.read_strip_data();
    }

    std::vector<std::byte> StrToByte (std::string str){
        std::vector<std::byte> res;
        for (int i = 0; i < str.size(); i++){
            res.push_back(static_cast<std::byte>(str[i]));
        }
        return res;
    }
    
    std::vector<std::string> GetBlocks(){
        storage::utils::FileRecover fl(data_);

        auto m = fl.get_strip_file();      

        return m; 
    }

    std::string GetParity(){
        storage::utils::FileRecover fl(data_);
        auto vec = fl.get_parity_data();
        return VecToStr(vec);
    }

    std::string VecToStr(std::vector<std::byte> vec){
        std::string str;
        for (int i = 0; i < vec.size(); i++){
            str.push_back(static_cast<char>(vec[i]));
        }
        return str;
    }

    std::string GetFullFile(std::vector<std::string> blocks){

        storage::utils::FileRecover tst(blocks);

        return tst.read_strip_data();

    }

    std::string GetHash(){
        boost::uuids::detail::md5 hash;
        boost::uuids::detail::md5::digest_type digest;

        hash.process_bytes(filename.data(), filename.size());
        hash.get_digest(digest);

        return toString(digest);
    }
    
    std::string toString(const boost::uuids::detail::md5::digest_type &digest)
    {
        const auto charDigest = reinterpret_cast<const char *>(&digest);
        std::string result;
        boost::algorithm::hex(charDigest, charDigest + sizeof(boost::uuids::detail::md5::digest_type), std::back_inserter(result));
        return result;
    }

    std::string data_;
    std::string filename;
    
};

#endif