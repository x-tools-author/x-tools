/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWheelEvent>

/**
 * @brief Constructor
 * @param parent
 */
MainWindow::MainWindow (QWidget *parent) :
  QMainWindow (parent),
  ui (new Ui::MainWindow),

  /* Populate colors */
  line_colors{
      /* For channel data (gruvbox palette) */
      /* Light */
      QColor ("#fb4934"),
      QColor ("#b8bb26"),
      QColor ("#fabd2f"),
      QColor ("#83a598"),
      QColor ("#d3869b"),
      QColor ("#8ec07c"),
      QColor ("#fe8019"),
      /* Light */
      QColor ("#cc241d"),
      QColor ("#98971a"),
      QColor ("#d79921"),
      QColor ("#458588"),
      QColor ("#b16286"),
      QColor ("#689d6a"),
      QColor ("#d65d0e"),
   },
  gui_colors {
      /* Monochromatic for axes and ui */
      QColor (48,  47,  47,  255), /**<  0: qdark ui dark/background color */
      QColor (80,  80,  80,  255), /**<  1: qdark ui medium/grid color */
      QColor (170, 170, 170, 255), /**<  2: qdark ui light/text color */
      QColor (48,  47,  47,  200)  /**<  3: qdark ui dark/background color w/transparency */
    },

  /* Main vars */
  connected (false),
  plotting (false),
  dataPointNumber (0),
  channels(0),
  serialPort (nullptr),
  STATE (WAIT_START),
  NUMBER_OF_POINTS (500)
{
    ui->setupUi(this);

    /* Init UI and populate UI controls */
    createUI();

    /* Setup plot area and connect controls slots */
    setupPlot();

    /* Slot for printing coordinates */
    connect(ui->plot,
            SIGNAL(mouseMove(QMouseEvent *)),
            this,
            SLOT(onMouseMoveInPlot(QMouseEvent *)));

    /* Channel selection */
    connect(ui->plot, SIGNAL(selectionChangedByUser()), this, SLOT(channel_selection()));
    connect(ui->plot,
            SIGNAL(legendDoubleClick(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)),
            this,
            SLOT(legend_double_click(QCPLegend *, QCPAbstractLegendItem *, QMouseEvent *)));

    /* Connect update timer to replot slot */
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(replot()));

    ui->pushButton_TestData->setCheckable(true);
    ui->textEdit_UartWindow->document()->setMaximumBlockCount(1024);

    m_csvFile = nullptr;
}

/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
    closeCsvFile();

    if (serialPort != nullptr) {
        delete serialPort;
    }
    delete ui;
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Create remaining elements and populate the controls
 */
void MainWindow::createUI()
{
    /* Check if there are any ports at all; if not, disable controls and return */
    if (QSerialPortInfo::availablePorts().size() == 0) {
        enable_com_controls(false);
        ui->statusBar->showMessage("No ports detected.");
        ui->savePNGButton->setEnabled(false);
        return;
    }

    /* List all available serial ports and populate ports combo box */
    for (QSerialPortInfo port : QSerialPortInfo::availablePorts()) {
        ui->comboPort->addItem(port.portName());
    }

    /* Populate baud rate combo box with standard rates */
    ui->comboBaud->addItem("1200");
    ui->comboBaud->addItem("2400");
    ui->comboBaud->addItem("4800");
    ui->comboBaud->addItem("9600");
    ui->comboBaud->addItem("19200");
    ui->comboBaud->addItem("38400");
    ui->comboBaud->addItem("57600");
    ui->comboBaud->addItem("115200");
    /* And some not-so-standard */
    ui->comboBaud->addItem("128000");
    ui->comboBaud->addItem("153600");
    ui->comboBaud->addItem("230400");
    ui->comboBaud->addItem("256000");
    ui->comboBaud->addItem("460800");
    ui->comboBaud->addItem("921600");

    /* Select 115200 bits by default */
    ui->comboBaud->setCurrentIndex(7);

    /* Populate data bits combo box */
    ui->comboData->addItem("8 bits");
    ui->comboData->addItem("7 bits");

    /* Populate parity combo box */
    ui->comboParity->addItem("none");
    ui->comboParity->addItem("odd");
    ui->comboParity->addItem("even");

    /* Populate stop bits combo box */
    ui->comboStop->addItem("1 bit");
    ui->comboStop->addItem("2 bits");

    /* Initialize the listwidget */
    ui->listWidget_Channels->clear();
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Setup the plot area
 */
void MainWindow::setupPlot()
{
    /* Remove everything from the plot */
    ui->plot->clearItems();

    /* Background for the plot area */
    ui->plot->setBackground(gui_colors[0]);

    /* Used for higher performance (see QCustomPlot real time example) */
    ui->plot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    ui->plot->legend->setFont(font);

    /** See QCustomPlot examples / styled demo **/
    /* X Axis: Style */
    ui->plot->xAxis->grid()->setPen(QPen(gui_colors[2], 1, Qt::DotLine));
    ui->plot->xAxis->grid()->setSubGridPen(QPen(gui_colors[1], 1, Qt::DotLine));
    ui->plot->xAxis->grid()->setSubGridVisible(true);
    ui->plot->xAxis->setBasePen(QPen(gui_colors[2]));
    ui->plot->xAxis->setTickPen(QPen(gui_colors[2]));
    ui->plot->xAxis->setSubTickPen(QPen(gui_colors[2]));
    ui->plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->plot->xAxis->setTickLabelColor(gui_colors[2]);
    ui->plot->xAxis->setTickLabelFont(font);
    /* Range */
    ui->plot->xAxis->setRange(dataPointNumber - ui->spinPoints->value(), dataPointNumber);

    /* Y Axis */
    ui->plot->yAxis->grid()->setPen(QPen(gui_colors[2], 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setSubGridPen(QPen(gui_colors[1], 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setSubGridVisible(true);
    ui->plot->yAxis->setBasePen(QPen(gui_colors[2]));
    ui->plot->yAxis->setTickPen(QPen(gui_colors[2]));
    ui->plot->yAxis->setSubTickPen(QPen(gui_colors[2]));
    ui->plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->plot->yAxis->setTickLabelColor(gui_colors[2]);
    ui->plot->yAxis->setTickLabelFont(font);
    /* Range */
    ui->plot->yAxis->setRange(ui->spinAxesMin->value(), ui->spinAxesMax->value());
    /* User can change Y axis tick step with a spin box */
    //ui->plot->yAxis->setAutoTickStep(false);
    //ui->plot->yAxis->(ui->spinYStep->value());

    /* User interactions Drag and Zoom are allowed only on X axis, Y is fixed manually by UI control */
    ui->plot->setInteraction(QCP::iRangeDrag, true);
    //ui->plot->setInteraction(QCP::iRangeZoom, true);
    ui->plot->setInteraction(QCP::iSelectPlottables, true);
    ui->plot->setInteraction(QCP::iSelectLegend, true);
    ui->plot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->plot->axisRect()->setRangeZoom(Qt::Horizontal);

    /* Legend */
    QFont legendFont;
    legendFont.setPointSize(9);
    ui->plot->legend->setVisible(true);
    ui->plot->legend->setFont(legendFont);
    ui->plot->legend->setBrush(gui_colors[3]);
    ui->plot->legend->setBorderPen(gui_colors[2]);
    /* By default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement */
    ui->plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Enable/disable COM controls
 * @param enable true enable, false disable
 */
void MainWindow::enable_com_controls(bool enable)
{
    /* Com port properties */
    ui->comboBaud->setEnabled(enable);
    ui->comboData->setEnabled(enable);
    ui->comboParity->setEnabled(enable);
    ui->comboPort->setEnabled(enable);
    ui->comboStop->setEnabled(enable);

    /* Toolbar elements */
    ui->actionConnect->setEnabled(enable);
    ui->actionPause_Plot->setEnabled(!enable);
    ui->actionDisconnect->setEnabled(!enable);
    ui->pushButton_TestData->setEnabled(enable);
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Open the inside serial port; connect its signals
 * @param portInfo
 * @param baudRate
 * @param dataBits
 * @param parity
 * @param stopBits
 */
void MainWindow::openPort(QSerialPortInfo portInfo,
                          int baudRate,
                          QSerialPort::DataBits dataBits,
                          QSerialPort::Parity parity,
                          QSerialPort::StopBits stopBits)
{
    serialPort = new QSerialPort(portInfo, nullptr); // Create a new serial port

    connect(this,
            SIGNAL(portOpenOK()),
            this,
            SLOT(portOpenedSuccess())); // Connect port signals to GUI slots
    connect(this, SIGNAL(portOpenFail()), this, SLOT(portOpenedFail()));
    connect(this, SIGNAL(portClosed()), this, SLOT(onPortClosed()));
    connect(this, SIGNAL(newData(QStringList)), this, SLOT(onNewDataArrived(QStringList)));
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));

    connect(this, SIGNAL(newData(QStringList)), this, SLOT(saveStream(QStringList)));

    if (serialPort->open(QIODevice::ReadWrite)) {
        serialPort->setBaudRate(baudRate);
        serialPort->setParity(parity);
        serialPort->setDataBits(dataBits);
        serialPort->setStopBits(stopBits);
        emit portOpenOK();
    } else {
        emit portOpenedFail();
        qDebug() << serialPort->errorString();
    }
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Slot for closing the port
 */
void MainWindow::onPortClosed()
{
    //qDebug() << "Port closed signal received!";
    updateTimer.stop();
    connected = false;
    plotting = false;

    //--
    closeCsvFile();

    disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));
    disconnect(this,
               SIGNAL(portOpenOK()),
               this,
               SLOT(portOpenedSuccess())); // Disconnect port signals to GUI slots
    disconnect(this, SIGNAL(portOpenFail()), this, SLOT(portOpenedFail()));
    disconnect(this, SIGNAL(portClosed()), this, SLOT(onPortClosed()));
    disconnect(this, SIGNAL(newData(QStringList)), this, SLOT(onNewDataArrived(QStringList)));

    disconnect(this, SIGNAL(newData(QStringList)), this, SLOT(saveStream(QStringList)));
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Port Combo Box index changed slot; displays info for selected port when combo box is changed
 * @param arg1
 */
void MainWindow::on_comboPort_currentTextChanged(const QString &arg1)
{
    QSerialPortInfo selectedPort(arg1); // Dislplay info for selected port
    ui->statusBar->showMessage(selectedPort.description());
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Slot for port opened successfully
 */
void MainWindow::portOpenedSuccess()
{
    //qDebug() << "Port opened signal received!";
    setupPlot(); // Create the QCustomPlot area
    ui->statusBar->showMessage("Connected!");
    enable_com_controls(false); // Disable controls if port is open

    if (ui->actionRecord_stream->isChecked()) {
        //--> Create new CSV file with current date/timestamp
        openCsvFile();
    }
    /* Lock the save option while recording */
    ui->actionRecord_stream->setEnabled(false);

    updateTimer.start(20); // Slot is refreshed 20 times per second
    connected = true;      // Set flags
    plotting = true;
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Slot for fail to open the port
 */
void MainWindow::portOpenedFail()
{
    //qDebug() << "Port cannot be open signal received!";
    ui->statusBar->showMessage("Cannot open port!");
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Replot
 */
void MainWindow::replot()
{
    ui->plot->xAxis->setRange(dataPointNumber - ui->spinPoints->value(), dataPointNumber);
    ui->plot->replot();
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Slot for new data from serial port . Data is comming in QStringList and needs to be parsed
 * @param newData
 */
void MainWindow::onNewDataArrived(QStringList newData)
{
    static int data_members = 0;
    static int channel = 0;
    static int i = 0;
    volatile bool you_shall_NOT_PASS = false;

    /* When a fast baud rate is set (921kbps was the first to starts to bug),
       this method is called multiple times (2x in the 921k tests), so a flag
       is used to throttle
       TO-DO: Separate processes, buffer data (1) and process data (2) */
    while (you_shall_NOT_PASS) {
    }
    you_shall_NOT_PASS = true;

    if (plotting) {
        /* Get size of received list */
        data_members = newData.size();

        /* Parse data */
        for (i = 0; i < data_members; i++) {
            /* Update number of axes if needed */
            while (ui->plot->plottableCount() <= channel) {
                /* Add new channel data */
                ui->plot->addGraph();
                ui->plot->graph()->setPen(QPen(line_colors[channels % CUSTOM_LINE_COLORS], 2));
                ui->plot->graph()->setName(QString("Channel %1").arg(channels));
                if (ui->plot->legend->item(channels)) {
                    auto color = line_colors[channels % CUSTOM_LINE_COLORS];
                    ui->plot->legend->item(channels)->setTextColor(color);
                }
                ui->listWidget_Channels->addItem(ui->plot->graph()->name());
                auto color = line_colors[channels % CUSTOM_LINE_COLORS];
                ui->listWidget_Channels->item(channel)->setForeground(QBrush(color));
                channels++;
            }

            /* [TODO] Method selection and plotting */
            /* X-Y */
            if (0) {
            }
            /* Rolling (v1.0.0 compatible) */
            else {
                /* Add data to Graph 0 */
                ui->plot->graph(channel)->addData(dataPointNumber, newData[channel].toDouble());
                /* Increment data number and channel */
                channel++;
            }
        }

        /* Post-parsing */
        /* X-Y */
        if (0) {
        }
        /* Rolling (v1.0.0 compatible) */
        else {
            dataPointNumber++;
            channel = 0;
        }
    }
    you_shall_NOT_PASS = false;
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Slot for spin box for plot minimum value on y axis
 * @param arg1
 */
void MainWindow::on_spinAxesMin_valueChanged(int arg1)
{
    ui->plot->yAxis->setRangeLower(arg1);
    ui->plot->replot();
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Slot for spin box for plot maximum value on y axis
 * @param arg1
 */
void MainWindow::on_spinAxesMax_valueChanged(int arg1)
{
    ui->plot->yAxis->setRangeUpper(arg1);
    ui->plot->replot();
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Read data for inside serial port
 */
void MainWindow::readData()
{
    if (serialPort->bytesAvailable()) {          // If any bytes are available
        QByteArray data = serialPort->readAll(); // Read all data in QByteArray

        if (!data.isEmpty()) {        // If the byte array is not empty
            char *temp = data.data(); // Get a '\0'-terminated char* to the data

            if (!filterDisplayedData) {
                ui->textEdit_UartWindow->append(data);
            }
            for (int i = 0; temp[i] != '\0'; i++) { // Iterate over the char*
                switch (STATE) {                    // Switch the current state of the message
                case WAIT_START:                    // If waiting for start [$], examine each char
                    if (temp[i] == START_MSG) {     // If the char is $, change STATE to IN_MESSAGE
                        STATE = IN_MESSAGE;
                        receivedData.clear(); // Clear temporary QString that holds the message
                        break;                // Break out of the switch
                    }
                    break;
                case IN_MESSAGE:              // If state is IN_MESSAGE
                    if (temp[i] == END_MSG) { // If char examined is ;, switch state to END_MSG
                        STATE = WAIT_START;
                        QStringList incomingData = receivedData.split(
                            ' '); // Split string received from port and put it into list
                        if (filterDisplayedData) {
                            ui->textEdit_UartWindow->append(receivedData);
                        }
                        emit newData(incomingData); // Emit signal for data received with the list
                        break;
                    } else if (isdigit(temp[i]) || isspace(temp[i]) || temp[i] == '-'
                               || temp[i] == '.') {
                        /* If examined char is a digit, and not '$' or ';', append it to temporary string */
                        receivedData.append(temp[i]);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Number of axes combo; when changed, display axes colors in status bar
 * @param index
 */
//void MainWindow::on_comboAxes_currentIndexChanged(int index)
//{
//    if(index == 0) {
//      ui->statusBar->showMessage("Axis 1: Red");
//    } else if(index == 1) {
//        ui->statusBar->showMessage("Axis 1: Red; Axis 2: Yellow");
//    } else {
//        ui->statusBar->showMessage("Axis 1: Red; Axis 2: Yellow; Axis 3: Green");
//    }
//}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Spin box for changing the Y Tick step
 * @param arg1
 */
void MainWindow::on_spinYStep_valueChanged(int arg1)
{
    ui->plot->yAxis->ticker()->setTickCount(arg1);
    ui->plot->replot();
    ui->spinYStep->setValue(ui->plot->yAxis->ticker()->tickCount());
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Save a PNG image of the plot to current EXE directory
 */
void MainWindow::on_savePNGButton_clicked()
{
    ui->plot->savePng(QString::number(dataPointNumber) + ".png", 1920, 1080, 2, 50);
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Prints coordinates of mouse pointer in status bar on mouse release
 * @param event
 */
void MainWindow::onMouseMoveInPlot(QMouseEvent *event)
{
    int xx = int(ui->plot->xAxis->pixelToCoord(event->position().x()));
    int yy = int(ui->plot->yAxis->pixelToCoord(event->position().y()));
    QString coordinates("X: %1 Y: %2");
    coordinates = coordinates.arg(xx).arg(yy);
    ui->statusBar->showMessage(coordinates);
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Send plot wheelmouse to spinbox
 * @param event
 */
void MainWindow::on_plot_mouseWheel(QWheelEvent *event)
{
    QWheelEvent inverted_event = QWheelEvent(event->position(),
                                             event->globalPosition(),
                                             -event->pixelDelta(),
                                             -event->angleDelta(),
                                             event->buttons(),
                                             event->modifiers(),
                                             event->phase(),
                                             event->inverted());
    QApplication::sendEvent(ui->spinPoints, &inverted_event);
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Select both line and legend (channel)
 * @param plottable
 * @param event
 */
void MainWindow::channel_selection(void)
{
    /* synchronize selection of graphs with selection of corresponding legend items */
    for (int i = 0; i < ui->plot->graphCount(); i++) {
        QCPGraph *graph = ui->plot->graph(i);
        QCPPlottableLegendItem *item = ui->plot->legend->itemWithPlottable(graph);
        if (item->selected()) {
            item->setSelected(true);
            //          graph->set (true);
        } else {
            item->setSelected(false);
            //        graph->setSelected (false);
        }
    }
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Rename a graph by double clicking on its legend item
 * @param legend
 * @param item
 */
void MainWindow::legend_double_click(QCPLegend *legend,
                                     QCPAbstractLegendItem *item,
                                     QMouseEvent *event)
{
    Q_UNUSED(legend)
    Q_UNUSED(event)
    /* Only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0) */
    if (item) {
        QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem *>(item);
        bool ok;
        QString newName = QInputDialog::getText(this,
                                                "Change channel name",
                                                "New name:",
                                                QLineEdit::Normal,
                                                plItem->plottable()->name(),
                                                &ok,
                                                Qt::Popup);
        if (ok) {
            plItem->plottable()->setName(newName);
            for (int i = 0; i < ui->plot->graphCount(); i++) {
                ui->listWidget_Channels->item(i)->setText(ui->plot->graph(i)->name());
            }
            ui->plot->replot();
        }
    }
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Spin box controls how many data points are collected and displayed
 * @param arg1
 */
void MainWindow::on_spinPoints_valueChanged(int arg1)
{
    Q_UNUSED(arg1)
    ui->plot->xAxis->setRange(dataPointNumber - ui->spinPoints->value(), dataPointNumber);
    ui->plot->replot();
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Shows a window with instructions
 */
void MainWindow::on_actionHow_to_use_triggered()
{
    helpWindow = new HelpWindow(this);
    helpWindow->setWindowTitle("How to use this application");
    helpWindow->show();
}

/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Connects to COM port or restarts plotting
 */
void MainWindow::on_actionConnect_triggered()
{
    if (connected) {
        /* Is connected, restart if paused */
        if (!plotting) {         // Start plotting
            updateTimer.start(); // Start updating plot timer
            plotting = true;
            ui->actionConnect->setEnabled(false);
            ui->actionPause_Plot->setEnabled(true);
            ui->statusBar->showMessage("Plot restarted!");
        }
    } else {
        /* If application is not connected, connect */
        /* Get parameters from controls first */
        QSerialPortInfo portInfo(
            ui->comboPort->currentText()); // Temporary object, needed to create QSerialPort
        int baudRate = ui->comboBaud->currentText().toInt(); // Get baud rate from combo box
        int dataBitsIndex = ui->comboData->currentIndex();   // Get index of data bits combo box
        int parityIndex = ui->comboParity->currentIndex();   // Get index of parity combo box
        int stopBitsIndex = ui->comboStop->currentIndex();   // Get index of stop bits combo box
        QSerialPort::DataBits dataBits;
        QSerialPort::Parity parity;
        QSerialPort::StopBits stopBits;

        /* Set data bits according to the selected index */
        switch (dataBitsIndex) {
        case 0:
            dataBits = QSerialPort::Data8;
            break;
        default:
            dataBits = QSerialPort::Data7;
        }

        /* Set parity according to the selected index */
        switch (parityIndex) {
        case 0:
            parity = QSerialPort::NoParity;
            break;
        case 1:
            parity = QSerialPort::OddParity;
            break;
        default:
            parity = QSerialPort::EvenParity;
        }

        /* Set stop bits according to the selected index */
        switch (stopBitsIndex) {
        case 0:
            stopBits = QSerialPort::OneStop;
            break;
        default:
            stopBits = QSerialPort::TwoStop;
        }

        /* Use local instance of QSerialPort; does not crash */
        serialPort = new QSerialPort(portInfo, nullptr);

        /* Open serial port and connect its signals */
        openPort(portInfo, baudRate, dataBits, parity, stopBits);
    }
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Keep COM port open but pause plotting
 */
void MainWindow::on_actionPause_Plot_triggered()
{
    if (plotting) {
        updateTimer.stop(); // Stop updating plot timer
        plotting = false;
        ui->actionConnect->setEnabled(true);
        ui->actionPause_Plot->setEnabled(false);
        ui->statusBar->showMessage("Plot paused, new data will be ignored");
    }
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Keep COM port open but pause plotting
 */
void MainWindow::on_actionRecord_stream_triggered()
{
    if (ui->actionRecord_stream->isChecked()) {
        ui->statusBar->showMessage(tr("Data will be stored in csv file"));
    } else {
        ui->statusBar->showMessage(tr("Data will not be stored anymore"));
    }
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Closes COM port and stop plotting
 */
void MainWindow::on_actionDisconnect_triggered()
{
    if (connected) {
        serialPort->close();  // Close serial port
        emit portClosed();    // Notify application
        delete serialPort;    // Delete the pointer
        serialPort = nullptr; // Assign NULL to dangling pointer

        ui->statusBar->showMessage(tr("Disconnected!"));

        connected = false; // Set connected status flag to false
        ui->actionConnect->setEnabled(true);

        plotting = false; // Not plotting anymore
        ui->actionPause_Plot->setEnabled(false);
        ui->actionDisconnect->setEnabled(false);
        ui->actionRecord_stream->setEnabled(true);
        receivedData.clear(); // Clear received string

        ui->savePNGButton->setEnabled(false);
        enable_com_controls(true);
    }
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Clear all channels data and reset plot area
 *
 * This function will not delete the channel itself (legend will stay)
 */
void MainWindow::on_actionClear_triggered()
{
    ui->plot->clearPlottables();
    ui->listWidget_Channels->clear();
    channels = 0;
    dataPointNumber = 0;
    emit setupPlot();
    ui->plot->replot();
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Open a new CSV file to save received data
 *
 */
void MainWindow::openCsvFile(void)
{
    m_csvFile = new QFile(QDateTime::currentDateTime().toString("yyyy-MM-d-HH-mm-ss-")
                          + "data-out.csv");
    if (!m_csvFile)
        return;
    if (!m_csvFile->open(QIODevice::ReadWrite | QIODevice::Text))
        return;
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Open a new CSV file to save received data
 *
 */
void MainWindow::closeCsvFile(void)
{
    if (!m_csvFile)
        return;
    m_csvFile->close();
    if (m_csvFile)
        delete m_csvFile;
    m_csvFile = nullptr;
}
/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Open a new CSV file to save received data
 *
 */
void MainWindow::saveStream(QStringList newData)
{
    if (!m_csvFile)
        return;
    if (ui->actionRecord_stream->isChecked()) {
        QTextStream out(m_csvFile);
        foreach (const QString &str, newData) {
            out << str << ",";
        }
        out << "\n";
    }
}

/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void MainWindow::on_pushButton_TextEditHide_clicked()
{
    if (ui->pushButton_TextEditHide->isChecked()) {
        ui->textEdit_UartWindow->setVisible(false);
        ui->pushButton_TextEditHide->setText(tr("Show TextBox"));
    } else {
        ui->textEdit_UartWindow->setVisible(true);
        ui->pushButton_TextEditHide->setText(tr("Hide TextBox"));
    }
}

void MainWindow::on_pushButton_ShowallData_clicked()
{
    if (ui->pushButton_ShowallData->isChecked()) {
        filterDisplayedData = false;
        ui->pushButton_ShowallData->setText(tr("Filter Incoming Data"));
    } else {
        filterDisplayedData = true;
        ui->pushButton_ShowallData->setText(tr("Show All Incoming Data"));
    }
}

void MainWindow::on_pushButton_AutoScale_clicked()
{
    ui->plot->yAxis->rescale(true);
    ui->spinAxesMax->setValue(int(ui->plot->yAxis->range().upper)
                              + int(ui->plot->yAxis->range().upper * 0.1));
    ui->spinAxesMin->setValue(int(ui->plot->yAxis->range().lower)
                              + int(ui->plot->yAxis->range().lower * 0.1));
}

void MainWindow::on_pushButton_ResetVisible_clicked()
{
    for (int i = 0; i < ui->plot->graphCount(); i++) {
        ui->plot->graph(i)->setVisible(true);
        ui->listWidget_Channels->item(i)->setBackground(Qt::NoBrush);
    }
}

void MainWindow::on_listWidget_Channels_itemDoubleClicked(QListWidgetItem *item)
{
    int graphIdx = ui->listWidget_Channels->currentRow();

    if (ui->plot->graph(graphIdx)->visible()) {
        ui->plot->graph(graphIdx)->setVisible(false);
        item->setBackground(Qt::black);
    } else {
        ui->plot->graph(graphIdx)->setVisible(true);
        item->setBackground(Qt::NoBrush);
    }
    ui->plot->replot();
}

void MainWindow::on_pushButton_clicked()
{
    ui->comboPort->clear();
    /* List all available serial ports and populate ports combo box */
    for (QSerialPortInfo port : QSerialPortInfo::availablePorts()) {
        ui->comboPort->addItem(port.portName());
    }
}

void MainWindow::on_pushButton_TestData_clicked(bool checked)
{
    static QTimer *generateDataTimer = nullptr;
    this->angle = 0;
    if (!generateDataTimer) {
        generateDataTimer = new QTimer(this);
        generateDataTimer->setInterval(50);

        connect(generateDataTimer, &QTimer::timeout, this, [this]() {
            QStringList data;
            data.append(QString::number(30 * sin(this->angle * 3.14159 / 180)));
            data.append(QString::number(60 * sin(this->angle * 3.14159 / 180)));
            data.append(QString::number(90 * sin(this->angle * 3.14159 / 180)));
            this->angle += 1;

            if (filterDisplayedData) {
                ui->textEdit_UartWindow->append(data.join(" "));
            } else {
                ui->textEdit_UartWindow->append("$" + data.join(" ") + ";");
            }

            onNewDataArrived(data);
        });
    }

    if (checked) {
        ui->actionConnect->setEnabled(false);
        plotting = true;
        updateTimer.start(20);
        generateDataTimer->start();
        ui->pushButton_TestData->setText(tr("Stop Test"));
    } else {
        ui->actionConnect->setEnabled(true);
        plotting = false;
        updateTimer.stop();
        generateDataTimer->stop();
        ui->pushButton_TestData->setText(tr("Start Test"));
    }
}
