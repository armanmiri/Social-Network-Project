#include "incomingPost.h"
#include <sstream>
#include <string>
#include <iostream>

// Pre: None
// Post: an incoming post object is created
IncomingPost::IncomingPost() : Post(), isPublic_(false) {}

// Pre: Provided with all of the parameters listed in constructor
// Post: an incoming post object created with the parameters given
IncomingPost::IncomingPost(std::size_t id, const std::string& message, std::size_t likes, bool isPublic, const std::string& author): Post(id, message + " From: " + author, likes), isPublic_(isPublic) {}

// Pre: None
// Post: returns the string that is the visibility of the post followed by the post
std::string IncomingPost::displayPost() const 
{
    std::string visibility = isPublic_ ? "Public: " : "Private: ";

    return visibility + Post::displayPost();
}


// Pre: None
// Post: returns the string is public
bool IncomingPost::isPublic() const 
{
    return isPublic_; 
}

