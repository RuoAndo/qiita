import org.apache.spark.SparkContext

object Main {
  def main(args: Array[String]) {

      val sc = new SparkContext("local[*]", "App")
      //val sc = new SparkContext("h-gpu05:9000", "App") 

      val rddFromFile = sc.textFile("hdfs://h-gpu05:9000/home/hadoop/test2/random_data.txt")
      // val rddFromFile = sc.textFile("hdfs:/test2/random_data.txt")
      // val rddFromFile = sc.textFile("random_data.txt")	
      // val rddFromFile = new sparkContext.textFile("hdfs://h-gpu05:9000/hadoop/random_data.txt")

      val rdd = rddFromFile.map(f=>{
			f.split(",")
      })

      rdd.foreach(f=>{
      println("Col1: "+ f(0) + ",Col2: "+ f(1))
      })

      sc.stop()
  }
}

