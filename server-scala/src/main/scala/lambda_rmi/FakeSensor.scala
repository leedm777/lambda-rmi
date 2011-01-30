package lambda_rmi

import Ice.Current
import akka.actor.Scheduler
import java.util.concurrent.TimeUnit
import LambdaRmi.{Reading, AMD_Sensor_getReading, _SensorDisp}
case class FakeSensor(zip: String, state: String, city: String, lat: Double, long: Double, maxResponseTimeMillis: Int) extends _SensorDisp {
  private val RandCount = 1
  val rand = new util.Random

  /** Average RandCount even distribution random numbers to get a bell curve */
  val nextDouble = (for (i <- 1.to(RandCount)) yield rand.nextDouble).foldLeft(0.0) { _ + _ } / RandCount

  def currentTemp = (WeatherMan.actor !! (zip, 5000)) match {
  case Some(temp :Double) => temp
  case None => throw new RuntimeException("Didn't return")
  case _ => throw new ClassCastException("Expected Double")
  }
  val currentReading = new Reading(lat, long, currentTemp)

  def getReading_async(cb: AMD_Sensor_getReading, current: Current) = {
    Scheduler.scheduleOnce(() => cb.ice_response(currentReading),
      (nextDouble * maxResponseTimeMillis).toInt, TimeUnit.MILLISECONDS)
  }
}