#include "logindlg.h"
#include "WindowCompositionAttribute.h"
#include <QtWinExtras/QtWin>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <Cffmpeg.h>
#include <QDesktopWidget>

#pragma execution_character_set("utf-8") 

CLoginDlg::CLoginDlg(CMgr *pMgr, QWidget *parent)
	: QDialog(parent)
{
	m_pMgr = pMgr;
	//FramelessWindowHint属性设置额窗口去除边框
	//windowMinimizeButtonHint 属性设置在窗口最小化时，点击任务栏窗口即可还原
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	//设置窗口背景透明
	//setAttribute(Qt::WA_TranslucentBackground);
	//关闭窗口时释放资源;
	//当窗口对象定义在堆上的时候才需要这句话 即定义方式为：Mainwindow *w = new Mainwindow;
	//如果定义方式是： Mainwindow w; w.show();则是将窗口定义在栈上;
	//setAttribute(Qt::WA_DeleteOnClose);
	//初始化标题栏,
	//！！自绘标题栏 内存占用太大！！
	initTitleBar();

	
	// 用的是 windowsApi
	//win10毛玻璃效果; 需要变成毛玻璃效果的地方 需要将背景设置为黑色;	
	if (QSysInfo::windowsVersion() == QSysInfo::WV_10_0)
	{
		setAttribute(Qt::WA_TranslucentBackground, true);
		HMODULE hUser = GetModuleHandle(L"user32.dll");
		HWND	hWnd = HWND(winId());
		if (hUser)
		{
			pfnSetWindowCompositionAttribute setWindowCompositionAttribute =
				(pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
			if (setWindowCompositionAttribute)
			{
				ACCENT_POLICY accent = { ACCENT_ENABLE_BLURBEHIND, 0, 0, 0 };
				WINDOWCOMPOSITIONATTRIBDATA data;
				data.Attrib = WCA_ACCENT_POLICY;
				data.pvData = &accent;
				data.cbData = sizeof(accent);
				setWindowCompositionAttribute(hWnd, &data);
			}
		}
	}
	
	/*Qt自带接口实现 毛玻璃，在win10上没有效果!
	this->setObjectName("loginWidget");
	if (QtWin::isCompositionEnabled())
	{
		QtWin::extendFrameIntoClientArea(this, -1, -1, -1, -1);
		setAttribute(Qt::WA_TranslucentBackground, true);
		setAttribute(Qt::WA_NoSystemBackground, false);
		setStyleSheet("loginWidget{background:transparent;}");
	}
	else
	{
		QtWin::resetExtendedFrame(this);
		setAttribute(Qt::WA_TranslucentBackground, false);
		setStyleSheet(QString("loginWidget {background:%1}").arg(QtWin::realColorizationColor().name()));
	}
	*/
	
	ui.setupUi(this);
	QRegExp rx("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
	ui.ipEdit->setValidator(new QRegExpValidator(rx, this));
	ui.fileEdit->setReadOnly(true);

	connect(this, SIGNAL(logSuccess()), this, SLOT(accept()));
}

CLoginDlg::~CLoginDlg()
{

}

void CLoginDlg::on_openBtn_clicked()
{
	QString filepath = QFileDialog::getOpenFileName(this, "打开文件", "", "ASF文件(*.asf);;All files(*)");
	if (!filepath.isEmpty())
	{
		ui.fileEdit->setText(filepath);
		m_pMgr->m_FilePath = filepath;
	}
}

void CLoginDlg::on_cameraBtn_clicked()
{
	QString  camera("rtsp://admin:");
	camera += ui.pwEdit->text();
	camera += "@";
	camera += ui.ipEdit->text();
	if (m_pMgr->m_pFFmpeg->OpenVideoFile(camera) == DJJ_FAILURE)
	{
		QMessageBox::information(this, "提示！", "无法获取RTSP流,请检查相机是否在线或密码是否正确！");
	}
	else
	{
		m_pMgr->m_ip = ui.ipEdit->text();
		m_pMgr->m_password = ui.pwEdit->text();
		m_pMgr->m_FilePath = camera;
		m_pMgr->m_videoType = REMOTE_CAMERA;
		emit logSuccess();
	}
}

void CLoginDlg::on_videoBtn_clicked()
{
	if (m_pMgr->m_FilePath.isEmpty())
	{
		QMessageBox::information(this, "提示!", "请选择需要播放的文件！");
	}
	else
	{
		if (m_pMgr->m_pFFmpeg->OpenVideoFile(m_pMgr->m_FilePath) == DJJ_FAILURE)
		{
			QMessageBox::information(this, "提示!", "无法打开此文件！");
		}
		else
		{
			m_pMgr->m_videoType = LOCAL_FILE;
			emit logSuccess();
		}
	
	}
}

void CLoginDlg::initTitleBar()
{
	m_titleBar = new MyTitleBar(this);
	m_titleBar->move(0, 0);

	connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

	m_titleBar->setTitleIcon(":btnImg/Resources/user.png");
	m_titleBar->setTitleContent(QStringLiteral("登陆"));
	m_titleBar->setButtonType(MIN_BUTTON);
	m_titleBar->setTitleWidth(this->width());
}

void CLoginDlg::loadStyleSheet(const QString &sheetName)
{
	/*
	QFile file(":/Resources/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
	*/
}

void CLoginDlg::onButtonMinClicked()
{
	showMinimized();
}

void CLoginDlg::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize  windowSize;
	m_titleBar->getRestoreInfo(windowPos, windowSize);
	this->setGeometry(QRect(windowPos, windowSize));
}

void CLoginDlg::onButtonMaxClicked()
{
	m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
	QDesktopWidget *desk = QApplication::desktop();
	QRect desktopRect = desk->availableGeometry();
	QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(FactRect);
}

void CLoginDlg::onButtonCloseClicked()
{
	close();
}