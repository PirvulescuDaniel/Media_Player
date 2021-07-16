#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    this->widget_init();
    this->images_init();
    this->connect_database();
}

Login::~Login()
{
    delete ui;
}

void Login::widget_init(){
    ui->Login_widget->setGeometry( this->width()/2 - ui->Login_widget->width()/2, this->height()/2 - ui->Login_widget->height()/2 , ui->Login_widget->width(), ui->Login_widget->height());
    ui->pushButton_login->setGeometry( this->width()/2 - ui->pushButton_login->width()/2, ui->pushButton_login->y(), ui->pushButton_login->width(), ui->pushButton_login->height() );
    ui->widget_create->setGeometry(this->width()/2 - ui->widget_create->width()/2 , ui->widget_create->y(), ui->widget_create->width(), ui->widget_create->height() );
    ui->statusbar->addPermanentWidget(ui->db_status);
}

void Login::images_init(){

    //change the background of label_user_img
    QPixmap user_pix(":/img/img/user.png");

    int width = ui->label_user_img->width();
    int height = ui->label_user_img->height();
    user_pix = user_pix.scaled(width, height, Qt::KeepAspectRatio);

    ui->label_user_img->setPixmap(user_pix);

    //change the background of label_pass_img
    QPixmap pass_pix(":/img/img/lock.png");

    width = ui->label_pass_img->width();
    height = ui->label_pass_img->height();
    pass_pix = pass_pix.scaled(width, height, Qt::KeepAspectRatio);

    ui->label_pass_img->setPixmap(pass_pix);

    //change the background of label_new_img
    QPixmap new_pix(":/img/img/new.png");

    width = ui->label_create->width();
    height = ui->label_create->height();
    new_pix = new_pix.scaled(width, height, Qt::KeepAspectRatio);

    ui->label_create->setPixmap(new_pix);

}

bool Login::verify(){

    QSqlQuery query;

    query.exec("SELECT username, password FROM users");

    QString database_name;
    QString database_pass;

    while( query.next() ){
        database_name = query.value(0).toString();
        database_pass = query.value(1).toString();

        if(database_name == ui->username_lineEdit->text() && database_pass == ui->password_lineEdit->text()){

            QSqlQuery update_query;
            update_query.prepare("UPDATE users SET active_session = 'true' WHERE username = :username;" );
            update_query.bindValue(":username", ui->username_lineEdit->text());
            update_query.exec();

            return true;
        }
    }

    //if the database don't work, you can acces the app with: user:github | pass:123456

    if( ui->username_lineEdit->text() == "github" && ui->password_lineEdit->text() == "123456" ){
        return true;
    }

    return false;

}

void Login::on_pushButton_login_clicked()
{
    if( this->verify() ){

        this->hide();
        this->open_player();

        ui->username_lineEdit->clear();
        ui->password_lineEdit->clear();

    }else{
        QMessageBox::warning(this,"Acces Denied","Wrong Username or Password");
    }
}


void Login::on_pushButton_create_clicked()
{
    this->dial = new create_account();
    dial->setWindowTitle("Create Account");
    dial->show();
}

void Login::connect_database(){

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("music_player");

    if( db.open() ){
        ui->db_status->setText( ui->db_status->text() + " Connected" );
    }else{
        ui->db_status->setText( ui->db_status->text() + " Error" );
    }

}

void Login::open_player(){

    this->music_player = new player(this);
    music_player->setWindowTitle("Music Player");
    music_player->show();

}
