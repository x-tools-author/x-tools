<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="en_US">
<context>
    <name>HelpWindow</name>
    <message>
        <location filename="../../Source/helpwindow.ui" line="14"/>
        <source>Dialog</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/helpwindow.ui" line="35"/>
        <source>INSTRUCTIONS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/helpwindow.ui" line="57"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;This program plots up to three axes of integer data collected from serial port. The data sent from the remote device needs to be in a certain format: every line must begin with &apos;$&apos; and end with &apos;;&apos;. The values must be space separated.&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Example of sending data from mbed to this application:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;pc.printf(&amp;quot;$%d %d;&amp;quot;, rawData, filteredData);&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;        wait_ms(10);&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;To start using the program, connect your device to your PC and start sending data. Open Serial Port Plotter and:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Specify number of axes you want to plot&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Specify number of data points that will be plotted. For example, if you send a        data point every 10 ms and you plot 500 data points, the application will display 5 seconds of data. &lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Steps and range for Y axis can be adjusted while running.&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Select a port, configure it and press&lt;/span&gt;&lt;span style=&quot; font-size:8pt; font-style:italic;&quot;&gt; Connect.&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-style:italic;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;You can start and stop the plot using the button provided. The application can export the current plot to a JPG that is named under the current data point number and is saved in the directory of the executable file. &lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;By scrolling and dragging with the mouse you can zoom in the plot; clicking on any point of the plot area displays the X and Y coordinates in the status bar. &lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Use Reset Plot button to set the graph to its defaults. &lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Axes colors:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Axis 1: red&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Axis 2: Yellow&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Axis 3: Green&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;The application is created using Qt and QCustomPlot and compiled with MinGW 4.8.2 32 bit&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Licence: GPL&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;Author: Borislav&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../../Source/mainwindow.ui" line="20"/>
        <source>MainWindow</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="66"/>
        <source>PORT CONTROLS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="124"/>
        <source>BAUD</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="165"/>
        <source>DATA</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="184"/>
        <source>STOP</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="219"/>
        <source>Refresh</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="238"/>
        <source>PARITY</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="257"/>
        <source>PORT</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="320"/>
        <source>PLOT CONTROLS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="390"/>
        <source>POINTS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="415"/>
        <source>MAX</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="440"/>
        <source>Y STEP</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="499"/>
        <source>MIN</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="665"/>
        <source>Reset All Visible</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="678"/>
        <source>Save PNG</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="652"/>
        <source>AutoScale Yaxis</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="685"/>
        <location filename="../../Source/mainwindow.cpp" line="958"/>
        <source>Start Test</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="709"/>
        <source>TEXT CONTROLS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="721"/>
        <location filename="../../Source/mainwindow.cpp" line="867"/>
        <source>Hide TextBox</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="740"/>
        <location filename="../../Source/mainwindow.cpp" line="878"/>
        <source>Show All Incoming Data</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="816"/>
        <source>toolBar</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="856"/>
        <source>Connect</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="859"/>
        <source>Connect to COM port</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="878"/>
        <source>Pause</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="881"/>
        <source>Pause plot</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="900"/>
        <source>Disconnect</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="903"/>
        <source>Disconnect COM port</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="922"/>
        <source>Clear</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="925"/>
        <source>Clear plot data</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="941"/>
        <source>How to use</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="961"/>
        <source>Record stream</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="964"/>
        <source>Record the incoming data to a .csv file ( shortcut : s )</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="760"/>
        <source>Data will be stored in csv file</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="762"/>
        <source>Data will not be stored anymore</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="778"/>
        <source>Disconnected!</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="864"/>
        <source>Show TextBox</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="875"/>
        <source>Filter Incoming Data</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="952"/>
        <source>Stop Test</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../../Source/main.cpp" line="57"/>
        <source>Serial Port Oscilloscope</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
