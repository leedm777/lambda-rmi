/*
 * lambda-rmi - a demonstration of using lambda funcitons for RMI
 *
 * Copyright (C) 2011, David M. Lee, II <leedm777@yahoo.com>
 *
 * This program is free software, distributed under the terms of
 * the BSD License. See the LICENSE.txt file at the top of the
 * source tree.
 */
module LambdaRmi
{
    /** Individual reading from the sensor */
    class Reading
    {
        double latitudeDegrees;
        double longitudeDegrees;
        double temperatureCelcius;
    };

    /** Individual sensor */
    interface Sensor
    {
        ["amd"] Reading getReading();
    };

    sequence<Sensor*> SensorSeq;

    /** API for a collection of Sensors */
    interface AllSensors
    {
        SensorSeq list();
    };
};
