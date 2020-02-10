#include "ClientModuleUI.h"
//#include "LogicCenter.h"
#include "StreamManager.h"
//#include "ForwardManager.h"
//#include "LocalRTKManager.h"
#include <QMessageBox>

ClientModuleUI::ClientModuleUI(QWidget *parent)
	: QWidget(parent)
	, m_nStates(emStop)
{
	ui.setupUi(this);

	m_pSingleClientConfigUI_1 = new SingleClientConfigUI(this);
	m_pSingleClientConfigUI_1->setName("Input");
	m_pSingleClientConfigUI_2 = new SingleClientConfigUI(this);
	m_pSingleClientConfigUI_2->setName("Output-1");
	m_pSingleClientConfigUI_3 = new SingleClientConfigUI(this);
	m_pSingleClientConfigUI_3->setName("Output-2");
	ui.gridLayout->addWidget(m_pSingleClientConfigUI_1, 0, 1);
	ui.gridLayout->addWidget(m_pSingleClientConfigUI_2, 0, 2);
	ui.gridLayout->addWidget(m_pSingleClientConfigUI_3, 0, 3);

	connect(ui.m_pushButtonStart, SIGNAL(clicked()), this, SLOT(OnStartOrStop()));
	connect(ui.m_pushButtonClearData, SIGNAL(clicked()), this, SLOT(OnClearData()));
	connect(ui.m_pushButtonSave, SIGNAL(clicked()), this, SLOT(OnSaveConfig()));
	
	connect(StreamManager::getInstance(), SIGNAL(sgnSetUIEnable(int, bool)), this, SLOT(OnUIEnable(int, bool)));
	connect(StreamManager::getInstance(), SIGNAL(sgnStreamDataSize(int, int)), this, SLOT(OnShowDataSize(int, int)));

	m_pConfigFile = new ConfigFile(this);
	loadConfig();
}

ClientModuleUI::~ClientModuleUI()
{
}

void ClientModuleUI::saveConfig()
{
	QJsonObject& configJson = m_pConfigFile->getConfig();
	QJsonObject rover;
	m_pSingleClientConfigUI_1->getConfig(rover);
	configJson.insert("Input", rover);
	QJsonObject base;
	m_pSingleClientConfigUI_2->getConfig(base);
	configJson.insert("Output_1", base);
	QJsonObject correction;
	m_pSingleClientConfigUI_3->getConfig(correction);
	configJson.insert("Output_2", correction);
	m_pConfigFile->writeConfigFile();
}

void ClientModuleUI::loadConfig()
{
	m_pConfigFile->readConfigFile();
	QJsonObject& configJson = m_pConfigFile->getConfig();
	if (configJson["Input"].isObject())
	{
		QJsonObject rover = configJson["Input"].toObject();
		m_pSingleClientConfigUI_1->setConfig(rover);
	}
	if (configJson["Output_1"].isObject())
	{
		QJsonObject base = configJson["Output_1"].toObject();
		m_pSingleClientConfigUI_2->setConfig(base);
	}
	if (configJson["Output_2"].isObject())
	{
		QJsonObject correction = configJson["Output_2"].toObject();
		m_pSingleClientConfigUI_3->setConfig(correction);
	}
}

void ClientModuleUI::openStream()
{
	if (m_pSingleClientConfigUI_1->isChecked() && m_pSingleClientConfigUI_1->isAllCompleted())
	{
		if (m_pSingleClientConfigUI_1->getType() == 0)
		{
			StreamManager::getInstance()->setNtripClientConfig(1, m_pSingleClientConfigUI_1->getIp(),
				m_pSingleClientConfigUI_1->getPort(), m_pSingleClientConfigUI_1->getMountPoint(),
				m_pSingleClientConfigUI_1->getUserName(), m_pSingleClientConfigUI_1->getPassword());
		}
		else
		{
			StreamManager::getInstance()->setSerialPortConfig(1, m_pSingleClientConfigUI_1->getPortName(),
				m_pSingleClientConfigUI_1->getBitrate(), m_pSingleClientConfigUI_1->getByteSize(),
				m_pSingleClientConfigUI_1->getParity(), m_pSingleClientConfigUI_1->getStopBits());
		}
		StreamManager::getInstance()->open(1);
	}
	if (m_pSingleClientConfigUI_2->isChecked() && m_pSingleClientConfigUI_2->isAllCompleted())
	{
		if (m_pSingleClientConfigUI_2->getType() == 0)
		{
			StreamManager::getInstance()->setNtripClientConfig(2, m_pSingleClientConfigUI_2->getIp(),
				m_pSingleClientConfigUI_2->getPort(), m_pSingleClientConfigUI_2->getMountPoint(),
				m_pSingleClientConfigUI_2->getUserName(), m_pSingleClientConfigUI_2->getPassword());
		}
		else
		{
			StreamManager::getInstance()->setSerialPortConfig(2, m_pSingleClientConfigUI_2->getPortName(),
				m_pSingleClientConfigUI_2->getBitrate(), m_pSingleClientConfigUI_2->getByteSize(),
				m_pSingleClientConfigUI_2->getParity(), m_pSingleClientConfigUI_2->getStopBits());
		}
		StreamManager::getInstance()->open(2);
	}
	if (m_pSingleClientConfigUI_3->isChecked() && m_pSingleClientConfigUI_3->isAllCompleted())
	{
		if (m_pSingleClientConfigUI_3->getType() == 0)
		{
			StreamManager::getInstance()->setNtripClientConfig(3, m_pSingleClientConfigUI_3->getIp(),
				m_pSingleClientConfigUI_3->getPort(), m_pSingleClientConfigUI_3->getMountPoint(),
				m_pSingleClientConfigUI_3->getUserName(), m_pSingleClientConfigUI_3->getPassword());
		}
		else
		{
			StreamManager::getInstance()->setSerialPortConfig(2, m_pSingleClientConfigUI_3->getPortName(),
				m_pSingleClientConfigUI_3->getBitrate(), m_pSingleClientConfigUI_3->getByteSize(),
				m_pSingleClientConfigUI_3->getParity(), m_pSingleClientConfigUI_3->getStopBits());
		}
		StreamManager::getInstance()->open(3);
	}
}

void ClientModuleUI::openForward()
{

}

void ClientModuleUI::openLocalRTK()
{

}

void ClientModuleUI::OnSaveConfig()
{
	saveConfig();
	QMessageBox::information(this, "Title", "Confg saved.");
}

void ClientModuleUI::OnStartOrStop()
{
	if (m_nStates == emStop)
	{
		saveConfig();
		if (m_pSingleClientConfigUI_1->isChecked() == false
			&& m_pSingleClientConfigUI_2->isChecked() == false
			&& m_pSingleClientConfigUI_3->isChecked() == false)
		{
			return;
		}
		openStream();
		m_nStates = emStart;
	}
	else if (m_nStates == emStart)
	{
		StreamManager::getInstance()->closeAll();
		m_nStates = emStop;
	}

	if (m_nStates == emStart)
	{	
		ui.m_pushButtonStart->setText("Stop");
	}
	else
	{
		ui.m_pushButtonStart->setText("Start");	
	}
}

void ClientModuleUI::OnClearData()
{
	ui.m_plainTextEditData_1->clear();
	ui.m_plainTextEditData_2->clear();
	ui.m_plainTextEditData_3->clear();
	ui.lineEdit_1->setText("0");
	ui.lineEdit_2->setText("0");
	ui.lineEdit_3->setText("0");
}

void ClientModuleUI::OnShowDataLog(int nIndex, QString log)
{
	switch (nIndex)
	{
	case 1:
		ui.m_plainTextEditData_1->appendPlainText(log.trimmed());
		break;
	case 2:
		ui.m_plainTextEditData_2->appendPlainText(log.trimmed());
		break;
	case 3:
		ui.m_plainTextEditData_3->appendPlainText(log.trimmed());
		break;
	}
}

void ClientModuleUI::OnShowDataSize(int nIndex, int size)
{
	switch (nIndex)
	{
	case 1:
		ui.lineEdit_1->setText(QString::number(size));
		break;
	case 2:
		ui.lineEdit_2->setText(QString::number(size));
		break;
	case 3:
		ui.lineEdit_3->setText(QString::number(size));
		break;
	}
}

void ClientModuleUI::OnUIEnable(int nIndex, bool enabel)
{
	switch (nIndex)
	{
	case 1:
		m_pSingleClientConfigUI_1->setEnabled(enabel);
		break;
	case 2:
		m_pSingleClientConfigUI_2->setEnabled(enabel);
		break;
	case 3:
		m_pSingleClientConfigUI_3->setEnabled(enabel);
		break;
	}
}
