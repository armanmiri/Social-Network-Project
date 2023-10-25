#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"

using namespace std;

//Pre: None
//Post: constructor
SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    // setup code
    ui->setupUi(this);

    SocialNetwork.readUsers_("social_network.txt");

    SocialNetwork.readPosts("posts.txt");

    // hides what is not needed on login
    ui->buttonBackToMyProfile->hide();

    ui->labelProfileName->hide();

    ui->tableFriends->hide();

    ui->labelPosts->hide();

    ui->tableLabel->hide();

    ui->postHeader->hide();

    ui->addFriend->hide();

    ui->suggestedFriends->hide();

    ui->suggestedFriendsLabel->hide();

    // assigns functions to buttons and other aspects
    connect(ui->pushButton, &QPushButton::clicked, this, &SocialNetworkWindow::checkLogin);

    connect(ui->tableFriends, &QTableWidget::cellClicked, this, &SocialNetworkWindow::onFriendNameClicked);

    connect(ui->buttonBackToMyProfile, &QPushButton::clicked, this, &SocialNetworkWindow::returnToUser);

    connect(ui->addFriend, &QPushButton::clicked, this, &SocialNetworkWindow::onAddFriendButtonClicked);

}

//Pre: None
//Post: constructor
SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}

//Pre: given username
//Post: checks user name on the login window if it is valid it continues to pop up other widgets if not says new name
void SocialNetworkWindow::checkLogin()
{
    // stores the input
    QString name = ui->textEnter->toPlainText();

    loggedInUser = name;

    // if name is right then show logged in user menu
    if (SocialNetwork.userExists(name.toStdString()))
    {
        ui->errorLabel->hide();

        ui->textEnter->hide();

        ui->pushButton->hide();

        ui->Label->hide();

        ui->labelProfileName->show();

        ui->tableFriends->show();

        ui->tableLabel->show();

        ui->labelPosts->show();

        ui->postHeader->show();

        ui->suggestedFriends->show();

        ui->suggestedFriendsLabel->show();

        displayFriendSuggestions();

        displayProfile(name);
    }
    // if not then it says new name
    else
    {
        ui->errorLabel->show();

        ui->errorLabel->setText("Invalid name. Please enter a valid name.");
    }
}

//Pre: usersname
//Post: provides the profile for the user, including suggest friends or additional buttons if not on the main user
void SocialNetworkWindow::displayProfile(const QString& name)
{
    // sets name on profile
    ui->labelProfileName->setText(name + "'s Profile");

    // sets table of firends size
    ui->tableFriends->setRowCount(0);

    ui->tableFriends->setColumnCount(1);

    // fills table of friends
    auto friendsNames = SocialNetwork.friendsCall(name.toStdString());

    ui->tableFriends->setRowCount(friendsNames.size());

    for(int i = 0; i < friendsNames.size(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(friendsNames[i]));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        ui->tableFriends->setItem(i, 0, item);
    }

    ui->tableFriends->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // fills the posts table
    auto recentPosts = SocialNetwork.readPostsForUser("posts.txt", name.toStdString(), name == loggedInUser);

    QString postsContent;

    for(const auto& post : recentPosts)
    {
        postsContent += QString::fromStdString(post) + "\n\n";
    }

    ui->labelPosts->setText(postsContent);

}

//Pre: click on friend name
//Post: goes to friends profile
void SocialNetworkWindow::onFriendNameClicked(int row, int column)
{
    QTableWidgetItem *item = ui->tableFriends->item(row, column);

    QString friendName = item->text();

    displayProfile(friendName);

    ui->buttonBackToMyProfile->show();

    ui->addFriend->show();

    ui->suggestedFriends->hide();

    ui->suggestedFriendsLabel->hide();

}

//Pre: click return to logged in user
//Post: return to user logged in
void SocialNetworkWindow::returnToUser()
{
    displayProfile(loggedInUser);

    ui->buttonBackToMyProfile->hide();

    ui->addFriend->hide();

    ui->suggestedFriends->show();

    ui->suggestedFriendsLabel->show();
}

//Pre: click add friend
//Post: adds friend to logged in users list of friends
void SocialNetworkWindow::onAddFriendButtonClicked()
{
    QString profileName = ui->labelProfileName->text();

    profileName = profileName.remove("'s Profile");

    SocialNetwork.addConnection(loggedInUser.toStdString(), profileName.toStdString());

}

//Pre: logged in user is on homepage
//Post: shwos list of suggest friends and an add button to add them if they want
void SocialNetworkWindow::displayFriendSuggestions()
{
    // gets the id of the logged in user to run suggest friends
    std::size_t userId = SocialNetwork.getId_(loggedInUser.toStdString());

    std::size_t score = 1;

    // finds suggested friends
    auto suggestedFriendsvar = SocialNetwork.suggestFriends(userId, score);

    ui->suggestedFriends->setRowCount(suggestedFriendsvar.size());

    ui->suggestedFriends->setColumnCount(2);

    // fills in the table
    for(size_t i = 0; i < suggestedFriendsvar.size(); ++i)
    {
        auto suggestedFriend = suggestedFriendsvar[i];

        std::string friendName = SocialNetwork.getUserName(suggestedFriend);

        QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(friendName));

        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        ui->suggestedFriends->setItem(i, 0, item);

        QPushButton *addButton = new QPushButton("Add", this);

        connect(addButton, &QPushButton::clicked, [this, friendName]() {
            SocialNetwork.addConnection(loggedInUser.toStdString(), friendName);
        });

        ui->tableFriends->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        ui->suggestedFriends->setCellWidget(i, 1, addButton);
    }
}
