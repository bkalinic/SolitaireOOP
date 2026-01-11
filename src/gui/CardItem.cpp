#include "CardItem.h"
#include "../header/types.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>
#include <QMap>
#include <QDebug>

namespace {
    QMap<QString, QPixmap>& cardImages() {
        static QMap<QString, QPixmap> instance;
        return instance;
    }
    QPixmap& cardBackImageInternal() {
        static QPixmap instance;
        return instance;
    }
    bool& imagesLoadedFlag() {
        static bool loaded = false;
        return loaded;
    }
}

CardItem::CardItem(std::shared_ptr<Card> card, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
    , m_card(card)
    , m_isDragging(false)
    , m_isSelectable(true) {

    setPixmap(QPixmap(getCardSize()));
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, false);

    ensureImagesLoaded();
    updateAppearance();
}

void CardItem::ensureImagesLoaded() {
    if (!areImagesLoaded()) {
        loadCardImages();
    }
}

bool CardItem::areImagesLoaded() {
    return imagesLoadedFlag();
}

QPixmap& CardItem::cardBackImage() {
    ensureImagesLoaded();
    return cardBackImageInternal();
}

QPixmap& CardItem::cardImage(const QString& imageName) {
    ensureImagesLoaded();
    return cardImages()[imageName];
}

void CardItem::loadCardImages() {
    if (areImagesLoaded()) {
        return;
    }
    qDebug() << "Ucitavam slike karata...";

    cardBackImageInternal().load(":/back.png");
    if (cardBackImageInternal().isNull()) {
        qWarning() << "Nije moguce ucitati sliku za ledja karata!";
        cardBackImageInternal() = QPixmap(getCardSize());
        cardBackImageInternal().fill(QColor(30, 60, 150));

        QPainter painter(&cardBackImageInternal());
        painter.setBrush(Qt::white);
        painter.drawEllipse(10, 10, getCardSize().width() - 20, getCardSize().height() - 20);
    }
    else {
        qDebug() << "Back image ucitana, original size:" << cardBackImageInternal().size();
        cardBackImageInternal() = cardBackImageInternal().scaled(getCardSize(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
        qDebug() << "Back image skalirana na:" << cardBackImageInternal().size();
    }

    QMap<int, QString> valueToEnglish = {
        {1, "ace"}, {2, "2"}, {3, "3"}, {4, "4"}, {5, "5"},
        {6, "6"}, {7, "7"}, {8, "8"}, {9, "9"}, {10, "10"},
        {11, "jack"}, {12, "queen"}, {13, "king"}
    };

    QString hrvatskiZnakovi[] = { "herc", "karo", "tref", "pik" };
    QString engleskiZnakovi[] = { "hearts", "diamonds", "clubs", "spades" };

    int loadedCount = 0;

    for (int i = 0; i < 4; ++i) {
        QString hrvatskiZnak = hrvatskiZnakovi[i];
        QString engleskiZnak = engleskiZnakovi[i];

        for (int value = 1; value <= 13; ++value) {
            QString engleskaVrijednost = valueToEnglish[value];
            QString imageName = QString("%1_%2").arg(hrvatskiZnak).arg(value);

            QString resourcePath = QString(":/%1_of_%2.png").arg(engleskaVrijednost).arg(engleskiZnak);

            QPixmap pixmap(resourcePath);
            if (pixmap.isNull()) {
                qWarning() << "Nije moguce ucitati:" << resourcePath;
                continue;
            }

            QPixmap scaledPixmap = pixmap.scaled(getCardSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            cardImages()[imageName] = scaledPixmap;
            loadedCount++;

            qDebug() << "Ucitano:" << resourcePath
                << "kao:" << imageName
                << "velicina:" << scaledPixmap.size();
        }
        imagesLoadedFlag() = true;
        qDebug() << "Ucitano" << loadedCount << "slika karata od 52.";

        qDebug() << "Primjeri image name-ova:";
        qDebug() << "  herc_1 ->" << (cardImages().contains("herc_1") ? "DA" : "NE");
        qDebug() << "  karo_13 ->" << (cardImages().contains("karo_13") ? "DA" : "NE");
        qDebug() << "  tref_7 ->" << (cardImages().contains("tref_7") ? "DA" : "NE");
        qDebug() << "  pik_10 ->" << (cardImages().contains("pik_10") ? "DA" : "NE");
    }

    imagesLoadedFlag() = true;
    qDebug() << "Ucitano" << cardImages().size() << "slika karata.";
}

QString CardItem::getImageNameForCard() const {
    if (!m_card) return "";

    if (m_card->isFaceUp()) {
        QString suitStr;
        switch (m_card->suit()) {
        case Solitaire::Suit::Herc: suitStr = "herc"; break;
        case Solitaire::Suit::Karo: suitStr = "karo"; break;
        case Solitaire::Suit::Tref: suitStr = "tref"; break;
        case Solitaire::Suit::Pik:  suitStr = "pik"; break;
        }
        return QString("%1_%2").arg(suitStr).arg(m_card->value());
    }
    else {
        return "back";
    }
}

void CardItem::updateAppearance() {
    if (!m_card) return;

    QString imageName = getImageNameForCard();
    qDebug() << "updateAppearance za" << m_card->toString()
        << "imageName:" << imageName
        << "faceUp:" << m_card->isFaceUp();

    if (imageName == "back") {
        qDebug() << "  Postavljam back image";
        setPixmap(cardBackImage());
    }
    else if (cardImages().contains(imageName)) {
        qDebug() << "  Postavljam front image:" << imageName;
        setPixmap(cardImages()[imageName]);
    }
    else {
        qWarning() << "Slika za" << imageName << "nije pronadjena, pokusavam direktno ucitati...";

        QStringList parts = imageName.split("_");
        if (parts.size() == 2) {
            QString hrvatskiZnak = parts[0];
            int value = parts[1].toInt();

            QString engleskiZnak;
            if (hrvatskiZnak == "herc") engleskiZnak = "hearts";
            else if (hrvatskiZnak == "karo") engleskiZnak = "diamonds";
            else if (hrvatskiZnak == "tref") engleskiZnak = "clubs";
            else if (hrvatskiZnak == "pik") engleskiZnak = "spades";
            else engleskiZnak = hrvatskiZnak;

            QString engleskaVrijednost;
            if (value == 1) engleskaVrijednost = "ace";
            else if (value == 11) engleskaVrijednost = "jack";
            else if (value == 12) engleskaVrijednost = "queen";
            else if (value == 13) engleskaVrijednost = "king";
            else engleskaVrijednost = QString::number(value);

            QString resourcePath = QString(":/cards/%1_of_%2").arg(engleskaVrijednost).arg(engleskiZnak);
            QPixmap pixmap(resourcePath);

            if (!pixmap.isNull()) {
                QPixmap scaledPixmap = pixmap.scaled(getCardSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                setPixmap(scaledPixmap);
                cardImages()[imageName] = scaledPixmap;
                qDebug() << "Direktno ucitano i spremljeno:" << resourcePath;
            }
            else {
                qWarning() << "Ne mogu ucitati ni direktno:" << resourcePath;
                QPixmap fallback(getCardSize());
                fallback.fill(Qt::white);
                setPixmap(fallback);
            }
        }
    }

    if (m_card->isFaceUp()) {
        QString suitName;
        switch (m_card->suit()) {
        case Solitaire::Suit::Herc: suitName = "Herc"; break;
        case Solitaire::Suit::Karo: suitName = "Karo"; break;
        case Solitaire::Suit::Tref: suitName = "Tref"; break;
        case Solitaire::Suit::Pik:  suitName = "Pik"; break;
        }

        QString valueStr;
        int value = m_card->value();
        if (value == 1) valueStr = "A";
        else if (value == 11) valueStr = "J";
        else if (value == 12) valueStr = "Q";
        else if (value == 13) valueStr = "K";
        else valueStr = QString::number(value);

        setToolTip(QString("%1 %2").arg(valueStr).arg(suitName));
    }
    else {
        setToolTip("Karta (lice dolje)");
    }
}

void CardItem::setSelectable(bool selectable) {
    m_isSelectable = selectable;
    setFlag(QGraphicsItem::ItemIsSelectable, selectable);
}

void CardItem::setHighlighted(bool highlighted) {
    if (highlighted) {
        setOpacity(0.8);
        setCursor(Qt::PointingHandCursor);
    }
    else {
        setOpacity(1.0);
        setCursor(Qt::ArrowCursor);
    }
}

void CardItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_isSelectable) {
        qDebug() << "CardItem kliknut:" << (m_card ? m_card->toString() : "null");
        qDebug() << "Face up:" << (m_card ? m_card->isFaceUp() : false);

        m_dragStartPosition = pos();
        m_isDragging = true;
        setZValue(1000);

        emit cardClicked(this);
    }

    QGraphicsPixmapItem::mousePressEvent(event);
}

void CardItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_isDragging && (event->buttons() & Qt::LeftButton)) {
        QPointF offset = event->scenePos() - event->buttonDownScenePos(Qt::LeftButton);
        setPos(m_dragStartPosition + offset);
        emit cardDragged(this, offset);
    }
}

void CardItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (m_isDragging && event->button() == Qt::LeftButton) {
        m_isDragging = false;
        emit cardDropped(this);
    }
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void CardItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
    if (m_isSelectable && m_card->isFaceUp()) {
        setHighlighted(true);
    }
    QGraphicsPixmapItem::hoverEnterEvent(event);
}

void CardItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
    setHighlighted(false);
    QGraphicsPixmapItem::hoverLeaveEvent(event);
}