#ifndef GUI_H
#define GUI_H

#include <QtGui>
#include <QtWidgets/QtWidgets>

#include <iostream>

#include "shared_memory.h"

class GUI:public QWidget
{
    Q_OBJECT

public:
    GUI(Shared_memory* share_memory);
    void updateThreadGUI();

private:
    Shared_memory* share_memory;

    QPushButton* button_laseron;
    QPushButton* button_laseroff;
    QLCDNumber* lcdnumber;

signals:
    void signal_updateGUI();

public slots:
    void on_updateGUI_recieved();

    void on_buttonlaseon_click();
    void on_buttonlaseoff_click();
};

#endif // GUI_H
