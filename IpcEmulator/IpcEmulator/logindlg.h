#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include "ui_logindlg.h"

class CLoginDlg : public QDialog
{
	Q_OBJECT

public:
	explicit CLoginDlg(QWidget *parent = 0);
	~CLoginDlg();

signals:
	void logSuccess();

private:
	Ui::Form ui;
	QString m_sFilepath;

private slots:
	void on_openBtn_clicked();
	void on_cameraBtn_clicked();
	void on_videoBtn_clicked();

};

#endif // LOGINDLG_H
