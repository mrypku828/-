
#ifndef ENEMY_H
#define ENEMY_H

// 包含Qt图形视图框架所需头文件
#include <QGraphicsScene>   // 图形场景（管理所有Item）
#include <QGraphicsItem>    // 所有可绘制图形项的基类
#include <QPainter>          // 绘图工具（画图片/形状）
#include <QMovie>             // 播放GIF动画
#include <QDebug>             // 调试输出（打印日志）
#include <QSoundEffect>       // 音效播放（需要Qt5::Multimedia）

// 状态宏定义（Enemy状态）
#define WALK  0   // 行走状态
#define ATTACK 1  // 攻击状态
#define DEAD  2   // 死亡状态
#define BURN  3   // 燃烧状态

// 减速宏
#define SLOW    1   // 被冰冻减速
#define NO_SLOW 0   // 正常速度

// 场景尺寸宏
#define WIDTH  1400 // 场景宽度
#define HEIGHT 602  // 场景高度

// Enemy类：继承QGraphicsItem，表示“敌人/僵尸”
class Enemy : public QGraphicsItem
{
public:
    // 成员变量
    qreal hp;          // 生命值（浮点，便于扣血）
    qreal atk;         // 攻击力
    int state;          // 当前状态：WALK/ATTACK/DEAD/BURN
    int slow;           // 是否减速：SLOW/NO_SLOW
    qreal speed;        // 移动速度
    int time;           // 解冻倒计时
    int counter;        // 冰冻/特效计数器

    // 自定义Item类型（用于qgraphicsitem_cast安全转换）
    enum { Type = UserType + 2 };

    // 构造/析构
    Enemy();
    ~Enemy();

    // -------- QGraphicsItem必须重写的两个纯虚函数 --------
    // 返回当前Item的边界矩形（决定绘制/碰撞范围）
    QRectF boundingRect() const override;

    // 绘制函数（在这里画Enemy的图片/GIF）
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

    // 碰撞检测函数（判断是否与其他Item碰撞）
    bool collidesWithItem(const QGraphicsItem* other,
                          Qt::ItemSelectionMode mode) const override;

    // 返回自定义类型（配合上面的enum Type）
    int type() const override;

    // 自定义方法：设置主体GIF动画
    void SetMovie(const QString &path);

    // 自定义方法：设置头部GIF/图片
    void Sethead(const QString &path);

    // 纯虚函数：冰冻解冻逻辑（子类必须实现）
    virtual void snowfree() = 0;

    // 成员指针：主体动画
    QMovie* movie;

    // 是否为“精英/特殊”敌人
    bool super;

protected:
    // 成员指针：头部动画（保护成员，子类可访问）
    QMovie* head;
};

#endif // ENEMY_H
