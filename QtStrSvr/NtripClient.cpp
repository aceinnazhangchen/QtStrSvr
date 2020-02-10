#include "NtripClient.h"

NtripClient::NtripClient(QObject *parent)
	: QTcpSocket(parent)
	, m_nType(0)
	, m_nNetPackageNum(0)
{

}

NtripClient::~NtripClient()
{
}

void NtripClient::doConnect()
{
	connectToHost(m_strIp, m_nPort);//set server ip and port
}

QString NtripClient::getUserNameAndPasswordBase64()
{
	QString nameAndPassword = m_sUserName + ":" + m_sPassword;
	return nameAndPassword.toLocal8Bit().toBase64();
}
