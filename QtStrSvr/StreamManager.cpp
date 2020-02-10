#include "StreamManager.h"
#include <iostream>
#include <thread>
#include <mutex>
//#include "LogicCenter.h"
//#include "WebsocketServer.h"

StreamManager * StreamManager::m_instance = NULL;
std::once_flag      StreamManager::m_flag;

StreamManager::StreamManager(QObject *parent)
	: QObject(parent)
{
	m_StreamInput_1 = new StreamInput(this);
	m_StreamInput_2 = new StreamInput(this);
	m_StreamInput_3 = new StreamInput(this);

	m_StreamInput_1->setIndex(1);
	m_StreamInput_2->setIndex(2);
	m_StreamInput_3->setIndex(3);

	connect(m_StreamInput_1,SIGNAL(sgnStream()),this,SLOT(OnStream()));
	connect(m_StreamInput_2, SIGNAL(sgnStream()), this, SLOT(OnStream()));
	connect(m_StreamInput_3, SIGNAL(sgnStream()), this, SLOT(OnStream()));
}

StreamManager::~StreamManager()
{
}

StreamManager* StreamManager::getInstance()
{
	if (m_instance == NULL)
	{
		try
		{
			std::call_once(m_flag, createInstance);
		}
		catch (...)
		{
			std::cout << "CreateInstance error\n";
		}
	}
	return m_instance;
}

void StreamManager::createInstance()
{
	m_instance = new(std::nothrow) StreamManager(NULL);
	if (NULL == m_instance)
	{
		throw std::exception();
	}
}

void StreamManager::setNtripClientConfig(int nIndex, QString sIp, int nPort, QString sMountPoint, QString sUserName, QString sPassword)
{
	switch (nIndex)
	{
	case 1:
	{
		m_StreamInput_1->setNtripClientParams(sIp, nPort, sMountPoint, sUserName, sPassword);
	}
	break;
	case 2:
	{
		m_StreamInput_2->setNtripClientParams(sIp, nPort, sMountPoint, sUserName, sPassword);
	}
	break;
	case 3:
	{
		m_StreamInput_3->setNtripClientParams(sIp, nPort, sMountPoint, sUserName, sPassword);
	}
	break;
	}
}

void StreamManager::setSerialPortConfig(int nIndex, QString sName, int nBaudRate, int nDataBits, int nParity, int nStopBits)
{
	switch (nIndex)
	{
	case 1:
	{
		m_StreamInput_1->setSerialPortParams(sName, nBaudRate, nDataBits, nParity, nStopBits);
	}
	break;
	case 2:
	{
		m_StreamInput_2->setSerialPortParams(sName, nBaudRate, nDataBits, nParity, nStopBits);
	}
	break;
	case 3:
	{
		m_StreamInput_3->setSerialPortParams(sName, nBaudRate, nDataBits, nParity, nStopBits);
	}
	break;
	}
}

void StreamManager::open(int nIndex)
{
	switch (nIndex)
	{
	case 1:
	{
		m_StreamInput_1->open();
	}
	break;
	case 2:
	{
		m_StreamInput_2->open();
	}
	break;
	case 3:
	{
		m_StreamInput_3->open();
	}
	break;
	}
}

void StreamManager::close(int nIndex)
{
	switch (nIndex)
	{
	case 1:
	{
		m_StreamInput_1->close();
	}
	break;
	case 2:
	{
		m_StreamInput_2->close();
	}
	break;
	case 3:
	{
		m_StreamInput_3->close();
	}
	break;
	}
}

void StreamManager::closeAll()
{
	close(1);
	close(2);
	close(3);
}

bool StreamManager::isAllPackageCompleted()
{
	if (m_StreamInput_1->m_nRtcmPackageNum > 0 && m_StreamInput_2->m_nRtcmPackageNum > 0 && m_StreamInput_3->m_nRtcmPackageNum > 0)
	{
		return true;
	}
	return false;
}

bool StreamManager::isFristRtkPackage()
{
	if (m_StreamInput_1->m_nRtcmPackageNum > 0 && m_StreamInput_2->m_nRtcmPackageNum == 0)
	{
		return true;
	}
	return false;
}

void StreamManager::sendToRtkServer(QString & gga)
{
	m_StreamInput_2->write(gga.toUtf8());
}

void StreamManager::OnStream()
{
	StreamInput *obj = (StreamInput*)sender();
	emit sgnStreamDataSize(obj->getIndex(), obj->m_nDateSize);
	//LogicCenter::getInstance()->transformRtcm(obj);
	if (obj == m_StreamInput_1) {
		if (m_StreamInput_2->m_isOpen) {
			m_StreamInput_2->write(m_StreamInput_1->m_receiveData);
			sgnStreamDataSize(2, obj->m_nDateSize);
		}
		if (m_StreamInput_3->m_isOpen) {
			m_StreamInput_3->write(m_StreamInput_1->m_receiveData);
			sgnStreamDataSize(3, obj->m_nDateSize);
		}
		m_StreamInput_1->m_receiveData.clear();
	}
}