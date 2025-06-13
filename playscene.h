#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QGraphicsView>
#include <QVector>
#include <QLabel>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include "plant.h"
#include "bullet.h"
#include "human.h"
#include "card.h"
#include "brick.h"
namespace Ui {
class PlayScene;
}

class PlayScene : public QGraphicsView
{
    Q_OBJECT

public:
    PlayScene(QString Path = "");
    ~PlayScene();
    void SetBackGround();//设置地图背景
    void SetCardSlot();//显示卡槽等其他素材
    void SetCard();//放置卡片，使其可点击
    void SetProp();//放置道具
    void LoadArchive(QString Path);//加载存档数据
    void SetArchive();//存入存档数据
    void LoadBrick();//加载地图砖块
    void HumanGenerate();//生成人类
    void Detecter();//每帧的检测器
    void SetPauseWidget();//设置暂停时的窗口
    void SetHumanGenerate();//生成人类
    void SetStar();//放置星星标签
    void UpdateStar();//更新星星显示
    void SetCoin();//放置金币标签
    void UpdateCoin();//更新金币显示
    void SetAudio();//设置音频
    void SetLabel();//设置进度
    void UpdateLabel();//更新标签
    void closeEvent(QCloseEvent *event);
private:
    QLabel *RoundLabel;//关卡标签
    QTimer *HumanG_Timer;//僵尸生成计时器
    int HumanG_RemainingTime=0;//防止暂停后出bug
    QTimer *Detect_Timer;//游戏帧计时器
    QGraphicsPixmapItem *BackGround;//背景图
    QGraphicsScene *LevelScene;//场景
    int Type=10;//标记当前选择的是哪种植物/铲子/道具
    QVector<QVector<bool> > map;//记录地图数据
    QVector<QVector<Brick*>> BrickList;//地砖数据
    QVector<Plant*> PlantList;//记录植物数据
    QVector<Bullet*> BulletList;//记录子弹数据
    QVector<Human*> HumanList;//记录人类数据
    QVector<Card*> CardList;//记录卡牌数据
    int Star=100;//记录有多少星星
    QLabel *StarLabel;//显示星星的标签
    int Coin=10;//记录有多少金币
    QLabel *CoinLabel;//显示金币的标签
    int UPdateStarCountdown=0;//每5s加一次星星
    bool Lose=false;
    QLabel* LoseLabel;//失败控制
    int MaxRound=30;//总轮数
    int Round=0;//剩余轮数
    bool Win=false;
    QLabel* WinLabel;//胜利控制
    int HumanDense=0;//刷怪密度
    int HumanType=0;//刷怪类型

    QAudioOutput *MusicOutput;
    QAudioOutput *HumanGenerateAudio;
    QMediaPlayer *HumanGeneratePlayer;
    QAudioOutput *PauseAudio;
    QMediaPlayer *PausePlayer;
    QAudioOutput *MoneyAudio;
    QMediaPlayer *MoneyPlayer;
    QAudioOutput *PlantAudio;
    QMediaPlayer *PlantPlayer;
    QAudioOutput *ShovelAudio;
    QMediaPlayer *ShovelPlayer;
    QAudioOutput *PropAudio;
    QMediaPlayer *PropPlayer;

signals:
    void back();
    void restart();
};




#endif // PLAYSCENE_H
