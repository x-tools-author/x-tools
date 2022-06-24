/******************************************************************************
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKSIGLETON_HH
#define SAKSIGLETON_HH

#include <QMutex>
#include <QtGlobal>
#include "SAKObj.hh"

template <class T>
class SpinQSigleton : public SAKObj
{
public:
    SpinQSigleton(const char *category = "SpinQ")
        : SAKObj(category) {

    }

    ~SpinQSigleton() {
        mInstanceMutex.lock();
        if (mInstance) {
            delete mInstance;
            mInstance = Q_NULLPTR;
        }
        mInstanceMutex.unlock();
    }

    static T* instance() {
        mInstanceMutex.lock();
        if (!mInstance) {
            mInstance = new T;
        }
        mInstanceMutex.unlock();

        Q_ASSERT_X(mInstance, __FUNCTION__,
                   "Can not allocate memory for sigleton!");
        return mInstance;
    }
private:
    static T* mInstance;
    static QMutex mInstanceMutex;
};

template <class T> T* SpinQSigleton<T>::mInstance = Q_NULLPTR;
template <class T> QMutex SpinQSigleton<T>::mInstanceMutex;

#endif // SAKSIGLETON_HH
