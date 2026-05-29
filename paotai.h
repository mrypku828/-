#ifndef PAOTAI_H
#define PAOTAI_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QMovie>
#include <QDebug>
#include <QSoundEffect>

// 宏定义：是否冰冻（false=不冰冻，true=冰冻）----------------》待修改部分
#define NO_SNOW false
#define SNOW true

// 游戏场景宽度宏定义
#define WIDTH 1400
// 游戏场景高度宏定义
#define HEIGHT 602

// Paotai类：炮台类，继承自QGraphicsItem，是所有炮台的父类
class Paotai : public QGraphicsItem
{
public:
    qreal hp;                  // 炮台生命值（被僵尸攻击会扣血）
    int state;                 // 炮台状态：1=正常工作，0=异常/停止攻击
    Paotai();                   // 构造函数：初始化炮台属性
    ~Paotai() override;         // 析构函数：释放动画资源

    // 重写：返回炮台的矩形边界（用于绘制范围+碰撞检测）
    QRectF boundingRect() const override;

    // 自定义图形项类型ID，用于区分炮台/僵尸/子弹
    enum { Type = UserType + 1 };

    // 重写：绘制函数，负责显示炮台图片/动画
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    // 重写：碰撞检测函数，判断是否与其他物体碰撞
    bool collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const override;

    // 自定义函数：设置炮台GIF动画路径
    void setMovie(QString path);

    // 重写：返回自定义类型ID，供场景识别
    int type() const override;

protected:
    QMovie* movie;             // 炮台动画指针（存储GIF）
    int atk;                   // 攻击力
    int counter;               // 攻击计时器（记录当前冷却时间）
    int time;                  // 攻击间隔（多少帧发射一次子弹）
};
#endif // PAOTAI_H
