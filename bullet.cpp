#include "bullet.h"
#include "enemy.h"

Bullet::Bullet(int atk_num, int snowor)
{
    atk = atk_num;
    speed = 7.0;
    snow = snowor;
}

Bullet::~Bullet()
{

}

QRectF Bullet::boundingRect() const
{
    return QRectF(-12, -28, 24, 24);
}

void Bullet::advance(int phase)
{
    if (!phase) return;
    update();
    QList<QGraphicsItem*> items = collidingItems();
    if (!items.empty())
    {
        Enemy* enemy = qgraphicsitem_cast<Enemy*>(items[0]);
        if (enemy)
        {
            enemy->hp -= atk;
            if (snow == 1)
                enemy->counter = 0;
            else if (snow == 2)
                enemy->counter = -100;
            enemy->slow = snow;
            delete this;
            return;
        }
    }
    setX(x() + speed);
    if (x() > WIDTH || x() < 0)
    {
        delete this;
    }
}

bool Bullet::collidesWithItem(const QGraphicsItem* other, Qt::ItemSelectionMode mode) const
{
    Q_UNUSED(mode)
    if (qAbs(other->x() - x()) < 30 && other->type() == Enemy::Type && other->y() == y())
        return true;
    return false;
}

void Bullet::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if (!snow)
        painter->drawPixmap(QRect(-12, -28, 24, 24), QPixmap(":/resources/pea.png"));
    else
        painter->drawPixmap(boundingRect(), QPixmap(":/resources/peasnow.png"), QRectF());
}