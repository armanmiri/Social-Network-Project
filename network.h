#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <vector>
#include "post.h"
#include "incomingPost.h"
#include <algorithm>

class User; //declaration of User class

class Network 
{
	public:

	    // Constructor
	    Network();

	    // Member functions
	    void addUser(User* user);

	    int addConnection(const std::string& s1, const std::string& s2);
	    
	    int removeConnection(const std::string& s1, const std::string& s2);
	    
	    std::size_t getId_(const std::string& name_) const;
	    
	    int readUsers_(const char* fname_);
	    
	    int writeUsers_(const char* fname_) const;
	    
	    std::size_t numUsers_() const;
	    
	    void printUsers();
    
    	void printFriends(std::string usersName);

		// network analysis
		std::vector<std::size_t> shortestPath(std::size_t from, std::size_t to);
		
		std::string getUserName(std::size_t id) const;

 		std::vector<std::size_t> distanceUser(std::size_t from, std::size_t& to, std::size_t distance);

		std::vector<std::size_t> suggestFriends(std::size_t who, std::size_t& score);

		std::vector<std::vector<std::size_t>> groups();

		// Post class
		void addPost(std::size_t ownerId, const std::string& message, std::size_t likes, bool incoming, std::size_t author, bool isPublic);
    	
		std::string displayPosts(const std::string& name, std::size_t howMany, bool showOnlyPublic) const;

		void readPosts(const std::string& filename);

        //UI Helpers
        bool userExists(const std::string& name) const;

        std::vector<std::string> friendsCall(const std::string& usersName);

        std::vector<std::string> readPostsForUser(const std::string& filename, const std::string& username, bool includePrivate);

	private:
	    
	    // object representing the networks users
	    std::vector<User*> users_;

		// analysis helper
		void dfsGroups(std::size_t userId, std::vector<bool>& visited, std::vector<std::size_t>& component);
};

#endif // NETWORK_H
