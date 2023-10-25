#include "user.h"
#include "network.h"
#include "post.h"
#include "incomingPost.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>

int main() 
{
    //Creates the network object which stores all the data
    Network network;

    network.readUsers_("social_network.txt");

    network.readPosts("posts.txt");

    //Variable to store user's choice from following menu
    std::size_t choice;

    // Starts the menu loop
    do {

        //This all displays the menu options
        std::cout << "\nMenu Options:\n";
        
        std::cout << "1. Add a user.\n";
        
        std::cout << "2. Add friend connection.\n";
        
        std::cout << "3. Remove friend connection.\n";
        
        std::cout << "4. Print users.\n";
        
        std::cout << "5. Print friends.\n";
        
        std::cout << "6. Write to file.\n";
        
        std::cout << "7. Shortest Path between Two Users.\n";
        
        std::cout << "8. Exploring Neighbors.\n";
        
        std::cout << "9. Friend suggestions\n";
        
        std::cout << "10. Display disjoint sets of users\n";
        
        std::cout << "11. Read Posts.\n";

        std::cout << "12. Exit.\n";
        
        std::cout << "Enter option: ";
        
        // Saves the user input for choice
        std::cin >> choice;

        //Ignores any newline characters 
        std::cin.ignore(); 

        //Uses a switch statment to address the cases the user can choose from
        switch (choice)
         {
            // option one add new user  
            case 1:
            {
        
                std::string name_;
        
                std::size_t year_, zip_;

                // asks user for details   
                std::cout << "\nEnter users name: ";
        
                std::getline(std::cin, name_);
                
                std::cout << "Enter birth year: ";
        
                std::cin >> year_;

                std::cout << "Enter zipcode: "; 

                std::cin >> zip_;

                //creates a friends list for the new user
                std::vector<std::size_t> emptyfriends_;
                
                // adds the new user to the network
                network.addUser(new User(network.numUsers_(), name_, year_, zip_, emptyfriends_));
        
                std::cout << "\n";

                break;
            }

            // option two add a friend connection between two users    
            case 2: 
            {
                
                std::string name_1, last_1, name_2, last_2, name1, name2;
                
                //asks the user for the name_s of the two they want to connect
                std::cout << "\nEnter names of the two users to make friends: ";
                
                std::cin >> name_1 >> last_1 >>name_2 >> last_2;
                
                name1 = name_1 + " " + last_1;

                name2 = name_2 + " " + last_2;

                //adds the friend connection to the network
                network.addConnection(name1, name2);
                
                std::cout << "\n";

                break;
                
            }

            // option three removes a friend connection between two users
            case 3: 
            {      
                std::string name_1, last_1, name_2, last_2, name1, name2;
                
                // asks the user for the name_s of the two they want to remove as friends_ 
                std::cout << "\nEnter names of the two users to remove friend connection: ";
                
                std::cin >> name_1 >> last_1 >>name_2 >> last_2;
                
                name1 = name_1 + " " + last_1;

                name2 = name_2 + " " + last_2;
              
                // removes the connection on the network
                network.removeConnection(name1, name2);
              
                std::cout << "\n";

                break;
            }

            // option 4 list the users in network
            case 4: 
            {
                std::cout << "\n";

                network.printUsers();
             
                std::cout << "\n";

                break;
            }

            // print the freinds of the specific user
            case 5: 
            {
                std::string name_;
                
                // asks for the name_ of the user they want to check
                std::cout << "\nEnter name of the user to print friends: ";
                
                std::getline(std::cin, name_);
                
                // lists the name_s of the friends_ of that user
                network.printFriends(name_);
                
                std::cout << "\n";

                break;
            }

            // write user data to file
            case 6:
            {
                char filename_[100];
            
                // asks the user for the filename_ to write to
                std::cout << "\nEnter the name of the file you want to write the data to: ";
            
                std::cin >> filename_;
            
                // adds the user data to the file listed
                network.writeUsers_(filename_);
                
                std::cout << "\n";

                break;
            }

            //option 7 Shortest Path
            case 7:
            {
                // initalize string to get the users names
                std::string fromName, toName;

                std::string name_1, last_1, name_2, last_2;
                
                std::cout << "Enter the names of the two users to find the shortest path: ";
                
                std::cin >> name_1 >> last_1 >>name_2 >> last_2;
                
                fromName = name_1 + " " + last_1;

                toName = name_2 + " " + last_2;

                // converts the username to id    
                std::size_t fromId = network.getId_(fromName);
                
                std::size_t toId = network.getId_(toName);

                // checks if users are found if not prints error
                if (fromId == -1 || toId == -1) 
                {
                    std::cerr << "User not found!" << std::endl;
                
                    break;
                }

                std::vector<std::size_t> path = network.shortestPath(fromId, toId);

                // if no path found
                if (path.empty()) 
                {
                    std::cout << "None" << std::endl;
                } 
                //prints distance and anmes as specified
                else 
                {
                    std::cout << "Distance: " << path.size() - 1 << std::endl;
                    
                    for (std::size_t id : path) 
                    {
                        std::cout << network.getUserName(id) << " -> ";
                    }
                    
                    std::cout << std::endl;
                }
                break;
            }

            //option 8 exploring neighbors
            case 8:
            {
                // initalize
                std::string userName, firstName, lastName;
                
                std::size_t distance;
                    
                //gets input and stores
                std::cout << "Enter the username and distance: ";
                
                std::cin >> firstName >> lastName >> distance;

                userName = firstName + " " + lastName;
                
                // ignores extra items and extreme resutls
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

                std::size_t userId = network.getId_(userName);

                // error check and says if found
                if (userId == std::numeric_limits<std::size_t>::max()) 
                {
                    std::cout << "User not found." << std::endl;

                    continue;
                }

                std::size_t to;

                auto path = network.distanceUser(userId, to, distance);

                if (!path.empty()) 
                {
                    std::cout << network.getUserName(to) << ":: "; 

                    for (auto id : path) 
                    {
                        std::cout << network.getUserName(id);
                        
                        if (id != path.back()) 
                        {
                            std::cout << ", ";
                        }
                    }

                    std::cout << std::endl;
                } 
                else 
                {
                    std::cout << "No user found at the given distance." << std::endl;
                }

                break;
            }

            // option 9 Friend Suggestions
            case 9:
            {
                std::string name, firstName, lastName;

                // Gets user input and stores it 
                std::cout << "Enter the name of the user for friend suggestions: ";
                
                std::cin >> firstName >> lastName;

                name = firstName + " " + lastName;

                std::size_t userId = network.getId_(name);

                // Checks if user is found
                if (userId == -1) 
                {
                    std::cerr << "User not found!" << std::endl;
                   
                    break;
                }

                std::size_t score;
                
                std::vector<std::size_t> suggestions = network.suggestFriends(userId, score);

                // If none found says none
                if (score == -1) 
                {
                    std::cout << "None" << std::endl;
                } 
                else 
                {
                    std::cout << "The suggested friend(s) is/are:" << std::endl;
                    
                    for (auto suggestion : suggestions) 
                    {
                        std::cout << network.getUserName(suggestion) << " Score: " << score << std::endl;
                    }
                }

                break;
            }


            // option 10 Disjoint Sets with DFS
            case 10:
            {
                std::cout << "Finding disjoint sets of users...\n";

                auto components = network.groups();
                
                std::size_t setNumber = 1;

                // iterate over each set in component vector
                for (const auto& component : components) 
                {
                    std::cout << "Set " << setNumber++ << " => ";
                
                    for (std::size_t userId : component) 
                    {
                        // prints the usernmaes of the users in componet set
                        std::cout << network.getUserName(userId) << ", ";
                    }
                    
                    std::cout << "\b\b \n"; // To remove the last comma and space
                }

                break;
            }

            // option 11 read posts
            case 11:
            {
                // initalizing user input
                std::string name, firstName, lastName;
               
                std::size_t howMany;
               
                // asks and stores user input
                std::cout << "Enter the name of the person and how many recent posts you want to see: ";
               
                std::cin >> firstName >> lastName >> howMany;

                name = firstName + " " + lastName;
               
                // calls the display post function
                std::string posts = network.displayPosts(name, howMany, true);  
               
                // if there is no post returns empty or displays if there is
                if (!posts.empty()) 
                {
                    std::cout << posts << std::endl;
                } 
                else 
                {
                    std::cout << "No posts to display." << std::endl;
                }
                
                break;
            }

            //exit the program
            case 12:
            {  
                // prompts exit
                std::cout << "Exiting...\n";
            
                break;
            }
            
            // invalid option exits the loops tells the user
            default:
            {   
                // tells user 
                std::cout << "Invalid option! Exiting...\n";
                
                // exit loop
                choice = 8; // Exit the loop
                
                break;
            }
         }

    } while (choice != 12);

    return 0;
}