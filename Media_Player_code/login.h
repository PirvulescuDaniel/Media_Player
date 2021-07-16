#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QWidget>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include "create_account.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_create_clicked();

private:
    Ui::Login *ui;
    create_account *dial;
    player *music_player;

    void widget_init();
    void images_init();
    bool verify();
    void connect_database();
    void open_player();

};
#endif // LOGIN_H
