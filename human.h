#ifndef HUMAN_H
#define HUMAN_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPropertyAnimation>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

class Plant;

class Human : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(int flashAlpha READ flashAlpha WRITE setFlashAlpha)
public:
    explicit Human(int type,int row);
    int Type;//记录僵尸种类
    int Health;//记录僵尸血量
    int MaxHealth;//记录僵尸最大血量
    float Speed;//记录僵尸速度
    int Damage;//记录僵尸伤害
    int PosR;//记录僵尸行数
    void Stop();//流血计时器关闭
    void Start();//流血计时器开启
    void GetHurt(int damage);//处理受伤情况
    void Attack(Plant* plant);//处理攻击情况
    int AttackCountdown=0;//攻击倒计时
    bool Moveable=true;//遇到植物时不可移动
    bool IsBlood=0;//是否流血
    void Blood();//开启流血
    bool IsSlow=0;//是否减速
    void Slow();//开启减速
private:
    QString TypeName;//种类名字
    QTimer *B_Timer;//流血计时器

    //受伤动画部分
    QPropertyAnimation *F_Anim;
    int m_flashAlpha = 0;
    int flashAlpha() const { return m_flashAlpha; }
    void setFlashAlpha(int alpha);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void HurtFlash();

    QMediaPlayer *AttackPlayer;
    QMediaPlayer *HurtPlayer;
signals:
    void dead(Human* newHuman);//死亡时处理
};

#endif // HUMAN_H
