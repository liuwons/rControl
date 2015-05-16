#include "mainwindow.h"

MainWindow::MainWindow(int x, int y, int w, int h, QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(QRect(x, y, w, h));

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
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::tryconnect()
{

    panel = new ControlPanel(QRect(100, 100, 100, 100));
    panel->show();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    emit keyPressed(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    emit keyReleased(e);
}
