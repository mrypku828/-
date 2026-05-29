#include "paotai.h"
#include "enemy.h"

// 炮台构造函数：初始化所有属性
Paotai::Paotai()
{
    hp = state = 0;          // 血量、状态初始化为0
    movie = nullptr;         // 动画指针先为空
    atk = counter = time = 0; // 攻击力、计时器、攻击间隔初始化为0
}

// 炮台析构函数：销毁时释放动画资源
Paotai::~Paotai()
{
    delete movie;            // 删除GIF动画，防止内存泄漏
}

// 炮台的矩形范围（决定大小 + 碰撞区域）
QRectF Paotai::boundingRect() const
{
    return QRectF(-35, -35, 70, 70); // 以中心为原点，宽70高70
}

// 绘制函数：每帧都会调用，用来画炮台
void Paotai::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)         // 忽略不用的参数，避免警告
    Q_UNUSED(widget)         // 忽略不用的参数

    QImage image = movie->currentImage(); // 获取动画当前帧
    painter->drawImage(boundingRect(), image); // 把动画画到炮台区域里
}

// 碰撞判断：只有碰到【敌人】才返回true
bool Paotai::collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)

    // 条件：距离近 + 同一行 + 目标是敌人
    if(qAbs(other->x() - x()) <= 30 && other->y() == y() && other->type() == Enemy::Type)
    {
        return true;
    }
    return false;
}

// 返回自定义类型（Qt用来识别这是炮台）
int Paotai::type() const
{
    return Type;
}

// 设置炮台动画（加载GIF并播放）
void Paotai::setMovie(QString path)
{
    if(movie) delete movie;      // 如果已有旧动画，先删除
    movie = new QMovie(path);    // 创建新动画

    if(!movie->isValid())        // 如果加载失败
        qDebug() << "炮台动画加载错误";

    movie->setSpeed(50);         // 设置动画速度（慢一点更自然）
    movie->start();              // 开始播放动画
}