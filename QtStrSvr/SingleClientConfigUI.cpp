#include "SingleClientConfigUI.h"
#include <QtSerialPort/QSerialPortInfo> 
#include <QFileDialog>

SingleClientConfigUI::SingleClientConfigUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QStringList baudList;
	baudList << "50" << "75" << "100" << "134" << "150" << "200" << "300"
		<< "600" << "1200" << "1800" << "2400" << "4800" << "9600"
		<< "14400" << "19200" << "38400" << "56000" << "57600"
		<< "76800" << "115200" << "128000" << "256000" << "460800";

	ui.BaudBox->addItems(baudList);
	ui.BaudBox->setCurrentIndex(19);
	ui.BitNumBox->setCurrentIndex(3);

	connect(ui.m_comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(onStreamTypeChanged(int)));


	ui.m_widgetSerialPotConfig->hide();
	ui.m_widgetFileConfig->hide();

	connect(ui.m_toolButtonRoverFile, SIGNAL(clicked()), this, SLOT(onOpenRoverFileClicked()));
	connect(ui.m_toolButtonBaseFile, SIGNAL(clicked()), this, SLOT(onOpenBaseFileClicked()));
}

SingleClientConfigUI::~SingleClientConfigUI()
{
}

int SingleClientConfigUI::getType()
{
	return ui.m_comboBoxType->currentIndex();
}

void SingleClientConfigUI::setName(QString name)
{
	ui.m_checkBoxStream->setText(name);
}

bool SingleClientConfigUI::isChecked()
{
	return ui.m_checkBoxStream->isChecked();
}

bool SingleClientConfigUI::isAllCompleted()
{
	if (ui.m_comboBoxType->currentIndex() == 0)
	{
		if (ui.m_comboBoxHost->currentText().trimmed().isEmpty()
			|| ui.m_comboBoxPort->currentText().trimmed().isEmpty()
			|| ui.m_comboBoxMountpoint->currentText().trimmed().isEmpty())
		{
			return false;
		}
	}
	else
	{
		if (ui.PortBox->currentText().trimmed().isEmpty())
		{
			return false;
		}
	}

	return true;
}

QString SingleClientConfigUI::getIp()
{
	return  ui.m_comboBoxHost->currentText().trimmed();
}

int SingleClientConfigUI::getPort()
{
	return ui.m_comboBoxPort->currentText().trimmed().toInt();
}

QString SingleClientConfigUI::getMountPoint()
{
	return ui.m_comboBoxMountpoint->currentText().trimmed();
}

QString SingleClientConfigUI::getUserName()
{
	return ui.m_comboBoxUser->currentText().trimmed();
}

QString SingleClientConfigUI::getPassword()
{
	return ui.m_comboBoxPassword->currentText().trimmed();
}

QString SingleClientConfigUI::getPortName()
{
	return  ui.PortBox->currentText().trimmed();
}

int SingleClientConfigUI::getBitrate()
{
	return ui.BaudBox->currentText().toInt();
}

int SingleClientConfigUI::getByteSize()
{
	return ui.BitNumBox->currentText().toInt();
}

int SingleClientConfigUI::getParity()
{
	return ui.ParityBox->currentIndex();
}

int SingleClientConfigUI::getStopBits()
{
	return ui.StopBox->currentIndex();
}

void SingleClientConfigUI::setConfig(QJsonObject & config)
{
	ui.m_checkBoxStream->setChecked(config["check"].toBool());
	ui.m_comboBoxType->setCurrentIndex(config["type"].toInt());

	ui.PortBox->setCurrentIndex(config["serialport"].toInt());
	ui.BaudBox->setCurrentIndex(config["baud"].toInt());
	ui.BitNumBox->setCurrentIndex(config["bitnum"].toInt());
	ui.ParityBox->setCurrentIndex(config["parity"].toInt());
	ui.StopBox->setCurrentIndex(config["stop"].toInt());

	ui.m_comboBoxHost->setEditText(config["host"].toString());
	ui.m_comboBoxPort->setEditText(config["port"].toString());
	ui.m_comboBoxMountpoint->setEditText(config["mountpoint"].toString());
	ui.m_comboBoxUser->setEditText(config["username"].toString());
	ui.m_comboBoxPassword->setEditText(config["password"].toString());

	ui.m_comboRoverFile->setEditText(config["roverfile"].toString());
	ui.m_comboBaseFile->setEditText(config["basefile"].toString());
}

void SingleClientConfigUI::getConfig(QJsonObject & config)
{
	config.insert("check", ui.m_checkBoxStream->isChecked());
	config.insert("type", ui.m_comboBoxType->currentIndex());

	config.insert("serialport", ui.PortBox->currentIndex());
	config.insert("baud", ui.BaudBox->currentIndex());
	config.insert("bitnum", ui.BitNumBox->currentIndex());
	config.insert("parity", ui.ParityBox->currentIndex());
	config.insert("stop", ui.StopBox->currentIndex());

	config.insert("host", ui.m_comboBoxHost->currentText());
	config.insert("port", ui.m_comboBoxPort->currentText());
	config.insert("mountpoint", ui.m_comboBoxMountpoint->currentText());
	config.insert("username", ui.m_comboBoxUser->currentText());
	config.insert("password", ui.m_comboBoxPassword->currentText());

	config.insert("roverfile", ui.m_comboRoverFile->currentText());
	config.insert("basefile", ui.m_comboBaseFile->currentText());
}

void SingleClientConfigUI::onStreamTypeChanged(int nindex)
{
	if (nindex == 0)
	{
		ui.m_widgetNetConfig->show();
		ui.m_widgetSerialPotConfig->hide();
		ui.m_widgetFileConfig->hide();
	}
	else if (nindex == 1)
	{
		ui.m_widgetNetConfig->hide();
		ui.m_widgetSerialPotConfig->show();
		ui.m_widgetFileConfig->hide();

		ui.PortBox->clear();

		foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
		{
			ui.PortBox->addItem(info.portName());
		}
	}
	else if (nindex == 2) {
		ui.m_widgetNetConfig->hide();
		ui.m_widgetSerialPotConfig->hide();
		ui.m_widgetFileConfig->show();
	}
}


void SingleClientConfigUI::onOpenRoverFileClicked() {
	QString path = QFileDialog::getOpenFileName(this, tr("Open Files"), ".", tr("RTCM Files(*.* )"));
	if (path.length() == 0) {
		return;
	}
	ui.m_comboRoverFile->setEditText(path);
}

void SingleClientConfigUI::onOpenBaseFileClicked() {
	QString path = QFileDialog::getOpenFileName(this, tr("Open Files"), ".", tr("RTCM Files(*.* )"));
	if (path.length() == 0) {
		return;
	}
	ui.m_comboBaseFile->setEditText(path);
}