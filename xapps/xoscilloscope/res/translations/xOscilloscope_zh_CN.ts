<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>HelpWindow</name>
    <message>
        <location filename="../../Source/helpwindow.ui" line="14"/>
        <source>Dialog</source>
        <translation>使用说明</translation>
    </message>
    <message>
        <location filename="../../Source/helpwindow.ui" line="35"/>
        <source>INSTRUCTIONS</source>
        <translation>使用说明</translation>
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
        <translation>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;MS Shell Dlg 2&apos;; font-size:8.25pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;该程序绘制多达三个轴的整数数据从串行端口收集。从远程设备发送的数据需要采用一定的格式：每行必须以‘$’开头，以‘;’结尾。值之间必须用空格分隔。&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;从mbed发送数据到此应用程序的示例：&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;pc.printf(&amp;quot;$%d %d;&amp;quot;, rawData, filteredData);&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt; font-weight:600; font-style:italic;&quot;&gt;        wait_ms(10);&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;要开始使用该程序，请将设备连接到PC并开始发送数据。打开串行端口绘图仪和：&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;-指定要绘制的轴的数量&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;-指定要绘制的数据点数目。例如，如果您每10毫秒发送一个数据点，并绘制500个数据点，则应用程序将显示5秒的数据。&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;-Y轴的步长和范围可以在运行时调整。&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;-选择要配置的端口，按&lt;/span&gt;&lt;span style=&quot; font-size:8pt; font-style:italic;&quot;&gt; 连接&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-style:italic;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;您可以使用提供的按钮开始和停止情节。应用程序可以将当前绘图导出为以当前数据点编号命名的JPG格式，并保存在可执行文件的目录中。&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;通过用鼠标滚动和拖动，你可以放大绘图；单击绘图区域的任何一点，将显示状态栏中的X和Y坐标。 &lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;使用“重置绘图”按钮将图形设置为默认值。 &lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;轴颜色:&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Axis 1: red&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Axis 2: Yellow&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:8pt;&quot;&gt;- Axis 3: Green&lt;/span&gt;&lt;/p&gt;
&lt;p style=&quot;-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;&quot;&gt;&lt;br /&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
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
        <translation>串口控制</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="124"/>
        <source>BAUD</source>
        <translation>波特率</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="165"/>
        <source>DATA</source>
        <translation>数据位</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="184"/>
        <source>STOP</source>
        <translation>停止位</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="219"/>
        <source>Refresh</source>
        <translation>刷新</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="238"/>
        <source>PARITY</source>
        <translation>校验位</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="257"/>
        <source>PORT</source>
        <translation>端口号</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="320"/>
        <source>PLOT CONTROLS</source>
        <translation>图表控制</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="390"/>
        <source>POINTS</source>
        <translation>坐标点</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="415"/>
        <source>MAX</source>
        <translation>最大值</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="440"/>
        <source>Y STEP</source>
        <translation>分割线</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="499"/>
        <source>MIN</source>
        <translation>最小值</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="665"/>
        <source>Reset All Visible</source>
        <translation>重置视图</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="678"/>
        <source>Save PNG</source>
        <translation>导出图片</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="652"/>
        <source>AutoScale Yaxis</source>
        <translation>自动缩放Y轴</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="685"/>
        <location filename="../../Source/mainwindow.cpp" line="958"/>
        <source>Start Test</source>
        <translation>开始测试</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="709"/>
        <source>TEXT CONTROLS</source>
        <translation>文本控制</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="721"/>
        <location filename="../../Source/mainwindow.cpp" line="867"/>
        <source>Hide TextBox</source>
        <translation>隐藏文本输出</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="740"/>
        <location filename="../../Source/mainwindow.cpp" line="878"/>
        <source>Show All Incoming Data</source>
        <translation>显示所有数据</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="816"/>
        <source>toolBar</source>
        <translation>工具栏</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="856"/>
        <source>Connect</source>
        <translation>连接</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="859"/>
        <source>Connect to COM port</source>
        <translation>连接串口</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="878"/>
        <source>Pause</source>
        <translation>暂停</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="881"/>
        <source>Pause plot</source>
        <translation>暂停绘制</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="900"/>
        <source>Disconnect</source>
        <translation>断卡连接</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="903"/>
        <source>Disconnect COM port</source>
        <translation>断开串口连接</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="922"/>
        <source>Clear</source>
        <translation>清空</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="925"/>
        <source>Clear plot data</source>
        <translation>清空图表数据</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="941"/>
        <source>How to use</source>
        <translation>使用说明</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="961"/>
        <source>Record stream</source>
        <translation>录制数据流</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.ui" line="964"/>
        <source>Record the incoming data to a .csv file ( shortcut : s )</source>
        <translation>将接收的数据保存至csv文件（快捷键：&apos;s&apos;）</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="760"/>
        <source>Data will be stored in csv file</source>
        <translation>数据将保存至csv文件</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="762"/>
        <source>Data will not be stored anymore</source>
        <translation>数据将不会保存至csv文件</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="778"/>
        <source>Disconnected!</source>
        <translation>链接已断开！</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="864"/>
        <source>Show TextBox</source>
        <translation>显示文本输出</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="875"/>
        <source>Filter Incoming Data</source>
        <translation>显示过滤数据</translation>
    </message>
    <message>
        <location filename="../../Source/mainwindow.cpp" line="952"/>
        <source>Stop Test</source>
        <translation>停止测试</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../../Source/main.cpp" line="57"/>
        <source>Serial Port Oscilloscope</source>
        <translation>串口示波器</translation>
    </message>
</context>
</TS>
