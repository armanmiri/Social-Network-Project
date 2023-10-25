#include "user.h"
#include "network.h"
#include "post.h"
#include "incomingPost.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

// Call the function to test User constructor and getter methods.
void testUserConstructorAndGetters() 
{
    std::cout << "\nTesting User Constructor and Getters...";

    std::vector<std::size_t> friendsList_;
    
    friendsList_.push_back(0);
    
    friendsList_.push_back(1);
    
    friendsList_.push_back(2);

    User* u = new User(3, "Jason Chen", 2000, 94087, friendsList_);
    
    assert(u -> getId_() == 3);
    
    assert(u -> getName_() == "Jason Chen");
    
    assert(u -> getYear_() == 2000);
    
    assert(u -> getZip_() == 94087);
    
    bool comparefriends_ = (u -> getFriends_() == friendsList_);
    
    assert(comparefriends_);

    delete u;
    
    std::cout << "Passed! \n" << std::endl;
}

// Call the function to test Network operations.
void testNetworkOperations() 
{
    std::cout << "Testing Network Operations... ";

    Network network;
    
    // Add users
    std::vector<std::size_t> emptyfriends_;

    User* u1 = new User(0, "John Doe", 1995, 12345, emptyfriends_);

    User* u2 = new User(1, "Jane Smith", 2000, 67890, emptyfriends_);

    network.addUser(u1);

    network.addUser(u2);
    
    // Test user addition
    assert(network.numUsers_() == 2);

    // Test friend connection
    network.addConnection("John Doe", "Jane Smith");
    
    std::vector<std::size_t> expectedfriendsU1;

    expectedfriendsU1.push_back(1);

    std::vector<std::size_t> expectedfriendsU2;

    expectedfriendsU2.push_back(0);

    bool comparefriendsU1 = (u1 -> getFriends_() == expectedfriendsU1);

    bool comparefriendsU2 = (u2 -> getFriends_() == expectedfriendsU2);

    assert(comparefriendsU1);

    assert(comparefriendsU2);
    
    // Test removal of friend connection

    network.removeConnection("John Doe", "Jane Smith");

    assert(u1 -> getFriends_().empty());

    assert(u2 -> getFriends_().empty());

    std::cout << "Passed! \n" << std::endl;
}

// Test printUsers function
void testPrintUsers() {
    std::cout << "Testing printUsers... " << std::endl;

    Network network;
    
    std::vector<std::size_t> friendsList_;

    // Create some User objects and add them to the network
    User* user1 = new User(0, "John Doe", 1995, 12345, friendsList_);
    
    User* user2 = new User(1, "Jane Smith", 2000, 67890, friendsList_);
    
    User* user3 = new User(2, "Alice Johnson", 1998, 54321, friendsList_);

    network.addUser(user1);
    
    network.addUser(user2);
    
    network.addUser(user3);

    // Call the printUsers function
    network.printUsers();

    //prompt test passed
    std::cout << "Print Users Test Passed! \n" << std::endl;
}

// Test printFriends function
void testPrintFriends() 
{
    // telling test is running
    std::cout << "Testing printFriends... " << std::endl;

    std::vector<std::size_t> friendsList_;

    Network network;

    // adds users to the network to test
    User* user1 = new User(0, "John Doe", 1995, 12345, friendsList_);
   
    User* user2 = new User(1, "Jane Smith", 2000, 67890, friendsList_);
   
    User* user3 = new User(2, "Alice Johnson", 1998, 54321, friendsList_);

    network.addUser(user1);
   
    network.addUser(user2);
   
    network.addUser(user3);

    // Establish friend connections
    network.addConnection("John Doe", "Jane Smith");
   
    network.addConnection("John Doe", "Alice Johnson");

    // Call the printFriends function
    std::cout << "\nTesting John Doe" << std::endl;
   
    network.printFriends("John Doe");

    std::cout << "\nTesting Jane Smith" << std::endl;
   
    network.printFriends("Jane Smith");

    std::cout << "\nTesting Alice Johnson" << std::endl;
   
    network.printFriends("Alice Johnson");

    std::cout << "\nTesting Nonexistent" << std::endl;
   
    network.printFriends("Nonexistent User");

    std::cout << "\nPrint Friends Test Passed!" << std::endl;
}

void testShortestPath() 
{
    std::cout << "Testing shortestPath... ";

    // creates test network
    Network network;
   
    network.addUser(new User(0, "Alice", 1990, 10000, {}));
   
    network.addUser(new User(1, "Bob", 1991, 10001, {}));
   
    network.addUser(new User(2, "Charlie", 1992, 10002, {}));
   
    network.addUser(new User(3, "David", 1993, 10003, {}));

    network.addConnection("Alice", "Bob");
   
    network.addConnection("Bob", "Charlie");
   
    network.addConnection("Charlie", "David");

    // checks for path and makes sure its right
    std::vector<std::size_t> path = network.shortestPath(0, 3);

    assert(path.size() == 4);  
   
    assert(path[0] == 0);      
   
    assert(path[1] == 1);     
   
    assert(path[2] == 2);      
   
    assert(path[3] == 3);      

    std::cout << "Passed!\n" << std::endl;
}

void testDistanceUser(Network& network) 
{
    // start test
    std::cout << "Testing distanceUser... ";

    // initalize the test
    std::size_t to;

    std::size_t from = 0;  

    std::size_t distance = 3;  

    std::vector<std::size_t> path = network.distanceUser(from, to, distance);

    // check to see value of to and if it is valid
    if (to != std::numeric_limits<std::size_t>::max()) 
    {
        //returns users
        std::cout << "User found at distance " << distance << ": " << network.getUserName(to) << std::endl;
    
        std::cout << "Path: ";
    
        // prints users
        for (auto id : path) 
        {
            std::cout << network.getUserName(id) << " -> ";
        }
        
        std::cout << std::endl;
    } 
    else 
    {
        // returns there is none
        std::cout << "No user found at the given distance." << std::endl;
    }

    // test completion
    std::cout << "Test completed!" << std::endl;
}

void testSuggestFriends(Network& network) 
{
    // initalize
    std::cout << "Testing suggestFriends... ";

    std::size_t userId = 0; 

    std::size_t score;

    std::vector<std::size_t> suggestions = network.suggestFriends(userId, score);

    // returns results
    if (score == -1) 
    {
        std::cout << "None" << std::endl;
    } 
    else 
    {
        std::cout << "The suggested friend(s) with score " << score << " is/are:" << std::endl;
        
        for (auto suggestion : suggestions) 
        {
            std::cout << network.getUserName(suggestion) << std::endl;
        }
    }

    // says complete
    std::cout << "Test completed!" << std::endl;
}

void testGroups(Network& network) 
{
    // test function this is start
    std::cout << "Testing groups... ";

    auto groups = network.groups();

    // checks the result and returns the groups if found
    if (!groups.empty()) 
    {
        std::size_t groupNum = 1;
        for (const auto& group : groups) 
        {
            std::cout << "Group " << groupNum++ << ": ";
        
            for (const auto& userId : group) 
            {
                std::cout << network.getUserName(userId) << " ";
            }
            
            std::cout << std::endl;
        }
    } 
    else 
    {
        // sayss nothing found
        std::cout << "No groups found." << std::endl;
    }

    // says completed function
    std::cout << "Test completed!" << std::endl;
}

// test post class
void testPostClass() 
{
    // creates post
    Post post(1, "This is a sample post.", 5);

    // displays the post
    std::cout << post.displayPost() << std::endl;

    // displays the users information
    std::cout << "ID: " << post.getId() << std::endl;

    std::cout << "Message: " << post.getMessage() << std::endl;

    std::cout << "Likes: " << post.getLikes() << std::endl;
}

// test the incoming post class
void testincomingPostClass() 
{
    // creates incoming post
    IncomingPost incomingPost(1, "This is an incoming post.", 5, true, "Author Name");
    
    // displays post
    std::cout << incomingPost.displayPost() << std::endl;

    // displays the users information
    std::cout << "ID: " << incomingPost.getId() << std::endl;

    std::cout << "Message: " << incomingPost.getMessage() << std::endl;

    std::cout << "Likes: " << incomingPost.getLikes() << std::endl;
}

// Test user class with new additons
void testUserClass() 
{
    // creates user object
    User user; 

    // creates post and incoming post
    Post* post1 = new Post(1, "This is the first owner's post.", 10);

    IncomingPost* post2 = new IncomingPost(2, "This is the first incoming post.", 5, true, "Alice");

    Post* post3 = new Post(3, "This is the second owner's post.", 15);

    IncomingPost* post4 = new IncomingPost(4, "This is the second incoming post.", 7, false, "Bob");

    // adds post to users
    user.addPost(post1);

    user.addPost(post2);

    user.addPost(post3);

    user.addPost(post4);

    // displays the posts
    std::cout << "All Posts (including private):" << std::endl;

    std::cout << user.displayPosts(4, false) << std::endl;

    std::cout << "Public Posts Only:" << std::endl;

    std::cout << user.displayPosts(4, true) << std::endl;

    // deallocates posts
    delete post1;

    delete post2;

    delete post3;

    delete post4;
}

int main() 
{

     Network network; 

    testUserConstructorAndGetters();
 
    testNetworkOperations();

    testPrintUsers();

    testPrintFriends();

    testShortestPath();

    testGroups(network);

    testDistanceUser(network);
    
    testSuggestFriends(network);

    testPostClass();

    testincomingPostClass();

    testUserClass();

    std::cout << "\n\nAll Tests Passed! \n \n" << std::endl;
    
    return 0;
}
