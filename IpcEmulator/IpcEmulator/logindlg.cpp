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
	//FramelessWindowHint�������ö��ȥ���߿�
	//windowMinimizeButtonHint ���������ڴ�����С��ʱ��������������ڼ��ɻ�ԭ
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
	//���ô��ڱ���͸��
	//setAttribute(Qt::WA_TranslucentBackground);
	//�رմ���ʱ�ͷ���Դ;
	//�����ڶ������ڶ��ϵ�ʱ�����Ҫ��仰 �����巽ʽΪ��Mainwindow *w = new Mainwindow;
	//������巽ʽ�ǣ� Mainwindow w; w.show();���ǽ����ڶ�����ջ��;
	//setAttribute(Qt::WA_DeleteOnClose);
	//��ʼ��������,
	//�����Ի������ �ڴ�ռ��̫�󣡣�
	initTitleBar();

	
	// �õ��� windowsApi
	//win10ë����Ч��; ��Ҫ���ë����Ч���ĵط� ��Ҫ����������Ϊ��ɫ;	
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
	
	/*Qt�Դ��ӿ�ʵ�� ë��������win10��û��Ч��!
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
	QString filepath = QFileDialog::getOpenFileName(this, "���ļ�", "", "ASF�ļ�(*.asf);;All files(*)");
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
		QMessageBox::information(this, "��ʾ��", "�޷���ȡRTSP��,��������Ƿ����߻������Ƿ���ȷ��");
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
		QMessageBox::information(this, "��ʾ!", "��ѡ����Ҫ���ŵ��ļ���");
	}
	else
	{
		if (m_pMgr->m_pFFmpeg->OpenVideoFile(m_pMgr->m_FilePath) == DJJ_FAILURE)
		{
			QMessageBox::information(this, "��ʾ!", "�޷��򿪴��ļ���");
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
	m_titleBar->setTitleContent(QStringLiteral("��½"));
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