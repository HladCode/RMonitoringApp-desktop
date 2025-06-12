#include "chartView.h"

ChartView::ChartView(QChart *chart, QLineSeries *series, QWidget *parent)
    : QChartView(chart, parent) {
    setDragMode(QGraphicsView::NoDrag);
    setMouseTracking(true);
    setInteractive(true);
    setCursor(Qt::OpenHandCursor);
    this->series = series;
}

void ChartView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_isPanning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event) {
    if (m_isPanning) {
        series->setPointLabelsVisible(false);
        QPointF delta = chart()->mapToValue(m_lastMousePos) - chart()->mapToValue(event->pos());

        auto* axisX = dynamic_cast<QDateTimeAxis*>(chart()->axisX());
        if (axisX) {
            qint64 min = axisX->min().toMSecsSinceEpoch();
            qint64 max = axisX->max().toMSecsSinceEpoch();
            qint64 diff = delta.x() * 10;  // TODO: зробити в налаштуваннях вибір параметру delta.x() * >dragSpeed<

            axisX->setMin(QDateTime::fromMSecsSinceEpoch(min + diff));
            axisX->setMax(QDateTime::fromMSecsSinceEpoch(max + diff));
        }

        auto* axisY = dynamic_cast<QValueAxis*>(chart()->axisY());
        if (axisY) {
            axisY->setMin(axisY->min() + delta.y());
            axisY->setMax(axisY->max() + delta.y());
        }

        m_lastMousePos = event->pos();
    }

    QChartView::mouseMoveEvent(event);
}

void ChartView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        series->setPointLabelsVisible(true);
        m_isPanning = false;
        setCursor(Qt::OpenHandCursor);
    }
    QChartView::mouseReleaseEvent(event);
}

void ChartView::wheelEvent(QWheelEvent *event) {
    if (event->angleDelta().y() > 0) {
        chart()->zoomIn();
    } else {
        chart()->zoomOut();
    }
    event->accept();
}
