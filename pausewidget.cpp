#include "pausewidget.h"
#include <QString>
#include <QPainter>
#include "pausebutton.h"
PauseWidget::PauseWidget()
{
    // 弹窗背景
    setRect(0, 0, 200, 250);
    setBrush(QBrush(QColor(240,240,240))); // 浅灰色背景
    setPen(QPen(Qt::black, 2));              // 黑色边框
    initButtons();
    layoutButtons();
}

void PauseWidget::initButtons() {
    m_CloseButton=new PauseButton("继续游戏",this);
    m_RestartButton=new PauseButton("重新开始",this);
    m_ExitButton=new PauseButton("返回主界面",this);
    connect(m_CloseButton, &PauseButton::clicked, this, &PauseWidget::onCloseButtonClicked);
    connect(m_ExitButton,&PauseButton::clicked,this,&PauseWidget::onExitButtonClicked);
    connect(m_RestartButton,&PauseButton::clicked,this,&PauseWidget::onRestartButtonClicked);
}

void PauseWidget::layoutButtons() {
    const int buttonHeight = 40;
    const int verticalSpacing = 20;
    const int startY = 30;
    // 按钮1位置
    m_CloseButton->setPos(50, startY);
    m_CloseButton->setRect(0, 0, 100, buttonHeight);
    // 按钮2位置
    m_RestartButton->setPos(50, startY + buttonHeight + verticalSpacing);
    m_RestartButton->setRect(0, 0, 100, buttonHeight);
    // 按钮3位置
    m_ExitButton->setPos(50, startY + 2*(buttonHeight + verticalSpacing));
    m_ExitButton->setRect(0, 0, 100, buttonHeight);
}

void PauseWidget::onCloseButtonClicked() {
    emit close(); // 关闭窗口信号
}

void PauseWidget::onExitButtonClicked(){
    emit exit();//退出游戏信号
}

void PauseWidget::onRestartButtonClicked(){
    emit restart();//重开游戏信号
}
