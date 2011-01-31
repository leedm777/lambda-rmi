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

