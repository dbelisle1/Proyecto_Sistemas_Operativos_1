#include "mainwindow.h"
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QFont>
#include <QBrush>
#include <QPen>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    runButton(new QPushButton("Ejecutar", this)),
    log(new QPlainTextEdit(this)),
    series(new QHorizontalStackedBarSeries()),
    chartView(nullptr)
{
    setupChart();
    setupUI();
    connect(runButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
}

MainWindow::~MainWindow()
{
    // El destructor puede estar vacío, Qt se encarga de la memoria
}

void MainWindow::setupUI() {
    log->setReadOnly(true);
    auto* layout = new QVBoxLayout;
    layout->addWidget(runButton);
    layout->addWidget(log);
    layout->addWidget(chartView);

    auto* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    setWindowTitle("Proyecto SO - FCFS");
    resize(800, 600);
}

void MainWindow::setupChart() {
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Diagrama de Gantt - FCFS");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Mejorar apariencia general
    chart->setBackgroundBrush(QBrush(QColor(250, 250, 250)));
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));

    auto* axisY = new QCategoryAxis();
    axisY->setTitleText("Procesos");
    axisY->setLabelsFont(QFont("Arial", 10, QFont::Bold));
    axisY->setGridLineVisible(true);
    axisY->setGridLinePen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    auto* axisX = new QValueAxis();
    axisX->setTitleText("Tiempo (unidades)");
    axisX->setTitleFont(QFont("Arial", 10, QFont::Bold));
    axisX->setLabelsFont(QFont("Arial", 9));
    axisX->setGridLineVisible(true);
    axisX->setMinorGridLineVisible(true);
    axisX->setGridLinePen(QPen(QColor(180, 180, 180), 1));
    axisX->setMinorGridLinePen(QPen(QColor(220, 220, 220), 1, Qt::DotLine));
    axisX->setTickCount(11);
    axisX->setMinorTickCount(1);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Mejorar márgenes
    chart->setMargins(QMargins(10, 10, 10, 10));
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setFont(QFont("Arial", 9));

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumHeight(400);
}

void MainWindow::onStartClicked() {
    log->clear();
    series->clear();

    struct Process { QString pid; int start; int duration; };
    QVector<Process> processes = {
        {"PID1", 0, 5},
        {"PID2", 5, 3},
        {"PID3", 8, 7}
    };

    int maxTime = 0;
    for (auto& p : processes)
        maxTime = std::max(maxTime, p.start + p.duration);

    QStringList categories;
    for (auto& p : processes)
        categories << p.pid;

    // Set transparente para offset
    auto* offsetSet = new QBarSet("Inicio");
    offsetSet->setColor(Qt::transparent);
    offsetSet->setBorderColor(Qt::transparent);

    for (auto& p : processes) {
        offsetSet->append(p.start);
    }
    series->append(offsetSet);

    // Paleta de colores más moderna y distinguible
    QVector<QColor> colors = {
        QColor(52, 152, 219),   // Azul
        QColor(46, 204, 113),   // Verde
        QColor(155, 89, 182),   // Púrpura
        QColor(241, 196, 15),   // Amarillo
        QColor(231, 76, 60)     // Rojo
    };

    int idx = 0;
    for (auto& p : processes) {
        auto* set = new QBarSet(p.pid);

        // Color con borde más oscuro para definición
        QColor baseColor = colors[idx % colors.size()];
        set->setColor(baseColor);
        set->setBorderColor(baseColor.darker(120));
        set->setPen(QPen(baseColor.darker(120), 2));

        for (int i = 0; i < processes.size(); i++) {
            if (i == idx)
                set->append(p.duration);
            else
                set->append(0);
        }

        series->append(set);

        // Log mejorado con formato
        log->appendPlainText(QString("━━━━━━━━━━━━━━━━━━━━━━━━━━"));
        log->appendPlainText(QString("Proceso: %1").arg(p.pid));
        log->appendPlainText(QString("  • Inicio:    %2").arg(p.start));
        log->appendPlainText(QString("  • Duración:  %3").arg(p.duration));
        log->appendPlainText(QString("  • Fin:       %4").arg(p.start + p.duration));

        idx++;
    }

    log->appendPlainText(QString("━━━━━━━━━━━━━━━━━━━━━━━━━━"));
    log->appendPlainText(QString("\nTiempo total de ejecución: %1 unidades").arg(maxTime));

    // Configurar ejes
    auto axesY = series->attachedAxes();
    QCategoryAxis* axisY = nullptr;
    for (auto* axis : axesY) {
        if (axis->orientation() == Qt::Vertical) {
            axisY = qobject_cast<QCategoryAxis*>(axis);
            break;
        }
    }
    if (axisY) {
        // Remover el eje viejo y crear uno nuevo
        series->detachAxis(axisY);
        chartView->chart()->removeAxis(axisY);
        delete axisY;

        // Crear nuevo eje Y
        axisY = new QCategoryAxis();
        axisY->setTitleText("Procesos");
        axisY->setLabelsFont(QFont("Arial", 10, QFont::Bold));
        axisY->setGridLineVisible(true);
        axisY->setGridLinePen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));

        for (int i = 0; i < categories.size(); i++) {
            axisY->append(categories[i], i);
        }
        axisY->setRange(-0.5, categories.size() - 0.5);

        chartView->chart()->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    }

    auto axesX = series->attachedAxes();
    QValueAxis* axisX = nullptr;
    for (auto* axis : axesX) {
        if (axis->orientation() == Qt::Horizontal) {
            axisX = qobject_cast<QValueAxis*>(axis);
            break;
        }
    }
    if (axisX) {
        axisX->setRange(0, maxTime + 1);
        axisX->setTickCount(maxTime + 2);
    }
}
