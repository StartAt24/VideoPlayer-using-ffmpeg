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

private:
	Ui::Form ui;

};

#endif // LOGINDLG_H
