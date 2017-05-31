#ifndef IPCEMULATOR_H
#define IPCEMULATOR_H

#include <QtWidgets/QMainWindow>
#include "ui_ipcemulator.h"
#include "VideoThread.h"
#include "logindlg.h"

class IpcEmulator : public QMainWindow
{
	Q_OBJECT

public:
	IpcEmulator(QWidget *parent = 0);
	~IpcEmulator();

	void SetLoginDlg(CLoginDlg* l);
	int  IpcEmulatorInit();
private:
	Ui::IpcEmulatorClass ui;
	
	CLoginDlg			*m_pLoginDlg;
};

#endif // IPCEMULATOR_H
