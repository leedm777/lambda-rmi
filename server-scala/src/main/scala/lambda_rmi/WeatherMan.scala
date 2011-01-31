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

import akka.actor._
import xml.XML
import java.net.URL
import akka.stm._

/**
 * An actor that replies with the current temperature (in Celsius) when sent
 * a string Zip code.
 * <p/>
 * Honestly, this is a result of me getting a little distracted playing around
 * with actors and STM from Akka.
 */
class WeatherMan extends Actor {
  /** When true, the current temperature is 25 C universally */
  val GetWeatherFromGoogle = false
  /** Min time to wait between requests. */
  val MinMillisBetweenRequests = 1000
  private val lastSentMillis = Ref(0L)

  def nowMillis = System.nanoTime / 1000000

  def receive = {
    case zip: String =>
      if (GetWeatherFromGoogle) {
        // a lock-free way to wait a minimum amount of time between requests
        // didn't have a big need for this; just wanted to avoid getting
        // blacklisted by Google
        atomic{
          val timeSinceLastRequestMillis: Long = nowMillis - lastSentMillis.get
          val delta = MinMillisBetweenRequests - timeSinceLastRequestMillis
          if (delta > 0) {
            Thread.sleep(delta)
          }
          lastSentMillis.set(nowMillis)
        }
        // quick hack to get the current temperature from Google
        val url = new URL("http://www.google.com/ig/api?weather=" + zip)
        val tempXML = XML.load(url.openConnection.getInputStream)
        val temp = (tempXML \\ "temp_c" \ "@data").toString
        if (temp != "") {
          self.reply(temp.toDouble)
        } else {
          self.reply(25.0)
        }
      } else {
        self.reply(25.0)
      }
    case _ =>
      log.error("Unexpected message")
  }
}

object WeatherMan {
  val actor = Actor.actorOf[WeatherMan].start
}
