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
	ui.TotalTime->setVisible(false);
	ui.CurrentTime->setVisible(false);
	connect(ui.progressBar, SIGNAL(valueChanged(int)), this, SLOT(setTimeLabel(int)));
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(clearTimer()));
	m_bSliderPressed = false;
	return DJJ_SUCCESS;
}

void IpcEmulator::SetLoginDlg(CLoginDlg* loginDlg)
{
	m_pLoginDlg = loginDlg;
}

void IpcEmulator::setTimeLabel(int currentNum)
{
	ui.TotalTime->setText(QString(currentNum));
}

void IpcEmulator::on_playBtn_clicked()
{
	if (m_pMgr->m_videoType == LOCAL_FILE)
	{
		ui.CurrentTime->setVisible(true);
		ui.progressBar->setVisible(true);
		ui.TotalTime->setVisible(true);
		ui.recordBtn->setVisible(false);
		//���ý����������ֵ, ��СֵĬ��Ϊ0;
		ui.progressBar->setMaximum(m_pMgr->m_pFFmpeg->m_duration / AV_TIME_BASE);
		//������ʱ��
		int hours, mins, secs, us;
		secs = m_pMgr->m_pFFmpeg->m_duration / AV_TIME_BASE;
		mins = secs / 60;
		secs %= 60;
		hours = mins / 60;
		mins %= 60;
		char time[12];
		sprintf(time, "%02d:%02d:%02d", hours, mins, secs);
		ui.TotalTime->setText(QString(time));
	}
	else
	{
		ui.progressBar->setVisible(false);
		ui.TotalTime->setVisible(false);
		ui.recordBtn->setVisible(true);
	}
	m_pVideoThread->start();
}

/*
void IpcEmulator::on_progressBar_sliderReleased()
{

	//ֹͣ�����߳�; ������terminate()����ֹͣ��Ȼ����start()�������quit()��������start();
	m_pMgr->m_pGive->stop();
	m_pMgr->m_pGive->terminate();
//	m_pMgr->m_pGive->quit();
	m_pMgr->m_pGive->wait();

	m_pMgr->m_pRcv->stop();
	m_pMgr->m_pRcv->terminate();
//	m_pMgr->m_pRcv->quit();
	m_pMgr->m_pRcv->wait();

	//��ն���
	m_pMgr->m_pQue->PktQueClear();
	
	//�����϶���λ���������ò���λ��
	int targetTime = ui.progressBar->value();
	m_pMgr->m_pFFmpeg->Seek(targetTime);

	//���²��ţ�
	m_pVideoThread->start();
	emit notifyRefresh(true);

	m_bSliderPressed = false;
}
*/
void IpcEmulator::on_progressBar_sliderReleased()
{
	//����ͣ�����߳�;
	m_pMgr->m_pGive->SetPause(true);
	m_pMgr->m_pRcv->SetPause(true);
	//����Ҫȷ�������߳���ͣ��
	while (!m_pMgr->m_pGive->GetMakeSure() || !m_pMgr->m_pGive->GetMakeSure())
	{
		Sleep(20);
	}


	//��ն���
	m_pMgr->m_pQue->PktQueClear();

	//�����϶�λ���������ò���λ��
	int targetTime = ui.progressBar->value();
	m_pMgr->m_pFFmpeg->Seek(targetTime);

	//���²���;
	m_pMgr->m_pGive->SetPause(false);
	m_pMgr->m_pRcv->SetPause(false);

	m_pMgr->m_pGive->SetMakeSure(false);
	m_pMgr->m_pRcv->SetMakeSure(false);

	emit notifyRefresh(true);

	m_bSliderPressed = false;
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
			ui.stateLabel->setText(tr("¼����...."));
			ui.recordBtn->setText(tr("ֹͣ¼��"));
		}
		else
		{
			QMessageBox::information(this, tr("��ʾ��"), tr("��ѡ�񱣴��ļ���"));
		}
	}
	else
	{
		m_pMgr->m_bIsRecord = false;
		m_pMgr->m_pFFmpeg->SaveEnd();
		ui.recordBtn->setText(tr("��ʼ¼��"));
		ui.stateLabel->setText("¼����ɣ�");
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
	m_pMgr->m_pGive->stop();
	m_pMgr->m_pGive->terminate();
	m_pMgr->m_pGive->wait();

	m_pMgr->m_pRcv->stop();
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


void IpcEmulator::receive_Sec(int sec)
{
	if (!m_bSliderPressed)
	{
		int hours, mins;
		ui.progressBar->setValue(sec);
		mins = sec / 60;
		sec %= 60;
		hours = mins / 60;
		mins %= 60;
		char time[12];
		//��ֹʱ�����
		sprintf(time, "%02d:%02d:%02d", hours>99?99:hours, mins, sec);
		ui.CurrentTime->setText(QString(time));
	}
}

void IpcEmulator::on_progressBar_sliderPressed()
{
	emit notifyRefresh(false);
	m_bSliderPressed = true;
}