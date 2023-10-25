#include "network.h"
#include "user.h"
#include "post.h"
#include "incomingPost.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string> 
#include <algorithm>
#include <cassert>
#include <queue>
#include <unordered_map>
#include <map>

//Default Constructor
//Pre: None
//Post: creates the network object
Network::Network() {}

//Pre: User must be a valid object
//Post: the user object added to the user vector adding them to networks list of users
void Network::addUser(User* user) 
{
    //add to vector
    users_.push_back(user);
}

//Pre: must have valid s1 and s2
//Post: user1 and user 2 are added as a connection if the function returns 0 otherwise it returns -1 to say user not found
int Network::addConnection(const std::string& s1, const std::string& s2) 
{
    // get the users
    std::size_t user1 = getId_(s1);

    std::size_t user2 = getId_(s2);

    // if user is not found return -1 to say no
    if (user1 == -1 || user2 == -1) 
    {
        std::cout << "Invalid Input Returning to main menu ....";

        return -1;

    }

    // add connection then return 0 to end
    users_[user1] -> addFriend(user2);

    users_[user2] -> addFriend(user1);

    return 0;
}

//Pre: must have valid s1 and s2
//Post: If both users_ found the connection removed otherwise returns -1 to say failure if successful returns 0
int Network::removeConnection(const std::string& s1, const std::string& s2) 
{
    // get the users 
    std::size_t user1 = getId_(s1);

    std::size_t user2 = getId_(s2);

    // if users not found return -1 to say invalid
    if (user1 == -1 || user2 == -1) 
    {
        std::cout << "Invalid Input Returning to main menu ....";

        return -1;
    }

    // removes the connection then return 0 to end
    users_[user1] -> deleteFriend(user2);

    users_[user2] -> deleteFriend(user1);

    return 0;
}

//Pre: name_, users_ vector need to be valid
//Post: If the user whos name_ is called is found it returns their id_ if not then returns -1 to indicate not found
std::size_t Network::getId_(const std::string& name_) const 
{
    // iterate through set of users
    for (std::size_t i = 0; i < users_.size(); ++i) 
    {
        // checks if the usersName matches the one searched for
        if(users_[i] -> getName_() == name_) 
        {
            // returns the new users name
            return users_[i] -> getId_();
        }
    }  

    // if no matching user returns -1 to say invalid
    return -1;
}

//Pre: must have a valid path to read user data
//Post: If fname_ can't be reat returns -1 counts the number of user in the file returns 0 if runs right and closes when done
int Network::readUsers_(const char* fname_) 
{
    std::ifstream file;
      
    file.open(fname_);

    if (!file.is_open()) 
    {
        return -1; // file not open or not found
    }

    int n;  
    
    file >> n;  
    
    for (int i = 0; i < n; i ++) 
    {
        std::size_t id;
    
        std::string first_name, last_name, name;
    
        int year;
    
        int zip;
    
        std::vector<std::size_t> friends;
    
        friends.clear();

        file >> id;
    
        file >> first_name >> last_name;
    
        file >> year;
    
        file >> zip;
    
        name = first_name + " " + last_name;

        std::string line;
    
        std::getline(file, line); // read in the \n
    
        std::getline(file, line);

        std::stringstream ss(line);

        std::size_t id2;

        while(ss >> id2) 
        {
          friends.push_back(id2);
        }

        User* u = new User(id, name, year, zip, friends);
        
        addUser(u);
      }

      return 0;
    

    // Orignal File before I go new workfrom professor 
    /*
    // open the file
    std::ifstream file(fname_);

    //check if file opened
    if (!file.is_open()) 
    {
        std::cout << "Invalid Input Returning to main menu ....";

        // return -1 if invalid
        return -1;
    }

    // set the total number of users from the file
    std::size_t accountedUser;
    file >> accountedUser;

    //loop through the entire user file
    for (std::size_t i = 0; i < accountedUser; ++i) 
    {
        std::size_t id_, year_, zip_;
        
        std::string name_;
        
        std::vector<std::size_t> friendsList_;

        // reads the user data from the file
        file >> id_;

        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::getline(file, name_, '\t');

        file >> year_ >> zip_;

        //reads the list of friends from the current user
        while (file >> std::ws >> id_) 
        {
            friendsList_.push_back(id_);
        }

        // creates a new user and adds them to the user vector
        User* user = new User(id_, name_, year_, zip_, friendsList_);

        users_.push_back(user);
    }

    // close the file 
    file.close();

    // return 0 for sucess
    return 0;
    */

}

//Pre:fname_ must be a valid c string to write data to and users_ needs to be a valid vector
//Post: If fname_ cant be opened it returns -1 to inid_cate failure, if working it returns 0 and writes all user data to the file
int Network::writeUsers_(const char* fname_) const 
{
    // open the file 
    std::ofstream file(fname_);

    // check if it is opened
    if (!file.is_open())
    { 
        std::cout << "Invalid Input Returning to main menu ....";

        // return -1 if invlaid
        return -1;
    }

    // captures the number of users to the file
    file << users_.size() << std::endl;

    // loop through each user and write their data in the file
    for (std::size_t i = 0; i < users_.size(); ++i) 
    {
        file << users_[i] -> getId_();
    
        file << std::endl;

        file << '\t' << users_[i] -> getName_() << std::endl;
    
        file << '\t' << users_[i] -> getYear_() << std::endl;
    
        file << '\t' << users_[i] -> getZip_() << std::endl;

        std::vector<std::size_t> friends_ = users_[i] -> getFriends_();
    
        // writes list of friends for current user
        file << '\t';
    
        for (std::size_t j = 0; j < friends_.size(); ++j) 
        {
            file << friends_[j] << " ";
        }

        file << std::endl;
    }

    // return 0 to indicate success
    return 0;
}

//Pre: user vector is valid
//Post: returns the number of users_ 
std::size_t Network::numUsers_() const 
{
    return users_.size();
}


//Pre: have a valid user vector
//Post: prints the users in the file
void Network::printUsers() 
{
    std::cout << "ID\tName\tYear\tZip" << std::endl;

    for (std::size_t i = 0; i < users_.size(); ++i) 
    {
        std::cout << users_[i] -> getId_() << "\t" << users_[i] -> getName_() << "\t" 
        << users_[i] -> getYear_() << "\t" << users_[i] -> getZip_() << std::endl;
    }
}

//Pre: have a valid user vector
//Post: prints the users friends
void Network::printFriends(std::string usersName) 
{
    std::size_t id = getId_(usersName);

    // if id is invalid return id invalid with -1
    if(id == -1) 
    {
        // say user not found and break loop
        std::cout << "User not found!" << std::endl;

        return;
    }

    // prints the information of the friends
    std::cout << "ID\tName\tYear\tZip" << std::endl;

    std::vector<std::size_t> friends = users_[id] -> getFriends_();

    for (std::size_t i = 0; i < friends.size(); ++i) 
    {
        User* friendUser = users_[friends[i]];

        std::cout << friendUser -> getId_() << "\t" << friendUser -> getName_() << "\t" 
        << friendUser -> getYear_() << "\t" << friendUser -> getZip_() << std::endl;
    }
}

//Pre: valid users funciton in user class
//Post: gives users name based off id
std::string Network::getUserName(std::size_t id) const 
{
    if (id < users_.size()) 
    {
        return users_[id] -> getName_();
    } 
    else 
    {
        return "";
    }
}

//Pre: to and from users
//Post: finds the shortest path between two users
std::vector<std::size_t> Network::shortestPath(std::size_t from, std::size_t to) 
{
    // initalizes path
    std::vector<std::size_t> path;
    
    // checks if users are valid
    if (from >= users_.size() || to >= users_.size()) 
    {
        std::cerr << "Invalid user ID" << std::endl;
        
        return path;
    }

    std::queue<std::size_t> q;
    
    std::unordered_map<std::size_t, std::size_t> prev;

    q.push(from);
    
    prev[from] = from;

    // iterates paths
    while (!q.empty())
    {
        std::size_t current = q.front();
    
        q.pop();

        if (current == to) 
        {
            while (current != from) 
            {
                path.push_back(current);
            
                current = prev[current];
            }
            
            path.push_back(from);
            
            std::reverse(path.begin(), path.end());
            
            return path;
        }

        User* user = users_[current];
        
        for (std::size_t friendId : user->getFriends_()) 
        {
            if (prev.find(friendId) == prev.end()) 
            {
                prev[friendId] = current;
            
                q.push(friendId);
            }
        }
    }

    // if path not found gives error returns path
    std::cerr << "Path not found" << std::endl;
    
    return path;
}

//Pre: Given to and from user along with the desired distance they want to search
//Post: Gives the users at the distance listed 
std::vector<std::size_t> Network::distanceUser(std::size_t from, std::size_t& to, std::size_t distance) 
{
    // Initalize
    std::vector<bool> visited(users_.size(), false);

    // checks if the starting point valid
    if (distance == 0 || from >= users_.size()) 
    {
        return {from};
    }

    // vars for comp
    std::queue<std::vector<std::size_t>> q;
    
    visited[from] = true;
    
    q.push({from});

    //loops to check the distance
    while (!q.empty()) 
    {
        std::vector<std::size_t> path = q.front();
    
        q.pop();
    
        std::size_t current = path.back();

        if (path.size() - 1 == distance) 
        {
            to = current;
        
            return path;
        }

        for (std::size_t friendId : users_[current]->getFriends_()) 
        {
            if (!visited[friendId]) 
            {
                visited[friendId] = true;
            
                std::vector<std::size_t> newPath = path;
            
                newPath.push_back(friendId);
            
                q.push(newPath);
            }
        }
    }

    to = from;
    
    return {};

}


//Pre: a user to suggest for and a score variable
//Post: gives suggested friends and their score 
std::vector<std::size_t> Network::suggestFriends(std::size_t who, std::size_t& score)
{
    //initalizng all factors
    score = 0;
    
    std::map<std::size_t, std::size_t> candidates;

    if (who >= users_.size()) 
    {
        score = -1;
        return {};
    }

    // finds and scores users
    auto userFriends = users_[who]->getFriends_();

    for (auto friendId : userFriends)
    {
        auto theirFriends = users_[friendId]->getFriends_();

        for (auto potential : theirFriends)
        {
            if (potential != who && std::find(userFriends.begin(), userFriends.end(), potential) == userFriends.end())
            {
                candidates[potential]++;
                
                score = std::max(score, candidates[potential]);
            }
        }
    }

    // if no ussers with score greateeer than 0 return error
    if (score == 0)
    {
        score = -1;
        return {};
    }

    // gives back the suggestions which are "candidates"
    std::vector<std::size_t> suggestions;

    for (const auto& candidate : candidates)
    {
        if (candidate.second == score)
        {
            suggestions.push_back(candidate.first);
        }
    }

    return suggestions;
}



//Pre: Valid network class
//Post: gives disjoint groups of members
std::vector<std::vector<std::size_t>> Network::groups() 
{
    std::vector<bool> visited(users_.size(), false);

    std::vector<std::vector<std::size_t>> components;

    for (std::size_t i = 0; i < users_.size(); ++i) 
    {
        if (!visited[i]) 
        {
            std::vector<std::size_t> component;
            
            dfsGroups(i, visited, component);
            
            components.push_back(component);
        }
    }

    return components;
}

//Pre: uses dfs search after giving valid userId and tracks using visited and component 
//Post: Provides list of dfs groups to analyze
void Network::dfsGroups(std::size_t userId, std::vector<bool>& visited, std::vector<std::size_t>& component) 
{
    visited[userId] = true;

    component.push_back(userId);

    for (std::size_t friendId : users_[userId] -> getFriends_()) 
    {
        if (!visited[friendId]) 
        {
            dfsGroups(friendId, visited, component);
        }
    }
}

// Pre: valid inputs of the designated function
// Post: adds a post to the users list
void Network::addPost(std::size_t ownerId, const std::string& message, std::size_t likes, bool incoming, std::size_t author, bool isPublic) 
{
    // checks if the owners id is valid
    if (ownerId >= users_.size()) 
    {
        std::cerr << "Invalid owner ID" << std::endl;
        
        return;
    }
    
    // gets the number of messages printed by the user
    std::size_t messageId = users_[ownerId] -> getMessagesCount();
    
    // creates a new post object
    Post* post;

    // if the post is incoming from another user
    if (incoming) 
    {
        // gets new name
        std::string authorName = getUserName(author);  
        
        // creates the new incoming post
        post = new IncomingPost(messageId, message + " From: " + authorName, likes, isPublic, authorName);
    } 
    else 
    {
        // otherwise makes a new post wiht old name
        post = new Post(messageId, message, likes);
    }

    // adds the object to the user
    users_[ownerId] -> addPost(post);
}

// Pre: valid input
// Post: displays post
std::string Network::displayPosts(const std::string& name, std::size_t howMany, bool showOnlyPublic) const 
{
    std::size_t userId = getId_(name);
    
    if (userId == (std::size_t)-1 || userId >= users_.size()) 
    {
        std::cerr << "User not found" << std::endl;
        
        return "";
    }

    return users_[userId] -> displayPosts(howMany, showOnlyPublic);

}

// Pre: valid file with posts
// Post: reads the posts in the fiels
void Network::readPosts(const std::string& filename) 
{
   // opens the file given
    std::ifstream file(filename);
    
    //error if can't open file
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // inialize get number of post and stores it 
    std::size_t totalPosts;
    
    file >> totalPosts;

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //For the numebr of posts in the file
    for(std::size_t i = 0; i < totalPosts; ++i) 
    {
        // initalize post id member and stores it
        std::size_t postId;
        
        file >> postId;
        
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Intialize message member and stores it
        std::string message;
        
        std::getline(file, message);

        // initalize the userid member and stores it
        std::size_t userId;
        
        file >> userId;
        
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // initalzie like member and stores it
        std::size_t likes;
        
        file >> likes;
        
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        //stores next line to determine what path to take 
        std::string nextLine;

        std::getline(file, nextLine);

        bool isPublic = true;
        
        // gets the authors name assuming there is no incoming post
        std::string authorName = getUserName(userId);

        // if there is an incoming post determined by public vs private
        if (!nextLine.empty())
        {  
            // If private or public make a new post 
            if (nextLine == "private")
            {
                isPublic = false;

                IncomingPost* post = new IncomingPost(postId, message, likes, isPublic, authorName);
            }
            else if (nextLine == "public")
            {
                isPublic = false;

                IncomingPost* post = new IncomingPost(postId, message, likes, isPublic, authorName);
            }
            
            // get the new authors name
            std::getline(file, authorName);
        }

        IncomingPost* post = new IncomingPost(postId, message, likes, isPublic, authorName);

        // adds the post to user
        if (userId < users_.size() && users_[userId] != nullptr) 
        {
            users_[userId] -> addPost(post);
        } 
        else 
        {
            std::cerr << "User with ID " << userId << " not found." << std::endl;
            
            delete post;
        }
    }

    // closes the file
    file.close();
}

//Pre: valid user input
//Post: provides if the user exists, used to help ui
bool Network::userExists(const std::string& name) const
{
    return getId_(name) != static_cast<std::size_t>(-1);
}


//Pre: given valid user name from file
//Post: provides the friends of a user to help the ui
std::vector<std::string> Network::friendsCall(const std::string& usersName)
{
    // intialize
    std::vector<std::string> friendsNames;

    //gets id
    std::size_t id = getId_(usersName);

    // checks for friends and returns them
    if(id == -1)
    {
        std::cerr << "User not found!" << std::endl;
        return friendsNames;
    }

    std::vector<std::size_t> friends = users_[id]->getFriends_();

    for (std::size_t i = 0; i < friends.size(); ++i)
    {
        User* friendUser = users_[friends[i]];
        friendsNames.push_back(friendUser->getName_());
    }

    return friendsNames;
}

//Pre: given users information looks for post
//Post: gives the users posts that are found as an object
std::vector<std::string> Network::readPostsForUser(const std::string& filename, const std::string& username, bool includePrivate)
{
    //initlialize
    std::vector<std::string> userPosts;
    std::ifstream file(filename);

    // check open file possible
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return userPosts;
    }

    // initalize total posts
    std::size_t totalPosts;
    file >> totalPosts;

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // checks if user is valid
    std::size_t userId = getId_(username);

    if(userId == static_cast<std::size_t>(-1))
    {
        std::cerr << "User not found" << std::endl;
        return userPosts;
    }

    // Looks for the posts by the user through the whole file
    for(std::size_t i = 0; i < totalPosts; ++i)
    {
        // stores post id
        std::size_t postId;

        file >> postId;

        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // stores the message
        std::string message;

        std::getline(file, message);

        // stores the users post id
        std::size_t postUserId;

        file >> postUserId;

        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // stores the number of likes
        std::size_t likes;

        file >> likes;

        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // initalize to check public or private
        std::string nextLine;

        std::getline(file, nextLine);

        // checks public or private
        bool isPublic = true;

        if (!nextLine.empty())
        {
            if (nextLine == "private")
            {
                isPublic = false;
            }
            std::getline(file, nextLine);  // Read the author name or the next post's ID
        }

        // if it is the user looked for then give the posts
        if(postUserId == userId && (includePrivate || isPublic))
        {
            userPosts.push_back(message);
        }
    }

    // closes file
    file.close();

    // returns posts
    return userPosts;
}

