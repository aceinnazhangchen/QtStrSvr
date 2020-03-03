#pragma once

#include <QWidget>
#include "ui_SingleClientConfigUI.h"
#include <QJsonObject>

class SingleClientConfigUI : public QWidget
{
	Q_OBJECT

public:
	SingleClientConfigUI(QWidget *parent = Q_NULLPTR);
	~SingleClientConfigUI();

	int getType();
	void setName(QString name);
	bool isChecked();
	bool isAllCompleted();
	QString getIp();
	int getPort();
	QString getMountPoint();
	QString getUserName();
	QString getPassword();

	QString getPortName();
	int getBitrate();
	int getByteSize();
	int getParity();
	int getStopBits();

	void setConfig(QJsonObject& config);
	void getConfig(QJsonObject& config);
private:
	Ui::SingleClientConfigUI ui;
private slots:
	void onStreamTypeChanged(int nindex);
	void onOpenRoverFileClicked();
	void onOpenBaseFileClicked();
};
