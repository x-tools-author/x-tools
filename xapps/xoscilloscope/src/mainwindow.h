/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "helpwindow.h"

#include <qcustomplot/qcustomplot.h>

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

#define START_MSG '$'
#define END_MSG ';'

#define WAIT_START 1
#define IN_MESSAGE 2
#define UNDEFINED 3

#define CUSTOM_LINE_COLORS 14
#define GCP_CUSTOM_LINE_COLORS 4

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_comboPort_currentTextChanged(const QString &arg1); // Slot displays message on status bar
    void portOpenedSuccess();                                  // Called when port opens OK
    void portOpenedFail();                                     // Called when port fails to open
    void onPortClosed();                                       // Called when closing the port
    void replot();                                             // Slot for repainting the plot
    void onNewDataArrived(QStringList newData);                // Slot for new data from serial port
    void saveStream(QStringList newData);       // Save the received data to the opened file
    void on_spinAxesMin_valueChanged(int arg1); // Changing lower limit for the plot
    void on_spinAxesMax_valueChanged(int arg1); // Changing upper limit for the plot
    void readData();                            // Slot for inside serial port
    //void on_comboAxes_currentIndexChanged(int index);                                     // Display number of axes and colors in status bar
    void on_spinYStep_valueChanged(int arg1); // Spin box for changing Y axis tick step
    void on_savePNGButton_clicked();          // Button for saving JPG
    void onMouseMoveInPlot(
        QMouseEvent *event); // Displays coordinates of mouse pointer when clicked in plot in status bar
    void on_spinPoints_valueChanged(
        int arg1); // Spin box controls how many data points are collected and displayed
    void on_plot_mouseWheel(QWheelEvent *event); // Makes wheel mouse works while plotting

    /* Used when a channel is selected (plot or legend) */
    void channel_selection(void);
    void legend_double_click(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);

    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionHow_to_use_triggered();
    void on_actionPause_Plot_triggered();
    void on_actionClear_triggered();
    void on_actionRecord_stream_triggered();

    void on_pushButton_TextEditHide_clicked();

    void on_pushButton_ShowallData_clicked();

    void on_pushButton_AutoScale_clicked();

    void on_pushButton_ResetVisible_clicked();

    void on_listWidget_Channels_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_pushButton_TestData_clicked(bool checked);

signals:
    void portOpenFail();            // Emitted when cannot open port
    void portOpenOK();              // Emitted when port is open
    void portClosed();              // Emitted when port is closed
    void newData(QStringList data); // Emitted when new data has arrived

private:
    Ui::MainWindow *ui;

    qreal angle = 0;

    /* Line colors */
    QColor line_colors[CUSTOM_LINE_COLORS];
    QColor gui_colors[GCP_CUSTOM_LINE_COLORS];

    /* Main info */
    bool connected;      // Status connection variable
    bool plotting;       // Status plotting variable
    int dataPointNumber; // Keep track of data points
    /* Channels of data (number of graphs) */
    int channels;

    /* Data format */
    int data_format;

    /* Textbox Related */
    bool filterDisplayedData = true;

    /* Listview Related */
    QStringListModel *channelListModel;
    QStringList channelStrList;

    //-- CSV file to save data
    QFile *m_csvFile = nullptr;
    void openCsvFile(void);
    void closeCsvFile(void);

    QTimer updateTimer;        // Timer used for replotting the plot
    QTime timeOfFirstData;     // Record the time of the first data point
    double timeBetweenSamples; // Store time between samples
    QSerialPort *serialPort;   // Serial port; runs in this thread
    QString receivedData;      // Used for reading from the port
    int STATE;                 // State of recieiving message from port
    int NUMBER_OF_POINTS;      // Number of points plotted
    HelpWindow *helpWindow;

    void createUI();                       // Populate the controls
    void enable_com_controls(bool enable); // Enable/disable controls
    void setupPlot();                      // Setup the QCustomPlot
                                           // Open the inside serial port with these parameters
    void openPort(QSerialPortInfo portInfo,
                  int baudRate,
                  QSerialPort::DataBits dataBits,
                  QSerialPort::Parity parity,
                  QSerialPort::StopBits stopBits);
};
