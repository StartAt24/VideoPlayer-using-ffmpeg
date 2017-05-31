#include "ipcemulator.h"
#include <QtWidgets/QApplication>
#include "logindlg.h"
#include <QTextCodec>
#include <iostream>
// 解决Qt5 + vs2013中文乱码的问题
#pragma execution_character_set("utf-8") 


int main(int argc, char *argv[])
{
	//解决Qt5 + vs2013中文乱码问题
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	std::cout << "Program Begin---" << std::endl;



	QApplication a(argc, argv);
	IpcEmulator w;				//主窗口
	CLoginDlg  login;			//登陆窗口
	
	w.setWindowTitle("模拟相机");
	login.setWindowTitle("登陆");

	w.SetLoginDlg(&login);
	/*
	if (login.exec() == QDialog::Accepted)
	{
		w.show();
		return a.exec();
	}
	else
	{
		return 0;
	}
	*/
	w.show();
	a.exec();
	return 0;
}
