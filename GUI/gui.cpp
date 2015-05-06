#include "gui.h"

GUI::GUI(Shared_memory *share_memory)
{
    this->share_memory = share_memory;

    QGridLayout* mainLayout = new QGridLayout();

    QGroupBox* boxLaser = new QGroupBox("ON/OFF");
    QGridLayout* layLaser = new QGridLayout();
    boxLaser->setLayout(layLaser);
    button_laseron = new QPushButton("Laser ON");
    button_laseroff = new QPushButton("Laser OFF");
    lcdnumber = new QLCDNumber(QLCDNumber::Flat);
    lcdnumber->setFixedSize(300, 100);
    lcdnumber->setPalette(Qt::red);
    lcdnumber->setDigitCount(8);

    layLaser->addWidget(button_laseron, 0, 0);
    layLaser->addWidget(button_laseroff, 0, 1);
    layLaser->addWidget(lcdnumber, 1, 0, 1, 2);

    mainLayout->addWidget(boxLaser, 0, 0);

    setLayout(mainLayout);

    setVisible(true);

    connect(button_laseron, SIGNAL(clicked()), this, SLOT(on_buttonlaseon_click()));
    connect(button_laseroff, SIGNAL(clicked()), this, SLOT(on_buttonlaseoff_click()));

    connect(this, SIGNAL(signal_updateGUI()), this, SLOT(on_updateGUI_recieved()));

    show();
}

void GUI::updateThreadGUI()
{
    emit signal_updateGUI();
}

void GUI::on_updateGUI_recieved()
{
    float measure = share_memory->getMeasure();
    if(measure==-1)
        return;
    lcdnumber->display(QString("%1").arg(measure));
}

void GUI::on_buttonlaseon_click()
{
    share_memory->laserON();
}

void GUI::on_buttonlaseoff_click()
{
    share_memory->laserOFF();
}
