#include "mainwindow.h"

#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QDeclarativeItem>
#include <QDeclarativeEngine>
#include <QtDebug>
#include <QDir>
#include <QWidget>
#include "vmvideo.h"
#include <QSettings>
MainWindow::MainWindow(QWidget *parent) :
    QDeclarativeView(parent)
{


    engine()->addImportPath("modules");
    this->engine()->addPluginPath("./plugins");
    //让qml随窗口变化而变化
    this->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setAttribute(Qt::WA_AutoOrientation,true);



    //注册组件到QML 让qml创建类 这种方式不好 让C++与qml通讯 采用上面的方式
    //提醒注册组件必须要在 this->setSource(url);之前进行 否则qml找不到组件
    qmlRegisterType<MainObject>("MainObject", 1, 0, "MainObject");
    qmlRegisterType<ProductObject>("ProductObject",1,0,"ProductObject");
    qmlRegisterType<VMVideo>("Qtvm",1,0,"VMVideo");

    QUrl url;
    QSettings *read = new QSettings("config.ini",QSettings::IniFormat);

    bool ok;
    int qmlDebug = read->value("CONFIG/QmlDebug").toInt(&ok);
    delete read;
    if(qmlDebug == 1)
        url = QUrl::fromLocalFile("../../qml/main.qml");
    else
        url = QUrl::fromLocalFile("qml/main.qml");
    this->setSource(url);
    // this->setFixedSize(QSize(768*0.5,1366*0.5));
    this->setMinimumSize(QSize(768*0.5,1366*0.5));


    mainObject = new MainObject(this);
    context = this->rootContext();
    context->setContextProperty("vm",mainObject);




    //setOpacity(0.8);
    mainItem = qobject_cast<QDeclarativeItem *>(this->rootObject());
    qDebug()<<"mainItem"<<mainItem;


    connect(mainObject,SIGNAL(vmcStateChanged(QVariant)),
            mainItem,SLOT(vmcStatehandle(QVariant)),Qt::DirectConnection);


    connect(mainObject,SIGNAL(sqlAddProductSignal()),
            mainItem,SLOT(vmcproductAdd()),Qt::DirectConnection);


    connect(mainItem,SIGNAL(qmlActionSignal(int,QString)),
            mainObject,SLOT(qmlActionSlot(int,QString)),Qt::QueuedConnection);

    connect(mainObject,SIGNAL(tradeOverSignal()),
            mainItem,SLOT(alipay_pic_ok()),Qt::QueuedConnection);
    connect(mainObject,SIGNAL(tradeResultSignal(QVariant)),
            mainItem,SLOT(tradeResult(QVariant)));
#if 0
    connect(mainObject,SIGNAL(vmProductListChanged()),
            mainItem,SLOT(vmcproductChanged()),Qt::DirectConnection);







#endif


    mainObject->setVmcState(EV_STATE_DISCONNECT);



}

MainWindow::~MainWindow()
{

}






void MainWindow::setUiState(quint8 state)
{
    QVariant returnVar;
    QVariant s = state;
    if(mainItem == NULL) return;
    QMetaObject::invokeMethod(mainItem,"vmcStatehandle",
                         Q_RETURN_ARG(QVariant, returnVar),
                         Q_ARG(QVariant, s));

}


void MainWindow::qmlCreateProduct()
{
    QVariant product;
    if(mainItem == NULL) return;
    QMetaObject::invokeMethod(mainItem,"vmCreateProduct",
                         Q_RETURN_ARG(QVariant, product));

    //productList.append(product);
}


void MainWindow::qmlDeleteProduct()
{
    QVariant product;
    if(mainItem == NULL) return;
    QMetaObject::invokeMethod(mainItem,"vmDeleteProduct",
                         Q_ARG(QVariant, product));

}


