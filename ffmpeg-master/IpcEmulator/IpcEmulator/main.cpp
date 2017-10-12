#include "ipcemulator.h"
#include <QtWidgets/QApplication>
#include "logindlg.h"
#include <QTextCodec>
#include <iostream>
#include "Mgr.h"
// 解决Qt5 + vs2013中文乱码的问题
#pragma execution_character_set("utf-8") 

HANDLE g_hRefreshEvent;		//重绘事件

int main(int argc, char *argv[])
{
	//解决Qt5 + vs2013中文乱码问题
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	std::cout << "------------------------DJJ Begin-------------------------" << std::endl;

	g_hRefreshEvent = CreateEvent(NULL, FALSE, TRUE, NULL);//初始无触发的匿名事件

	QApplication a(argc, argv);
	CMgr	   mgr;				//全局的操作类;
	IpcEmulator w(&mgr);				//主窗口
	CLoginDlg  login(&mgr);			//登陆窗口
	
	a.connect(mgr.m_pFFmpeg, SIGNAL(toIpcwindow(int)), &w, SLOT(receive_Sec(int)));
	a.connect(&w, SIGNAL(notifyRefresh(bool)), mgr.m_pFFmpeg, SLOT(refresh(bool)));
	w.setWindowTitle("模拟相机");
	w.setWindowIcon(QIcon(":/btnImg/Resources/video.png"));
	//如果自绘了标题栏不需要这部分
	login.setWindowTitle("登陆");
	login.setWindowIcon(QIcon(":/btnImg/Resources/user.png"));
	w.SetLoginDlg(&login);
	
	
	if (login.exec() == QDialog::Accepted)
	{
		w.show();
		return a.exec();
	}
	else
	{
		return 0;
	}
	
	//w.show();
	//a.exec();

	CloseHandle(g_hRefreshEvent);
	return 0;
}
