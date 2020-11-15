import akka.actor.ActorSystem
import akka.actor.Props
import akka.actor.Actor
import akka.event.Logging

class Actor_with_String extends Actor {
  val log = Logging(context.system, this)

  def receive = {
    case s: String => {
      log.info(s)
    }
    case _ => {
    }
  }
}

object Main {
  def main(args: Array[String]): Unit = {
    val system = ActorSystem("mySystem")

    val props = Props[Actor_with_String]
    val actor = system.actorOf(props, name = "myActor")

    while(true) {
      Thread.sleep(1000)
      actor ! "hi"
    }
  }
}