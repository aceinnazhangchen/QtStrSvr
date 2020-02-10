#pragma once

#include <QWidget>
#include "ui_ClientModuleUI.h"
#include "SingleClientConfigUI.h"
#include "NtripClient.h"
#include "ConfigFile.h"

enum emStates
{
	emStart,
	emStop,
};

class ClientModuleUI : public QWidget
{
	Q_OBJECT

public:
	ClientModuleUI(QWidget *parent = Q_NULLPTR);
	~ClientModuleUI();

	void saveConfig();
	void loadConfig();

	void openStream();
	void openForward();
	void openLocalRTK();
private:
	Ui::ClientModuleUI ui;
	emStates m_nStates;

	SingleClientConfigUI* m_pSingleClientConfigUI_1;
	SingleClientConfigUI* m_pSingleClientConfigUI_2;
	SingleClientConfigUI* m_pSingleClientConfigUI_3;

	ConfigFile* m_pConfigFile;

public slots:
	void OnSaveConfig();
	void OnStartOrStop();
	void OnClearData();
	void OnShowDataLog(int nIndex, QString log);
	void OnShowDataSize(int nIndex, int size);
	void OnUIEnable(int nIndex, bool enabel);
};
