#include "plant.h"
#include <QPixmap>
#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMovie>
#include <QPainter>
#include <QDebug>
#include "bullet.h"
#include "playscene.h"
Plant::Plant(int type,int R,int C,QGraphicsPixmapItem *parent)
    : QGraphicsPixmapItem{parent}
{
    PosR=R;
    PosC=C;
    Health=100;
    Type=type;
    BulletType=1;
    A_Timer=new QTimer(this);
    setCacheMode(DeviceCoordinateCache);
    F_Anim=new QPropertyAnimation(this, "flashAlpha");
    F_Anim->setDuration(100); // 200ms完成闪烁
    F_Anim->setStartValue(200);
    F_Anim->setEndValue(0);
    setAcceptedMouseButtons(Qt::LeftButton);
    QString path="";
    switch(type){
    case 1:
        path=QString(":/Emoji/resource/EmojiGIF/Emoji1.gif");
        break;
    case 2:
        path=QString(":/Emoji/resource/EmojiGIF/Emoji6.gif");
        AttackTime=10000;
        break;
    case 3:
        path=QString(":/Emoji/resource/EmojiGIF/Emoji4.gif");
        BulletType=2;
        break;
    case 4:
        path=QString(":/Emoji/resource/EmojiGIF/Emoji3.gif");
        BulletType=3;
        break;
    case 5:
        path=QString(":/Emoji/resource/EmojiGIF/Emoji7.gif");
        BulletType=4;
        break;
    case 6:
        path=QString(":/Emoji/resource/EmojiGIF/Emoji2.gif");
        break;
    }
    connect(A_Timer,&QTimer::timeout,[=](){
        Attack();
        A_Timer->start(AttackTime);
    });//植物到点，发射子弹
    M_Movie=new QMovie(path);
    M_Movie->setParent(this);
    connect(M_Movie, &QMovie::frameChanged, this, [this](int frameNumber) {
        Q_UNUSED(frameNumber);
        setPixmap(M_Movie->currentPixmap());
    });
    M_Movie->start();
    setScale(0.25);
}

void Plant::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        emit clicked();
    }
}

void Plant::Stop(){
    if(A_Timer->isActive())RemainingTime=A_Timer->remainingTime();
    A_Timer->stop();
    M_Movie->setPaused(true);
}

void Plant::Start(){
    if(RemainingTime){
        A_Timer->start(RemainingTime);
        RemainingTime=0;
    }
    M_Movie->setPaused(false);
}

void Plant::Attack(){
    Bullet *newBullet;
    switch(BulletType){
    case 1:
        newBullet = new Bullet();
        break;
    case 2:
        newBullet = new Heart();
        break;
    case 3:
        newBullet = new Blood();
        break;
    case 4:
        newBullet = new Snow();
    }
    newBullet->setPos(x()+30,y()+10);
    emit attack(newBullet);//连接到playscene里添加子弹
}

void Plant::GetHurt(int damage){
    Health-=damage;
    F_Anim->start();
    if(Health<=0){
        emit dead(this);
    }
}

void Plant::setFlashAlpha(int alpha) {
    m_flashAlpha = alpha;
    update();
}

void Plant::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsPixmapItem::paint(painter, option, widget);
    // 2. 叠加白色高光层
    if (m_flashAlpha > 0) {
        painter->setCompositionMode(QPainter::CompositionMode_Screen);
        painter->fillRect(boundingRect(), QColor(255, 255, 255, m_flashAlpha));
    }
}

Laugh::Laugh(int X,int Y):Plant(6,X,Y)
{
    A_Timer2=new QTimer(this);
    connect(A_Timer2,&QTimer::timeout,[this](){
        Bullet *newBullet = new Bullet();
        newBullet->setPos(x()+30,y()+10);
        emit attack(newBullet);//连接到playscene里添加子弹
        A_Timer2->stop();
    });
}

void Laugh::Attack(){
    Bullet *newBullet = new Bullet();
    newBullet->setPos(x()+30,y()+10);
    emit attack(newBullet);
    A_Timer2->start(297);
}

void Laugh::Stop(){
    if(A_Timer->isActive())RemainingTime=A_Timer->remainingTime();
    if(A_Timer2->isActive())RemainingTime2=A_Timer2->remainingTime();
    A_Timer->stop();
    A_Timer2->stop();
    M_Movie->setPaused(true);
}

void Laugh::Start(){
    if(RemainingTime){
        A_Timer->start(RemainingTime);
        RemainingTime=0;
    }
    if(RemainingTime2){
        A_Timer2->start(RemainingTime2);
        RemainingTime2=0;
    }
    M_Movie->setPaused(false);
}

Stars::Stars(int X,int Y):Plant(2,X,Y)
{
    A_Timer->start(AttackTime);
}

void Stars::Attack(){
    Starb *newStar=new Starb();
    newStar->setupAnimation(x(),y());
    newStar->setPos(x()+40,y()+10);
    newStar->startBounce(x(),y());
    emit G_Star(newStar);
}
