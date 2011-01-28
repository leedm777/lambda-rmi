#include <iostream>
#include <string>

#include <Ice/Ice.h>
#include <Sensor.h>

std::ostream& operator<<(std::ostream& o, LambdaRmi::ReadingPtr reading)
{
    if (reading)
    {
        return o << "(" << reading->latitudeDegrees << ","
                 << reading->longitudeDegrees << ") - "
                 << reading->temperatureCelcius;
    }
    else
    {
        return o << "(null)";
    }
}

int main(int argc, const char* argv[])
{
    std::string sensorsPrxStr = "Sensors -t:tcp -h localhost -p 1099";

    switch(argc)
    {
    case 2:
        sensorsPrxStr = argv[1];
        // fall through
    case 1:
        // no-op
        break;
    }

    Ice::CommunicatorPtr ic;
    try
    {
        ic = Ice::initialize();
        LambdaRmi::SensorsPrx sensors = LambdaRmi::SensorsPrx::checkedCast(
            ic->stringToProxy(sensorsPrxStr));
        if (!sensors)
        {
            std::clog << "Invalid proxy" << sensorsPrxStr << "\n";
            return EXIT_FAILURE;
        }

        LambdaRmi::SensorSeq sensorSeq = sensors->getSensors();
        for (LambdaRmi::SensorSeq::iterator i = sensorSeq.begin();
             i != sensorSeq.end();
             ++i)
        {
            std::cout << (*i)->getReading() << '\n';
        }
    }
    catch (const std::exception& e)
    {
        std::clog << "Exception: " << e.what() << '\n';
    }
    catch (...)
    {
        std::clog << "Exception: " << "unknown" << '\n';
    }

    if (ic)
    {
        ic->destroy();
    }
}
