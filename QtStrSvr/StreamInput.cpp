#include "StreamInput.h"
//#include "LogicCenter.h"
#include "StreamManager.h"

StreamInput::StreamInput(QObject *parent)
	: QObject(parent)
	, m_nType(emNtripClientStream)
	, m_nIndex(0)
	, m_nRtcmPackageNum(0)
	, m_nDateSize(0)
	, m_Forwarding(false)
	, m_OtherNtrip(false)
	, m_sendConfig(false)
	, m_isOpen(false)
{
	m_pNtripClient = new NtripClient(this);
	m_QSerialPort = new QSerialPort(this);
	m_receiveData.clear();

	connect(m_pNtripClient, SIGNAL(connected()), this, SLOT(onClientConnected()));
	connect(m_pNtripClient, SIGNAL(readyRead()), this, SLOT(onClientReceiveData()));
	connect(m_pNtripClient, SIGNAL(disconnected()), this, SLOT(onClientDisconnect()));

	connect(m_QSerialPort, SIGNAL(readyRead()), this, SLOT(onReadReady()));
}

StreamInput::~StreamInput()
{
	delete m_pNtripClient;
	delete m_QSerialPort;
}

void StreamInput::setType(int nType)
{
	m_nType = nType;
}

void StreamInput::setIndex(int nIndex)
{
	m_nIndex = nIndex;
}

int StreamInput::getIndex()
{
	return m_nIndex;
}

void StreamInput::setNtripClientParams(QString sIp, int nPort, QString sMountPoint, QString sUserName, QString sPassword)
{
	m_nType = emNtripClientStream;
	m_pNtripClient->m_strIp = sIp;
	m_pNtripClient->m_nPort = nPort;
	m_pNtripClient->m_sMountPoint = sMountPoint;
	m_pNtripClient->m_sUserName = sUserName;
	m_pNtripClient->m_sPassword = sPassword;
}

void StreamInput::setSerialPortParams(QString sName, int nBaudRate, int nDataBits, int nParity, int nStopBits)
{
	m_nType = emSerialPortStream;
	m_QSerialPort->setPortName(sName);
	//
	m_QSerialPort->setBaudRate(nBaudRate);
	switch (nDataBits)
	{
	case 5: m_QSerialPort->setDataBits(QSerialPort::Data5); break;
	case 6: m_QSerialPort->setDataBits(QSerialPort::Data6); break;
	case 7: m_QSerialPort->setDataBits(QSerialPort::Data7); break;
	case 8: m_QSerialPort->setDataBits(QSerialPort::Data8); break;
	default: break;
	}
	switch (nParity)
	{
	case 0: m_QSerialPort->setParity(QSerialPort::NoParity); break;
	case 1: m_QSerialPort->setParity(QSerialPort::OddParity); break;
	case 2: m_QSerialPort->setParity(QSerialPort::EvenParity); break;
	case 3: m_QSerialPort->setParity(QSerialPort::SpaceParity); break;
	case 4: m_QSerialPort->setParity(QSerialPort::MarkParity); break;
	default: break;
	}
	switch (nStopBits)
	{
	case 0: m_QSerialPort->setStopBits(QSerialPort::OneStop); break;
	case 1: m_QSerialPort->setStopBits(QSerialPort::OneAndHalfStop); break;
	case 2: m_QSerialPort->setStopBits(QSerialPort::TwoStop); break;
	default: break;
	}
	m_QSerialPort->setFlowControl(QSerialPort::NoFlowControl);

	
}

void StreamInput::setOtherNtrip(QString sIp, int nPort, QString sMountPoint, QString sUserName, QString sPassword)
{
	m_OtherNtrip = true;
	m_OtherNtripConfig.insert("host", sIp);
	m_OtherNtripConfig.insert("port", nPort);
	m_OtherNtripConfig.insert("mountPoint", sMountPoint);
	m_OtherNtripConfig.insert("user", sUserName);
	m_OtherNtripConfig.insert("password", sPassword);
}

void StreamInput::closeOtherNtrip()
{
	m_OtherNtrip = false;
}

void StreamInput::sendNtripHead()
{
	QString NtripData;
	if (m_Forwarding)
	{
		if (m_OtherNtrip)
		{
			NtripData = QString("SOURCE %1 /%2\r\nSource-Agent: NTRIP Mobile / ble1.0\r\nAccept: text/other\r\n\r\n").arg(m_pNtripClient->m_sPassword,m_pNtripClient->m_sMountPoint);
		}
		else
		{
			NtripData = QString("SOURCE %1 /%2\r\nSource-Agent: NTRIP Mobile / ble1.0\r\n\r\n").arg(m_pNtripClient->m_sPassword,m_pNtripClient->m_sMountPoint);
		}
	}
	else
	{
		NtripData = QString("GET /%1 HTTP/1.1\r\nUser-Agent: NTRIP JS Client/0.2\r\nAuthorization: Basic %2\r\n\r\n").arg(m_pNtripClient->m_sMountPoint, m_pNtripClient->getUserNameAndPasswordBase64());
	}
	m_pNtripClient->write(NtripData.toUtf8());
}

void StreamInput::sendOtherNtripConfig()
{
	if (m_OtherNtrip)
	{
		QJsonDocument document;
		document.setObject(m_OtherNtripConfig);
		QByteArray byteArray = document.toJson(QJsonDocument::Compact);
		m_pNtripClient->write(byteArray);
		QString end = "\r\n";
		m_pNtripClient->write(end.toUtf8());
		m_sendConfig = true;
	}
}

void StreamInput::open()
{
	m_nRtcmPackageNum = 0;
	m_receiveData.clear();
	m_nDateSize = 0;
	m_sendConfig = false;
	if (m_nType == emNtripClientStream)
	{
		m_pNtripClient->doConnect();
	}
	else
	{
		m_QSerialPort->open(QIODevice::ReadWrite);
	}
	emit StreamManager::getInstance()->sgnSetUIEnable(m_nIndex, false);
	m_isOpen = true;
}

void StreamInput::close()
{
	m_OtherNtrip = false;
	if (m_nType == emNtripClientStream)
	{
		m_pNtripClient->close();
	}
	else
	{
		m_QSerialPort->clear();
		m_QSerialPort->close();
	}
	emit StreamManager::getInstance()->sgnSetUIEnable(m_nIndex, true);
	m_isOpen = false;
}

void StreamInput::write(const QByteArray & data)
{
	if (m_nType == emNtripClientStream)
	{
		m_pNtripClient->write(data);
	}
	else
	{
		m_QSerialPort->write(data);
	}
}

void StreamInput::setForwarding(bool forward)
{
	m_Forwarding = forward;
}

bool StreamInput::isOk()
{
	if(m_OtherNtrip && m_sendConfig && m_pNtripClient->m_nNetPackageNum >= 1)
	{
		return true;
	}
	if (!m_OtherNtrip && m_pNtripClient->m_nNetPackageNum >= 1)
	{
		return true;
	}
	return false;
}

bool StreamInput::isFirst()
{
	return (m_pNtripClient->m_nNetPackageNum == 1);
}

void StreamInput::onClientConnected()
{
	m_pNtripClient->m_nNetPackageNum = 0;
	sendNtripHead();
	//disable ui;
	emit StreamManager::getInstance()->sgnSetUIEnable(m_nIndex,false);
}

void StreamInput::onClientDisconnect()
{
	//enable ui;
	emit StreamManager::getInstance()->sgnSetUIEnable(m_nIndex,true);
}

void StreamInput::onClientReceiveData()
{
	QByteArray byteArray = m_pNtripClient->readAll();
	m_nDateSize += byteArray.size();
	m_pNtripClient->m_nNetPackageNum++;
	if (m_pNtripClient->m_nNetPackageNum == 1)//Skip first pagkage
	{
		sendOtherNtripConfig();
	}
	else
	{
		m_receiveData.append(byteArray);
	}
	emit sgnStream();
}

void StreamInput::onReadReady()
{
	QByteArray byteArray = m_QSerialPort->readAll();
	m_nDateSize += byteArray.size();
	m_receiveData.append(byteArray);
	emit sgnStream();
}