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
	bool				m_bSliderPressed;
private slots:
	void on_playBtn_clicked();
	void on_progressBar_sliderReleased();
	void on_recordBtn_clicked();
	void on_backBtn_clicked();
	void clearTimer();

	void on_progressBar_sliderPressed();
public slots:
	void receive_Sec(int sec);

private:
	void setTimeLabel(int currentNum);
	
protected:
	void resizeEvent(QResizeEvent *event);
signals:
	void notifyRefresh(bool);
};

#endif // IPCEMULATOR_H
