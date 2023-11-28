#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include "network.h"
#include "user.h"

#include <QMainWindow>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class SocialNetworkWindow; }
QT_END_NAMESPACE

class SocialNetworkWindow : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkWindow(QWidget *parent = nullptr);
    ~SocialNetworkWindow();

private:
    Ui::SocialNetworkWindow *ui;

    Network SocialNetwork;

    void checkLogin();

    void displayProfile(const QString& name);

    void onFriendNameClicked(int row, int column);

    QString loggedInUser;

    void returnToUser();

    void onAddFriendButtonClicked();

    void displayFriendSuggestions();

    // used to declare action type added for undo button
    enum ActionType
    {
        UI_CHANGE,

        ADD_FRIEND
    };

    // used to mark the current state of the function added for the undo button
    struct UIState
    {
        bool labelProfileNameVisible;

        bool tableFriendsVisible;

        bool errorLabelVisible;

        bool pushButtonVisible;

        bool textEnterVisible;

        bool labelVisible;

        bool tableLabelVisible;

        bool labelPostsVisible;

        bool postHeaderVisible;

        bool suggestedFriendsVisible;

        bool suggestedFriendsLabelVisible;

        bool undoButtonVisible;

        QString currentProfileName;

        QStringList friendsList;

        QString postsContent;
    };

    struct Action
    {
        ActionType type;

        // used for ADD_FRIEND type
        QString friendName;
        // used for UI_CHANGE type
        UIState uiState;
    };

    // create stack
    std::stack<Action> actions;

    // Methods for the undo
    void pushAction(ActionType type, const QString& friendName = QString());

    void undoLastAction();

    UIState getCurrentUIState();

    void restoreUIState(const UIState& state);

    bool isRevertingState = false;

};
#endif // SOCIALNETWORKWINDOW_H
