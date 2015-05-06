#include <QApplication>

#include "GUI/threadgui.h"
#include "GUI/thread_sensors.h"
#include "GUI/shared_memory.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    Shared_memory* share_memory = new Shared_memory();

    threadGUI gui(share_memory);
    Thread_sensors sensors(share_memory);
    gui.start();
    sensors.start();

    return a.exec();
}

