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

    ui->undoButton->hide();

    // assigns functions to buttons and other aspects
    connect(ui->pushButton, &QPushButton::clicked, this, &SocialNetworkWindow::checkLogin);

    connect(ui->tableFriends, &QTableWidget::cellClicked, this, &SocialNetworkWindow::onFriendNameClicked);

    connect(ui->buttonBackToMyProfile, &QPushButton::clicked, this, &SocialNetworkWindow::returnToUser);

    connect(ui->addFriend, &QPushButton::clicked, this, &SocialNetworkWindow::onAddFriendButtonClicked);

    connect(ui->undoButton, &QPushButton::clicked, this, &SocialNetworkWindow::undoLastAction);
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

    pushAction(UI_CHANGE);

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

        ui->undoButton->show();

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
    QString displayName;

    // Check if the profile is of the logged-in user
    if (name == loggedInUser)
    {
        displayName = "My Profile";
    }
    else if (name == "My Profile")
    {
        displayName = "My Profile";
    }
    else
    {
        // For other users, append "'s Profile" if not already present
        displayName = name.endsWith("'s Profile") ? name : name + "'s Profile";
    }

    ui->labelProfileName->setText(displayName);


    // sets table of friends size
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

    if (name.isEmpty() || name == loggedInUser) {
        ui->buttonBackToMyProfile->hide();
        ui->addFriend->hide();
    }
    //pushAction(UI_CHANGE);

}

//Pre: click on friend name
//Post: goes to friends profile
void SocialNetworkWindow::onFriendNameClicked(int row, int column)
{
   pushAction(UI_CHANGE);

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

    pushAction(ADD_FRIEND, profileName);
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

            displayProfile(loggedInUser);

            displayFriendSuggestions();

            pushAction(ADD_FRIEND, QString::fromStdString(friendName));
        });

        ui->tableFriends->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        ui->suggestedFriends->setCellWidget(i, 1, addButton);
    }
}

//pre: have a stack
// post: pushed the action
void SocialNetworkWindow::pushAction(ActionType type, const QString& friendName)
{
    Action action;

    action.type = type;

    // based off action type pushed the change to stack
    if (type == UI_CHANGE)
    {
        action.uiState = getCurrentUIState();

        QString profileName = ui->labelProfileName->text();

        // to fix issue of profile's profile
        if (profileName.endsWith("'s Profile"))
        {
            profileName.chop(10);
        }

        // stores ui state
        action.uiState.currentProfileName = profileName;
    }
    else if (type == ADD_FRIEND)
    {
        // gets freind name to store for undo
        action.friendName = friendName;
    }

    // pushes the final action
    actions.push(action);
}


// Pre: have a valid stack
// Post: Method to undo the last action, given info from stack if empty it does nothing
void SocialNetworkWindow::undoLastAction()
{
    // if empty return blank
    if (actions.empty()) return;

    // lists the last action first
    Action lastAction = actions.top();

    // pops action for change
    actions.pop();

    // if it is ui change or if add friend
    if (lastAction.type == UI_CHANGE)
    {
        restoreUIState(lastAction.uiState);

        QString currentProfile = ui->labelProfileName->text();

        // conditonal to make sure main users page says my profile
        if (currentProfile == loggedInUser || currentProfile == "My Profile")
        {
            displayProfile(loggedInUser);
        }
    }
    else if (lastAction.type == ADD_FRIEND)
    {
        // removes connection of friend
        SocialNetwork.removeConnection(loggedInUser.toStdString(), lastAction.friendName.toStdString());

        displayProfile(loggedInUser);
    }
}

// Pre: have items on ui
// Post: stores the current visibility of items
SocialNetworkWindow::UIState SocialNetworkWindow::getCurrentUIState()
{
    UIState state;

    state.labelProfileNameVisible = ui->labelProfileName->isVisible();

    state.tableFriendsVisible = ui->tableFriends->isVisible();

    state.errorLabelVisible = ui->errorLabel->isVisible();

    state.pushButtonVisible = ui->pushButton->isVisible();

    state.textEnterVisible = ui->textEnter->isVisible();

    state.labelVisible = ui->Label->isVisible();

    state.tableLabelVisible = ui->tableLabel->isVisible();

    state.labelPostsVisible = ui->labelPosts->isVisible();

    state.postHeaderVisible = ui->postHeader->isVisible();

    state.suggestedFriendsVisible = ui->suggestedFriends->isVisible();

    state.suggestedFriendsLabelVisible = ui->suggestedFriendsLabel->isVisible();

    state.undoButtonVisible = ui->undoButton->isVisible();

    state.currentProfileName = ui->labelProfileName->text();

    state.friendsList.clear();

    // stores freinds
    for (int i = 0; i < ui->tableFriends->rowCount(); ++i)
    {
        state.friendsList << ui->tableFriends->item(i, 0)->text();
    }

    // stores posts
    state.postsContent = ui->labelPosts->text();

    // returns current states
    return state;
}

// Pre: Have stored visibility for ui
// Post: Method to restore the UI state based off previous state stored, includes conditional for special cases that are given such as Profile profile popping up when undo
void SocialNetworkWindow::restoreUIState(const UIState& state)
{
    ui->labelProfileName->setVisible(state.labelProfileNameVisible);

    ui->tableFriends->setVisible(state.tableFriendsVisible);

    ui->errorLabel->setVisible(state.errorLabelVisible);

    ui->pushButton->setVisible(state.pushButtonVisible);

    ui->textEnter->setVisible(state.textEnterVisible);

    ui->Label->setVisible(state.labelVisible);

    ui->tableLabel->setVisible(state.tableLabelVisible);

    ui->labelPosts->setVisible(state.labelPostsVisible);

    ui->postHeader->setVisible(state.postHeaderVisible);

    ui->suggestedFriends->setVisible(state.suggestedFriendsVisible);

    ui->suggestedFriendsLabel->setVisible(state.suggestedFriendsLabelVisible);

    ui->undoButton->setVisible(state.undoButtonVisible);

    ui->labelProfileName->setText(state.currentProfileName);

    QString restoredProfileName = state.currentProfileName;

    // to fix profiles profile
    if (restoredProfileName.endsWith("'s Profile"))
    {
        restoredProfileName.chop(10);
    }

    displayProfile(restoredProfileName);

    ui->tableFriends->setRowCount(state.friendsList.size());

    // to update friends list
    for (int i = 0; i < state.friendsList.size(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(state.friendsList[i]);

        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        ui->tableFriends->setItem(i, 0, item);
    }

    // to update posts
    ui->labelPosts->setText(state.postsContent);

    // to set ui state correctly if not caught
    if (!state.labelProfileNameVisible)
    {
        ui->buttonBackToMyProfile->hide();

        ui->addFriend->hide();
    }

    // to make sure there is not back to my button proifle or add friend on main page if ui state is wrong
    if (restoredProfileName == loggedInUser || restoredProfileName == "My Profile")
    {
        ui->buttonBackToMyProfile->hide();

        ui->addFriend->hide();
    }
    else
    {
        ui->buttonBackToMyProfile->setVisible(state.labelProfileNameVisible);

        ui->addFriend->setVisible(state.labelProfileNameVisible);
    }
}
