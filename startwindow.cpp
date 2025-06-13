#include "startwindow.h"
#include "ui_startwindow.h"
#include <QPixmap>
#include <QLabel>
#include <QPainter>
#include <QFont>
#include <QPushButton>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include "pausebutton.h"
StartWindow::StartWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StartWindow)
{
    ui->setupUi(this);
    setFixedSize(794,446);
    setWindowIcon(QPixmap(":/Emoji/resource/EmojiPNG/Emoji1.png"));
    Scene=new QGraphicsScene(this);
    View=new QGraphicsView(Scene,this);
    Scene->setSceneRect(0,0,794,446);
    setTitleLabel();
    setStartButton();
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::setTitleLabel(){
    this->setWindowTitle("Emoji-Vs-Human");
    QLabel *Title=new QLabel("Emoji-Vs-Human",this);
    Title->move((this->width()-Title->width())*0.43,height()*0.2);
    QFont font;
    font.setBold(true);
    font.setPointSize(20);
    Title->setFont(font);
}

void StartWindow::setStartButton(){
    QAudioOutput *AudioOutput = new QAudioOutput(this);
    QMediaPlayer *StartPlayer = new QMediaPlayer(this);
    StartPlayer->setAudioOutput(AudioOutput);
    StartPlayer->setSource(QUrl("qrc:/Music/resource/Music/buttonclick.ogg"));
    PauseButton *startButton=new PauseButton("新的游戏");
    Scene->addItem(startButton);
    startButton->setPos((this->width()-100)*0.5,this->height()*0.5);
    PauseButton *Load = new PauseButton("读取存档");
    Scene->addItem(Load);
    Load->setPos((this->width()-100)*0.5,this->height()*0.6);
    connect(startButton,&PauseButton::clicked,[=](){
        StartPlayer->play();
        play=new PlayScene(QString("Normal.txt"));
        play->setGeometry(this->geometry());
        play->show();
        this->hide();
        emit newPlayScene();
    });
    connect(this,&StartWindow::newPlayScene,this,[=](){
        connect(play,&PlayScene::back,[=](){
            setGeometry(play->geometry());
            play->hide();
            this->show();
            play->deleteLater();
        });
        connect(play,&PlayScene::restart,[=](){
            this->setGeometry(play->geometry());
            play->hide();
            PlayScene *todelete=play;
            play=new PlayScene(QString("Normal.txt"));
            play->setGeometry(this->geometry());
            play->show();
            todelete->deleteLater();
            emit newPlayScene();
        });
    });
    connect(Load,&PauseButton::clicked,[=](){
        StartPlayer->play();
        play=new PlayScene(QString("Archive.txt"));
        play->setGeometry(this->geometry());
        play->show();
        this->hide();
        emit newPlayScene();
    });
}

void StartWindow::paintEvent(QPaintEvent *ev){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/others/resource/others/StartWindowBackground.png");
    painter.drawPixmap(0,30,this->width(),this->height(),pix);
}
