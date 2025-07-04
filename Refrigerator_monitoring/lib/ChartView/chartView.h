#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts>

class ChartView : public QChartView {
public:
    ChartView(QChart* chart, QLineSeries *series, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
private:
    bool m_isPanning;
    QPoint m_lastMousePos;
    QLineSeries *series;
};
#endif // CHARTVIEW_H
