#include "user.h"
#include <vector>
#include <algorithm>
#include <cassert>

// Default Constructor
//Pre: None
//Post: creates a user
User::User() {}

// Default Constructor
//Pre: None
//Post: assigns user parameters
User::User(std::size_t userId_, const std::string& userName_, std::size_t birthYear_, std::size_t zipCode_, const std::vector<std::size_t>& friendsList_) : id_(userId_), name_(userName_), year_(birthYear_), zip_(zipCode_), friends_(friendsList_) {}

//Pre: None
//Post: returns the user id_
std::size_t User::getId_() const 
{
    return id_;
}

//Pre: None
//Post: returns the users_ name_
std::string User::getName_() const 
{
    return name_;
}

//Pre: None
//Post: returns the users_ birthYear_
std::size_t User::getYear_() const 
{
    return year_;
}

//Pre: None
//Post: returns the users_ zipCode_
std::size_t User::getZip_() const 
{
    return zip_;
}

//Pre: None
//Post: returns the users_ friends_
std::vector<std::size_t> User::getFriends_() const 
{
    return friends_;
}

//Pre: must have a valid user id_
//Post: adds friend if user id_ is not added otherwise just returns
void User::addFriend(std::size_t userId_)
{
    for (std::size_t i = 0; i < friends_.size(); ++i) 
    {
        // check if the friend is already in the user list based off of id
        if (friends_[i] == userId_) 
        {
            // friend is already in list break loops
            return; 
        }
    }
    
    // adds friend 
    friends_.push_back(userId_);
}

//Pre: must have a valid user id_
//Post: if friend is found then they are removed if not found list unchanged
void User::deleteFriend(std::size_t userId_) 
{
     for (std::size_t i = 0; i < friends_.size(); ++i) 
     {
        // if freind is found 
        if (friends_[i] == userId_) 
        {
            // removes friend then breaks loops
            friends_.erase(friends_.begin() + i);

            return; 
        }
    }
}

//Pre: Must have posts location set up
//Post: adds a post to the file
void User::addPost(Post* post) 
{
    // adds new post
    messages_.push_back(post);
}

// Pre: how many posts to show
// Post: shows the number of the posts lsited
std::string User::displayPosts(std::size_t howMany, bool showOnlyPublic) const 
{
    // initalize the output
    std::stringstream ss;
    
    std::size_t count = 0;

    std::cout << "Total messages for user: " << messages_.size() << std::endl;  // Debug
    
    // cycle for the number of posts
    for (int i = messages_.size() - 1; i >= 0 && count < howMany; --i) 
    {
        // checks if message is public
        if (showOnlyPublic && !messages_[i]->isPublic()) 
        {
            continue;
        }

        // writes the message
        ss << messages_[i]->displayPost() << "\n\n";
        
        ++count;
    }

    //prints out the posts messages
    return ss.str();
}

// Pre: Have messages written
// Post: returns the number of messages
std::size_t User::getMessagesCount() const 
{
    // returns number of messages
    return messages_.size();
}