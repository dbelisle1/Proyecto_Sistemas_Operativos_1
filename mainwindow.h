#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QHorizontalStackedBarSeries>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartClicked();

private:
    void setupUI();
    void setupChart();

    QPushButton* runButton;
    QPlainTextEdit* log;
    QHorizontalStackedBarSeries* series;
    QChartView* chartView;
};

#endif // MAINWINDOW_H
