#include "create_account.h"
#include "ui_create_account.h"

create_account::create_account(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create_account)
{
    ui->setupUi(this);

    //default profile picture:
    QPixmap profile_pic(":/img/img/man.png");

    int width = ui->profile_pic->width();
    int height = ui->profile_pic->height();

    profile_pic = profile_pic.scaled(width, height, Qt::KeepAspectRatio);

    ui->profile_pic->setPixmap(profile_pic);


}

create_account::~create_account()
{
    delete ui;
}

void create_account::geometry_init(){
    ui->centralWidget->setGeometry( this->width()/2 - ui->centralWidget->width()/2, ui->centralWidget->y(), ui->centralWidget->width(), ui->centralWidget->height() );
    ui->pushButton_create->setGeometry( this->width()/2 - ui->pushButton_create->width()/2, ui->pushButton_create->y(), ui->pushButton_create->width(), ui->pushButton_create->height() );
}

bool create_account::can_create(){

    QSqlQuery query;
    query.exec("SELECT username FROM users");

    QString database_username;

    while( query.next() ){

        database_username = query.value(0).toString();

        if( ui->lineEdit->text() == database_username ){
            return false;
        }

    }

    return true;
}

void create_account::on_pushButton_create_clicked()
{
    if( this->can_create() ){
        this->add_account();
        QMessageBox::information(this,"Accepted","Account successfully created!");
        this->hide();
    }else{
        QMessageBox::warning(this,"Error","Username already exists!");
    }
}


void create_account::on_pushButton_browse_clicked()
{
    QString fisier = QFileDialog::getOpenFileName(this,"Choose a photo","C://");

    QFile file(fisier);

    //convert QFile to QPixmap

    if( !file.open(QIODevice::ReadOnly) ){
        QMessageBox::warning(this,"Error","Can't upload the photo!");
        return;
    }

    QByteArray image = file.readAll(); // convert QFile to QByteArray
    QImage bitm;
    bitm.loadFromData(image); // convert QByteArray to QImage
    QPixmap bmp;
    bmp = QPixmap::fromImage(bitm); //convert QImage ti QPixmap

    int width = ui->profile_pic->width();
    int height = ui->profile_pic->height();

    bmp = bmp.scaled(width,height,Qt::KeepAspectRatio);

    //set the choosen image

    ui->profile_pic->setPixmap(bmp);
}


void create_account::add_account(){


        QSqlQuery query;
        query.prepare( "INSERT INTO users (username, password, mail, image) VALUES (:username, :password, :mail, :image);" );

        query.bindValue(":username", ui->lineEdit->text() );
        query.bindValue(":password", ui->lineEdit_2->text());
        query.bindValue(":mail", ui->lineEdit_3->text());

        //transform QPixmap to QByteArray
        QPixmap user_image = ui->profile_pic->pixmap( Qt::ReturnByValueConstant() );

        QByteArray bArray;
        QBuffer buffer(&bArray);
        buffer.open(QIODevice::WriteOnly);
        user_image.save(&buffer,"PNG");

        query.bindValue(":image", bArray);
        query.exec();

        query.exec(" UPDATE users SET songs_index = id; ");

}

