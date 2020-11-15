import akka.actor.ActorSystem
import akka.actor.Props
import akka.actor.Actor
import akka.event.Logging

case class CaseClass_for_Actor(prop1: Int, prop2: String)

class Actor_with_CaseClass extends Actor {
  val log = Logging(context.system, this)

  def receive = {
    case CaseClass_for_Actor(prop1, prop2) => {
      log.info("prop1: %d, prop2: %s" format (prop1, prop2))
    }
    case _ => {
    }
  }
}

object Main {
  def main(args: Array[String]): Unit = {
    val system = ActorSystem("mySystem")

    val props = Props[Actor_with_CaseClass]
    val actor = system.actorOf(props, name = "myActor")

    val msg = new CaseClass_for_Actor(987654321, "any string")

    while(true) {
      Thread.sleep(1000)
      actor ! msg
    }
  }
}