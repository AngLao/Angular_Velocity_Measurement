
#ifndef WAVEFORMDISPLAY_H
#define WAVEFORMDISPLAY_H


#include <QObject>

#include "qcustomplot.h"
#include "ui_Waveform.h"

#include <mavlink/V2.0/common/mavlink.h>

class WaveformDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformDisplay(void);

    QWidget* Widget(){
        return pWidget;
    };

    void paintGyroData(float gyroX, float gyroY, float gyroZ);
private:
    void lineInit();
    unsigned long frameCount = 0;

public:

private:
    class Ui_Form *ui;
    QWidget* pWidget;

    QCustomPlot* pQCustomPlot;
    //按钮标志位
    bool startFlag = false;
    bool dynamicDisplayFlag =true;
};

#endif // WAVEFORMDISPLAY_H
