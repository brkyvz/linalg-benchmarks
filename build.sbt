// Your sbt build file. Guides on how to write one can be found at
// http://www.scala-sbt.org/0.13/docs/index.html

scalaVersion := "2.10.4"

sparkVersion := "1.2.1"

sparkPackageName := "brkyvz/matrix-operations"

// Don't forget to set the version
version := "0.0.1"

// All Spark Packages need a license
licenses := Seq("Apache-2.0" -> url("http://opensource.org/licenses/Apache-2.0"))

libraryDependencies += "com.github.scopt" %% "scopt" % "3.3.0"

libraryDependencies += "com.github.fommil.netlib" % "all" % "1.1.2" pomOnly()

resolvers += Resolver.sonatypeRepo("public")

// Add Spark components this package depends on, e.g, "mllib", ....
sparkComponents ++= Seq("mllib", "sql")

mainClass in (Compile, run) := Some("brkyvz.linalg.Main")

// uncomment and change the value below to change the directory where your zip artifact will be created
// spDistDirectory := target.value

// add any sparkPackageDependencies using sparkPackageDependencies.
// e.g. sparkPackageDependencies += "databricks/spark-avro:0.1"
