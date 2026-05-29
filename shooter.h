#ifndef SHOOTER_H
#define SHOOTER_H


#include "paotai.h"


// Shooter：射击炮台
// 功能：检测前方敌人 → 定时发射子弹
class Shooter : public Paotai
{
public:
    Shooter();    // 构造函数：初始化血量、攻击间隔、动画等

    // 重写：每帧更新 → 检测敌人 + 发射子弹
    void advance(int phase) override;

    // 重写：碰撞检测 → 判断敌人是否进入攻击范围
    bool collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const override;

    // 重写：鼠标事件 → 支持铲子铲除、强化模式
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif // SHOOTER_H