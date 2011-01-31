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
import akka.actor.Scheduler
import java.util.concurrent.TimeUnit
import LambdaRmi.{Reading, AMD_Sensor_getReading, _SensorDisp}
case class FakeSensor(zip: String, state: String, city: String, lat: Double, long: Double, maxResponseTimeMillis: Int) extends _SensorDisp {
  private val RandCount = 1
  val rand = new util.Random

  def currentTemp = (WeatherMan.actor !! (zip, 5000)) match {
  case Some(temp :Double) => temp
  case None => throw new RuntimeException("Didn't return")
  case _ => throw new ClassCastException("Expected Double")
  }
  val currentReading = new Reading(lat, long, currentTemp)

  def getReading_async(cb: AMD_Sensor_getReading, current: Current) = {
    Scheduler.scheduleOnce(() => cb.ice_response(currentReading),
      (rand.nextDouble * maxResponseTimeMillis).toInt, TimeUnit.MILLISECONDS)
  }
}
