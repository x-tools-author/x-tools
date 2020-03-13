/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKCRCINTERFACE_HH
#define SAKCRCINTERFACE_HH

#include <QObject>
#include <QStringList>

class SAKCRCInterface:public QObject
{
    Q_OBJECT
public:
    SAKCRCInterface(QObject *parent = nullptr);


    /// crc算法模型（crc参数模型）
    enum CRCModel{
        CRC_8,
        CRC_8_ITU,
        CRC_8_ROHC,
        CRC_8_MAXIM,

        CRC_16_IBM,
        CRC_16_MAXIM,
        CRC_16_USB,
        CRC_16_MODBUS,
        CRC_16_CCITT,
        CRC_16_CCITT_FALSE,
        CRC_16_x25,
        CRC_16_XMODEM,
        CRC_16_DNP,

        CRC_32,
        CRC_32_MPEG2
    };
    Q_ENUM(CRCModel)

    /**
     * @brief crcCalculate  -- crc计算
     * @param input         -- 输入数据
     * @param length        -- 输入数据长度
     * @param crc           -- crc计算结果
     * @param model         -- crc参数模型
     */
    template<typename T>
    T crcCalculate(uint8_t *input, uint64_t length, SAKCRCInterface::CRCModel model){
        T crcReg = static_cast<T>(getInitValue(model));
        T poly = static_cast<T>(getPoly(model));
        uint8_t byte = 0;

        T temp = 1;
        while (length--){
            byte = *(input++);

            if (getInputReversal(model)){
                reverseInt(byte,byte);
            }

            crcReg ^= static_cast<T>((byte << 8*(sizeof (T)-1)));
            for(int i = 0;i < 8;i++){
                if(crcReg & (temp << (sizeof (T)*8-1))){
                    crcReg = static_cast<T>((crcReg << 1) ^ poly);
                }else {
                    crcReg = static_cast<T>(crcReg << 1);
                }
            }
        }

        if (getOutputReversal(model)){
            reverseInt(crcReg,crcReg);
        }

        T crc = (crcReg ^ static_cast<T>(getXorValue(model))) ;
        return crc;
    }


public:
    /**
     * @brief supportedParameterModels  -- 支持的crc参数模型
     * @return                          -- 参数模型列表
     */
    QStringList supportedParameterModels();

    /**
     * @brief getPolyFormula    -- 获取多项式公式
     * @param model             -- crc参数模型
     * @return                  -- 多项式公式
     */
    QString getPolyFormula(CRCModel model);          

public:
    /**
     * @brief getInitValue  -- 获取crc初始值
     * @param model         -- crc参数模型
     * @return              -- 指定参数模型的crc初始值(根据实际情况进项强制转换)
     */
    uint32_t getInitValue(SAKCRCInterface::CRCModel model);

    /**
     * @brief getPoly   -- 获取多项式
     * @param model     -- crc参数模型
     * @return          -- 执行的参数模型的多项式(根据实际情况进项强制转换)
     */
    uint32_t getPoly(SAKCRCInterface::CRCModel model);

    /**
     * @brief xorValue  -- 结果异或值
     * @param model     -- crc参数模型
     * @return          -- 结果异或值
     */
    uint32_t getXorValue(SAKCRCInterface::CRCModel model);

    /**
     * @brief getInputReversal  -- 判断输入是否需要翻转
     * @param model             -- crc参数模型
     * @return                  -- 输入数据需要翻转返回true，否则返回false
     */
    bool getInputReversal(SAKCRCInterface::CRCModel model);

    /**
     * @brief getOutputReversal -- 判断输入是否需要翻转
     * @param model             -- crc参数模型
     * @return                  -- 输入数据需要翻转返回true，否则返回false
     */
    bool getOutputReversal(SAKCRCInterface::CRCModel model);

    /**
     * @brief getBitsWidth  -- 获取位宽
     * @param model         -- 参数模型
     * @return              -- 位宽
     */
    int getBitsWidth(SAKCRCInterface::CRCModel model);


private:
    QStringList modelStrings;

private:
    /**
     * @brief reverseInt    -- 将一个整数按位反转
     * @param input         -- 输入数据
     * @param output        -- 输出输出
     * @return              -- 转换是否成功
     */
    template<typename T>
    bool reverseInt(const T &input, T &output){
        int bitsWidth = sizeof (input)*8;

        QString inputStr = QString("%1").arg(QString::number(input, 2), bitsWidth, '0');
        QString outputStr;
        outputStr.resize(bitsWidth);
        for (int i = 0; i < bitsWidth; i++){
            outputStr.replace(i, 1, inputStr.at(bitsWidth-1-i));
        }

        bool ok;
        output = static_cast<T>(outputStr.toULongLong(&ok, 2));
        return ok;
    }
};

#endif
