#include "SensorProcessor.h"

#include <boost/function.hpp>

struct CollectionData : public Ice::LocalObject
{
    CollectionData(boost::function<void (double)> callback, std::size_t expected) :
        callback(callback), expected(expected), sum(0), count(0)
    {
    }

    void sample(double v)
    {
        sum += v;
        ++count;
    }

    bool isFinished() const { return count == expected; }

    void sendAverage() const { callback(sum / count); }

private:
    boost::function<void (double)> callback;
    std::size_t expected;
    double sum;
    int count;

};
typedef IceUtil::Handle<CollectionData> CollectionDataPtr;

class AsyncProcessor : public SensorProcessor
{
public:
    AsyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors) :
        SensorProcessor(allSensors)
    {}
    void getAverageTemperatureCelsius(
        boost::function<void (double)> callback) const
    {

        Ice::CallbackPtr iceCallback =
            Ice::newCallback((AsyncProcessor*)this, &AsyncProcessor::finished);

        LambdaRmi::SensorSeq sensorSeq = getAllSensors()->list();
        assert(sensorSeq.size() > 0);
        CollectionDataPtr collectionData =
            new CollectionData(callback, sensorSeq.size());

        for (LambdaRmi::SensorSeq::iterator i = sensorSeq.begin();
             i != sensorSeq.end();
             ++i)
        {
            (*i)->begin_getReading(iceCallback, collectionData);
        }
    }
private:
    void finished(const Ice::AsyncResultPtr& r)
    {
        LambdaRmi::SensorPrx sensor =
            LambdaRmi::SensorPrx::uncheckedCast(r->getProxy());
        LambdaRmi::ReadingPtr reading = sensor->end_getReading(r);
        std::cout << reading << '\n';

        CollectionDataPtr data =
            CollectionDataPtr::dynamicCast(r->getCookie());
        data->sample(reading->temperatureCelcius);

        if (data->isFinished())
        {
            data->sendAverage();
        }
    }
};

SensorProcessorPtr newAsyncProcessor(const LambdaRmi::AllSensorsPrx& allSensors)
{
    return new AsyncProcessor(allSensors);
}
