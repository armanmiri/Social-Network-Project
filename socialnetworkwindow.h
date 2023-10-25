#ifndef SOCIALNETWORKWINDOW_H
#define SOCIALNETWORKWINDOW_H

#include "network.h"
#include "user.h"

#include <QMainWindow>

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

};
#endif // SOCIALNETWORKWINDOW_H
