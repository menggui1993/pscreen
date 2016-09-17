#include "controller.h"
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QGroupBox>
#include <QLineEdit>
#include <QDate>
#include <QFile>
#include <QTextStream>

#include <iostream>

Controller::Controller(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("HSSE控制");
    this->setAttribute(Qt::WA_QuitOnClose, true);
    ps = NULL;

    QGroupBox *winBox = new QGroupBox("窗口控制");
    QHBoxLayout *winLayout = new QHBoxLayout;
    QPushButton *openBt = new QPushButton(this);
    openBt->setText("打开窗口");
    QPushButton *closeBt = new QPushButton(this);
    closeBt->setText("关闭窗口");
    connect(openBt, SIGNAL(clicked()), this, SLOT(openScreen()));
    connect(closeBt, SIGNAL(clicked()), this, SLOT(closeScreen()));
    QHBoxLayout *wLayout = new QHBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    QHBoxLayout *fLayout = new QHBoxLayout;
    QLabel *wLabel = new QLabel(this);
    wLabel->setText("宽度");
    wLEdit = new QLineEdit(this);
    QLabel *hLabel = new QLabel(this);
    hLabel->setText("高度");
    hLEdit = new QLineEdit(this);
    QLabel *fLabel = new QLabel(this);
    fLabel->setText("字号");
    flEdit = new QLineEdit(this);
    wLayout->addWidget(wLabel);
    wLayout->addWidget(wLEdit);
    hLayout->addWidget(hLabel);
    hLayout->addWidget(hLEdit);
    fLayout->addWidget(fLabel);
    fLayout->addWidget(flEdit);
    QVBoxLayout *ocLayout = new QVBoxLayout;
    ocLayout->addWidget(openBt);
    ocLayout->addWidget(closeBt);
    ocLayout->addLayout(wLayout);
    ocLayout->addLayout(hLayout);
    ocLayout->addLayout(fLayout);
    QPushButton *upBt = new QPushButton("上", this);
    QPushButton *downBt = new QPushButton("下", this);
    QPushButton *lftBt = new QPushButton("左", this);
    QPushButton *rgtBt = new QPushButton("右", this);
    connect(upBt, SIGNAL(clicked()), this, SLOT(moveUp()));
    connect(downBt, SIGNAL(clicked()), this, SLOT(moveDown()));
    connect(lftBt, SIGNAL(clicked()), this, SLOT(moveLeft()));
    connect(rgtBt, SIGNAL(clicked()), this, SLOT(moveRight()));
    QGridLayout *moveLayout = new QGridLayout;
    moveLayout->addWidget(upBt, 0, 1);
    moveLayout->addWidget(downBt, 1, 1);
    moveLayout->addWidget(lftBt, 1, 0);
    moveLayout->addWidget(rgtBt, 1, 2);
    winLayout->addLayout(ocLayout);
    winLayout->addLayout(moveLayout);
    winBox->setLayout(winLayout);

    QGroupBox *lightBox = new QGroupBox("指示灯控制");
    QLabel *lightLabel = new QLabel(this);
    lightLabel->setText("事故指示灯");
    lightCBox = new QComboBox(this);
    lightCBox->addItem("红灯");
    lightCBox->addItem("黄灯");
    lightCBox->addItem("绿灯");
    lightCBox->addItem("无");
    QHBoxLayout *lightLayout = new QHBoxLayout;
    lightLayout->addWidget(lightLabel);
    lightLayout->addWidget(lightCBox);
    lightBox->setLayout(lightLayout);

    QGroupBox *rdBox = new QGroupBox("可记录事故");
    QLabel *rdLabel = new QLabel(this);
    rdLabel->setText("上次可记录事故日期");
    rdDEdit = new QDateEdit(this);
    rdDEdit->setCalendarPopup(true);
    QHBoxLayout *rdLayout = new QHBoxLayout;
    rdLayout->addWidget(rdLabel);
    rdLayout->addWidget(rdDEdit);
    rdBox->setLayout(rdLayout);

    QGroupBox *ldBox = new QGroupBox("损失工时事故");
    QLabel *ldLabel1 = new QLabel(this);
    ldLabel1->setText("上次损失工时事故日期");
    ldDEdit = new QDateEdit(this);
    ldDEdit->setCalendarPopup(true);
    QHBoxLayout *ldLayout1 = new QHBoxLayout;
    ldLayout1->addWidget(ldLabel1);
    ldLayout1->addWidget(ldDEdit);
    QLabel *ldLabel2 = new QLabel(this);
    ldLabel2->setText("平均每天累加工时数");
    ldLEdit = new QLineEdit("16161", this);
    QHBoxLayout *ldLayout2 = new QHBoxLayout;
    ldLayout2->addWidget(ldLabel2);
    ldLayout2->addWidget(ldLEdit);
    QVBoxLayout *ldLayout = new QVBoxLayout;
    ldLayout->addLayout(ldLayout1);
    ldLayout->addLayout(ldLayout2);
    ldBox->setLayout(ldLayout);

    QHBoxLayout *tailLayout = new QHBoxLayout;
    QPushButton *applyBt = new QPushButton(this);
    applyBt->setText("应用");
    connect(applyBt, SIGNAL(clicked()), this, SLOT(apply()));
    QPushButton *recoverBt = new QPushButton(this);
    recoverBt->setText("复原");
    connect(recoverBt, SIGNAL(clicked()), this, SLOT(recover()));
    QPushButton *quitBt = new QPushButton(this);
    quitBt->setText("退出");
    connect(quitBt, SIGNAL(clicked(bool)), this, SLOT(close()));
    tailLayout->addWidget(applyBt);
    tailLayout->addWidget(recoverBt);
    tailLayout->addWidget(quitBt);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(winBox);
    mainLayout->addWidget(lightBox);
    mainLayout->addWidget(rdBox);
    mainLayout->addWidget(ldBox);
    mainLayout->addLayout(tailLayout);
    this->setLayout(mainLayout);

    recover();
    openScreen();
    showMinimized();
}

Controller::~Controller()
{
    closeScreen();
}

void Controller::openScreen()
{
    if (ps == NULL){
        ps = new Screen(0, width, height);
        int x, y;
        QFile file("pos.txt");
        if (file.open(QIODevice::ReadOnly)){
            QTextStream stream(&file);
            x = stream.readLine().toInt();
            y = stream.readLine().toInt();
            ps->move(x, y);
        }
        ps->show();
    }
    apply();
}

void Controller::closeScreen()
{
    if (ps != NULL){
        QFile file("pos.txt");
        if (file.open(QIODevice::WriteOnly)){
            QTextStream stream(&file);
            stream << ps->pos().x() << "\n";
            stream << ps->pos().y() << "\n";
            file.close();
        }
        ps->close();
        delete(ps);
        ps = NULL;
    }
}

void Controller::apply()
{
    int light = lightCBox->currentIndex();
    QDate rdate = rdDEdit->date();
    QString rstr = rdate.toString();
    QDate ldate = ldDEdit->date();
    QString lstr = ldate.toString();
    int lle = ldLEdit->text().toInt();
    width = wLEdit->text().toInt();
    height = hLEdit->text().toInt();
    fontsize = flEdit->text().toInt();

    QFile file("save.txt");
    if (file.open(QIODevice::WriteOnly)){
        QTextStream stream(&file);
        stream << light << "\n";
        stream << rstr << "\n";
        stream << lstr << "\n";
        stream << lle << "\n";
        stream << width << "\n";
        stream << height << "\n";
        stream << fontsize << "\n";
        file.close();
    }

    if (ps != NULL){
        QPoint pos = ps->pos();
        closeScreen();
        ps = new Screen(0, width, height, fontsize, light);
        ps->move(pos);
        ps->applyAll(light, lle, rdate, ldate);
        ps->show();
    }
}

void Controller::recover()
{
    int light , lle;
    QString rstr, lstr;

    QFile file("save.txt");
    if (file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        light = stream.readLine().toInt();
        rstr = stream.readLine();
        lstr = stream.readLine();
        lle = stream.readLine().toInt();
        width = stream.readLine().toInt();
        height = stream.readLine().toInt();
        fontsize = stream.readLine().toInt();
        file.close();

        lightCBox->setCurrentIndex(light);
        rdDEdit->setDate(QDate::fromString(rstr));
        ldDEdit->setDate(QDate::fromString(lstr));
        ldLEdit->setText(QString::number(lle));
        wLEdit->setText(QString::number(width));
        hLEdit->setText(QString::number(height));
        flEdit->setText(QString::number(fontsize));
    }
}

void Controller::moveUp()
{
    if (ps != NULL)
        ps->move(ps->x(), ps->y() - 1);
}

void Controller::moveDown()
{
    if (ps != NULL)
        ps->move(ps->x(), ps->y() + 1);
}

void Controller::moveLeft()
{
    if (ps != NULL)
        ps->move(ps->x() - 1, ps->y());
}

void Controller::moveRight()
{
    if (ps != NULL)
        ps->move(ps->x() + 1, ps->y());
}
