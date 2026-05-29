#include "shooter.h"
#include "button.h"
#include "bullet.h"
#include "enemy.h"
// 构造函数：射击炮台初始化
Shooter::Shooter()
{
    hp = 100;        // 血量
    state = 1;       // 状态：正常
    time = 100;      // 攻击间隔（冷却时间）
    counter = 0;     // 攻击计时器
    atk = 20;        // 攻击力
    //setMovie(":/resources/ps.gif");
    movie->setSpeed(50);
}

// 每帧更新：攻击逻辑 + 状态检测
void Shooter::advance(int phase)
{
    // 鼠标样式：保留 强化+铲子 光标
    if(Button::shovel_activate || Button::power_activate)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    if(!phase) return;
    update();

    // 血量为空，删除自己
    if(hp <= 0)
    {
        delete this;
    }
    // 计时器到达，尝试攻击
    else if(++counter % time == 0)
    {
        if(counter > 80) {
            counter = 0;
            time = 100;
        }
        // 获取碰撞列表（同一行的敌人）
        QList<QGraphicsItem*> items = collidingItems();
        if(!items.isEmpty()) // 当前行有敌人
        {
            // 创建子弹（原豌豆）
            Bullet* bullet = new Bullet(atk, NO_SNOW);
            bullet->setX(x() + 32);
            bullet->setY(y());
            scene()->addItem(bullet);
            return;
        }
    }
}

// 碰撞判断：只检测【同一行 + 右侧 + 敌人】
bool Shooter::collidesWithItem(const QGraphicsItem *other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)
    // 同一行 + 是敌人 + 在炮台右侧
    return other->y() == y() && other->type() == Enemy::Type && other->x() > x();
}

// 鼠标点击：保留【铲子删除】+【强化模式】
void Shooter::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        // 铲子：删除炮台
        if(Button::shovel_activate)
        {
            QGraphicsItem* item = scene()->itemAt(QPoint(798,40), transform());
            Button* button = qgraphicsitem_cast<Button*>(item);
            button->shovel_activate = false;
            button->counter = 0;
            button->state = 0;
            delete this;
        }
        // 强化模式：极速射击
        else if(Button::power_activate)
        {
            QGraphicsItem* item = scene()->itemAt(QPoint(918,60), transform());
            Button* button = qgraphicsitem_cast<Button*>(item);
            time = 5;            // 极短冷却
            counter = 0;        // 重置计时器
            button->counter = 0;
            button->state = 0;
            state = 0;
        }
    }
}