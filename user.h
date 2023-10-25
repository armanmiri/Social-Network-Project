#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "post.h"
#include "incomingPost.h"

class User 
{
	public:
	    // Constructors
	    User();  
	   
	    User(std::size_t userId_, const std::string& userName_, std::size_t birthYear_, std::size_t zipCode_, const std::vector<std::size_t>& friendsList_);

	    // Member functions
	    void addFriend(std::size_t userId_);
	   
	    void deleteFriend(std::size_t userId_);

	    // Accessors (getters)
	    std::size_t getId_() const;
	    
	    std::string getName_() const;
	    
	    std::size_t getYear_() const;
	    
	    std::size_t getZip_() const;
	    
	    std::vector<std::size_t> getFriends_() const;

		// Post class
		void addPost(Post* post);

        std::string displayPosts(std::size_t howMany, bool showOnlyPublic) const;

		std::size_t getMessagesCount() const;

	private:

		// private member variables
	    std::size_t id_;
	    
	    std::string name_;
	    
	    std::size_t year_;
	    
	    std::size_t zip_;
	    
	    std::vector<std::size_t> friends_;

		// message member
		std::vector<Post*> messages_;
		
};

#endif // USER_H