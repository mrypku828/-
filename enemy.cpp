#include "enemy.h"
#include "paotai.h"
#include <QDebug>

Enemy::Enemy()
{
    movie = head = nullptr;     // 动画先为空
    hp = atk = 0;               // 血量、攻击为0
    state = WALK;               // 初始状态：行走
    slow = NO_SLOW;             // 不减速
    speed = 0.0;                // 速度
    time = 0;                   // 解冻时间
    counter = 0;                // 减速计时器
    super = false;              // 不是超级加强的敌人
}

Enemy::~Enemy()
{
    delete head;
    delete movie;
}

QRectF Enemy::boundingRect() const
{
    return QRectF(-30, -100, 100, 140);//碰撞范围-----------------》后续可能需要修改
}

void Enemy::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QImage image = movie->currentImage();

    if (slow) {//如果被减速，把图片变蓝-----------------------》后续可能需要修改
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                QColor color = image.pixelColor(x, y);
                color.setRed(0);
                color.setGreen(0);
                color.setBlue(250);
                image.setPixelColor(x, y, color);
            }
        }
    }
    if(head)
    {
        QImage head_image = head->currentImage();
        if(slow)
        {
            for (int y = 0; y < head_image.height(); ++y) {
                for (int x = 0; x < head_image.width(); ++x) {
                    QColor color = head_image.pixelColor(x, y);
                    color.setRed(0);
                    color.setGreen(0);
                    color.setBlue(250);
                    head_image.setPixelColor(x, y, color);
                }
            }
        }
        painter->drawImage(QRectF(-70,-100,140,140), head_image);
    }
    painter->drawImage(QRectF(-70,-100,140,140), image);
}
//攻击炮台：距离小于30像素，对面是炮台类对象，同一行
bool Enemy::collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)
    if(qAbs(other->x() - x()) <= 30 && other->y() == y() && other->type() == Paotai::Type)
    {
        return true;
    }
    return false;
}

int Enemy::type() const
{
    return Type;
}

void Enemy::SetMovie(const QString &path)//设置身体动画
// 删除旧动画，加载新GIF，如果是超级僵尸 → 加速播放，开始播放动画
{
    if(movie)
        delete movie;
    movie = new QMovie(path);
    if (!movie->isValid()) {
        qDebug() << "Movie loading failed: Invalid file" << path;
    } else {
        if(super)//如果是超级敌人，加速播放--------------------》待删除部分
        {
            movie->setSpeed(300);
            speed = 3.0;
        }
        movie->start();
    }
}

void Enemy::Sethead(const QString &path)//设置头部动画（如果需要按照头部穿戴物品辨识的话
//删除旧头部，加载新头部GIF，开始播放
{
    if(head)
        delete head;
    head = new QMovie(path);
    if (!head->isValid()) {
        qDebug() << "Movie loading failed: Invalid file" << path;
    } else {
        if(super)
            head->setSpeed(300);
        head->start();
    }
}