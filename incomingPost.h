#ifndef INCOMINGPOST_H
#define INCOMINGPOST_H

#include "post.h"

class Post;

class IncomingPost : public Post 
{
    private:
       
        // indicates if post is public
        bool isPublic_;

    public:
    
        // defualt constructor
        IncomingPost(); 
        
        IncomingPost(std::size_t id, const std::string& message, std::size_t likes, bool isPublic, const std::string& author); 
        
        std::string displayPost() const override;

        bool isPublic() const override;
};

#endif // INCOMINGPOST_H
