#include "ConfigFile.h"
#include <QFile>
#include <QDebug>

ConfigFile::ConfigFile(QObject *parent)
	: QObject(parent)
{
}

ConfigFile::~ConfigFile()
{
}

void ConfigFile::readConfigFile()
{
	QFile file("../config.json");
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	if (!file.isOpen()) return;
	QString value = file.readAll();
	file.close();

	QJsonParseError jsonError;
	QJsonDocument doucement = QJsonDocument::fromJson(value.toUtf8(), &jsonError);

	if (!doucement.isNull() && (jsonError.error == QJsonParseError::NoError))
	{
		if (doucement.isObject())
		{
			m_ConfigJson = doucement.object();
		}
	}
}

void ConfigFile::writeConfigFile()
{
	QJsonDocument document;
	document.setObject(m_ConfigJson);
	QByteArray byteArray = document.toJson(QJsonDocument::Indented);
	QString jsonStr = (byteArray);

	QFile file("../config.json");
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
	{
		qDebug() << "file error";
	}
	QTextStream in(&file);
	in << jsonStr;
	file.close();
}

QJsonObject & ConfigFile::getConfig()
{
	return m_ConfigJson;
}
