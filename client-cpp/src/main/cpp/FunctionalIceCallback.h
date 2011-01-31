/*
 * lambda-rmi - a demonstration of using lambda funcitons for RMI
 *
 * Copyright (C) 2011, David M. Lee, II <leedm777@yahoo.com>
 *
 * This program is free software, distributed under the terms of
 * the BSD License. See the LICENSE.txt file at the top of the
 * source tree.
 */
#include <functional>
#include <Ice/Ice.h>

/** Function type for Ice callbacks */
typedef std::function<void (const Ice::AsyncResultPtr&)> IceCallbackFunc;

/**
 * Wrapper class that allows passing an IceCallbackFunc to an Ice AMI begin_
 * function.  Note that this still needs to be wrapped via Ice::newCallback.
 */
class IceCallbackFuncWrapper : public IceUtil::Shared
{
public:
    IceCallbackFuncWrapper(IceCallbackFunc callback) :
        callback(callback)
    {}

    void invoke(const Ice::AsyncResultPtr& r)
    {
        callback(r);
    }

private:
    IceCallbackFunc callback;
};
typedef IceUtil::Handle<IceCallbackFuncWrapper> IceCallbackFuncWrapperPtr;

/**
 * Wraps an IceCallbackFunc with an Ice::CallbackPtr.
 */
Ice::CallbackPtr toIce(IceCallbackFunc callback)
{
    IceCallbackFuncWrapperPtr lambdaWrapper =
        new IceCallbackFuncWrapper(callback);
    return Ice::newCallback(lambdaWrapper, &IceCallbackFuncWrapper::invoke);
}

