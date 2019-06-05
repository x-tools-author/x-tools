/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifndef CRCINTERFACE_HH
#define CRCINTERFACE_HH

#include <QObject>
#include <QStringList>

class CRCInterface:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief CRCInterface  -- 构造函数， nothing to say...
     * @param parent        -- 父控件
     */
    CRCInterface(QObject *parent = nullptr);

    /**
     * @brief The CRCModel enum -- crc算法模型（crc参数模型）
     */
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
    template<typename Tint>
    void crcCalculate(uint8_t *input, uint64_t length, Tint &crc, CRCInterface::CRCModel model){
        Tint crcReg = static_cast<Tint>(getInitValue(model));
        Tint poly = static_cast<Tint>(getPoly(model));
        uint8_t byte = 0;

        Tint temp = 1;
        while (length--){
            byte = *(input++);

            if (getInputReversal(model)){
                reverseInt(byte,byte);
            }

            crcReg ^= static_cast<Tint>((byte << 8*(sizeof (Tint)-1)));
            for(int i = 0;i < 8;i++){
                if(crcReg & (temp << (sizeof (Tint)*8-1))){
                    crcReg = static_cast<Tint>((crcReg << 1) ^ poly);
                }else {
                    crcReg = static_cast<Tint>(crcReg << 1);
                }
            }
        }

        if (getOutputReversal(model)){
            reverseInt(crcReg,crcReg);
        }

        crc = (crcReg ^ static_cast<Tint>(getXorValue(model))) ;
    }
public:
    /**
     * @brief supportParameterModel --获取支持的参数模型
     * @return                      -- 参数模型列表
     */
    QStringList getSupportParameterModels();

    /**
     * @brief getPolyFormula    -- 获取多项式公式
     * @param model             -- crc参数模型
     * @return                  -- 多项式公式
     */
    QString getPolyFormula(CRCModel model);
private:
    /**
     * @brief reverseInt    -- 将一个整数按位反转
     * @param input         -- 输入数据
     * @param output        -- 输出输出
     * @return              -- 转换是否成功
     */
    template<typename Tint>
    bool reverseInt(const Tint &input, Tint &output){
        int bitsWidth = sizeof (input)*8;

        QString inputStr = QString("%1").arg(QString::number(input, 2), bitsWidth, '0');
        QString outputStr;
        outputStr.resize(bitsWidth);
        for (int i = 0; i < bitsWidth; i++){
            outputStr.replace(i, 1, inputStr.at(bitsWidth-1-i));
        }

        bool ok;
        output = static_cast<Tint>(outputStr.toULongLong(&ok, 2));
        return ok;
    }

public:
    /**
     * @brief getInitValue  -- 获取crc初始值
     * @param model         -- crc参数模型
     * @return              -- 指定参数模型的crc初始值(根据实际情况进项强制转换)
     */
    uint32_t getInitValue(CRCInterface::CRCModel model);

    /**
     * @brief getPoly   -- 获取多项式
     * @param model     -- crc参数模型
     * @return          -- 执行的参数模型的多项式(根据实际情况进项强制转换)
     */
    uint32_t getPoly(CRCInterface::CRCModel model);

    /**
     * @brief xorValue  -- 结果异或值
     * @param model     -- crc参数模型
     * @return          -- 结果异或值
     */
    uint32_t getXorValue(CRCInterface::CRCModel model);

    /**
     * @brief getInputReversal  -- 判断输入是否需要翻转
     * @param model             -- crc参数模型
     * @return                  -- 输入数据需要翻转返回true，否则返回false
     */
    bool getInputReversal(CRCInterface::CRCModel model);

    /**
     * @brief getOutputReversal -- 判断输入是否需要翻转
     * @param model             -- crc参数模型
     * @return                  -- 输入数据需要翻转返回true，否则返回false
     */
    bool getOutputReversal(CRCInterface::CRCModel model);

    /**
     * @brief getBitsWidth  -- 获取位宽
     * @param model         -- 参数模型
     * @return              -- 位宽
     */
    int getBitsWidth(CRCInterface::CRCModel model);
private:
    QStringList modelStrings;
};

#endif
