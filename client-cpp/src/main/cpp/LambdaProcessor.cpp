#include "SensorProcessor.h"

#include <functional>

typedef std::function<void (const Ice::AsyncResultPtr&)> CallbackFunction;
class LambdaCallback : public IceUtil::Shared
{
public:
    LambdaCallback(CallbackFunction callback) :
        callback(callback)
    {}

    void invoke(const Ice::AsyncResultPtr& r)
    {
        callback(r);
    }

private:
    CallbackFunction callback;
};
typedef IceUtil::Handle<LambdaCallback> LambdaCallbackPtr;

Ice::CallbackPtr toIce(CallbackFunction callback)
{
    LambdaCallbackPtr lambdaWrapper = new LambdaCallback(callback);
    return Ice::newCallback(lambdaWrapper, &LambdaCallback::invoke);
}

struct CollectionData
{
    CollectionData() : sum(0), count(0) {}

    double sum;
    std::size_t count;
};

class LambdaProcessor : public SensorProcessor
{
public:
    LambdaProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        SensorProcessor(allSensors) {}
    void getAverageTemperatureCelsius(
        boost::function<void (double)> callback) const
    {
        std::shared_ptr<CollectionData> data(new CollectionData);
        const LambdaRmi::SensorSeq& sensorSeq = getAllSensors()->list();
        assert(sensorSeq.size() > 0);
        std::size_t expected = sensorSeq.size();
        for (LambdaRmi::SensorSeq::const_iterator i = sensorSeq.begin();
             i != sensorSeq.end();
             ++i)
        {
            (*i)->begin_getReading(toIce([=](const Ice::AsyncResultPtr&r)
            {
                LambdaRmi::SensorPrx sensor =
                    LambdaRmi::SensorPrx::uncheckedCast(r->getProxy());
                LambdaRmi::ReadingPtr reading = sensor->end_getReading(r);
                std::cout << reading << '\n';
                data->sum += reading->temperatureCelcius;
                ++(data->count);

                if (data->count == expected)
                {
                    callback(data->sum / data->count);
                }
            }));
        }
    }
};

SensorProcessorPtr newLambdaProcessor(const LambdaRmi::AllSensorsPrx& allSensors)
{
    return new LambdaProcessor(allSensors);
}
