#include "coinmaker.h"
#include "shop.h"
#include "button.h"
#include <cstdlib>

CoinMaker::CoinMaker()
{
    hp = 100;
    state = 1;
    time = 500;
    counter = 0;
    atk = 0;
    setMovie(":/resources/sf.gif");
}

void CoinMaker::advance(int phase)
{
    if (!phase) return;
    update();

    if (Button::shovel_activate || Button::power_activate)
        setCursor(Qt::PointingHandCursor);
    else
        setCursor(Qt::ArrowCursor);

    if (hp <= 0)
    {
        delete this;
        return;
    }

    /* 正常产钱 */
    if (state && ++counter >= time)
    {
        counter = 0;

        QList<QGraphicsItem *> items =
            scene()->items(QPointF(270, 0));

        for (auto *item : items)
        {
            if (auto *shop = qgraphicsitem_cast<Shop *>(item))
            {
                shop->coin_val += 50;
                break;
            }
        }
    }
}

void CoinMaker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (Button::shovel_activate)
    {
        Button::shovel_activate = false;
        delete this;
    }
}