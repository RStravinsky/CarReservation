CREATE DATABASE  IF NOT EXISTS `sigmacars` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `sigmacars`;
-- MySQL dump 10.13  Distrib 5.6.27, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: sigmacars
-- ------------------------------------------------------
-- Server version	5.6.27-0ubuntu1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Car`
--

DROP TABLE IF EXISTS `Car`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Car` (
  `idCar` int(11) NOT NULL AUTO_INCREMENT,
  `Brand` varchar(45) NOT NULL,
  `Model` varchar(45) NOT NULL,
  `InspectionDate` date NOT NULL,
  `InsuranceDate` date NOT NULL,
  `Mileage` int(11) NOT NULL,
  `Notes` varchar(400) DEFAULT NULL,
  `Status` bit(1) NOT NULL DEFAULT b'0',
  `PhotoPath` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`idCar`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Car`
--

LOCK TABLES `Car` WRITE;
/*!40000 ALTER TABLE `Car` DISABLE KEYS */;
INSERT INTO `Car` VALUES (1,'Toyota','Corolla','2016-01-19','2016-01-20',234400,'Toyota Corolla notes','','home/bartek/Dokumenty/SoftwareDev/QtApps/SigmaCars/images/corolla.jpg'),(2,'Mitsubishi','Lancer','2016-01-21','2016-01-22',178560,'Mitsubishi Lancer notes','\0','home/bartek/Dokumenty/SoftwareDev/QtApps/SigmaCars/images/lancer.jpg'),(3,'Mitsubishi','Outlander','2016-01-23','2016-01-24',112345,'Mitsubishi Outlander notes','','home/bartek/Dokumenty/SoftwareDev/QtApps/SigmaCars/images/outlander.jpg'),(4,'Citroen','Berlingo','2016-01-25','2016-01-26',275600,'Citroen Berlingo notes','\0','home/bartek/Dokumenty/SoftwareDev/QtApps/SigmaCars/images/berlingo.jpg'),(5,'Citroen','Jumper','2016-01-27','2016-01-28',300456,'Citroen Jumper notes','\0','home/bartek/Dokumenty/SoftwareDev/QtApps/SigmaCars/images/jumper.jpg'),(6,'Dodge','Charger','2016-01-29','2016-01-30',80200,'Dodge Charger notes','','home/bartek/Dokumenty/SoftwareDev/QtApps/SigmaCars/images/charger.jpg');
/*!40000 ALTER TABLE `Car` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Order`
--

DROP TABLE IF EXISTS `Order`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Order` (
  `idOrder` int(11) NOT NULL AUTO_INCREMENT,
  `Name` varchar(45) NOT NULL,
  `Surename` varchar(45) NOT NULL,
  `Begin` datetime NOT NULL,
  `End` datetime NOT NULL,
  `idCar` int(11) NOT NULL,
  PRIMARY KEY (`idOrder`),
  KEY `fk_Order_Car_idx` (`idCar`),
  CONSTRAINT `fk_Order_Car` FOREIGN KEY (`idCar`) REFERENCES `Car` (`idCar`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Order`
--

LOCK TABLES `Order` WRITE;
/*!40000 ALTER TABLE `Order` DISABLE KEYS */;
/*!40000 ALTER TABLE `Order` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-01-13 23:44:59
