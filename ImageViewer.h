#pragma once

#include <QWidget>
class ImageViewer : public QWidget
{
public:
	enum ViewType
	{
		ViewColorImage = 0,
		ViewTypeEnd,
	};
public:
	ImageViewer(QWidget *parent);
	~ImageViewer();

public:
	void setViewType(ViewType type);
	ViewType getViewType()const;

	void setColorImage(const QImage& img);
	const QImage* getColorImage()const;

	float getViewScale()const;
	QPointF image2view(QPointF imgPt);
	QPointF view2image(QPointF viewPt);
protected:
	void paintEvent(QPaintEvent* ev);

	void resizeEvent(QResizeEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
	void mouseReleaseEvent(QMouseEvent* ev);
	void mouseMoveEvent(QMouseEvent* ev);
	void wheelEvent(QWheelEvent* ev);
	void keyPressEvent(QKeyEvent* ev);

	void scaleViewRect(QPoint c, float s);
	void updateViewRect(bool allowScale = true);

	void timerEvent(QTimerEvent* ev);
private:
	void drawStrokes(QPainter& painter);
	void drawMaskAsPath(QPainter& painter);
private:
	QImage m_colorImage;

	QRectF m_viewRect;
	QPoint m_mousePos;
	Qt::MouseButtons m_buttons;
	ViewType m_viewType;
};