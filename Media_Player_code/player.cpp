#include "player.h"
#include "ui_player.h"

player::player(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::player)
{
    ui->setupUi(this);

    this->setProfile();
    this->music_tab_init();
    this->media_player_init();
    this->songs_init();

}

player::~player()
{
    delete ui;
}

void player::setProfile(){

    QSqlQuery query;

    //set photo

    if( !query.exec( "SELECT image FROM users WHERE active_session = 'true' ;" ) ){
            qDebug() << "Error getting image from table:\n" << query.lastError();
     }

    query.first();
    QByteArray outByteArray = query.value( 0 ).toByteArray();
    QPixmap user_pixmap = QPixmap();
    user_pixmap.loadFromData( outByteArray );

    int width = ui->image_profile->width();
    int height = ui->image_profile->height();

    user_pixmap = user_pixmap.scaled( width, height, Qt::KeepAspectRatio );

    ui->image_profile->setPixmap(user_pixmap);

    //set username

    QSqlQuery username_query;

    username_query.exec(" SELECT username FROM users WHERE active_session = 'true' ;");
    username_query.first();
    QString username;
    username = username_query.value(0).toString();

    ui->label_username->setText(username);

    //ComboBox

    ui->options->insertItem(0,"Options");

    QIcon logout(":/img/img/logout.png");
    ui->options->insertItem(1, logout, "Logout" );

    QIcon quit(":/img/img/delete.png");
    ui->options->insertItem(2, quit, "Quit");

}

void player::on_options_textActivated(const QString &arg1)
{
    if( arg1 == "Logout" ){
        music_player->stop();
        this->logout();
    }else if( arg1 == "Quit" ){
        this->quit_app();
    }
}

void player::quit_app(){

    QSqlQuery query;
    query.exec( "UPDATE users SET active_session = 'false' WHERE active_session = 'true' ;" );

    QApplication::quit();

}

void player::logout(){

    QSqlQuery query;
    query.exec( "UPDATE users SET active_session = 'false' WHERE active_session = 'true' ;" );

    this->close();

    parentWidget()->show();

}

void player::music_tab_init(){

    ui->tabWidget->setCurrentIndex(0);

}

void player::songs_init(){

    QSqlQuery query;
    query.exec( "SELECT user_song, image, song_path FROM songs ;");

    while( query.next()  ){

        if( query.value(0) == ui->label_username->text() ){

            QVBoxLayout *v_layout =  qobject_cast<QVBoxLayout*>( ui->songs_layout->layout() );

            QHBoxLayout *h_layout = new QHBoxLayout(ui->verticalWidget);

            QByteArray outByteArray = query.value(1).toByteArray();
            QPixmap song_photo = QPixmap();
            song_photo.loadFromData(outByteArray);

            QString song_path = query.value(2).toString();

            //upload photo

            QLabel *song_pic = new QLabel;
            song_pic->setGeometry( song_pic->x(), song_pic->y(), 60, 60 );
            song_photo = song_photo.scaled(60, 60, Qt::KeepAspectRatio);

            song_pic->setPixmap(song_photo);
            h_layout->addWidget(song_pic);

            //upload song

            QFileInfo file(song_path);

            QLabel *song_name = new QLabel();
            song_name->setGeometry( song_name->x(), song_name->y(), 300, 60 );
            song_name->setStyleSheet(" color: white; font-size: 15px; ");
            song_name->setText( file.completeBaseName() );

            QString temp_name = song_name->text();

            h_layout->addWidget(song_name);

            //play button

            QPushButton *play_button = new QPushButton();
            play_button->setStyleSheet( " QPushButton{ image: url(:/img/img/play-button.png); border-style: hidden; width: 60px; height: 60px; padding: 5px; } QPushButton:hover{ background-color: #e68a00;  }  " );

            h_layout->addWidget(play_button);

            QObject::connect( play_button, &QPushButton::clicked, this, &player::play_song );

            //Prepare player

            buttonToStringMap.insert(play_button,temp_name);
            buttonToPixmap.insert(play_button, song_photo);
            buttonToUrlMap.insert(play_button,song_path);

            //set stretch
            h_layout->setStretch(1,3);

            v_layout->insertLayout(0,h_layout);

        }

    }

}

void player::add_song(){

    QVBoxLayout *v_layout =  qobject_cast<QVBoxLayout*>( ui->songs_layout->layout() );

    QHBoxLayout *h_layout = new QHBoxLayout(ui->verticalWidget);

    //choose photo for song
    QLabel *song_pic = new QLabel;
    song_pic->setGeometry( song_pic->x(), song_pic->y(), 60, 60 );

    QString fisier = QFileDialog::getOpenFileName(this,"Choose a picture for the song...",QDir::homePath());

    QPixmap song_photo(fisier);
    song_photo = song_photo.scaled(60, 60, Qt::KeepAspectRatio);

    song_pic->setPixmap(song_photo);

    h_layout->addWidget(song_pic);

    //choose song

    QString song_file = QFileDialog::getOpenFileName(this,"Choose the song", QDir::homePath());
    QFileInfo file(song_file);

    QLabel *song_name = new QLabel();
    song_name->setGeometry( song_name->x(), song_name->y(), 300, 60 );
    song_name->setStyleSheet(" color: white; font-size: 15px; ");
    song_name->setText( file.completeBaseName() );

    QString temp_name = song_name->text();

    h_layout->addWidget(song_name);

    //save song_photo and song path to db

    QByteArray inByteArray;
    QBuffer inBuffer(&inByteArray);
    inBuffer.open( QIODevice::WriteOnly );
    song_photo.save( &inBuffer, "PNG" ); //inByteArray contain the photo

    QSqlQuery query;
    query.prepare(" INSERT INTO songs (user_song, image, song_path) VALUES(:user, :img, :song) ; ");
    query.bindValue(":user", ui->label_username->text());
    query.bindValue(":img", inByteArray);
    query.bindValue(":song", song_file);

    query.exec();


    //play button

    QPushButton *play_button = new QPushButton();
    play_button->setStyleSheet( " QPushButton{ image: url(:/img/img/play-button.png); border-style: hidden; width: 60px; height: 60px; padding: 5px; } QPushButton:hover{ background-color: #e68a00;  }  " );

    h_layout->addWidget(play_button);

    QObject::connect( play_button, &QPushButton::clicked, this, &player::play_song );

    //Prepare player

    buttonToStringMap.insert(play_button,temp_name);
    buttonToPixmap.insert(play_button, song_photo);
    buttonToUrlMap.insert(play_button,song_file);

    //set stretch
    h_layout->setStretch(1,3);

    v_layout->insertLayout(0,h_layout);

}


void player::on_add_song_button_clicked()
{
    this->add_song();
}

void player::play_song(){

    QPushButton *button = qobject_cast<QPushButton*>(sender());

    QString song_namee = buttonToStringMap.value(button);
    ui->current_song->setText(song_namee);

    QPixmap song_photoo(buttonToPixmap.value(button));
    song_photoo = song_photoo.scaled( 81, 81, Qt::KeepAspectRatio );
    ui->song_photo->setPixmap(song_photoo);

    QString url = buttonToUrlMap.value(button);
    music_player->setMedia(QUrl::fromLocalFile(url));

    qDebug() << url;

    this->on_play_clicked();

}

void player::media_player_init(){

    music_player = new QMediaPlayer(this);

    music_player->setVolume(60);

    connect(music_player, &QMediaPlayer::durationChanged, ui->timeElapseSlider, &QSlider::setMaximum);
    connect(music_player, &QMediaPlayer::positionChanged, ui->timeElapseSlider, &QSlider::setValue);
    connect(ui->timeElapseSlider, &QSlider::sliderMoved, music_player, &QMediaPlayer::setPosition);
    connect(ui->volumeSlider, &QSlider::valueChanged, music_player, &QMediaPlayer::volumeChanged);
    connect(ui->volumeSlider, &QSlider::valueChanged, music_player, &QMediaPlayer::setVolume);

}


void player::on_play_clicked()
{

    music_player->play();

}


void player::on_pause_clicked()
{
    music_player->pause();
}

