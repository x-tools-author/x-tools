/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QButtonGroup>
#include <QPointF>
#include <QStackedLayout>
#include <QToolButton>
#include <QVariantMap>
#include <QWidget>

class ChartPanel;
class ChartsView : public QWidget
{
    Q_OBJECT
public:
    enum class DataFormat { BinaryY, BinaryXY, TextY, TextXY };
    Q_ENUM(DataFormat);

public:
    ChartsView(QWidget *parent = Q_NULLPTR);
    virtual ~ChartsView() override;

    QList<QToolButton *> chartControllers();

    void resetCharts();
    void inputBytes(const QByteArray &bytes);
    QVariantMap save();
    void load(const QVariantMap &parameters);

private:
    QList<ChartPanel *> m_chartViews;
    QList<QToolButton *> m_chartControllers;
    QStackedLayout *m_layout;
    QButtonGroup *m_buttonGroup;

private:
    template<typename T>
    void addChartView(const QString &icon, int index)
    {
        T *chartView = new T();
        m_layout->addWidget(chartView);
        m_chartViews.append(chartView);

        QToolButton *controller = new QToolButton();
        controller->setCheckable(true);
        controller->setIcon(QIcon(icon));
        controller->setMenu(chartView->chartSettingsMenu());
        controller->setPopupMode(QToolButton::MenuButtonPopup);
        m_chartControllers.append(controller);
        m_buttonGroup->addButton(controller);
        connect(controller, &QToolButton::clicked, this, [=]() {
            m_layout->setCurrentIndex(index);
        });
    }
};
