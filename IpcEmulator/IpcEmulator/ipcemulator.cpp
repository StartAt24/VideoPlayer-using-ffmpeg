#include "ipcemulator.h"
#include "common_def.h"
#include <QSlider>
#include <QGraphicsOpacityEffect>
#include <QFileDialog>
#include <QMessageBox>
#include <QResizeEvent>
#pragma execution_character_set("utf-8") 

extern HANDLE g_hRefreshEvent;

IpcEmulator::IpcEmulator(CMgr *mgr,QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_pMgr = mgr;
	IpcEmulatorInit();
	//CVideoThread* test = new CVideoThread(mgr);
	//test->SetWindow((HWND)this->ui.centralWidget->winId());
	//test->start();
}

IpcEmulator::~IpcEmulator()
{
	delete m_pVideoThread;
}
 
int IpcEmulator::IpcEmulatorInit()
{
	m_pLoginDlg = NULL;
	m_pVideoThread = new CVideoThread(m_pMgr);
	m_pVideoThread->SetWindow((HWND) this->ui.VideoWidget->winId());
	m_pTimer = new QTimer(this);

	ui.progressBar->setVisible(false);
	ui.playTime->setVisible(false);
	connect(ui.progressBar, SIGNAL(valueChanged(int)), this, SLOT(setTimeLabel(int)));
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(clearTimer()));
	return DJJ_SUCCESS;
}

void IpcEmulator::SetLoginDlg(CLoginDlg* loginDlg)
{
	m_pLoginDlg = loginDlg;
}

void IpcEmulator::setTimeLabel(int currentNum)
{
	ui.playTime->setText(QString(currentNum));
}

void IpcEmulator::on_playBtn_clicked()
{
	if (m_pMgr->m_videoType == LOCAL_FILE)
	{
		ui.progressBar->setVisible(true);
		ui.playTime->setVisible(true);
		ui.recordBtn->setVisible(false);
	}
	else
	{
		ui.progressBar->setVisible(false);
		ui.playTime->setVisible(false);
		ui.recordBtn->setVisible(true);
	}
	m_pVideoThread->start();
}


void IpcEmulator::on_progressBar_valueChanged(int value)
{
	ui.playTime->setText(QString(value));
	printf("sliderbar value changed!\n");
}

void IpcEmulator::on_recordBtn_clicked()
{
	if (m_pMgr->m_bIsRecord == false)
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Record Video"), "", tr("Video Files (*.flv);;(*.mp4)"));
		if (!fileName.isNull())
		{
			m_pMgr->m_FileSavePath = fileName;
			m_pMgr->m_pFFmpeg->SavePre(fileName);
			m_pMgr->m_bIsRecord = true;
			ui.stateLabel->setText(tr("录像中...."));
			ui.recordBtn->setText(tr("停止录制"));
		}
		else
		{
			QMessageBox::information(this, tr("提示！"), tr("请选择保存文件！"));
		}
	}
	else
	{
		m_pMgr->m_bIsRecord = false;
		m_pMgr->m_pFFmpeg->SaveEnd();
		ui.recordBtn->setText(tr("开始录像"));
		ui.stateLabel->setText("录像完成！");
		m_pTimer->start(2000);
	}
}

void IpcEmulator::resizeEvent(QResizeEvent *event)
{
	m_pMgr->m_pSDLPlay->m_bResize = true;	
	WaitForSingleObject(g_hRefreshEvent, INFINITE);
	QWidget::resizeEvent(event);
	m_pMgr->m_pSDLPlay->m_bRefresh = false;
}

void IpcEmulator::on_backBtn_clicked()
{
	m_pMgr->m_pGive->terminate();
	m_pMgr->m_pGive->wait();
	m_pMgr->m_pRcv->terminate();
	m_pMgr->m_pRcv->wait();
	m_pVideoThread->terminate();
	m_pVideoThread->wait();

	m_pMgr->m_pSDLPlay->SDLClose();

	m_pMgr->m_pFFmpeg->ffmpegClose();
}

void IpcEmulator::clearTimer()
{
	ui.stateLabel->setText("");
	if (m_pTimer->isActive())
		m_pTimer->stop();
}