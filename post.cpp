#include "post.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string> 
#include <algorithm>
#include <cassert>
#include <queue>
#include <unordered_map>
#include <map>

// Pre: none
// Post: creates post object
Post::Post() : id_(0), message_(""), likes_(0) {}

// Pre: Valid input parameters
// Post: Creates post object with given parameters
Post::Post(std::size_t id, const std::string& message, std::size_t likes): id_(id), message_(message), likes_(likes) {}

// Pre: None
// Post: Disaplys the post given
std::string Post::displayPost() const 
{
    return message_ + "\nLiked by " + std::to_string(likes_) + " people";
}

// Pre: None
// Post: gives the post id
std::size_t Post::getId() const 
{ 
    return id_; 
}

// Pre: None
// Post: gives the message
std::string Post::getMessage() const 
{ 
    return message_; 
}

// Pre: None
// Post: gives the number of likes
std::size_t Post::getLikes() const 
{ 
    return likes_; 
}

// Pre: None
// Post: gives the public status of the post
bool Post::isPublic() const
{
    return true;
}
