#include "mainwindow.h"
#include "values.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(QRect(screen_width / 3, screen_height / 3, screen_width / 5, screen_height / 5));

    QWidget* mw = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* addrLayout = new QHBoxLayout();
    QHBoxLayout* widthLayout = new QHBoxLayout();
    QHBoxLayout* heightLayout = new QHBoxLayout();

    QLabel* lbAddr = new QLabel();
    QLabel* lbWidth = new QLabel();
    QLabel* lbHeight = new QLabel();
    lbAddr->setText("ip address:");
    lbHeight->setText("height:");
    lbWidth->setText("width:");

    edAddr = new QLineEdit();
    edWidth = new QLineEdit();
    edHeight = new QLineEdit();

    addrLayout->addWidget(lbAddr);
    addrLayout->addWidget(edAddr);
    addrLayout->addStretch();

    widthLayout->addWidget(lbWidth);
    widthLayout->addWidget(edWidth);
    widthLayout->addStretch();

    heightLayout->addWidget(lbHeight);
    heightLayout->addWidget(edHeight);
    heightLayout->addStretch();

    mainLayout->addLayout(addrLayout);
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(heightLayout);

    QHBoxLayout* btLayout = new QHBoxLayout;
    QPushButton* bt = new QPushButton();
    bt->setText("connect");
    btLayout->addWidget(bt);
    btLayout->addStretch();
    mainLayout->addLayout(btLayout);
    mainLayout->addStretch();
    mw->setLayout(mainLayout);
    this->setCentralWidget(mw);

    connect(bt, SIGNAL(clicked()), this, SLOT(tryconnect()));

    /*controlPanel = new ControlPanel(QRect(0, 0, screen_width / 3 * 2, screen_height / 3 * 2), this);
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), controlPanel, SLOT(keyPressSlot(QKeyEvent*)));
    connect(this, SIGNAL(keyReleased(QKeyEvent*)), controlPanel, SLOT(keyReleaseSlot(QKeyEvent*)));*/
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::tryconnect()
{
    addr = edAddr->text();
    rq_width = edWidth->text().toInt();
    rq_height = edHeight->text().toInt();

    //controlPanel = new ControlPanel(QRect(0, 0, screen_width / 3 * 2, screen_height / 3 * 2), this);
    controlPanel = new ControlPanel(QRect(0, 0, rq_width, rq_height), this);
    this->setCentralWidget(controlPanel);
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), controlPanel, SLOT(keyPressSlot(QKeyEvent*)));
    connect(this, SIGNAL(keyReleased(QKeyEvent*)), controlPanel, SLOT(keyReleaseSlot(QKeyEvent*)));
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    emit keyPressed(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    emit keyReleased(e);
}
