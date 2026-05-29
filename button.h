#ifndef BUTTON_H
#define BUTTON_H


#include "other.h"
#include <QCursor>

// 按钮类型宏定义
#define FREEZE 1    // 冰冻按钮：冻结所有敌人-----------》待优化部分
#define POWER  2    // 激励按钮：给炮台加攻击buff-------》同上
#define SHOVEL 3    // 铲子按钮：铲除炮台

// 按钮类：继承自 other，实现游戏功能按钮
class Button : public other
{
public:
    Button(int t);                // 构造函数，参数 t=按钮类型

    void advance(int phase) override;        // 帧更新（冷却计时）
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override; // 绘制按钮
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override; // 鼠标点击事件
    QRectF boundingRect() const override;    // 按钮矩形范围

    static bool shovel_activate;  // 铲子是否激活（全局静态）
    static bool power_activate;   // 激励buff是否激活（全局静态）

    int counter;      // 冷却计时器
    int state;        // 按钮状态：0=冷却中 1=可点击

protected:
    int cool_time;     // 冷却总时长
    int type;          // 按钮类型（FREEZE/POWER/SHOVEL）
};


#endif // BUTTON_H
