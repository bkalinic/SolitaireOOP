#include "../guiHeader/MainWindow.h"
#include "../guiHeader/CardItem.h"
#include "../guiHeader/PileItem.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_scene(new QGraphicsScene(this))
    , m_view(new QGraphicsView(m_scene, this))
    , m_gameManager(GameManager::instance())
    , m_selectedCard(nullptr)
    , m_scoreLabel(nullptr)
    , m_movesLabel(nullptr)
    , m_timeLabel(nullptr) {
    
    qDebug() << "=== MainWindow konstruktor ===";
    setupUI();

    resize(1000, 800);
    setWindowTitle("Solitaire OOP");
    setMinimumSize(900, 700);

    m_gameManager.setScene(m_scene);
    setupGameScene();

    onNewGame();

    connect(&m_gameManager, &GameManager::gameStateChanged,
        this, &MainWindow::updateStatus);
    connect(&m_gameManager, &GameManager::scoreUpdated,
        this, [this](int score) {
            if (m_scoreLabel) {
                m_scoreLabel->setText(QString("Score: %1").arg(score));
            }
        });

    qDebug() << "MainWindow kreiran";
    qDebug() << "Scene items:" << m_scene->items().size();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setRenderHint(QPainter::SmoothPixmapTransform);
    m_view->setSceneRect(0, 0, 900, 700);
    m_view->centerOn(450, 350);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(m_view);

    setCentralWidget(centralWidget);

    QToolBar* toolbar = addToolBar("Main");

    QAction* newGameAction = toolbar->addAction("Nova igra");
    QAction* undoAction = toolbar->addAction("Undo");
    QAction* redoAction = toolbar->addAction("Redo");

    connect(newGameAction, &QAction::triggered, this, &MainWindow::onNewGame);
    connect(undoAction, &QAction::triggered, this, &MainWindow::onUndo);
    connect(redoAction, &QAction::triggered, this, &MainWindow::onRedo);

    QStatusBar* statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    m_scoreLabel = new QLabel("Score: 0", statusBar);
    m_movesLabel = new QLabel("Moves: 0", statusBar);
    m_timeLabel = new QLabel("Time: 00:00", statusBar);

    statusBar->addPermanentWidget(m_scoreLabel);
    statusBar->addPermanentWidget(m_movesLabel);
    statusBar->addPermanentWidget(m_timeLabel);
}

void MainWindow::setupGameScene() {
    m_scene->clear();
    m_selectedCard = nullptr;

    m_scene->setSceneRect(0, 0, 900, 700);
    qDebug() << "Scene rect postavljen na:" << m_scene->sceneRect();

    m_scene->setBackgroundBrush(QBrush(QColor(0, 100, 0)));

    m_scene->addRect(m_scene->sceneRect(), QPen(Qt::yellow, 2, Qt::DashLine));

    qDebug() << "Scene setup zavrsen";
}

void MainWindow::positionPileItems() {
    const int TOP_MARGIN = 50;
    const int LEFT_MARGIN = 50;
    const int HORIZONTAL_SPACING = 20;
    const int VERTICAL_SPACING = 100;

    auto pileItems = m_gameManager.getPileItems();

    qDebug() << "=== positionPileItems ===";
    qDebug() << "Broj pile items:" << pileItems.size();

    if (pileItems.empty()) {
        qWarning() << "Nema pile items!";
        return;
    }

    const int CARD_WIDTH = CardItem::getCardSize().width();
    const int CARD_HEIGHT = CardItem::getCardSize().height();

    QRectF sceneRect = m_scene->sceneRect();
    qDebug() << "Scene rect:" << sceneRect;

    int topY = TOP_MARGIN;

    if (pileItems.size() > 0) {
        int stockX = LEFT_MARGIN;
        pileItems[0]->setPos(stockX, topY);
        qDebug() << "Stock pile na:" << pileItems[0]->pos();

        auto* stockText = new QGraphicsTextItem("Stock");
        stockText->setPos(pileItems[0]->pos() + QPointF(0, -20));
        stockText->setDefaultTextColor(Qt::white);
        m_scene->addItem(stockText);
    }

    if (pileItems.size() > 1) {
        int wasteX = LEFT_MARGIN + CARD_WIDTH + HORIZONTAL_SPACING;
        pileItems[1]->setPos(wasteX, topY);
        qDebug() << "Waste pile na:" << pileItems[1]->pos();

        auto* wasteText = new QGraphicsTextItem("Waste");
        wasteText->setPos(pileItems[1]->pos() + QPointF(0, -20));
        wasteText->setDefaultTextColor(Qt::white);
        m_scene->addItem(wasteText);
    }

    int foundationStartX = LEFT_MARGIN + 2 * (CARD_WIDTH + HORIZONTAL_SPACING) + 40;
    QString foundationLabels[] = { "Pik", "Herc", "Karo", "Tref" };
    for (int i = 2; i <= 5 && i < pileItems.size(); i++) {
        int foundationIndex = i - 2;
        int foundationX = foundationStartX + foundationIndex * (CARD_WIDTH + HORIZONTAL_SPACING);
        pileItems[i]->setPos(foundationX, topY);
        qDebug() << "Foundation" << foundationIndex << "na:" << pileItems[i]->pos();

        auto* foundationText = new QGraphicsTextItem(foundationLabels[i - 2]);
        foundationText->setPos(pileItems[i]->pos() + QPointF(25, -25));
        foundationText->setDefaultTextColor(Qt::white);
        foundationText->setFont(QFont("Arial", 24));
        m_scene->addItem(foundationText);
    }

    int tableY = topY + CARD_HEIGHT + VERTICAL_SPACING;
    for (int i = 6; i < pileItems.size() && i < 13; i++) {
        int tableIndex = i - 6;
        int tableX = LEFT_MARGIN + tableIndex * (CARD_WIDTH + HORIZONTAL_SPACING);
        pileItems[i]->setPos(tableX, tableY);
        qDebug() << "Table" << tableIndex << "na:" << pileItems[i]->pos();

        auto* tableText = new QGraphicsTextItem(QString("T%1").arg(tableIndex + 1));
        tableText->setPos(pileItems[i]->pos() + QPointF(0, -20));
        tableText->setDefaultTextColor(Qt::white);
        m_scene->addItem(tableText);
    }

    for (int i = 0; i < pileItems.size(); i++) {
        QPointF pos = pileItems[i]->pos();
        QRectF bounds = pileItems[i]->boundingRect().translated(pos);

        if (!sceneRect.contains(bounds)) {
            qWarning() << "Pile" << i << "je izvan scene!";
            qWarning() << "  Position:" << pos;
            qWarning() << "  Bounds:" << bounds;
            qWarning() << "  Scene:" << sceneRect;
        }
    }
    qDebug() << "=== Pozicioniranje zavrseno ===";
}

void MainWindow::fitViewToScene() {
    qDebug() << "fitViewToScene()";

    if (!m_scene || !m_view) return;

    QRectF sceneRect = m_scene->sceneRect();
    if (sceneRect.isEmpty()) {
        qWarning() << "Scene rect je prazan!";
        return;
    }

    qDebug() << "Fitting view to scene:" << sceneRect;
    qDebug() << "View size:" << m_view->size();

    m_view->resetTransform();

    qreal scaleX = m_view->viewport()->width() / sceneRect.width();
    qreal scaleY = m_view->viewport()->height() / sceneRect.height();
    qreal scale = qMin(scaleX, scaleY) * 0.95;

    qDebug() << "Scale factors - X:" << scaleX << "Y:" << scaleY << "Using:" << scale;

    if (scale < 0.1 || scale > 10.0) {
        scale = 1.0;
        qDebug() << "Scale out of bounds, using 1.0";
    }

    m_view->scale(scale, scale);

    m_view->centerOn(sceneRect.center());

    qDebug() << "View transformed, center:" << m_view->mapToScene(m_view->viewport()->rect().center());
}

void MainWindow::onNewGame() {
    setupGameScene();
    m_gameManager.initializeNewGame();

    positionPileItems();
    fitViewToScene();
    updateStatus();

    qDebug() << "Nova igra pokrenuta";
    qDebug() << "Scene items:" << m_scene->items().size();

    auto pileItems = m_gameManager.getPileItems();
    qDebug() << "Broj pile items:" << pileItems.size();

    for (int i = 0; i < pileItems.size(); i++) {
        qDebug() << "Pile" << i << "pos:" << pileItems[i]->pos();
    }
}

void MainWindow::onUndo() {
    m_gameManager.undo();
    updateStatus();
}

void MainWindow::onRedo() {
    m_gameManager.redo();
    updateStatus();
}

void MainWindow::onDrawCard() {
    m_gameManager.drawFromStock();
    updateStatus();
}

void MainWindow::onCardClicked(CardItem* card) {
    m_selectedCard = card;
    card->setHighlighted(true);
}

void MainWindow::onCardDropped(CardItem* card) {
    if (!m_selectedCard) return;

    QPointF scenePos = card->scenePos();
    auto itemsUnder = m_scene->items(scenePos);

    for (auto item : itemsUnder) {
        if (auto pileItem = dynamic_cast<PileItem*>(item)) {
            if (m_gameManager.attemptMove(m_selectedCard, pileItem)) {
                break;
            }
        }
    }
    if (m_selectedCard) {
        m_selectedCard->setHighlighted(false);
        m_selectedCard = nullptr;
    }
}

void MainWindow::updateStatus() {
    auto engine = m_gameManager.engine();
    if (engine) {
        m_scoreLabel->setText(QString("Score: %1").arg(engine->score()));
        m_movesLabel->setText(QString("Moves: %1").arg(engine->moveCount()));
    }
}