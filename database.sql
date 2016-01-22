CREATE DATABASE  IF NOT EXISTS `sigmacars` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `sigmacars`;
-- MySQL dump 10.13  Distrib 5.6.17, for Win32 (x86)
--
-- Host: 192.168.1.7    Database: sigmacars
-- ------------------------------------------------------
-- Server version	5.6.23-log

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
-- Table structure for table `booking`
--

DROP TABLE IF EXISTS `booking`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `booking` (
  `idBooking` int(11) NOT NULL AUTO_INCREMENT,
  `Name` varchar(45) NOT NULL,
  `Surename` varchar(45) NOT NULL,
  `Begin` datetime NOT NULL,
  `End` datetime NOT NULL,
  `idCar` int(11) NOT NULL,
  PRIMARY KEY (`idBooking`),
  KEY `fk_Order_Car_idx` (`idCar`),
  CONSTRAINT `fk_Order_Car` FOREIGN KEY (`idCar`) REFERENCES `car` (`idCar`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `booking`
--

LOCK TABLES `booking` WRITE;
/*!40000 ALTER TABLE `booking` DISABLE KEYS */;
INSERT INTO `booking` VALUES (1,'Emil','Wiśniewski','2016-01-12 13:31:25','2016-01-15 14:00:00',6),(2,'Rafał','Strawiński','2016-01-13 11:37:21','2016-01-16 15:10:11',2),(3,'Jan','Kowalski','2016-01-13 09:00:00','2016-01-13 10:00:00',2),(10,'Bartłomiej','Pokrzywa','2016-01-12 12:24:27','2016-01-13 11:24:27',2);
/*!40000 ALTER TABLE `booking` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `car`
--

DROP TABLE IF EXISTS `car`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `car` (
  `idCar` int(11) NOT NULL AUTO_INCREMENT,
  `Brand` varchar(45) NOT NULL,
  `Model` varchar(45) NOT NULL,
  `LicensePlate` varchar(10) NOT NULL,
  `InspectionDate` date NOT NULL,
  `InsuranceDate` date NOT NULL,
  `Mileage` int(11) NOT NULL,
  `Status` tinyint(1) NOT NULL DEFAULT '0',
  `PhotoPath` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`idCar`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `car`
--

LOCK TABLES `car` WRITE;
/*!40000 ALTER TABLE `car` DISABLE KEYS */;
INSERT INTO `car` VALUES (1,'Toyota','Corolla','LU 3456T','2016-01-19','2016-01-20',234400,1,'images/corolla.png'),(2,'Mitsubishi','Lancer','LU 7890M','2016-01-21','2016-01-22',178560,0,'images/lancer.png'),(3,'Mitsubishi','Outlander','LUB 4410','2016-01-23','2016-01-24',112345,1,'images/outlander.png'),(4,'Citroen','Berlingo','LU 1234B','2016-01-25','2016-01-26',275600,0,'images/berlingo.png'),(5,'Citroen','Jumper','LU 2345J','2016-01-27','2016-01-28',300456,0,'images/jumper.png'),(6,'Dodge','Charger','LUB 4567','2016-01-29','2016-01-30',80200,1,'images/charger.png');
/*!40000 ALTER TABLE `car` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `history`
--

DROP TABLE IF EXISTS `history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `history` (
  `idHistory` int(11) NOT NULL AUTO_INCREMENT,
  `Name` varchar(45) NOT NULL,
  `Surename` varchar(45) NOT NULL,
  `Begin` datetime NOT NULL,
  `End` datetime DEFAULT NULL,
  `idCar` int(11) NOT NULL,
  PRIMARY KEY (`idHistory`),
  KEY `fk_history_car_idx` (`idCar`),
  CONSTRAINT `fk_history_car` FOREIGN KEY (`idCar`) REFERENCES `car` (`idCar`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `history`
--

LOCK TABLES `history` WRITE;
/*!40000 ALTER TABLE `history` DISABLE KEYS */;
INSERT INTO `history` VALUES (1,'Emil','Wiśniewski','2016-01-12 13:35:00','2016-01-15 14:05:00',6),(2,'Rafał','Strawiński','2016-01-13 11:35:00','2016-01-16 15:05:00',2);
/*!40000 ALTER TABLE `history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `notes`
--

DROP TABLE IF EXISTS `notes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `notes` (
  `idNotes` int(11) NOT NULL AUTO_INCREMENT,
  `Contents` varchar(200) NOT NULL,
  `Name` varchar(45) NOT NULL,
  `Surename` varchar(45) NOT NULL,
  `DateTime` datetime NOT NULL,
  `idCar` int(11) NOT NULL,
  PRIMARY KEY (`idNotes`),
  KEY `fk_notes_car_idx_idx` (`idCar`),
  CONSTRAINT `fk_notes_car_idx` FOREIGN KEY (`idCar`) REFERENCES `car` (`idCar`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `notes`
--

LOCK TABLES `notes` WRITE;
/*!40000 ALTER TABLE `notes` DISABLE KEYS */;
INSERT INTO `notes` VALUES (3,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',2),(4,'nie działa nawiew','Jan','Kowalski','2016-01-14 13:14:23',2),(5,'spalona żarówka w bagażniku','Marek','Nowak','2016-01-15 14:15:28',2),(6,'urwane lusterko','Rafał','Strawiński','2016-01-16 18:23:16',2);
/*!40000 ALTER TABLE `notes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'sigmacars'
--

--
-- Dumping routines for database 'sigmacars'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-01-21 14:51:14
