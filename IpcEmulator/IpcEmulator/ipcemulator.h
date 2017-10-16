#ifndef IPCEMULATOR_H
#define IPCEMULATOR_H

#include <QtWidgets/QMainWindow>
#include "ui_ipcemulator.h"
#include "VideoThread.h"
#include "logindlg.h"
#include "mgr.h"

class IpcEmulator : public QMainWindow
{
	Q_OBJECT

public:
	IpcEmulator(CMgr *mgr,QWidget *parent = 0);
	~IpcEmulator();

	void SetLoginDlg(CLoginDlg* l);
	int  IpcEmulatorInit();

private:
	Ui::IpcEmulatorClass ui;
	CLoginDlg			*m_pLoginDlg;
	CMgr				*m_pMgr;
	CVideoThread		*m_pVideoThread;
	QTimer				*m_pTimer;

private slots:
	void on_playBtn_clicked();
	void on_progressBar_valueChanged(int value);
	void on_recordBtn_clicked();
	void on_backBtn_clicked();
	void clearTimer();

private:
	void setTimeLabel(int currentNum);
	
protected:
	void resizeEvent(QResizeEvent *event);	
};

#endif // IPCEMULATOR_H
