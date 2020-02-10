#pragma once

#include <QObject>
#include "NtripClient.h"
#include <QtSerialPort/QSerialPort>  
#include <QtSerialPort/QSerialPortInfo> 
#include <QTcpSocket>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

enum StreamType
{
	emNtripClientStream = 0,
	emSerialPortStream = 1,
};

class StreamInput : public QObject
{
	Q_OBJECT

public:
	StreamInput(QObject *parent);
	~StreamInput();

	void setType(int nType);
	void setIndex(int nIndex);
	int getIndex();
	void setNtripClientParams(QString sIp, int nPort, QString sMountPoint, QString sUserName, QString sPassword);
	void setSerialPortParams(QString sName, int nBaudRate,int nDataBits, int nParity, int nStopBits);
	void setOtherNtrip(QString sIp, int nPort, QString sMountPoint, QString sUserName, QString sPassword);
	void closeOtherNtrip();
	void sendNtripHead();
	void sendOtherNtripConfig();

	void open();
	void close();
	void write(const QByteArray & data);
	void setForwarding(bool forward);
	bool isOk();
	bool isFirst();
private:
	int m_nType;
	int m_nIndex;
	NtripClient* m_pNtripClient;
	QSerialPort* m_QSerialPort;

	bool m_Forwarding;
	bool m_OtherNtrip;
	bool m_sendConfig;
	QJsonObject m_OtherNtripConfig;
public:
	int m_nRtcmPackageNum;
	int m_nDateSize;
	QByteArray m_receiveData;
	bool m_isOpen;
public slots:
	void onClientConnected();
	void onClientDisconnect();
	void onClientReceiveData();
	void onReadReady();
signals:
	void sgnSetUIEnable(int nIndex, bool enable);
	void sgnStream();
};
