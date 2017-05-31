#include "ipcemulator.h"
#include "common_def.h"

IpcEmulator::IpcEmulator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	IpcEmulatorInit();

	CVideoThread* test = new CVideoThread();
	test->SetWindow((HWND)this->ui.centralWidget->winId());
	test->start();
}

IpcEmulator::~IpcEmulator()
{

}

int IpcEmulator::IpcEmulatorInit()
{
	m_pLoginDlg = NULL;
	return DJJ_SUCCESS;
}

void IpcEmulator::SetLoginDlg(CLoginDlg* l)
{
	m_pLoginDlg = l;
}