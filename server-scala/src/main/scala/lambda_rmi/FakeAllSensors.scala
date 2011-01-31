/*
 * lambda-rmi - a demonstration of using lambda funcitons for RMI
 *
 * Copyright (C) 2011, David M. Lee, II <leedm777@yahoo.com>
 *
 * This program is free software, distributed under the terms of
 * the BSD License. See the LICENSE.txt file at the top of the
 * source tree.
 */
package lambda_rmi

import Ice.Current
import LambdaRmi.{SensorPrx, _AllSensorsDisp}

/**
 * Fake implementation of the AllSensors interface.  All it does is just return
 * the given list of sensors
 */
class FakeAllSensors(sensors: Array[SensorPrx]) extends _AllSensorsDisp {
  def list(c: Current) = sensors
}
