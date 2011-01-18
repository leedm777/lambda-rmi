package lambda_rmi

import io.Source
import LambdaRmi.SensorPrxHelper
class InvalidConfigurationException(cfg: Any) extends Exception(cfg.toString)

object App {

  val Rand = new util.Random

  val SensorCount = System.getProperty("lambda-rmi.sensor-count", "50").toInt
  val MaxResponseTimeMillis = 1000


  def main(args: Array[String]) {
    val zipCodesStream = getClass.getClassLoader.getResourceAsStream("zips.txt")
    assert(zipCodesStream != null)

    val zipCodes = Rand.shuffle(
      Source.fromInputStream(zipCodesStream).getLines.toSeq
        .map(_.split(",").toList))

    val sensors = zipCodes
      .take(SensorCount)
      .map{
      case _ :: zip :: state :: city :: lat :: long :: _ :: _ :: Nil =>
        FakeSensor(zip, state, city, lat.toDouble, long.toDouble, MaxResponseTimeMillis)
      case wtf =>
        throw new InvalidConfigurationException(wtf)
    }

    val communicator = Ice.Util.initialize(args);
    val adapter = communicator.createObjectAdapterWithEndpoints(
      "LambdaAdapter", "default -h localhost -p 1099")

    val sensorProxies = sensors.map((v: Ice.Object) =>
      SensorPrxHelper.uncheckedCast(adapter.addWithUUID(v)))

    val fakeSensors = new FakeSensors(sensorProxies.toArray)
    val proxy = adapter.add(fakeSensors, communicator.stringToIdentity("Sensors"))
    println(proxy)
    adapter.activate()

    communicator.waitForShutdown
  }
}