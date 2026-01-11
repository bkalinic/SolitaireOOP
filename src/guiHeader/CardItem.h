#ifndef CARDITEM_H
#define CARDITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <memory>
#include "../header/card.h"

class CardItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    explicit CardItem(std::shared_ptr<Card> card, QGraphicsItem* parent = nullptr);

    std::shared_ptr<Card> getCard() const { return m_card; }
    void updateAppearance();
    void setSelectable(bool selectable);
    void setHighlighted(bool highlighted);

    static QSize getCardSize() { return QSize(67, 96); }

    static QPixmap& cardBackImage();
    static QPixmap& cardImage(const QString& imageName);
    static bool areImagesLoaded();
    static void ensureImagesLoaded();

    static void loadCardImages();

    bool isCardFaceUp() const { return m_card ? m_card->isFaceUp() : false; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

signals:
    void cardClicked(CardItem* card);
    void cardDragged(CardItem* card, const QPointF& offset);
    void cardDropped(CardItem* card);

private:
    QString getImageNameForCard() const;

    std::shared_ptr<Card> m_card;
    QPointF m_dragStartPosition;
    bool m_isDragging;
    bool m_isSelectable;
};

#endif