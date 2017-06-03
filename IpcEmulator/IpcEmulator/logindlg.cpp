#include "logindlg.h"
//#include "WindowCompositionAttribute.h"
#include <QtWinExtras/QtWin>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#pragma execution_character_set("utf-8") 

CLoginDlg::CLoginDlg(QWidget *parent)
	: QDialog(parent)
{
	/* �õ��� windowsApi
	//win10ë����Ч��; ��Ҫ���ë����Ч���ĵط� ��Ҫ����������Ϊ��ɫ;
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
	*/

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
	m_sFilepath = QFileDialog::getOpenFileName(this, "���ļ�", "", "ASF�ļ�(*.asf);;All files(*)");
	if (!m_sFilepath.isEmpty())
	{
		ui.fileEdit->setText(m_sFilepath);
	}
}

void CLoginDlg::on_cameraBtn_clicked()
{

}

void CLoginDlg::on_videoBtn_clicked()
{
	if (m_sFilepath.isEmpty())
	{
		QMessageBox::information(this, "���ܴ�!", "��ѡ����Ҫ���ŵ��ļ���");
	}
	else
	{
		emit logSuccess();
	}
}