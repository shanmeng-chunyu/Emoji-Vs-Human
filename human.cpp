#include "human.h"
#include <QPixmap>
#include <QString>
#include <QPainter>
#include <QMediaPlayer>
#include "plant.h"
Human::Human(int type,int row)
{
    QAudioOutput *AudioOutput = new QAudioOutput(this);
    AttackPlayer = new QMediaPlayer(this);
    AttackPlayer->setAudioOutput(AudioOutput);
    AttackPlayer->setSource(QUrl("qrc:/Music/resource/Music/chomp.ogg"));
    QAudioOutput *AudioOutput2 = new QAudioOutput(this);
    HurtPlayer = new QMediaPlayer(this);
    HurtPlayer->setAudioOutput(AudioOutput2);
    HurtPlayer->setSource(QUrl("qrc:/Music/resource/Music/kernelpult.ogg"));
    PosR=row;
    Type=type;
    QString path;
    switch(type){
    case 1:
        path=QString(":/Enemy/resource/EnemyPNG/1Boy.png");
        TypeName="Boy";
        Speed=1;
        Health=100;
        MaxHealth=100;
        Damage=20;
        setScale(0.2);
        break;
    case 2:
        path=QString(":/Enemy/resource/EnemyPNG/1Father.png");
        TypeName="Father";
        Speed=0.5;
        Health=200;
        MaxHealth=200;
        Damage=20;
        setScale(0.35);
        break;
    case 3:
        path=QString(":/Enemy/resource/EnemyPNG/1Grandpa.png");
        TypeName="Grandpa";
        Speed=0.5;
        Health=400;
        MaxHealth=400;
        Damage=20;
        setScale(0.4);
        break;
    case 4:
        path=QString(":/Enemy/resource/EnemyPNG/1Oldman.png");
        TypeName="Oldman";
        Speed=0.5;
        Health=600;
        MaxHealth=600;
        Damage=20;
        setScale(0.4);
        break;
    case 5:
        path=QString(":/Enemy/resource/EnemyPNG/1Onepunch.png");
        TypeName="Onepunch";
        Speed=0.35;
        Health=800;
        MaxHealth=800;
        Damage=200;
        setScale(0.5);
        break;
    }
    setPixmap(QPixmap(path));
    B_Timer=new QTimer(this);
    setCacheMode(DeviceCoordinateCache);
    F_Anim=new QPropertyAnimation(this, "flashAlpha");
    F_Anim->setDuration(100); // 200ms完成闪烁
    F_Anim->setStartValue(200);
    F_Anim->setEndValue(0);
}

void Human::Stop(){
    B_Timer->stop();
}

void Human::Start(){
    if(IsBlood)B_Timer->start(100);
}

void Human::GetHurt(int damage){
    HurtPlayer->play();
    Health-=damage;
    F_Anim->start();
    if(0.8*MaxHealth<Health && Health<=MaxHealth){
        setPixmap(QPixmap(QString(":/Enemy/resource/EnemyPNG/1%1.png").arg(TypeName)));
    }else if(0.6*MaxHealth<Health && Health<=0.8*MaxHealth){
        setPixmap(QPixmap(QString(":/Enemy/resource/EnemyPNG/2%1.png").arg(TypeName)));
    }else if(0.4*MaxHealth<Health && Health<=0.6*MaxHealth){
        setPixmap(QPixmap(QString(":/Enemy/resource/EnemyPNG/3%1.png").arg(TypeName)));
    }else if(0.2*MaxHealth<Health && Health<=0.4*MaxHealth){
        setPixmap(QPixmap(QString(":/Enemy/resource/EnemyPNG/4%1.png").arg(TypeName)));
    }else if(0<Health && Health<=0.2*MaxHealth){
        setPixmap(QPixmap(QString(":/Enemy/resource/EnemyPNG/5%1.png").arg(TypeName)));
    }
    if(Health<=0){
        emit dead(this);
    }
}

void Human::Attack(Plant* plant){
    AttackPlayer->play();
    plant->GetHurt(Damage);
}

void Human::Blood(){
    if(IsBlood){
        B_Timer->start(1000);
        connect(B_Timer,&QTimer::timeout,this,[this](){GetHurt(5);});
    }
}

void Human::Slow(){
    if(IsSlow){
        Speed*=0.6;
    }
}

void Human::setFlashAlpha(int alpha) {
    m_flashAlpha = alpha;
    update();
}

void Human::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QGraphicsPixmapItem::paint(painter, option, widget);
    // 2. 叠加白色高光层
    if (m_flashAlpha > 0) {
        painter->setCompositionMode(QPainter::CompositionMode_Screen);
        painter->fillRect(boundingRect(), QColor(255, 255, 255, m_flashAlpha));
    }
}
