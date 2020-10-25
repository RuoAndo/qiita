import akka.actor.Actor
import akka.actor.{ActorSystem,Props}

class HelloActor extends Actor{
  def receive = {
    case "Hello" => println("World")
  }
}

object HelloActor {
  def main(args: Array[String]): Unit = {
    println("Hello World!")

    val system = ActorSystem("system")
    val actor = system.actorOf(Props[HelloActor])

    actor ! "Hello"
  }
}

