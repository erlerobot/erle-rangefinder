SOURCES += \
    main_gui.cpp \
    GUI/gui.cpp \
    GUI/shared_memory.cpp \
    GUI/threadgui.cpp \
    LRM30_serial.cpp \
    GUI/thread_sensors.cpp


QT += widgets

HEADERS += \
    GUI/gui.h \
    GUI/shared_memory.h \
    GUI/threadgui.h \
    LRM30_serial.h \
    GUI/thread_sensors.h

INCLUDEPATH+= /usr/local/include

LIBS+= -L/usr/local/lib \
 -lserial
