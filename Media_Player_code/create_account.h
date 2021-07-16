#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <QDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QFileDialog>
#include <QFile>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class create_account;
}

class create_account : public QDialog
{
    Q_OBJECT

public:
    explicit create_account(QWidget *parent = nullptr);
    ~create_account();

private slots:
    void on_pushButton_create_clicked();

    void on_pushButton_browse_clicked();

private:
    Ui::create_account *ui;

    void geometry_init();
    bool can_create();
    void add_account();
};

#endif // CREATE_ACCOUNT_H
