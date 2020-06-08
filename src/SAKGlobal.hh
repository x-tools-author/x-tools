/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKGLOBAL_HH
#define SAKGLOBAL_HH

#include <QObject>
#include <QComboBox>

class SAKGlobal:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief SAKGlobal -- 构造函数
     * @param parent    -- 父控件
     */
    SAKGlobal(QObject* parent = Q_NULLPTR);

    /**
     * @brief logFile   -- 获取日志文件（全路径）
     * @return          -- 日志文件全路径
     */
    static QString logFile();

    /**
     * @brief dataPath 获取软件数据缓存目录
     * @return 软件数据缓存目录
     */
    static QString dataPath();

    /**
     * @brief mkMutiDir -- 递归创建指定的目录路径
     * @param path      -- 目录路径
     * @return          -- 已创建的路径
     */
    static QString mkMutiDir(const QString path);

    /**
     * @brief getNameOfDebugPage    -- 获取设备调试页面的名称
     * @param type                  -- 设备类型，详见SAKDataStruct::SAKEnumDebugPageType
     * @return                      -- 设备类型名称
     */
    static QString getNameOfDebugPage(int type);

#ifdef SAK_IMPORT_COM_MODULE
    /// @brief 串口相关参数显示初始化
    static void initComComboBox(QComboBox *comboBox);
    static void initBaudRateComboBox(QComboBox *comboBox);
    static void initDataBitsComboBox(QComboBox *comboBox);
    static void initStopBitsComboBox(QComboBox *comboBox);
    static void initParityComboBox(QComboBox *comboBox);
    static void initFlowControlComboBox(QComboBox *comboBox);
#endif

    /**
     * @brief initIpComboBox 获取本机所有ip v4地址，并添加至comboBox中
     * @param comboBox 需要初始化的comboBox实例指针
     */
    static void initIpComboBox(QComboBox *comboBox);

    /**
     * @brief initTextFormatComboBox 初始化输入文本格式，格式详情查看SAKTextInputFormat
     * @param comboBox 需要初始化的comboBox实例指针
     */
    static void initInputTextFormatComboBox(QComboBox *comboBox);

    /**
     * @brief initOutputTextFormatComboBox 初始化输出文本格式，格式详情查看SAKTextOutputFormat
     * @param comboBox 需要初始化的comboBox实例指针
     */
    static void initOutputTextFormatComboBox(QComboBox *comboBox);

    /**
     * @brief initCRCComboBox 初始化crc列表
     * @param comboBox 需要初始化的comboBox实例指针
     */
    static void initCRCComboBox(QComboBox *comboBox);
};

#endif
