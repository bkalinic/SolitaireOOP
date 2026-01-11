#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "../guiHeader/gamemanager.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onNewGame();
    void onUndo();
    void onRedo();
    void onDrawCard();
    void onCardClicked(CardItem* card);
    void onCardDropped(CardItem* card);
    void updateStatus();

private:
    void setupUI();
    void setupGameScene();
    void fitViewToScene();
    void positionPileItems();

    QList<QGraphicsTextItem*> m_debugTexts;
    QGraphicsScene* m_scene;
    QGraphicsView* m_view;
    QLabel* m_scoreLabel;
    QLabel* m_movesLabel;
    QLabel* m_timeLabel;
    QPushButton* m_undoButton;
    QPushButton* m_redoButton;

    GameManager& m_gameManager;
    CardItem* m_selectedCard;
};

#endif