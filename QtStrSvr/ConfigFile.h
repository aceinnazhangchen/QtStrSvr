#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

class ConfigFile : public QObject
{
	Q_OBJECT

public:
	ConfigFile(QObject *parent);
	~ConfigFile();

	void readConfigFile();
	void writeConfigFile();
	QJsonObject& getConfig();
private:
	QJsonObject m_ConfigJson;
};
