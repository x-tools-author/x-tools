/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

// -------------------------------------------------------------------------------------------------
#define X_DECLARE_PRIVATE(Class) \
    Class##Private *x_func() \
    { \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<Class##Private *>(x_d_ptr);) \
    } \
    const Class##Private *x_func() const \
    { \
        Q_CAST_IGNORE_ALIGN(return reinterpret_cast<const Class##Private *>(x_d_ptr);) \
    } \
    Class##Private *x_d_ptr{nullptr}; \
    friend class Class##Private;

#define X_D(Class) Class##Private *const x_d = x_d_func();
#define X_DC(Class) const Class##Private *const x_d = x_d_func();

// -------------------------------------------------------------------------------------------------
#define X_DECLARE_PUBLIC(Class) \
    Class *x_q_func() \
    { \
        return reinterpret_cast<Class *>(x_q_ptr); \
    } \
    const Class *x_q_func() const \
    { \
        return reinterpret_cast<const Class *>(x_q_ptr); \
    } \
    Class *x_q_ptr{nullptr}; \
    friend class Class;

#define X_Q(Class) Class *const x_q = x_q_func();
#define X_QC(Class) const Class *const x_q = x_q_func();