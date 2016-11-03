#include "ImageViewer.h"
#include <qevent.h>
#include <qpainter.h>
#include <set>
#include <stack>
#include "util.h"
#include "global_data_holder.h"

ImageViewer::ImageViewer(QWidget* parent)
:QWidget(parent)
{
	setMouseTracking(true);
	setBackgroundRole(QPalette::ColorRole::Dark);
	setAutoFillBackground(true);
}

ImageViewer::~ImageViewer()
{
}

void ImageViewer::setViewType(ViewType type)
{
	m_viewType = type;
	update();
}

ImageViewer::ViewType ImageViewer::getViewType()const
{
	return m_viewType;
}

void ImageViewer::setColorImage(const QImage& img)
{
	m_colorImage = img;
	updateViewRect();
	setViewType(ViewColorImage);
}

const QImage* ImageViewer::getColorImage()const
{
	return &m_colorImage;
}

static std::vector<QRgb> gen_color_table()
{
	std::vector<QRgb> table;
	table.push_back(qRgb(0, 0, 0));
	table.push_back(qRgb(200, 150, 100));
	table.push_back(qRgb(150, 200, 100));
	table.push_back(qRgb(100, 200, 150));
	table.push_back(qRgb(200, 100, 150));
	table.push_back(qRgb(100, 150, 200));
	table.push_back(qRgb(150, 100, 200));
	table.push_back(qRgb(160, 120, 80));
	table.push_back(qRgb(120, 160, 80));
	table.push_back(qRgb(80, 160, 120));
	table.push_back(qRgb(160, 80, 120));
	table.push_back(qRgb(80, 120, 160));
	table.push_back(qRgb(120, 80, 160));
	while (table.size() < 255)
		table.push_back(qRgb(rand()%255, rand()%255, rand()%255));
	return table;
}

float ImageViewer::getViewScale()const
{
	if (m_viewRect.isEmpty() || m_colorImage.width()==0)
		return 1.f;
	return (float)m_viewRect.width() / (float)m_colorImage.width();
}

void ImageViewer::updateViewRect(bool allowScale)
{
	// calc view rect
	float scale = std::max(float(m_colorImage.rect().width()) / float(rect().width()),
		float(m_colorImage.rect().height()) / float(rect().height()));

	float oldScale = getViewScale();
	m_viewRect = m_colorImage.rect();
	m_viewRect.setSize(m_viewRect.size()*oldScale);
	if (allowScale)
	{
		if (scale > 1)
			m_viewRect.setSize(m_colorImage.rect().size() / scale*0.9);
		else
			m_viewRect = m_colorImage.rect();
	}

	m_viewRect.moveCenter(rect().center());

	QPointF lt = m_viewRect.topLeft();
	lt.setX(std::max(0., lt.x()));
	lt.setY(std::max(0., lt.y()));
	m_viewRect.moveTopLeft(lt);

	update();
}

void ImageViewer::scaleViewRect(QPoint c, float s)
{
	float newScale = s * getViewScale();
	if (newScale > 64 || newScale < 1.f / 64)
		return;

	QPointF sz1 = m_viewRect.topLeft() - c;
	QPointF sz2 = m_viewRect.bottomRight() - c;
	m_viewRect.setTopLeft(sz1*s + c);
	m_viewRect.setBottomRight(sz2*s + c);
	update();
}

void ImageViewer::timerEvent(QTimerEvent* ev)
{
	update();
}

QPointF ImageViewer::image2view(QPointF imgPt)
{
	QPointF viewPt = imgPt * getViewScale() + QPointF(m_viewRect.x(), m_viewRect.y());
	return viewPt;
}

QPointF ImageViewer::view2image(QPointF viewPt)
{
	QPointF imgPt = (viewPt - QPointF(m_viewRect.x(), m_viewRect.y())) / getViewScale();
	return imgPt;
}

void ImageViewer::paintEvent(QPaintEvent* ev)
{
	QPainter painter(this);
	painter.fillRect(rect(), Qt::gray);

	switch (m_viewType)
	{
	case ImageViewer::ViewColorImage:
		painter.drawImage(m_viewRect, m_colorImage);
		break;
	default:
		break;
	}
}

void ImageViewer::resizeEvent(QResizeEvent* ev)
{
	updateViewRect(true);
}

void ImageViewer::mousePressEvent(QMouseEvent* ev)
{
	m_buttons = ev->buttons();

	if (m_buttons == Qt::MiddleButton)
	{
		updateViewRect(true);
	}

	if (m_buttons == Qt::LeftButton)
	{

	}
}

void ImageViewer::mouseReleaseEvent(QMouseEvent* ev)
{
	if (m_buttons == Qt::LeftButton)
	{

	}

	m_buttons = Qt::NoButton;
}

void ImageViewer::mouseMoveEvent(QMouseEvent* ev)
{
	m_mousePos = ev->pos();
	setFocus();

	// selection event
	QPointF imgPt = view2image(m_mousePos);

	update();
}

void ImageViewer::wheelEvent(QWheelEvent* ev)
{
	int delta = ev->delta();
	float s = 0.8;
	if (delta > 0)
		s = 1.2;

	scaleViewRect(m_mousePos, s);
}

void ImageViewer::keyPressEvent(QKeyEvent* ev)
{
	switch (ev->key())
	{
	default:
		break;
	case Qt::Key_Space:
		setViewType(ViewType(((int)getViewType()+1)%ViewTypeEnd));
		printf("view mode: %d\n", getViewType());
		break;
	}
}