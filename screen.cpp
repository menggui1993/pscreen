#include "screen.h"
#include <QLayout>
#include <QImage>
#include <QLabel>
#include <QPalette>
#include <QFont>
#include <QColor>
#include <QTimer>

Screen::Screen(QWidget *parent, int w, int h, int fs, int l)
    : QWidget(parent)
{
    width = w;
    height = h;
    fontsize = fs;
    light = l;
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setWindowTitle("HSSE");
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setMaximumSize(width, height);
    this->setMinimumSize(width, height);

    QFont chFont;
    chFont.setFamily("微软黑体");
    chFont.setPointSize(fontsize);

    QFont enFont;
    enFont.setFamily("Arial Unicode MS");
    enFont.setPointSize(fontsize);

    QFont strFont;
    strFont.setFamily("微软黑体");
    strFont.setPointSize(fontsize);
    strFont.setBold(true);

    QPalette lgFontPal;
    lgFontPal.setColor(QPalette::WindowText, Qt::blue);

    QPalette smFontPal;
    smFontPal.setColor(QPalette::WindowText, Qt::gray);

    QPalette bgPal;
    bgPal.setColor(QPalette::Background, Qt::white);
    this->setPalette(bgPal);

    lightLabel = new QLabel(this);
    QImage *img = new QImage;
    img->load(":/image/red");
    *img = img->scaled(width*3/5, height*3/5, Qt::KeepAspectRatio);
    lightLabel->setPixmap(QPixmap::fromImage(*img));

    QVBoxLayout *redLayout = new QVBoxLayout;
    QLabel *redLabel = new QLabel("损失工时事故", this);
    redLabel->setFont(chFont);
    //redLabel->setStyleSheet(QString("QLabel{background:#FFCCCC}"));
    redLayout->addWidget(redLabel);
    QLabel *redeLabel = new QLabel("Serious Recordable HSSE Incident", this);
    redeLabel->setFont(enFont);
    if (light == 0){
        redLabel->setPalette(lgFontPal);
        redeLabel->setPalette(lgFontPal);
    }
    else{
        redLabel->setPalette(smFontPal);
        redeLabel->setPalette(smFontPal);
    }
    //redeLabel->setStyleSheet(QString("QLabel{background:#FFCCCC}"));
    redLayout->addWidget(redeLabel);
    redLayout->setSpacing(0);

    QVBoxLayout *yellowLayout = new QVBoxLayout;
    QLabel *yellowLabel = new QLabel("可记录伤害事故", this);
    yellowLabel->setFont(chFont);
    //yellowLabel->setStyleSheet(QString("QLabel{background:#FFEE99}"));
    yellowLayout->addWidget(yellowLabel);
    QLabel *yelloweLabel = new QLabel("Minor Recordable HSSE Incident");
    yelloweLabel->setFont(enFont);
    if (light == 1){
        yellowLabel->setPalette(lgFontPal);
        yelloweLabel->setPalette(lgFontPal);
    }
    else{
        yellowLabel->setPalette(smFontPal);
        yelloweLabel->setPalette(smFontPal);
    }
    //yelloweLabel->setStyleSheet(QString("QLabel{background:#FFEE99}"));
    yellowLayout->addWidget(yelloweLabel);
    yellowLayout->setSpacing(0);

    QVBoxLayout *greenLayout = new QVBoxLayout;
    QLabel *greenLabel = new QLabel("无可记录事故", this);
    greenLabel->setFont(chFont);
    //greenLabel->setStyleSheet(QString("QLabel{background:#CCFF99}"));
    greenLayout->addWidget(greenLabel);
    QLabel *greeneLabel = new QLabel("No Recordable HSSE Incident");
    greeneLabel->setFont(enFont);
    if (light == 2){
        greenLabel->setPalette(lgFontPal);
        greeneLabel->setPalette(lgFontPal);
    }
    else{
        greenLabel->setPalette(smFontPal);
        greeneLabel->setPalette(smFontPal);
    }
    //greeneLabel->setStyleSheet(QString("QLabel{background:#CCFF99}"));
    greenLayout->addWidget(greeneLabel);
    greenLayout->setSpacing(0);

    dayLabel = new QLabel(this);
    dayLabel->setFont(strFont);
    dayLabel->setPalette(lgFontPal);
    dayLabel->setText("0");

    QVBoxLayout *dayLayout = new QVBoxLayout;
    QLabel *rdLabel = new QLabel("自上次可记录伤害事故天数", this);
    rdLabel->setFont(chFont);
    rdLabel->setPalette(lgFontPal);
    //rdLabel->setStyleSheet(QString("QLabel{background:#BBFFEE}"));
    dayLayout->addWidget(rdLabel);
    QLabel *rdeLabel = new QLabel("Days Since Last Recordable HSSE Incident", this);
    rdeLabel->setFont(enFont);
    rdeLabel->setPalette(lgFontPal);
    //rdeLabel->setStyleSheet(QString("QLabel{background:#BBFFEE}"));
    dayLayout->addWidget(rdeLabel);
    dayLayout->setSpacing(0);

    hourLabel = new QLabel(this);
    hourLabel->setFont(strFont);
    hourLabel->setPalette(lgFontPal);
    hourLabel->setText("0");

    QVBoxLayout *hourLayout = new QVBoxLayout;
    QLabel *hdLabel = new QLabel("自上次损失工时事故发生后的安全工时", this);
    hdLabel->setFont(chFont);
    hdLabel->setPalette(lgFontPal);
    //hdLabel->setStyleSheet(QString("QLabel{background:#BBFFEE}"));
    hourLayout->addWidget(hdLabel);
    QLabel *hdeLabel = new QLabel("Safe Hours Since Last DAFWC", this);
    hdeLabel->setFont(enFont);
    hdeLabel->setPalette(lgFontPal);
    //hdeLabel->setStyleSheet(QString("QLabel{background:#BBFFEE}"));
    hourLayout->addWidget(hdeLabel);
    hourLayout->setSpacing(0);

    QGridLayout *screenLayout = new QGridLayout;
    screenLayout->addWidget(lightLabel, 0, 0, 3, 1, Qt::AlignCenter);
    screenLayout->addLayout(redLayout, 0, 1);
    screenLayout->addLayout(yellowLayout, 1, 1);
    screenLayout->addLayout(greenLayout, 2, 1);
    screenLayout->addWidget(dayLabel, 3, 0, Qt::AlignCenter);
    screenLayout->addLayout(dayLayout, 3, 1);
    screenLayout->addWidget(hourLabel, 4, 0, Qt::AlignCenter);
    screenLayout->addLayout(hourLayout, 4, 1);

    /*QVBoxLayout *headLayout = new QVBoxLayout;
    QLabel *header = new QLabel("健康安全环保HSSE", this);
    header->setStyleSheet(QString("QLabel{background:lightGray}"));
    header->setFont(strFont);
    header->setPalette(lgFontPal);
    headLayout->addWidget(header);
    headLayout->addLayout(screenLayout);*/
    this->setLayout(screenLayout);

    rdate = new QDate;
    ldate = new QDate;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(60000);
}

Screen::~Screen()
{

}

void Screen::applyAll(int newLight, int newAdd, QDate newrDate, QDate newlDate)
{
    light = newLight;
    add = newAdd;
    *rdate = newrDate;
    *ldate = newlDate;

    QImage *img = new QImage;
    if (light == 0){
        img->load(":/image/red");
    }
    else if (light == 1){
        img->load(":/image/yellow");
    }
    else if (light == 2){
        img->load(":/image/green");
    }
    else{
        img->load(":/image/black");
    }
    *img = img->scaled(width*3/5, height*3/5, Qt::KeepAspectRatio);
    lightLabel->setPixmap(QPixmap::fromImage(*img));

    dayLabel->setText(QString::number(rdate->daysTo(QDate::currentDate())));
    hourLabel->setText(QString::number(ldate->daysTo(QDate::currentDate())*add));
}

void Screen::updateTime()
{
    dayLabel->setText(QString::number(rdate->daysTo(QDate::currentDate())));
    hourLabel->setText(QString::number(ldate->daysTo(QDate::currentDate())*add));
}

void Screen::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos();
}

void Screen::mouseMoveEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    last = e->globalPos();
    move(x()+dx,y()+dy);
}

void Screen::mouseReleaseEvent(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
}
