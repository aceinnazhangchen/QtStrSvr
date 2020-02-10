#pragma once

#include <QObject>
#include "StreamInput.h"

class StreamManager : public QObject
{
	Q_OBJECT

private:
	StreamManager(QObject *parent);
	~StreamManager();
public:
	static StreamManager* getInstance();
	static void createInstance();
private:
	static StreamManager* m_instance;
	static std::once_flag m_flag;

	StreamInput* m_StreamInput_1;
	StreamInput* m_StreamInput_2;
	StreamInput* m_StreamInput_3;
public:
	void setNtripClientConfig(int nIndex, QString sIp, int nPort, QString sMountPoint, QString sUserName, QString sPassword);
	void setSerialPortConfig(int nIndex, QString sName, int nBaudRate, int nDataBits, int nParity, int nStopBits);
	void open(int nIndex);
	void close(int nIndex);
	void closeAll();
	bool isAllPackageCompleted();
	bool isFristRtkPackage();
	void sendToRtkServer(QString& gga);
public slots:
	void OnStream();
signals:
	void sgnSetUIEnable(int, bool);
	void sgnStreamDataSize(int, int);
};
