#ifndef PAUSEWIDGET_H
#define PAUSEWIDGET_H

#include <QObject>
#include <QGraphicsRectItem>
#include "pausebutton.h"
class PauseWidget : public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit PauseWidget();

signals:
    void close(); // 当第一个按钮点击时发射的信号
    void exit();//退出按钮
    void restart();//重开按钮
private slots:
    void onCloseButtonClicked();//继续游戏
    void onExitButtonClicked();//退出游戏
    void onRestartButtonClicked();//重新开始
private:
    void initButtons();
    void layoutButtons();
    PauseButton* m_CloseButton; // 继续按钮
    PauseButton* m_RestartButton;     // 重开按钮
    PauseButton* m_ExitButton;  //退出按钮
};

#endif // PAUSEWIDGET_H
