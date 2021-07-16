#ifndef PLAYER_H
#define PLAYER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPixmap>
#include <QSqlError>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QFileInfo>
#include <QHash>
#include <QMediaPlayer>
#include <QProgressBar>
#include <QMediaContent>

namespace Ui {
class player;
}

class player : public QDialog
{
    Q_OBJECT

    QHash<QPushButton*, QString> buttonToStringMap;
    QHash<QPushButton*, QPixmap> buttonToPixmap;
    QHash<QPushButton*, QString> buttonToUrlMap;

public:
    explicit player(QWidget *parent = nullptr);
    ~player();



private slots:
    void on_options_textActivated(const QString &arg1);

    void on_add_song_button_clicked();
    void play_song();
    void on_play_clicked();
    void on_pause_clicked();


private:
    Ui::player *ui;

    void setProfile();
    void logout();
    void quit_app();
    void music_tab_init();
    void add_song();
    void media_player_init();
    void songs_init();

    QMediaPlayer *music_player;



};

#endif // PLAYER_H
