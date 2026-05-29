#include "bomb.h"
#include "button.h"
#include <QTime>
#include <QDebug>
#include"enemy.h"

// 构造函数：初始化炸弹属性
Bomb::Bomb()
{
    hp = 50;                // 生命值
    atk = 400;              // 爆炸攻击力
    state = 1;              // 状态1：准备爆炸
    setMovie(":/resources/cb.gif");  // 炸弹预热动画
    movie->setSpeed(50);      // 动画播放速度
    counter = 0;            // 计时器
    time = 50;              // 爆炸前摇时间
}

// 每帧更新：动画、爆炸、伤害判定
void Bomb::advance(int phase)
{
    if(!phase) return;
    update();

    // 铲子/强化模式激活时，鼠标变为手型
    if(Button::shovel_activate || Button::power_activate)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    // 血量为空，直接删除
    if(hp <= 0){
        delete this;
    }
    // 预热动画播放完毕 → 切换爆炸动画
    else if(state == 1 && movie->currentFrameNumber() == movie->frameCount()-1)
    {
        state = 2;
        setMovie(":/resources/boom.gif");

        // 对范围内所有僵尸造成秒杀伤害
        QList<QGraphicsItem*> items = collidingItems();
        if(!items.isEmpty())
        {
            int count = 0;
            foreach(QGraphicsItem* item,items)
            {
                Enemy* enemy = qgraphicsitem_cast<Enemy*>(item);
                enemy->state = BURN;
                enemy->hp = 0;
                count++;
                qDebug()<<"enemy"<<count<<" state:"<<enemy->state;
            }
        }
    }
    // 爆炸动画播放完毕 → 销毁炸弹
    else if(state == 2 && movie->currentFrameNumber() == movie->frameCount()-1)
    {
        qDebug()<<"bomb_deleted";
        delete this;
    }
}

// 爆炸范围碰撞判定
bool Bomb::collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)
    return other->type() == Enemy::Type
           && qAbs(other->y() - y()) <= GAP_Y
           && ((other->x() - x() >= 0 && other->x() - x() <= GAP_X)
               || (other->x() - x() >= -1.5*GAP_X && other->x() - x() <= 0));
}

// 鼠标点击事件：铲子移除 / 强化复制
void Bomb::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        // ---------------- 铲子模式：移除植物 ----------------
        if(Button::shovel_activate)
        {
            QGraphicsItem* item = scene()->itemAt(QPoint(798,40), transform());
            Button* button = qgraphicsitem_cast<Button*>(item);

            // 空指针判断（防止崩溃）
            if(button)
            {
                button->shovel_activate = false;
                button->counter = 0;
                button->state = 0;
            }
            delete this;
        }
        // ---------------- 强化模式：复制炸弹 ----------------
        else if(Button::power_activate)
        {
            QGraphicsItem* item = scene()->itemAt(QPoint(918,60), transform());
            Button* button = qgraphicsitem_cast<Button*>(item);

            if(button)
            {
                button->counter = 0;
                button->state = 0;
                button->power_activate = false;
            }

            // 随机位置生成新炸弹
            srand(uint(QTime(0,0,0).secsTo(QTime::currentTime())));
            int dx = rand()%3-1;
            int dy = rand()%3-1;
            Paotai* paotai = new Bomb;
            paotai->setPos(x() + dx * GAP_X, y() + dy * GAP_Y);
            scene()->addItem(paotai);
        }
    }
}

// 绘制范围：爆炸时扩大碰撞区域
QRectF Bomb::boundingRect() const
{
    return state == 2 ? QRectF(-100, -100, 200, 200) : Paotai::boundingRect();
}