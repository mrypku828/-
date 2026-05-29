#include "shop.h"
#include <QPainter>

Shop::Shop()
{
    coin_val = 100;
    counter = 0;
    coin_generate_time = 700;
}

QRectF Shop::boundingRect() const
{
    return QRectF(0, 0, 530, 90);
}

void Shop::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *,
                 QWidget *)
{
    painter->drawPixmap(0, 0, 528, 95,
                        QPixmap(":/resources/shop.png"));

    QFont font;
    font.setPointSizeF(15);
    painter->setFont(font);
    painter->drawText(QRectF(-255, 23, 600, 110),
                      Qt::AlignCenter,
                      QString::number(coin_val));
}

void Shop::advance(int phase)
{
    if (!phase) return;
    update();

    if (++counter >= coin_generate_time)
    {
        counter = 0;
        coin_val += 25; // 定期自动加钱
    }
}