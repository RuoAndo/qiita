import akka.actor._

/**
 * Intercepting Start/Stop of Actor
 */
class HandlingActor extends Actor {
  // Hooking start
  override def preStart() =
    println("preStart")

  // Hooking stop
  override def postStop() =
    println("postStop")

  def receive = {
    case s: String => println("Received! Hello " + s + " !")
    case _ => throw new IllegalArgumentException
  }
}

object ActorHandlingSample extends App {
  val system = ActorSystem()
  val ref = system.actorOf(Props[HandlingActor], name = "mainActor")

  // Sending Messages
  ref ! "Sender"

  system.terminate()
}