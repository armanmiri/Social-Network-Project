#ifndef POST_H
#define POST_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string> 
#include <algorithm>
#include <cassert>
#include <queue>
#include <unordered_map>
#include <map>
#include <string>

class Post 
{
    public:
       
        // default constructor 
        Post(); 

        Post(std::size_t id, const std::string& message, std::size_t likes); 

        virtual std::string displayPost() const;

        // getter functions
        std::size_t getId() const;
        
        std::string getMessage() const;
        
        std::size_t getLikes() const;

        virtual bool isPublic() const;

    private:
        
        // storages for information
        std::size_t id_;
       
        std::string message_;
       
        std::size_t likes_;

};

#endif //POST_H
