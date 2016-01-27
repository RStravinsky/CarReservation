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
  CONSTRAINT `fk_Order_Car` FOREIGN KEY (`idCar`) REFERENCES `car` (`idCar`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `booking`
--

LOCK TABLES `booking` WRITE;
/*!40000 ALTER TABLE `booking` DISABLE KEYS */;
INSERT INTO `booking` VALUES (1,'Emil','Wiśniewski','2016-01-12 13:31:25','2016-01-15 14:00:00',6),(2,'Rafał','Strawiński','2016-01-13 11:37:21','2016-01-16 15:10:11',2),(3,'Jan','Kowalski','2016-01-13 09:00:00','2016-01-13 10:00:00',2),(10,'Bartłomiej','Pokrzywa','2016-01-12 12:24:27','2016-01-13 11:24:27',2),(11,'Rowek','Typu C','2016-01-16 16:30:33','2016-01-18 11:00:33',2),(12,'Marian','Kowalski','2016-01-18 11:59:53','2016-01-19 13:00:53',2),(13,'dd','dd','2016-01-13 09:00:00','2016-01-13 10:00:00',2),(14,'x','x','2016-01-13 09:00:00','2016-01-13 10:00:00',2),(15,'z','z','2016-01-13 09:00:00','2016-01-13 09:00:00',3),(16,'test','test','2016-01-26 12:00:00','2016-01-26 13:00:00',2),(17,'Marcin','Maar','2016-01-26 13:00:30','2016-01-26 14:59:30',2);
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
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `car`
--

LOCK TABLES `car` WRITE;
/*!40000 ALTER TABLE `car` DISABLE KEYS */;
INSERT INTO `car` VALUES (1,'Toyota','Corolla','LU 3456T','2016-01-19','2018-01-20',234400,1,'images/corolla.png'),(2,'Mitsubishi','Lancer','LU 7890M','2016-01-21','2016-01-22',178560,1,'images/lancer.png'),(3,'Mitsubishi','Outlander','LUB 4410','2016-01-23','2016-01-24',112345,0,'images/outlander.png'),(4,'Citroen','Berlingo','LU 1234B','2016-01-25','2016-01-26',275600,1,'images/berlingo.png'),(5,'Citroen','Jumper','LU 2345J','2016-02-27','2016-01-28',300456,0,'images/jumper.png'),(6,'Dodge','Charger','LUB 4567','2016-01-29','2019-01-30',80200,1,'images/charger.png');
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
  `Contents` varchar(500) NOT NULL,
  `Name` varchar(45) NOT NULL,
  `Surename` varchar(45) NOT NULL,
  `Datetime` datetime NOT NULL,
  `isRead` tinyint(1) NOT NULL DEFAULT '0',
  `idCar` int(11) NOT NULL,
  PRIMARY KEY (`idNotes`),
  KEY `fk_notes_car_idx_idx` (`idCar`),
  CONSTRAINT `fk_notes_car_idx` FOREIGN KEY (`idCar`) REFERENCES `car` (`idCar`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=1611 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `notes`
--

LOCK TABLES `notes` WRITE;
/*!40000 ALTER TABLE `notes` DISABLE KEYS */;
INSERT INTO `notes` VALUES (3,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(6,'urwane lusterko','Rafał','Strawiński','2016-01-16 18:23:16',1,2),(10,'hamulce do sprawdzenia','Admin','Admin','2016-01-22 09:49:13',1,2),(11,'niski poziom oleju','Emil','Wiśniewski','2016-01-12 14:11:33',1,2),(12,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',0,2),(13,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(14,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(15,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(16,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(17,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',0,2),(18,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(19,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(20,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',0,2),(21,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',0,2),(22,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',0,2),(23,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(24,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',0,2),(25,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',0,2),(26,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(27,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',0,2),(28,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(29,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(30,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(31,'niski poziom płynu do spryskiwaczy','Bartłomiej','Pokrzywa','2016-01-13 12:13:47',1,2),(1584,'Koło odpadło.','Admin','Admin','2016-01-27 12:15:48',1,2),(1589,'Wpisz treść uwagi','Admin','Admin','2016-01-27 12:39:47',0,6),(1590,'AAAAA','Admin','Admin','2016-01-27 13:25:13',0,1),(1591,'TEST UWAG','Admin','Admin','2016-01-27 13:26:36',0,1),(1592,'UWAGA TEST TEST','Admin','Admin','2016-01-27 13:46:06',0,2),(1593,'GOWNO','Admin','Admin','2016-01-27 13:48:30',0,2),(1594,'JAJKO','Admin','Admin','2016-01-27 13:52:14',0,3),(1595,'csdsdasdsa','Admin','Admin','2016-01-27 13:57:41',0,2),(1596,'aaaalllllaaaaa','Admin','Admin','2016-01-27 13:59:42',0,2),(1597,'sdasdasdsadsadasdasdsa','Admin','Admin','2016-01-27 14:00:34',0,2),(1598,'lalalalala','Admin','Admin','2016-01-27 14:06:10',0,4),(1599,'ddddd','Admin','Admin','2016-01-27 14:08:15',0,4),(1601,'dfdssfg77777777777777','Admin','Admin','2016-01-27 14:12:13',0,3),(1602,'xxxxxxxx','Admin','Admin','2016-01-27 14:15:40',0,5),(1603,'Wpisz treść uwagi','Admin','Admin','2016-01-27 14:19:03',0,5),(1604,'CO???','Admin','Admin','2016-01-27 14:23:45',0,2),(1605,'sadasddasddhuji','Admin','Admin','2016-01-27 14:25:29',0,2),(1606,'Wpisz treść uwagi','Admin','Admin','2016-01-27 14:27:18',0,3),(1607,'UUUUUUUUUUUUUUUUWAFA','Admin','Admin','2016-01-27 14:30:27',0,3),(1608,'jacek','Admin','Admin','2016-01-27 14:32:35',0,4),(1609,'WARIAT','Admin','Admin','2016-01-27 14:40:54',0,6),(1610,'ffff','Admin','Admin','2016-01-27 14:44:38',0,4);
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

-- Dump completed on 2016-01-27 14:47:19
