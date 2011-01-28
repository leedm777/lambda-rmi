package lambda_rmi

import akka.actor._
import xml.XML
import java.net.URL
import akka.stm._

class WeatherMan extends Actor {
  val MinMillisBetweenRequests = 1000
  val lastSentMillis = Ref(0L)

  def nowMillis = System.nanoTime / 1000000

  var fast = true

  def receive = {
    case zip: String =>

      if (fast) {
        self.reply(25.0)
      } else {
        atomic{
          // wait between requests
          val delta = MinMillisBetweenRequests - (nowMillis - lastSentMillis.get)
          if (delta > 0) {
            Thread.sleep(delta)
          }
          lastSentMillis.set(nowMillis)
        }
        val url = new URL("http://www.google.com/ig/api?weather=" + zip)
        val tempXML = XML.load(url.openConnection.getInputStream)
        val temp = (tempXML \\ "temp_c" \ "@data").toString
        if (temp != "") {
          self.reply(temp.toDouble)
        } else {
          self.reply(25.0)
        }
      }
    case _ =>
      log.error("Unexpected message")
  }
}

object WeatherMan {
  val actor = Actor.actorOf[WeatherMan].start
}