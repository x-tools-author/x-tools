/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDateTime>
#include <QString>
#include <QWidget>

#define xSetNoneBorder(w) xTools::setNoneBorder(w)

namespace xTools {

QString systemDateFormat();
QString systemTimeFormat();
QString dateTimeString(const QString &format);
QDateTime buildDateTime();
QString buildDateTimeString(const QString &format);

void setNoneBorder(QWidget *widget);

} // namespace xTools

// -------------------------------------------------------------------------------------------------
#define X_DECLARE_PRIVATE(Class) \
    inline Class##Private *x_d_func() \
    { \
        return reinterpret_cast<Class##Private *>(x_d_ptr); \
    } \
    inline const Class##Private *x_d_func() const \
    { \
        return reinterpret_cast<const Class##Private *>(x_d_ptr); \
    } \
    friend class Class##Private; \
    class Class##Private *x_d_ptr{nullptr};

#define X_D(Class) Class##Private *const x_d = x_d_func();
#define X_DC(Class) const Class##Private *const x_d = x_d_func();

// -------------------------------------------------------------------------------------------------
#define X_DECLARE_PUBLIC(Class) \
    inline Class *x_q_func() \
    { \
        return reinterpret_cast<Class *>(x_q_ptr); \
    } \
    inline const Class *x_q_func() const \
    { \
        return reinterpret_cast<const Class *>(x_q_ptr); \
    } \
    friend class Class; \
    Class *x_q_ptr{nullptr};

#define X_Q(Class) Class *const x_q = x_q_func();
#define X_QC(Class) const Class *const x_q = x_q_func();