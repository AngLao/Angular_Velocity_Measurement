#include "WaveformDisplay.h"


WaveformDisplay::WaveformDisplay(void)
{
    pWidget = new QWidget();

    ui = new class Ui_Form();
    ui->setupUi(pWidget);

    //开始显示按钮
    connect(ui->startWaveformButton,&QPushButton::clicked,this,[=](){
        if(ui->startWaveformButton->text() == "开始显示"){
            ui->startWaveformButton->setText("暂停显示");
            startFlag = true;
        }else{
            ui->startWaveformButton->setText("开始显示");
            startFlag = false;
        }
    });
    //清空波形按钮
    connect(ui->clearWaveformButton,&QPushButton::clicked,this,[=](){
        int count=pQCustomPlot->graphCount();//获取曲线条数
        for(int i=0;i<count;++i)
        {
            pQCustomPlot->graph(i)->data().data()->clear();
        }
        pQCustomPlot->replot();
    });
    //动态显示按钮
    connect(ui->dynamicDisplayButton,&QPushButton::clicked,this,[=](){
        if(ui->dynamicDisplayButton->text() == "动态显示"){
            ui->dynamicDisplayButton->setText("停止跟随");
            dynamicDisplayFlag = true;
        }else{
            ui->dynamicDisplayButton->setText("动态显示");
            dynamicDisplayFlag = false;
        }
    });
    //全显波形按钮
    connect(ui->seeAllLineButton,&QPushButton::clicked,this,[=](){
        ui->dynamicDisplayButton->setText("动态显示");
        dynamicDisplayFlag = false;
        pQCustomPlot->xAxis->rescale(true);//调整X轴的范围，使之能显示出所有的曲线的X值
        pQCustomPlot->yAxis->rescale(true);//调整Y轴的范围，使之能显示出所有的曲线的Y值
        pQCustomPlot->replot();  // 刷新
    });

    //波形线条初始化
    lineInit();
}

void WaveformDisplay::lineInit(){
    //提升widget类为QCustomPlot类
    pQCustomPlot = new QCustomPlot(this);
    ui->widgetLayout->addWidget(pQCustomPlot);

    //新增波形
    QStringList colorStrList, nameList;
    colorStrList<<"green"<<"glod"<<"red";
    nameList<<"X"<<"Y"<<"Z";
    for (int var = 0; var < 3; ++var) {
        QPen pen;
        pen.setWidth(2);//设置线宽
        pen.setStyle(Qt::PenStyle::SolidLine);//设置为实线
        pen.setColor(QColor(colorStrList.at(var)));//设置线条颜色

        pQCustomPlot->addGraph();
        pQCustomPlot->graph(var)->setName(nameList.at(var));
        pQCustomPlot->graph(var)->setPen(pen);
        pQCustomPlot->graph(var)->setVisible(true);
    }

    //显示图例
    pQCustomPlot->legend->setVisible(true);
    //允许拖拽
    pQCustomPlot->setInteraction( QCP::iRangeDrag , true);
    //允许缩放
    pQCustomPlot->setInteraction( QCP::iRangeZoom , true);

}


void WaveformDisplay::paintGyroData(mavlink_raw_imu_t &rawImuData)
{
    static bool isChange = false;
    static unsigned long frameCount = 0;
    if(startFlag){
        //添加数据
        isChange=true;

        pQCustomPlot->graph(0)->addData(frameCount,rawImuData.xgyro);
        pQCustomPlot->graph(1)->addData(frameCount,rawImuData.ygyro);
        pQCustomPlot->graph(2)->addData(frameCount,rawImuData.zgyro);

        frameCount++;
    }
    if(dynamicDisplayFlag){
        //刷新数据的时候才动态显示
        if(isChange){
            //动态x轴
            pQCustomPlot->xAxis->setRange(frameCount-20, 300, Qt::AlignHorizontal_Mask);
            //设置y轴范围
            QCPRange a = pQCustomPlot->yAxis->range();
            if(rawImuData.xgyro < a.lower)
                pQCustomPlot->yAxis->setRange(rawImuData.xgyro,a.upper);
            else if(rawImuData.xgyro > a.upper)
                pQCustomPlot->yAxis->setRange(a.lower,rawImuData.xgyro);

            if(rawImuData.ygyro < a.lower)
                pQCustomPlot->yAxis->setRange(rawImuData.ygyro,a.upper);
            else if(rawImuData.ygyro > a.upper)
                pQCustomPlot->yAxis->setRange(a.lower,rawImuData.ygyro);

            if(rawImuData.zgyro < a.lower)
                pQCustomPlot->yAxis->setRange(rawImuData.zgyro,a.upper);
            else if(rawImuData.zgyro > a.upper)
                pQCustomPlot->yAxis->setRange(a.lower,rawImuData.zgyro);

        }
    }

    pQCustomPlot->replot(QCustomPlot::rpQueuedReplot); //刷新
    //    static QTimer* thisTimer = new QTimer();
    //    thisTimer->setTimerType(Qt::PreciseTimer);
    //    connect(thisTimer , &QTimer::timeout , this , [=](){
    //        if(isChange){
    //            pQCustomPlot->replot(QCustomPlot::rpQueuedReplot); //刷新
    //            isChange=false;
    //        }
    //    });
    //    thisTimer->start(1);

}
