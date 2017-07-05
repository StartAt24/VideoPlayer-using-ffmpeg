#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include "ui_logindlg.h"
#include "mytitlebar.h"
#include "mgr.h"

class CLoginDlg : public QDialog
{
	Q_OBJECT

public:
	explicit CLoginDlg(CMgr *pMgr, QWidget *parent = 0);
	~CLoginDlg();

signals:
	void logSuccess();

private:
	Ui::Form ui;
	void initTitleBar();
	//void paintEvent(QPaintEvent *event);
	void loadStyleSheet(const QString &sheetName);
private slots:
	void on_openBtn_clicked();
	void on_cameraBtn_clicked();
	void on_videoBtn_clicked();

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	MyTitleBar *m_titleBar;

private:
	CMgr*		m_pMgr;
};

#endif // LOGINDLG_H
