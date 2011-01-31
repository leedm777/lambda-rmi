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

import io.Source
import LambdaRmi.SensorPrxHelper

class InvalidConfigurationException(cfg: Any) extends Exception(cfg.toString)

/**
 * Simple application just for te purpose of demonstrating the use of lambda
 * functions to make RMI less sucky.  This app presents a really simple
 * interface mocking out temperature sensors randomly distributed around the
 *  country.
 */
object App {
  /**
   * Number of sensors to simulate.  Defaults to 10.  Override with system
   * property lambda-rmi.sensor-count.
   */
  val SensorCount = System.getProperty("lambda-rmi.sensor-count", "10").toInt
  /**
   * Max response time for a sensor.  Default to 3000 ms.  Override with system
   * property lambda-rmi.max-response-time-millis.
   */
  val MaxResponseTimeMillis =
    System.getProperty("lambda-rmi.max-response-time-millis", "3000").toInt

  val Endpoint = System.getProperty("lambda-rmi.endpoint", "default -p 1099")


  def main(args: Array[String]) {
    // some list of zip codes I snagged off the Internet
    val zipCodesStream = getClass.getClassLoader.getResourceAsStream("zips.txt")
    assert(zipCodesStream != null)

    // parse the file into a lists of lists
    val zipCodes =
      Source.fromInputStream(zipCodesStream).getLines.toSeq
        .map(_.split(",").toList.map(_.replaceAll("\"", "")))

    // Shuffle the list, take SensorCount of them, map them to FakeSensor's
    val fakeSensors = util.Random.shuffle(zipCodes)
      .take(SensorCount)
      .map {
      case _ :: zip :: state :: city :: lat :: long :: _ :: _ :: Nil =>
        FakeSensor(zip, state, city, lat.toDouble, long.toDouble,
          MaxResponseTimeMillis)
      case wtf =>
        throw new InvalidConfigurationException(wtf)
    }

    // Ice init
    val communicator = Ice.Util.initialize(args);
    val adapter = communicator.createObjectAdapterWithEndpoints(
      "LambdaAdapter", Endpoint)

    // create proxies for our sensors
    val sensorProxies = fakeSensors.map((v: Ice.Object) =>
      SensorPrxHelper.uncheckedCast(adapter.addWithUUID(v)))

    val fakeAllSensors = new FakeAllSensors(sensorProxies.toArray)
    val proxy = adapter.add(fakeAllSensors,
      communicator.stringToIdentity("AllSensors"))
    println(proxy)
    adapter.activate()

    communicator.waitForShutdown
  }

}
