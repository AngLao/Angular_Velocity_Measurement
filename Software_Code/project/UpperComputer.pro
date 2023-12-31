QT       += core gui
QT       += serialport      #加入串口模块
QT       += printsupport    #加入画图模块
QT       += network         #加入网络编程

#DEFINES += QT_NO_WARNING_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    component/MyComboBox.cpp \
    SerialPort/SerialPortBase.cpp \
    Waveform/WaveformDisplay.cpp \

HEADERS += \
    main.h \
    component/MyComboBox.h \
    SerialPort/SerialPortBase.h \
    Waveform/WaveformDisplay.h \
    mavlink/V2.0 \
    mavlink/V2.0/common \
    mavlink/V2.0/standard \

FORMS += \
    SerialPort/SerialPortBase.ui \
    Waveform/Waveform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    main.qrc

#设置图标
RC_ICONS = icon/software.ico

#波形图qcustomplot2 dll路径
LIBS += -L$$PWD/Waveform/ -lqcustomplot2

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
