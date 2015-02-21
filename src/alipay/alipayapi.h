#ifndef ALIPAYAPI_H
#define ALIPAYAPI_H

#include <QObject>
#include <QMap>
#include <QPixmap>
#include "alipayconfig.h"
class QTimer;
class QNetworkReply;
class QNetworkAccessManager;



class AlipayAPI : public QObject
{
    Q_OBJECT
public:
    explicit AlipayAPI(QObject *parent = 0);
    ~AlipayAPI();


    void tradBegin();

    QString buildRequest(const QMap<QString,QString> &mapArr);
    QMap<QString,QString> filterPara(const QMap<QString,QString> &mapArr);
    QString createLinkString(const QMap<QString,QString> &mapArr);
    QString buildRequestMysign(const QMap<QString,QString> &mapArr);
signals:
    void tradeOverSignal(QPixmap map);
    void tradeResultSignal(int res);
public slots:
    void network_recved(QNetworkReply *reply); //支付http回应处理
    void network_pic_recved(QNetworkReply *reply);//下载二维码回应处理
    void timerout_ali_checked();
private:
    QNetworkAccessManager *network_man; //支付htpp管理
    QNetworkAccessManager *network_pic_man;//下载二维码图片管理
    QTimer *timer_check;//查询订单状态定时器


    bool ali_isChecked;
    QString str_cur_ali_type;//当前与支付宝通信类型
    QString str_cur_ali_trade_no;//当前的订单交易号


    AlipayConfig *aliConfig;

};

#endif // ALIPAYAPI_H
