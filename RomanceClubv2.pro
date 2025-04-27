QT += widgets svg

CONFIG += c++17
CONFIG += debug_and_release

TARGET = RomanceClubv2
TEMPLATE = app

SOURCES += main.cpp \
           romanceclub.cpp

HEADERS += romanceclub.h

RESOURCES += romanceclub.qrc

QMAKE_CXXFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8


win32: {
    DESTDIR = $$PWD/bin
    MOC_DIR = $$PWD/build/moc
    OBJECTS_DIR = $$PWD/build/obj
    RCC_DIR = $$PWD/build/rcc
    UI_DIR = $$PWD/build/ui
}
