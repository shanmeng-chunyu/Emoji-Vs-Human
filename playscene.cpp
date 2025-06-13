#include "playscene.h"
#include "ui_playscene.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QRandomGenerator>
#include <QList>
#include <QMediaPlayer>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QMediaDevices>
#include <QMediaFormat>
#include <QAudioDevice>
#include <QLabel>
#include "card.h"
#include "brick.h"
#include "plant.h"
#include "human.h"
#include "pausebutton.h"
#include "pausewidget.h"

PlayScene::PlayScene(QString Path)
{
    setFixedSize(800,460);
    this->setWindowTitle("Emoji-Vs-Human");
    setWindowIcon(QPixmap(":/Emoji/resource/EmojiPNG/Emoji1.png"));
    LevelScene=new QGraphicsScene(this);
    LevelScene->setSceneRect(this->rect());
    Star=100;//初始星星
    MaxRound=30;//总轮数
    Coin=10;//初始金币

    HumanDense=1;//控制出怪密度
    HumanType=2;//控制出怪类型


    SetAudio();//设置音频
    setScene(LevelScene);//设置场景
    SetBackGround();//设置背景地图
    SetLabel();//设置标签
    SetCardSlot();//设置卡槽
    SetCard();//设置卡片
    SetProp();//设置道具
    SetStar();//设置星星系统
    SetCoin();//设置金币系统
    LoadArchive(Path);//读取存档
    Detecter();//设置侦测器
    SetPauseWidget();//设置暂停窗口
    SetHumanGenerate();//设置人类生成系统
}

PlayScene::~PlayScene()
{
    if(!Win && !Lose){
        SetArchive();
    }else{
        QString Normalpath = QDir(QCoreApplication::applicationDirPath()).filePath("Normal.txt");
        QFile NormalFile(Normalpath);
        NormalFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&NormalFile);
        QString content=in.readAll();
        NormalFile.close();
        QString Archivepath = QDir(QCoreApplication::applicationDirPath()).filePath("Archive.txt");
        QFile ArchiveFile(Archivepath);
        ArchiveFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&ArchiveFile);
        out<<content;
        ArchiveFile.close();
    }
    qDeleteAll(PlantList);
    qDeleteAll(BulletList);
    qDeleteAll(HumanList);
    qDeleteAll(CardList);
}

void PlayScene::closeEvent(QCloseEvent *event){
    if(!Win && !Lose){
        SetArchive();
    }else{
        QString Normalpath = QDir(QCoreApplication::applicationDirPath()).filePath("Normal.txt");
        QFile NormalFile(Normalpath);
        NormalFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&NormalFile);
        QString content=in.readAll();
        NormalFile.close();
        QString Archivepath = QDir(QCoreApplication::applicationDirPath()).filePath("Archive.txt");
        QFile ArchiveFile(Archivepath);
        ArchiveFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&ArchiveFile);
        out<<content;
        ArchiveFile.close();
    }
}

void PlayScene::SetBackGround(){
    BackGround=new QGraphicsPixmapItem();
    BackGround->setPixmap(QPixmap(":/map/resource/maps/map.png"));
    BackGround->setZValue(-100);
    LevelScene->addItem(BackGround);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(BackGround->boundingRect());
}//显示背景底图

void PlayScene::SetCardSlot(){
    QGraphicsPixmapItem *CardSlot=new QGraphicsPixmapItem();
    QPixmap board=QPixmap(QPixmap(":/others/resource/others/SelectBoard.png"));
    CardSlot->setPixmap(board);
    CardSlot->setZValue(-100);
    CardSlot->setPos((width()-board.width())*0.5,0);
    LevelScene->addItem(CardSlot);
}//显示emoji卡槽与道具卡槽

void PlayScene::SetLabel(){
    RoundLabel = new QLabel(this->viewport());
    RoundLabel->setStyleSheet("QLabel {"
                            "  background: transparent;"    // 完全透明背景
                            "  border: none;"               // 无边框
                            "  color: red;"               // 文字颜色
                            "  font: bold 17px Arial;"      // 推荐使用清晰字体
                            "}");
    RoundLabel->setAlignment(Qt::AlignCenter);
    RoundLabel->setText(QString("波数:%1 / %2").arg(Round).arg(MaxRound));
    RoundLabel->adjustSize();
    RoundLabel->move((width()-RoundLabel->width())*0.9,height()*0.9);
    RoundLabel->show();
}

void PlayScene::UpdateLabel(){
    RoundLabel->setText(QString("波数:%1 / %2").arg(Round).arg(MaxRound));
    RoundLabel->adjustSize();
}

void PlayScene::SetCard(){
    Card *shovel=new Card(0);
    shovel->setPos(598,7);
    LevelScene->addItem(shovel);
    CardList.append(shovel);
    connect(shovel,&Card::clicked,this,[this](int type){
        Type=type;
        ShovelPlayer->play();
    });//先单独设置铲子
    for (int i = 0; i < 6; ++i) {
        Card *card=new Card(i+1);
        card->setPos(213+i*55,7);
        switch(i){
        case 1:
            card->setPos(215+i*55,7);
            break;
        case 2:
            card->setPos(217+i*55,7);
            break;
        case 3:
            card->setPos(215+i*55,7);
            break;
        }
        LevelScene->addItem(card);
        CardList.append(card);
        connect(card,&Card::clicked,this,[this](int type){
            Type=type;
        });
    }
    //链接信号与槽，点击卡片会将type置为其值，方便后续放置
}

void PlayScene::SetProp(){
    Card *Rocket = new Card(7);
    Rocket->setPos(10,100);
    LevelScene->addItem(Rocket);
    CardList.append(Rocket);
    connect(Rocket,&Card::clicked,this,[this](int type){Type=type;});
    QLabel *RocketLabel=new QLabel(this->viewport());
    RocketLabel->setStyleSheet("QLabel {"
                               "  background: transparent;"    // 完全透明背景
                               "  border: none;"               // 无边框
                               "  color: black;"               // 文字颜色
                               "  font: bold 17px Arial;"      // 推荐使用清晰字体
                               "}");
    RocketLabel->setAlignment(Qt::AlignCenter);
    RocketLabel->setText(QString("<b>10金币"));
    RocketLabel->move(60,130);
    RocketLabel->show();
    connect(Rocket,&Card::clicked,this,[=](){
        PropPlayer->play();
        for (int i = 0; i < 5; ++i) {
            Rocketb *rocket=new Rocketb();
            rocket->setPos(30,100+i*53);
            LevelScene->addItem(rocket);
            BulletList.append(rocket);
        }
        Coin-=10;
        UpdateCoin();
    });//火箭部分
    Card *Candy = new Card(8);
    Candy->setPos(10,175);
    LevelScene->addItem(Candy);
    CardList.append(Candy);
    QLabel *CandyLabel=new QLabel(this->viewport());
    CandyLabel->setStyleSheet("QLabel {"
                               "  background: transparent;"    // 完全透明背景
                               "  border: none;"               // 无边框
                               "  color: black;"               // 文字颜色
                               "  font: bold 17px Arial;"      // 推荐使用清晰字体
                               "}");
    CandyLabel->setAlignment(Qt::AlignCenter);
    CandyLabel->setText(QString("<b>5金币"));
    CandyLabel->move(60,205);
    CandyLabel->show();
    connect(Candy,&Card::clicked,this,[this](){
        PropPlayer->play();
        QLabel *HealthLabel=new QLabel(this->viewport());
        HealthLabel->setStyleSheet("QLabel {"
                                   "  background: transparent;"    // 完全透明背景
                                   "  border: none;"               // 无边框
                                   "  color: red;"               // 文字颜色
                                   "  font: bold 35px Arial;"      // 推荐使用清晰字体
                                   "}");
        HealthLabel->setAlignment(Qt::AlignCenter);
        HealthLabel->setText(QString("<b>Emoji全都充满了活力！"));
        HealthLabel->move((width()-HealthLabel->width())*0.4,height()*0.4);
        HealthLabel->show();
        QTimer::singleShot(2000,[=](){
            delete HealthLabel;
        });
        for(Plant* plant:PlantList){
            plant->Health=100;
        }
        Coin-=5;
        UpdateCoin();
    });//糖果部分
}

void PlayScene::SetStar(){
    StarLabel = new QLabel(this->viewport());
    StarLabel->setStyleSheet("QLabel {"
                             "  background: transparent;"    // 完全透明背景
                             "  border: none;"               // 无边框
                             "  color: black;"               // 文字颜色
                             "  font: bold 17px Arial;"      // 推荐使用清晰字体
                            "}");
    StarLabel->setAlignment(Qt::AlignCenter);
    StarLabel->move(164,68);
    UpdateStar();//设置星星标签
}

void PlayScene::UpdateStar(){
    StarLabel->setText(QString::number(Star));
    StarLabel->adjustSize();//每次更新
}

void PlayScene::SetCoin(){
    QGraphicsPixmapItem *CoinPixmap=new QGraphicsPixmapItem();
    CoinPixmap->setPixmap(QPixmap(":/others/resource/others/Money.png"));
    CoinPixmap->setScale(0.4);
    LevelScene->addItem(CoinPixmap);
    CoinPixmap->setPos(10,10);
    CoinLabel=new QLabel(this->viewport());
    CoinLabel->setStyleSheet("QLabel {"
                             "  background: transparent;"    // 完全透明背景
                             "  border: none;"               // 无边框
                             "  color: black;"               // 文字颜色
                             "  font: bold 17px Arial;"      // 推荐使用清晰字体
                             "}");
    CoinLabel->setAlignment(Qt::AlignCenter);
    CoinLabel->move(30,63);
    UpdateCoin();//设置金币标签
}

void PlayScene::UpdateCoin(){
    CoinLabel->setText(QString::number(Coin));
    CoinLabel->adjustSize();
}

void PlayScene::LoadArchive(QString Path){
    QString path = QDir(QCoreApplication::applicationDirPath()).filePath(Path);
    QFile ArchiveFile(path);
    if(ArchiveFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&ArchiveFile);

        QString nothing;
        in>>nothing;
        int humangremainingtime;
        in>>humangremainingtime;
        HumanG_RemainingTime=humangremainingtime;//僵尸生成倒计时

        in>>nothing;
        int star;
        in>>star;
        Star=star;
        UpdateStar();//记录星星数量

        in>>nothing;
        int coin;
        in>>coin;
        Coin=coin;
        UpdateCoin();//记录金币数量

        in>>nothing;
        int updatestarcountdown;
        in>>updatestarcountdown;
        UPdateStarCountdown=updatestarcountdown;//记录阳光生成倒计时


        in>>nothing;
        int maxround,round;
        in>>maxround>>nothing>>round;
        MaxRound=maxround;
        Round=round;
        UpdateLabel();//记录轮数

        in.readLine();
        in.readLine();
        for (int i = 0; i < 5; ++i) {
            const QString line = in.readLine().trimmed();
            QVector<bool> row;
            const QStringList bits = line.split(' ', Qt::SkipEmptyParts);
            for (const QString &bit : bits) {
                row.append(bit=='1');
            }
            map.append(row);
        }
        LoadBrick();//设置砖块
        //读取地图部分

        in.readLine();
        int Plantnum;
        in>>Plantnum;
        for (int i = 0; i < Plantnum; ++i) {
            int type,health,r,c,remainingtime,x,y;
            in>>type>>health>>r>>c>>remainingtime>>x>>y;
            Plant *plant;
            if(type==2){
                plant=new Stars(r,c);
            }else if(type==6){
                plant=new Laugh(r,c);
            }else{
                plant=new Plant(type,r,c);
            }
            PlantList.append(plant);
            plant->Health=health;
            plant->RemainingTime=remainingtime;
            plant->setPos(x,y);
            LevelScene->addItem(plant);
            plant->Start();
            Brick *brick=BrickList[r-1][c-1];
            brick->IsPlant=true;
            connect(plant,&Plant::clicked,[=](){
                if(Type!=0)return;
                LevelScene->removeItem(plant);
                PlantList.removeOne(plant);
                Type=10;
                brick->IsPlant=0;
                delete plant;
            });//绑定，使得铲子能够铲除Emoji
            connect(plant,&Plant::attack,[this](Bullet *newBullet){
                BulletList.append(newBullet);
                LevelScene->addItem(newBullet);
                connect(newBullet,&Bullet::destoryed,[=](){
                    LevelScene->removeItem(newBullet);
                    BulletList.removeOne(newBullet);
                    delete newBullet;
                });//移去子弹
            });//添加子弹
            if(Stars *stars=dynamic_cast<Stars*>(plant)){
                connect(stars,&Stars::G_Star,[=](Starb* starb){
                    LevelScene->addItem(starb);
                    BulletList.append(starb);
                    connect(starb,&Starb::Pickup,[=](){
                        Star+=25;
                        starb->ungrabMouse();
                        LevelScene->removeItem(starb);
                        BulletList.removeOne(starb);
                        starb->deleteLater();
                    });
                });
            }//星星独立检测
            connect(plant,&Plant::dead,[=](){
                PlantList.removeOne(plant);
                LevelScene->removeItem(plant);
                brick->IsPlant=false;
                delete plant;
            });//死亡时移除
        }
        //读取植物部分

        in>>nothing;
        int Bulletnum;
        in>>Bulletnum;
        for (int i = 0; i < Bulletnum; ++i) {
            int type;
            float x,y;
            in>>type>>x>>y;
            Bullet *bullet;
            if(type==1){
                bullet=new Bullet();
            }else if(type==2){
                bullet=new Heart();
            }else if(type==3){
                bullet=new Blood();
            }else if(type==4){
                bullet=new Snow();
            }else if(type==5){
                bullet=new Starb();
            }else if(type==6){
                bullet=new Rocketb();
            }
            BulletList.append(bullet);
            LevelScene->addItem(bullet);
            bullet->setPos(x,y);
            if(Starb *starb = dynamic_cast<Starb*>(bullet)){
                connect(starb,&Starb::Pickup,[=](){
                    Star+=25;
                    starb->ungrabMouse();
                    LevelScene->removeItem(starb);
                    BulletList.removeOne(starb);
                    starb->deleteLater();
                });
            }
            connect(bullet,&Bullet::destoryed,[=](){
                LevelScene->removeItem(bullet);
                BulletList.removeOne(bullet);
                delete bullet;
            });//移去子弹
        }//读取子弹数据

        in>>nothing;
        int Humannum;
        in>>Humannum;
        for (int i = 0; i < Humannum; ++i) {
            int type,health,row,attack,blood,slow;
            float x,y;
            in>>type>>health>>row>>attack>>blood>>slow>>x>>y;
            Human *human=new Human(type,row);
            human->Health=health;
            human->AttackCountdown=attack;
            human->IsBlood=blood;
            human->IsSlow=slow;
            LevelScene->addItem(human);
            HumanList.append(human);
            human->setPos(x,y);
            connect(human,&Human::dead,[=](){
                int DropCoin=QRandomGenerator::global()->bounded(1,11);
                if(DropCoin<=2){
                    Coinb* newCoin=new Coinb();
                    newCoin->setupAnimation(human->x(),human->y());
                    newCoin->setPos(human->x()+40,human->y()+10);
                    newCoin->startBounce(human->x(),human->y());
                    LevelScene->addItem(newCoin);
                    MoneyPlayer->play();
                    connect(newCoin,&Starb::Pickup,[=](){
                        Coin+=1;
                        newCoin->ungrabMouse();
                        LevelScene->removeItem(newCoin);
                        newCoin->deleteLater();
                        UpdateCoin();
                    });
                }//僵尸有概率掉落金币
                LevelScene->removeItem(human);
                HumanList.removeOne(human);
                delete human;
            });
        }//读取僵尸数据
        ArchiveFile.close();
    }else{
        ArchiveFile.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&ArchiveFile);
        out<<"HumanGRemainingTime: 0"
            <<"\nStar: 100"
            <<"\nCoin: 10"
            <<"\nUPdateStarCountdown: 0"
            <<"\nMaxRound: 30"
            <<"\nRound: 0"
            <<"\nMAP:"
            <<"\n1 1 1 1 1 1 1"
            <<"\n1 1 1 1 1 1 1"
            <<"\n1 1 1 1 1 1 1"
            <<"\n1 1 1 1 1 1 1"
            <<"\n1 1 1 1 1 1 1"
            <<"\nPlantList:"
            <<"\n0"
            <<"\nBulletList:"
            <<"\n0"
            <<"\nHumanList:"
            <<"\n0";
        ArchiveFile.close();
        LoadArchive(Path);
    }
}

void PlayScene::SetArchive(){
    QString path = QDir(QCoreApplication::applicationDirPath()).filePath("Archive.txt");
    QFile ArchiveFile(path);
    if(ArchiveFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream in(&ArchiveFile);

        in<<"HumanGRemainingTime: "<<HumanG_RemainingTime<<"\n";
        in<<"Star: "<<Star<<"\n";
        in<<"Coin: "<<Coin<<"\n";
        in<<"UPdateStarCountdown: "<<UPdateStarCountdown<<"\n";
        in<<"MaxRound: "<<MaxRound<<"\n";
        in<<"Round: "<<Round<<"\n";

        in<<"MAP:"<<"\n";
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 7; ++j) {
                if(BrickList[i][j]){
                    in<<1<<" ";
                }else{
                    in<<0<<" ";
                }
            }
            in<<"\n";
        }//存地图数据

        in<<"PlantList:"<<"\n";
        in<<PlantList.size()<<"\n";
        for (int i = 0; i < PlantList.size(); ++i) {
            Plant *plant=PlantList[i];
            in<<plant->Type<<" "<<plant->Health<<" "<<plant->PosR<<" "<<plant->PosC<<" "<<plant->RemainingTime<<" "<<plant->x()<<" "<<plant->y()<<"\n";
        }//存植物数据

        in<<"BulletList:"<<"\n";
        in<<BulletList.size()<<"\n";
        for (int i = 0; i < BulletList.size(); ++i) {
            Bullet *bullet=BulletList[i];
            if(dynamic_cast<Heart*>(bullet)){
                in<<2<<" ";
            }else if(dynamic_cast<Blood*>(bullet)){
                in<<3<<" ";
            }else if(dynamic_cast<Snow*>(bullet)){
                in<<4<<" ";
            }else if(dynamic_cast<Starb*>(bullet)){
                in<<5<<" ";
            }else if(dynamic_cast<Rocketb*>(bullet)){
                in<<6<<" ";
            }else{
                in<<1<<" ";
            }
            in<<bullet->x()<<" "<<bullet->y()<<"\n";
        }//存子弹数据

        in<<"HumanList:"<<"\n";
        in<<HumanList.size()<<"\n";
        for (int i = 0; i < HumanList.size(); ++i) {
            Human *human=HumanList[i];
            in<<human->Type<<" "<<human->Health<<" "<<human->PosR<<" "<<human->AttackCountdown<<" "<<human->IsBlood<<" "<<human->IsSlow<<" "<<human->x()<<" "<<human->y()<<"\n";
        }//存僵尸数据
        ArchiveFile.close();
    }
}

void PlayScene::LoadBrick(){
    for (int i = 0; i < 5; ++i) {
        QVector<Brick*> row;
        for (int j = 0; j < 7; ++j) {
            if(map[i][j]==true){
                Brick *newBrick=new Brick(i+1,j+1);
                newBrick->setPos(195+j*80,90+i*53);
                if(j==5 || j==6){
                    newBrick->setPos(195+j*83,90+i*53);
                }
                LevelScene->addItem(newBrick);
                row.append(newBrick);
                //新建地砖
                connect(newBrick,&Brick::clicked,this,[=](int r,int c){
                    if(Type==10 || Type==0 || Type==7 || Type==8)return;
                    Plant *newPlant;
                    if(Type==1 || (3<=Type && Type<=5)){
                        newPlant = new Plant(Type,r,c);
                    }else if(Type==2){
                        newPlant = new Stars(r,c);
                    }else{
                        newPlant = new Laugh(r,c);
                    }
                    newPlant->setPos(195+(c-1)*80,90+(r-1)*53);
                    if(c==6 || c==7){
                        newPlant->setPos(195+(c-1)*83,90+(r-1)*53);
                    }
                    LevelScene->addItem(newPlant);//添加新Emoji
                    PlantPlayer->play();
                    Star-=CardList[Type]->Star_n;//减少星星数
                    connect(newPlant,&Plant::clicked,[=](){
                        if(Type!=0)return;
                        LevelScene->removeItem(newPlant);
                        PlantList.removeOne(newPlant);
                        Type=10;
                        newBrick->IsPlant=0;
                        delete newPlant;
                    });//绑定，使得铲子能够铲除Emoji
                    connect(newPlant,&Plant::attack,[this](Bullet *newBullet){
                        BulletList.append(newBullet);
                        LevelScene->addItem(newBullet);
                        connect(newBullet,&Bullet::destoryed,[=](){
                            LevelScene->removeItem(newBullet);
                            BulletList.removeOne(newBullet);
                            delete newBullet;
                        });//移去子弹
                    });//添加子弹
                    if(Stars *stars=dynamic_cast<Stars*>(newPlant)){
                        connect(stars,&Stars::G_Star,[=](Starb* starb){
                            LevelScene->addItem(starb);
                            BulletList.append(starb);
                            connect(starb,&Starb::Pickup,[=](){
                                Star+=25;
                                starb->ungrabMouse();
                                LevelScene->removeItem(starb);
                                BulletList.removeOne(starb);
                                starb->deleteLater();
                            });
                        });
                    }//星星独立检测
                    connect(newPlant,&Plant::dead,[=](){
                        PlantList.removeOne(newPlant);
                        LevelScene->removeItem(newPlant);
                        newBrick->IsPlant=false;
                        delete newPlant;
                    });//死亡时移除
                    PlantList.append(newPlant);
                    Type=10;
                    newBrick->IsPlant=1;
                });//处理点击事件（已选择植物并且能种植就种植，否则无效）
            }else{
                QGraphicsPixmapItem *NotPlant=new QGraphicsPixmapItem;
                NotPlant->setPixmap(QPixmap(":/others/resource/others/NotPlant.png"));
                NotPlant->setScale(0.5);
                NotPlant->setPos(195+j*83,80+i*53);
                LevelScene->addItem(NotPlant);
                row.append(nullptr);
            }
        }
        BrickList.append(row);
    }
}

void PlayScene::SetHumanGenerate(){
    HumanG_Timer=new QTimer(this);
    HumanG_Timer->start(10000);
    connect(HumanG_Timer,&QTimer::timeout,this,&PlayScene::HumanGenerate);
    connect(HumanG_Timer,&QTimer::timeout,this,[this](){HumanG_Timer->start(10000);});
}

void PlayScene::HumanGenerate(){
    if(Round>=MaxRound)return;
    HumanDense=Round/2+1;
    HumanType=(Round/5>3)?5:(Round/5+2);
    int count=HumanDense;
    Round++;
    UpdateLabel();
    for(int i=0;i<count;i++){
        int row=QRandomGenerator::global()->bounded(1,6);
        int type=QRandomGenerator::global()->bounded(1,HumanType+1);
        Human *newHuman=new Human(type,row);
        newHuman->setPos(width()-(0.1*i)*newHuman->pixmap().width(),90+(row-1)*53);
        switch(type){
        case 1:
            newHuman->setPos(width()-0.3*newHuman->pixmap().width(),100+(row-1)*53);
            break;
        case 5:
            newHuman->setPos(width()-0.3*newHuman->pixmap().width(),87+(row-1)*53);
            break;
        }
        HumanList.append(newHuman);
        LevelScene->addItem(newHuman);
        connect(newHuman,&Human::dead,[=](){
            int DropCoin=QRandomGenerator::global()->bounded(1,11);
            if(DropCoin<=2){
                Coinb* newCoin=new Coinb();
                newCoin->setupAnimation(newHuman->x(),newHuman->y());
                newCoin->setPos(newHuman->x()+40,newHuman->y()+10);
                newCoin->startBounce(newHuman->x(),newHuman->y());
                LevelScene->addItem(newCoin);
                MoneyPlayer->play();
                connect(newCoin,&Starb::Pickup,[=](){
                    Coin+=1;
                    newCoin->ungrabMouse();
                    LevelScene->removeItem(newCoin);
                    newCoin->deleteLater();
                    UpdateCoin();
                });
            }//僵尸有概率掉落金币
            LevelScene->removeItem(newHuman);
            HumanList.removeOne(newHuman);
            delete newHuman;
        });
    }
    HumanGeneratePlayer->play();
}

void PlayScene::SetPauseWidget(){
    PauseButton *pausebuttun=new PauseButton("菜单");
    pausebuttun->setPos(width()-100,0);
    LevelScene->addItem(pausebuttun);
    PauseWidget *newWidget=new PauseWidget();
    newWidget->setPos(width()*0.5-100,height()*0.5-125); // 居中弹窗
    connect(pausebuttun,&PauseButton::clicked,[=](){
        for(Plant *plant:PlantList){
            plant->Stop();
        }
        for(Human *human:HumanList){
            human->Stop();
        }
        for(QVector<Brick*> row:BrickList){
            for(Brick* brick:row){
                if(brick){
                    brick->IsPause=true;
                }
            }
        }
        Detect_Timer->stop();
        HumanG_RemainingTime=HumanG_Timer->remainingTime();
        HumanG_Timer->stop();
        LevelScene->addItem(newWidget);
        if(Lose){
            LoseLabel->hide();
        }
        if(Win){
            WinLabel->hide();
        }
        PausePlayer->play();
    });
    connect(newWidget,&PauseWidget::close,[=](){
        LevelScene->removeItem(newWidget);
        if(Lose){
            LoseLabel->show();
            return;
        }
        if(Win){
            WinLabel->show();
            return;
        }
        for(Plant *plant:PlantList){
            plant->Start();
        }
        for(Human *human:HumanList){
            human->Start();
        }
        for(QVector<Brick*> row:BrickList){
            for(Brick* brick:row){
                if(brick){
                    brick->IsPause=false;
                }
            }
        }
        Detect_Timer->start(33);
        HumanG_Timer->start(HumanG_RemainingTime);
    });
    connect(newWidget,&PauseWidget::exit,this,[this](){emit back();});
    connect(newWidget,&PauseWidget::restart,this,[this](){emit restart();});
}

void PlayScene::Detecter(){
    Detect_Timer=new QTimer(this);
    Detect_Timer->start(33);
    connect(Detect_Timer,&QTimer::timeout,[=](){
        UPdateStarCountdown++;
        if(UPdateStarCountdown>=150){
            Star+=30;
            UPdateStarCountdown-=150;
        }
        UpdateStar();//检测星星数量变化

        for(Card *card:CardList){
            if(Star<card->Star_n || Coin<card->Coin_n){
                card->setDisabledState(true);
                card->Selectable=false;
            }else{
                card->setDisabledState(false);
                card->Selectable=true;
            }
        }//检测卡牌是否可用

        for(Plant *plant:PlantList){
            if(dynamic_cast<Stars*>(plant)){
                continue;
            }
            int flag=0;//本行是否有敌人的标志
            for(Human *human:HumanList){
                if(plant->PosR==human->PosR){
                    if(!plant->A_Timer->isActive())plant->A_Timer->start(2500);
                    flag=1;
                }
            }
            if(!flag){
                plant->A_Timer->stop();
            }
        }//植物攻击检测

        for(Human *human:HumanList){
            if(human->Moveable){
                human->setPos(human->x()-human->Speed,human->y());
            }
            if(human->x()<=90){
                Lose=true;
                for(Plant *plant:PlantList){
                    plant->Stop();
                }
                for(Human *human:HumanList){
                    human->Stop();
                }
                for(QVector<Brick*> row:BrickList){
                    for(Brick* brick:row){
                        if(brick){
                            brick->IsPause=true;
                        }
                    }
                }
                Detect_Timer->stop();
                HumanG_Timer->stop();
                LoseLabel = new QLabel(this->viewport());
                LoseLabel->setStyleSheet("QLabel {"
                                         "  background: transparent;"    // 完全透明背景
                                         "  border: none;"               // 无边框
                                         "  color: red;"               // 文字颜色
                                         "  font: bold 35px Arial;"      // 推荐使用清晰字体
                                         "}");
                LoseLabel->setAlignment(Qt::AlignCenter);
                LoseLabel->setText(QString("<b>失败！<br>人类最终战胜了抽象！"));
                LoseLabel->adjustSize();
                LoseLabel->move((width()-LoseLabel->width())*0.5,height()*0.3);
                LoseLabel->show();
                QAudioOutput *LoseAudio = new QAudioOutput(this);
                QMediaPlayer *LosePlayer = new QMediaPlayer(this);
                LosePlayer->setAudioOutput(LoseAudio);
                LosePlayer->setSource(QUrl("qrc:/Music/resource/Music/losemusic.ogg"));
                LosePlayer->play();
            }//失败检测
        }//Human移动

        for (int i = BulletList.size()-1; i >= 0; --i) {
            if(dynamic_cast<Starb*>(BulletList[i]))continue;
            BulletList[i]->setPos(BulletList[i]->x()+3.5,BulletList[i]->y());
            if(BulletList[i]->x()>=width()){
                BulletList[i]->destoryed();
            }
        }//子弹移动

        for(int i = BulletList.size()-1; i >= 0; --i){
            if(dynamic_cast<Starb*>(BulletList[i]))continue;
            Bullet *bullet = BulletList[i];
            QList<QGraphicsItem*> Colliding=bullet->collidingItems(Qt::IntersectsItemBoundingRect);
            for(QGraphicsItem* item:Colliding){
                if(Human* human = dynamic_cast<Human*>(item)){
                    if(Heart* heart = dynamic_cast<Heart*>(bullet)){
                        if(!heart->A_Human.contains(human)){
                            human->GetHurt(heart->damage);
                            heart->A_Human.append(human);
                        }//穿透子弹
                    }else if(dynamic_cast<Blood*>(bullet)){
                        if(!human->IsBlood){
                            human->IsBlood=true;
                            human->Blood();
                        }
                        human->GetHurt(bullet->damage);
                        bullet->destoryed();
                        break;//流血子弹
                    }else if(dynamic_cast<Snow*>(bullet)){
                        if(!human->IsSlow){
                            human->IsSlow=true;
                            human->Slow();
                        }
                        human->GetHurt(bullet->damage);
                        bullet->destoryed();
                        break;//减速子弹
                    }else if(dynamic_cast<Rocketb*>(bullet)){
                        human->GetHurt(bullet->damage);
                        break;
                    }else{
                        human->GetHurt(bullet->damage);
                        bullet->destoryed();
                        break;//普通子弹
                    }
                }
            }
        }//子弹碰撞判定

        for(Human *human:HumanList){
            QList<QGraphicsItem*> Colliding=human->collidingItems(Qt::IntersectsItemBoundingRect);
            int flag=1;
            for(QGraphicsItem* item:Colliding){
                if(Plant* plant = dynamic_cast<Plant*>(item)){
                    if(plant->PosR!=human->PosR)return;
                    human->Moveable=false;
                    flag=0;
                    human->AttackCountdown+=1;
                    if(human->AttackCountdown>=30){
                        human->Attack(plant);
                        human->AttackCountdown=0;
                    }
                    break;
                }
            }
            if(flag){
                human->Moveable=true;
            }
        }//Human攻击检测

        if(Round>=MaxRound && HumanList.size()==0){
            Win=true;
            for(Plant *plant:PlantList){
                plant->Stop();
            }
            for(Human *human:HumanList){
                human->Stop();
            }
            for(QVector<Brick*> row:BrickList){
                for(Brick* brick:row){
                    if(brick){
                        brick->IsPause=true;
                    }
                }
            }
            Detect_Timer->stop();
            HumanG_Timer->stop();
            WinLabel = new QLabel(this->viewport());
            WinLabel->setStyleSheet("QLabel {"
                                     "  background: transparent;"    // 完全透明背景
                                     "  border: none;"               // 无边框
                                     "  color: red;"               // 文字颜色
                                     "  font: bold 35px Arial;"      // 推荐使用清晰字体
                                     "}");
            WinLabel->setAlignment(Qt::AlignCenter);
            WinLabel->setText(QString("<b>胜利！<br>抽象最终统治了人类！"));
            WinLabel->adjustSize();
            WinLabel->move((width()-WinLabel->width())*0.5,height()*0.3);
            WinLabel->show();
            QAudioOutput *WinAudio = new QAudioOutput(this);
            QMediaPlayer *WinPlayer = new QMediaPlayer(this);
            WinPlayer->setAudioOutput(WinAudio);
            WinPlayer->setSource(QUrl("qrc:/Music/resource/Music/winmusic.ogg"));
            WinPlayer->play();
        }
    });
}

void PlayScene::SetAudio(){
    MusicOutput = new QAudioOutput(this);
    QMediaPlayer *MusicPlayer = new QMediaPlayer(this);
    MusicPlayer->setAudioOutput(MusicOutput);
    MusicPlayer->setSource(QUrl("qrc:/Music/resource/Music/KitanaiSekai.ogg"));
    MusicPlayer->play();

    connect(MusicPlayer,&QMediaPlayer::playbackStateChanged,[MusicPlayer](QMediaPlayer::PlaybackState state){
        if (state == QMediaPlayer::StoppedState) {
            // 当播放结束时，重置进度并重新播放
            MusicPlayer->setPosition(0);
            MusicPlayer->play();
        }

    });
    QAudioOutput *StartAudio = new QAudioOutput(this);
    QMediaPlayer *StartPlayer = new QMediaPlayer(this);
    StartPlayer->setAudioOutput(StartAudio);
    StartPlayer->setSource(QUrl("qrc:/Music/resource/Music/awooga.ogg"));
    StartPlayer->play();

    HumanGenerateAudio = new QAudioOutput(this);
    HumanGeneratePlayer = new QMediaPlayer(this);
    HumanGeneratePlayer->setAudioOutput(HumanGenerateAudio);
    HumanGeneratePlayer->setSource(QUrl("qrc:/Music/resource/Music/groan.ogg"));

    PauseAudio = new QAudioOutput(this);
    PausePlayer = new QMediaPlayer(this);
    PausePlayer->setAudioOutput(PauseAudio);
    PausePlayer->setSource(QUrl("qrc:/Music/resource/Music/pause.ogg"));

    MoneyAudio = new QAudioOutput(this);
    MoneyPlayer = new QMediaPlayer(this);
    MoneyPlayer->setAudioOutput(MoneyAudio);
    MoneyPlayer->setSource(QUrl("qrc:/Music/resource/Music/moneyfalls.ogg"));

    PlantAudio = new QAudioOutput(this);
    PlantPlayer = new QMediaPlayer(this);
    PlantPlayer->setAudioOutput(PlantAudio);
    PlantPlayer->setSource(QUrl("qrc:/Music/resource/Music/plant.ogg"));

    ShovelAudio = new QAudioOutput(this);
    ShovelPlayer = new QMediaPlayer(this);
    ShovelPlayer->setAudioOutput(ShovelAudio);
    ShovelPlayer->setSource(QUrl("qrc:/Music/resource/Music/shovel.ogg"));

    PropAudio = new QAudioOutput(this);
    PropPlayer = new QMediaPlayer(this);
    PropPlayer->setAudioOutput(PropAudio);
    PropPlayer->setSource(QUrl("qrc:/Music/resource/Music/tap.ogg"));

}
