#include "mytitlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>
#include "WindowCompositionAttribute.h"

#define		BUTTON_HEIGHT  30		//按钮高度
#define		BUTTON_WIDTH	30		//按钮宽度
#define		TITLE_HEIGHT	30		//标题栏高度;

MyTitleBar::MyTitleBar(QWidget *parent)
	: QWidget(parent)
	, m_colorR(203)
	, m_colorG(203)
	, m_colorB(203)
	, m_isPressed(false)
	, m_buttonType(MIN_MAX_BUTTON)
	, m_windowBorderWidth(0)
{
	//初始化;
	initControl();
	initConnections();
	//加载本地样式 MyTitle.css文件
	loadStyleSheet("Mytitle");

}

MyTitleBar::~MyTitleBar()
{

}

void MyTitleBar::initControl()
{
	m_pIcon = new QLabel;
	m_pTitleContent = new QLabel;

	m_pButtonMin = new QPushButton;
	m_pButtonRestore = new QPushButton;
	m_pButtonMax = new QPushButton;
	m_pButtonClose = new QPushButton;

	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	this->setObjectName("bar");
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	m_pButtonMin->setToolTip(QStringLiteral("最小化"));
	m_pButtonRestore->setToolTip(QStringLiteral("还原"));
	m_pButtonMax->setToolTip(QStringLiteral("最大化"));
	m_pButtonClose->setToolTip(QStringLiteral("关闭"));

	/*
	m_pButtonClose->setStyleSheet(QLatin1String("QPushButton#ButtonClose{\n"
		"border-image:url(:btnImg/Resources/MyTitle/close.png) 0 81 0 0; \n"
		"border-top-right-radius:3; \n"
		"}"
	"QPushButton#ButtonClose:hover\n"
	"{\n"
		"border-image:url(:btnImg/Resources/MyTitle/close.png) 0 54 0 27;\n"
		"border-top-right-radius:3;\n"
	"}\n"));

	
	m_pButtonMax->setStyleSheet(QLatin1String(
		"QPushButton#ButtonMax\n"
	"{\n"
		"border-image:url(:/Resources/MyTitle/max.png) 0 81 0 0;\n"
	"}\n"
	"QPushButton#ButtonMax:hover\n"
	"{\n"
		"border-image:url(:/Resources/MyTitle/max.png) 0 54 0 27;\n"
	"}\n"
	"QPushButton#ButtonMax:pressed\n"
	"{\n"
		"border-image:url(:/Resources/MyTitle/max.png) 0 27 0 54;\n"
	"}\n"));
	*/

	QHBoxLayout *mylayout = new QHBoxLayout(this);
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);

	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	mylayout->setContentsMargins(5, 0, 0, 0);
	mylayout->setSpacing(0);

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setFixedHeight(TITLE_HEIGHT);
	this->setWindowFlags(Qt::FramelessWindowHint);
}

//信号槽的绑定
void MyTitleBar::initConnections()
{
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

//设置标题栏背景色，在paintEvent事件中进行绘制标题栏背景色
//在构造函数中给了默认值，可以外部设置颜色值改变标题栏背景色
void MyTitleBar::setBackgroundColor(int r, int g, int b)
{
	m_colorB = r;
	m_colorG = g;
	m_colorB = b;

	//重新绘制(调用paintEvent事件)
	update();
}

//设置标题栏图标
void MyTitleBar::setTitleIcon(QString filePath, QSize IconSize)
{
	QPixmap titleIcon(filePath);
	m_pIcon->setPixmap(titleIcon.scaled(IconSize));
}

//设置标题内容
void MyTitleBar::setTitleContent(QString titleContent, int titleFontSize /* = 9 */)
{
	//设置标题字体大小
	QFont font = m_pTitleContent->font();
	font.setPointSize(titleFontSize);
	m_pTitleContent->setFont(font);
	//设置标题内容
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

//设置标题栏长度
void MyTitleBar::setTitleWidth(int width)
{
	this->setFixedWidth(width);
}

//设置标题栏上按钮类型
//由于不同窗口标题栏上的按钮都不一样，所以可以自定义标题栏中的按钮；
//这里提供了四个按钮，分别为最小化，还原，最大化，关闭，如果需要其他按钮可自行添加设置
void MyTitleBar::setButtonType(ButtonType buttonType)
{
	m_buttonType = buttonType;

	switch (buttonType)
	{
	case MIN_BUTTON:
	{
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(false);
	}
	break;
	case MIN_MAX_BUTTON:
	{
		m_pButtonRestore->setVisible(false);
	}
	break;
	case ONLY_CLOSE_BUTTON:
	{
		m_pButtonMin->setVisible(false);
		m_pButtonRestore->setVisible(false); 
		m_pButtonMax->setVisible(false);
	}
	break;
	default:
		break;

	}
}

//设置标题栏中的标题是否会自动滚动，跑马灯效果
//一般情况下标题栏中的标题内容是不滚动的
void MyTitleBar::setTitleRoll()
{
	connect(&m_titleRollTimer, SIGNAL(timeout()), this, SLOT(onRollTitle()));
	//定时器（每200ms触发一次 timeout信号）
	m_titleRollTimer.start(200);
}

//设置窗口边框宽度
void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
	m_windowBorderWidth = borderWidth;
}

//保存窗口最大化前窗口的位置及大小
void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
	m_restorePos = point;
	m_restoreSize = size;
}

//获取窗口最大化前窗口的位置及大小；
void MyTitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point = m_restorePos;
	size = m_restoreSize;
}


//绘制标题栏背景色;
void MyTitleBar::paintEvent(QPaintEvent *event)
{
	//如果绘制背景色的话，毛玻璃的效果会被覆盖掉；
	//设置背景色;
	QPainter painter(this);
	//线条填充类
	QPainterPath pathBack;
	//填充规则
	pathBack.setFillRule(Qt::WindingFill);
	pathBack.addRoundedRect(QRect(0, 0,this->width(),this->height()), 3, 3);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
	
	//当窗口最大化或者还原后，窗口长度变了，标题栏的长度应该一起改变
	//这里减去m_windowBorderWidth,是因为窗口可能设置了不同宽度的边框;
	//如果窗口有边框则需要设置m_windowBorderWidth值，否则m_windowBorderWidth默认为0;

	if (this->width() != (this->parentWidget()->width() - m_windowBorderWidth))
	{
		this->setFixedWidth(this->parentWidget()->width() - m_windowBorderWidth);
	}
	QWidget::paintEvent(event);
}


//双击响应事件,主要是实现双击标题栏进行最大化和最小化的操作
void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
	//只有存在最大化、还原按钮时双击才有效;
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		//通过最大化按钮的状态判断当前窗口是处于最大化还是原始大小
		//或者通过单独设置变量来表示当前窗口状态；
		if (m_pButtonMax->isVisible())
		{
			onButtonMaxClicked();
		}
		else
		{
			onButtonRestoreClicked();
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

//通过mousePressEvent, mouseMoveEvent, mouseReleaseEvent三个事件实现了鼠标拖动标题栏移动窗口效果
void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
	if (m_buttonType == MIN_MAX_BUTTON)
	{
		//当窗口最大化时禁止拖动窗口;
		if (m_pButtonMax->isVisible())
		{
			m_isPressed = true;
			m_startMovePos = event->globalPos();
		}
	}
	else
	{
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}

void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{
	if (m_isPressed)
	{
		QPoint movePoint = event->globalPos() - m_startMovePos;
		QPoint widgetPos = this->parentWidget()->pos();
		m_startMovePos = event->globalPos();
		this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
	}
	return QWidget::mouseMoveEvent(event);
}

void MyTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

//加载本地样式文件；可以将样式直接卸载文件中，程序运行时直接加载进来
void MyTitleBar::loadStyleSheet(const QString &sheetName)
{
	QFile file("./Resources/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		//QLatin1String 是对const char*的浅封装.
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
	}
}

//以下为按钮操作相应的槽
void MyTitleBar::onButtonMinClicked()
{
	emit signalButtonMinClicked();
}

void MyTitleBar::onButtonRestoreClicked()
{
	m_pButtonRestore->setVisible(false);
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}

void MyTitleBar::onButtonMaxClicked()
{
	m_pButtonMax->setVisible(false);
	m_pButtonRestore->setVisible(true);
	emit signalButtonMaxClicked();
}

void MyTitleBar::onButtonCloseClicked()
{
	emit signalButtonCloseClicked();
}

//标题栏中标题显示为滚动的效果;
void MyTitleBar::onRollTitle()
{
	static int nPos = 0;
	QString titleContent = m_titleContent;
	if (nPos > titleContent.length())
		nPos = 0;

	m_pTitleContent->setText(titleContent.mid(nPos));
	nPos++;
}