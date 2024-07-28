CREATE DATABASE  IF NOT EXISTS `packets` /*!40100 DEFAULT CHARACTER SET utf8 COLLATE utf8_bin */;
USE `packets`;
-- MySQL dump 10.13  Distrib 5.7.36, for Linux (x86_64)
--
-- Host: localhost    Database: packets
-- ------------------------------------------------------
-- Server version	5.7.36

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
-- Table structure for table `auth_group`
--

DROP TABLE IF EXISTS `auth_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(150) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_group`
--

LOCK TABLES `auth_group` WRITE;
/*!40000 ALTER TABLE `auth_group` DISABLE KEYS */;
/*!40000 ALTER TABLE `auth_group` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_group_permissions`
--

DROP TABLE IF EXISTS `auth_group_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_group_permissions` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `group_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `auth_group_permissions_group_id_permission_id_0cd325b0_uniq` (`group_id`,`permission_id`),
  KEY `auth_group_permissio_permission_id_84c5c92e_fk_auth_perm` (`permission_id`),
  CONSTRAINT `auth_group_permissio_permission_id_84c5c92e_fk_auth_perm` FOREIGN KEY (`permission_id`) REFERENCES `auth_permission` (`id`),
  CONSTRAINT `auth_group_permissions_group_id_b120cbf9_fk_auth_group_id` FOREIGN KEY (`group_id`) REFERENCES `auth_group` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_group_permissions`
--

LOCK TABLES `auth_group_permissions` WRITE;
/*!40000 ALTER TABLE `auth_group_permissions` DISABLE KEYS */;
/*!40000 ALTER TABLE `auth_group_permissions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_permission`
--

DROP TABLE IF EXISTS `auth_permission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_permission` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) COLLATE utf8_bin NOT NULL,
  `content_type_id` int(11) NOT NULL,
  `codename` varchar(100) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `auth_permission_content_type_id_codename_01ab375a_uniq` (`content_type_id`,`codename`),
  CONSTRAINT `auth_permission_content_type_id_2f476e4b_fk_django_co` FOREIGN KEY (`content_type_id`) REFERENCES `django_content_type` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=69 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_permission`
--

LOCK TABLES `auth_permission` WRITE;
/*!40000 ALTER TABLE `auth_permission` DISABLE KEYS */;
INSERT INTO `auth_permission` VALUES (1,'Can add log entry',1,'add_logentry'),(2,'Can change log entry',1,'change_logentry'),(3,'Can delete log entry',1,'delete_logentry'),(4,'Can view log entry',1,'view_logentry'),(5,'Can add permission',2,'add_permission'),(6,'Can change permission',2,'change_permission'),(7,'Can delete permission',2,'delete_permission'),(8,'Can view permission',2,'view_permission'),(9,'Can add group',3,'add_group'),(10,'Can change group',3,'change_group'),(11,'Can delete group',3,'delete_group'),(12,'Can view group',3,'view_group'),(13,'Can add user',4,'add_user'),(14,'Can change user',4,'change_user'),(15,'Can delete user',4,'delete_user'),(16,'Can view user',4,'view_user'),(17,'Can add content type',5,'add_contenttype'),(18,'Can change content type',5,'change_contenttype'),(19,'Can delete content type',5,'delete_contenttype'),(20,'Can view content type',5,'view_contenttype'),(21,'Can add session',6,'add_session'),(22,'Can change session',6,'change_session'),(23,'Can delete session',6,'delete_session'),(24,'Can view session',6,'view_session'),(25,'Can add request',7,'add_request'),(26,'Can change request',7,'change_request'),(27,'Can delete request',7,'delete_request'),(28,'Can view request',7,'view_request'),(29,'Can add response',8,'add_response'),(30,'Can change response',8,'change_response'),(31,'Can delete response',8,'delete_response'),(32,'Can view response',8,'view_response'),(33,'Can add whois',9,'add_whois'),(34,'Can change whois',9,'change_whois'),(35,'Can delete whois',9,'delete_whois'),(36,'Can view whois',9,'view_whois'),(37,'Can add domain management',10,'add_domainmanagement'),(38,'Can change domain management',10,'change_domainmanagement'),(39,'Can delete domain management',10,'delete_domainmanagement'),(40,'Can view domain management',10,'view_domainmanagement'),(41,'Can add permissions',11,'add_permissions'),(42,'Can change permissions',11,'change_permissions'),(43,'Can delete permissions',11,'delete_permissions'),(44,'Can view permissions',11,'view_permissions'),(45,'Can add role',12,'add_role'),(46,'Can change role',12,'change_role'),(47,'Can delete role',12,'delete_role'),(48,'Can view role',12,'view_role'),(49,'Can add role permissons',13,'add_rolepermissons'),(50,'Can change role permissons',13,'change_rolepermissons'),(51,'Can delete role permissons',13,'delete_rolepermissons'),(52,'Can view role permissons',13,'view_rolepermissons'),(53,'Can add user',14,'add_user'),(54,'Can change user',14,'change_user'),(55,'Can delete user',14,'delete_user'),(56,'Can view user',14,'view_user'),(57,'Can add user role',15,'add_userrole'),(58,'Can change user role',15,'change_userrole'),(59,'Can delete user role',15,'delete_userrole'),(60,'Can view user role',15,'view_userrole'),(61,'Can add consumer',16,'add_consumer'),(62,'Can change consumer',16,'change_consumer'),(63,'Can delete consumer',16,'delete_consumer'),(64,'Can view consumer',16,'view_consumer'),(65,'Can add consumer role',17,'add_consumerrole'),(66,'Can change consumer role',17,'change_consumerrole'),(67,'Can delete consumer role',17,'delete_consumerrole'),(68,'Can view consumer role',17,'view_consumerrole');
/*!40000 ALTER TABLE `auth_permission` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_user`
--

DROP TABLE IF EXISTS `auth_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `password` varchar(128) COLLATE utf8_bin NOT NULL,
  `last_login` datetime(6) DEFAULT NULL,
  `is_superuser` tinyint(1) NOT NULL,
  `username` varchar(150) COLLATE utf8_bin NOT NULL,
  `first_name` varchar(150) COLLATE utf8_bin NOT NULL,
  `last_name` varchar(150) COLLATE utf8_bin NOT NULL,
  `email` varchar(254) COLLATE utf8_bin NOT NULL,
  `is_staff` tinyint(1) NOT NULL,
  `is_active` tinyint(1) NOT NULL,
  `date_joined` datetime(6) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_user`
--

LOCK TABLES `auth_user` WRITE;
/*!40000 ALTER TABLE `auth_user` DISABLE KEYS */;
/*!40000 ALTER TABLE `auth_user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_user_groups`
--

DROP TABLE IF EXISTS `auth_user_groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user_groups` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `group_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `auth_user_groups_user_id_group_id_94350c0c_uniq` (`user_id`,`group_id`),
  KEY `auth_user_groups_group_id_97559544_fk_auth_group_id` (`group_id`),
  CONSTRAINT `auth_user_groups_group_id_97559544_fk_auth_group_id` FOREIGN KEY (`group_id`) REFERENCES `auth_group` (`id`),
  CONSTRAINT `auth_user_groups_user_id_6a12ed8b_fk_auth_user_id` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_user_groups`
--

LOCK TABLES `auth_user_groups` WRITE;
/*!40000 ALTER TABLE `auth_user_groups` DISABLE KEYS */;
/*!40000 ALTER TABLE `auth_user_groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `auth_user_user_permissions`
--

DROP TABLE IF EXISTS `auth_user_user_permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `auth_user_user_permissions` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `permission_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `auth_user_user_permissions_user_id_permission_id_14a6b632_uniq` (`user_id`,`permission_id`),
  KEY `auth_user_user_permi_permission_id_1fbb5f2c_fk_auth_perm` (`permission_id`),
  CONSTRAINT `auth_user_user_permi_permission_id_1fbb5f2c_fk_auth_perm` FOREIGN KEY (`permission_id`) REFERENCES `auth_permission` (`id`),
  CONSTRAINT `auth_user_user_permissions_user_id_a95ead1b_fk_auth_user_id` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `auth_user_user_permissions`
--

LOCK TABLES `auth_user_user_permissions` WRITE;
/*!40000 ALTER TABLE `auth_user_user_permissions` DISABLE KEYS */;
/*!40000 ALTER TABLE `auth_user_user_permissions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `consumer`
--

DROP TABLE IF EXISTS `consumer`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `consumer` (
  `id` int(11) NOT NULL,
  `name` varchar(10) COLLATE utf8_bin NOT NULL,
  `account` varchar(10) COLLATE utf8_bin NOT NULL,
  `password` varchar(10) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `consumer`
--

LOCK TABLES `consumer` WRITE;
/*!40000 ALTER TABLE `consumer` DISABLE KEYS */;
INSERT INTO `consumer` VALUES (1,'张三','manager','123'),(2,'李四','comsumer','123');
/*!40000 ALTER TABLE `consumer` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `consumer_role`
--

DROP TABLE IF EXISTS `consumer_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `consumer_role` (
  `id` int(11) NOT NULL,
  `consumer_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_consumer_role_1_idx` (`consumer_id`),
  KEY `fk_consumer_role_2_idx` (`role_id`),
  CONSTRAINT `fk_consumer_role_1` FOREIGN KEY (`consumer_id`) REFERENCES `consumer` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_consumer_role_2` FOREIGN KEY (`role_id`) REFERENCES `role` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `consumer_role`
--

LOCK TABLES `consumer_role` WRITE;
/*!40000 ALTER TABLE `consumer_role` DISABLE KEYS */;
INSERT INTO `consumer_role` VALUES (1,1,1),(2,2,2);
/*!40000 ALTER TABLE `consumer_role` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `django_admin_log`
--

DROP TABLE IF EXISTS `django_admin_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_admin_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `action_time` datetime(6) NOT NULL,
  `object_id` longtext COLLATE utf8_bin,
  `object_repr` varchar(200) COLLATE utf8_bin NOT NULL,
  `action_flag` smallint(5) unsigned NOT NULL,
  `change_message` longtext COLLATE utf8_bin NOT NULL,
  `content_type_id` int(11) DEFAULT NULL,
  `user_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `django_admin_log_content_type_id_c4bce8eb_fk_django_co` (`content_type_id`),
  KEY `django_admin_log_user_id_c564eba6_fk_auth_user_id` (`user_id`),
  CONSTRAINT `django_admin_log_content_type_id_c4bce8eb_fk_django_co` FOREIGN KEY (`content_type_id`) REFERENCES `django_content_type` (`id`),
  CONSTRAINT `django_admin_log_user_id_c564eba6_fk_auth_user_id` FOREIGN KEY (`user_id`) REFERENCES `auth_user` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_admin_log`
--

LOCK TABLES `django_admin_log` WRITE;
/*!40000 ALTER TABLE `django_admin_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `django_admin_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `django_content_type`
--

DROP TABLE IF EXISTS `django_content_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_content_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `app_label` varchar(100) COLLATE utf8_bin NOT NULL,
  `model` varchar(100) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `django_content_type_app_label_model_76bd3d3b_uniq` (`app_label`,`model`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_content_type`
--

LOCK TABLES `django_content_type` WRITE;
/*!40000 ALTER TABLE `django_content_type` DISABLE KEYS */;
INSERT INTO `django_content_type` VALUES (1,'admin','logentry'),(3,'auth','group'),(2,'auth','permission'),(4,'auth','user'),(5,'contenttypes','contenttype'),(10,'packets','domainmanagement'),(7,'packets','request'),(8,'packets','response'),(9,'packets','whois'),(16,'rbac','consumer'),(17,'rbac','consumerrole'),(11,'rbac','permissions'),(12,'rbac','role'),(13,'rbac','rolepermissons'),(14,'rbac','user'),(15,'rbac','userrole'),(6,'sessions','session');
/*!40000 ALTER TABLE `django_content_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `django_migrations`
--

DROP TABLE IF EXISTS `django_migrations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_migrations` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `app` varchar(255) COLLATE utf8_bin NOT NULL,
  `name` varchar(255) COLLATE utf8_bin NOT NULL,
  `applied` datetime(6) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_migrations`
--

LOCK TABLES `django_migrations` WRITE;
/*!40000 ALTER TABLE `django_migrations` DISABLE KEYS */;
INSERT INTO `django_migrations` VALUES (1,'contenttypes','0001_initial','2022-02-19 05:06:52.960908'),(2,'auth','0001_initial','2022-02-19 05:06:53.113134'),(3,'admin','0001_initial','2022-02-19 05:06:53.153892'),(4,'admin','0002_logentry_remove_auto_add','2022-02-19 05:06:53.160877'),(5,'admin','0003_logentry_add_action_flag_choices','2022-02-19 05:06:53.169204'),(6,'contenttypes','0002_remove_content_type_name','2022-02-19 05:06:53.213172'),(7,'auth','0002_alter_permission_name_max_length','2022-02-19 05:06:53.231903'),(8,'auth','0003_alter_user_email_max_length','2022-02-19 05:06:53.252602'),(9,'auth','0004_alter_user_username_opts','2022-02-19 05:06:53.260997'),(10,'auth','0005_alter_user_last_login_null','2022-02-19 05:06:53.282032'),(11,'auth','0006_require_contenttypes_0002','2022-02-19 05:06:53.285603'),(12,'auth','0007_alter_validators_add_error_messages','2022-02-19 05:06:53.294768'),(13,'auth','0008_alter_user_username_max_length','2022-02-19 05:06:53.319832'),(14,'auth','0009_alter_user_last_name_max_length','2022-02-19 05:06:53.341557'),(15,'auth','0010_alter_group_name_max_length','2022-02-19 05:06:53.360362'),(16,'auth','0011_update_proxy_permissions','2022-02-19 05:06:53.366987'),(17,'auth','0012_alter_user_first_name_max_length','2022-02-19 05:06:53.388685'),(18,'sessions','0001_initial','2022-02-19 05:06:53.405678'),(19,'packets','0001_initial','2022-02-19 05:23:15.356468'),(20,'packets','0002_domainmanagement','2022-02-20 04:05:42.450935'),(21,'rbac','0001_initial','2022-02-20 09:45:10.507578'),(22,'rbac','0002_consumer_consumerrole_delete_user_delete_userrole','2022-02-20 11:36:13.181472');
/*!40000 ALTER TABLE `django_migrations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `django_session`
--

DROP TABLE IF EXISTS `django_session`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `django_session` (
  `session_key` varchar(40) COLLATE utf8_bin NOT NULL,
  `session_data` longtext COLLATE utf8_bin NOT NULL,
  `expire_date` datetime(6) NOT NULL,
  PRIMARY KEY (`session_key`),
  KEY `django_session_expire_date_a5c62663` (`expire_date`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `django_session`
--

LOCK TABLES `django_session` WRITE;
/*!40000 ALTER TABLE `django_session` DISABLE KEYS */;
INSERT INTO `django_session` VALUES ('wewlvuy4ghxb8vdax8xqv7649c6lj8jz','eyJyb2xlIjoxfQ:1nLlNh:Dxn0xtBBbOTAU3yCYClxH9NkxE9eXvrLnqJpXy4yyi0','2022-03-06 12:32:01.435438');
/*!40000 ALTER TABLE `django_session` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `domain_management`
--

DROP TABLE IF EXISTS `domain_management`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `domain_management` (
  `domain_name` varchar(100) COLLATE utf8_bin NOT NULL,
  `registrant_organization` varchar(100) COLLATE utf8_bin DEFAULT NULL,
  `domain_type` varchar(50) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`domain_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `domain_management`
--

LOCK TABLES `domain_management` WRITE;
/*!40000 ALTER TABLE `domain_management` DISABLE KEYS */;
INSERT INTO `domain_management` VALUES ('BAIDU.COM','Beijing Baidu Netcom Science Technology Co., Ltd.','科技公司（不含游戏）'),('MSFTCONNECTTEST.COM','Microsoft Corporation','科技公司（不含游戏）'),('get.sogou.com','none','政府事业单位（不含高校）');
/*!40000 ALTER TABLE `domain_management` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `permissions`
--

DROP TABLE IF EXISTS `permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permissions` (
  `id` int(11) NOT NULL,
  `name` varchar(100) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `permissions`
--

LOCK TABLES `permissions` WRITE;
/*!40000 ALTER TABLE `permissions` DISABLE KEYS */;
INSERT INTO `permissions` VALUES (1,'request_list'),(2,'show_host'),(3,'response_list'),(4,'show_payload'),(5,'request_sourceip_number'),(6,'request_type'),(7,'destinationip_number'),(8,'response_content_type'),(9,'specify_host_type'),(10,'alter_host_type'),(11,'show_host_type'),(12,'request_frame_size'),(13,'response_frame_size'),(14,'analysis_webpage'),(15,'account_management'),(16,'create_consumer'),(17,'create_role'),(18,'create_permissions');
/*!40000 ALTER TABLE `permissions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `request`
--

DROP TABLE IF EXISTS `request`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `request` (
  `number` int(11) NOT NULL,
  `source_ip` varchar(20) NOT NULL,
  `destination_ip` varchar(20) NOT NULL,
  `source_port` int(11) NOT NULL,
  `destination_port` int(11) NOT NULL,
  `host` varchar(100) NOT NULL,
  `url` varchar(500) NOT NULL,
  `user_agent` varchar(100) NOT NULL,
  `cookie` varchar(500) DEFAULT NULL,
  `frame_size` int(11) NOT NULL,
  `file_path` varchar(100) CHARACTER SET utf8 NOT NULL,
  PRIMARY KEY (`number`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `request`
--

LOCK TABLES `request` WRITE;
/*!40000 ALTER TABLE `request` DISABLE KEYS */;
INSERT INTO `request` VALUES (1,'172.23.198.220','42.99.128.161',59239,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',246,'/home/ricardo/桌面/RequestPackets/1.txt'),(3,'172.23.198.220','39.156.66.18',59232,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/3.txt'),(4,'172.23.198.220','42.99.128.161',59248,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/4.txt'),(7,'172.23.198.220','39.156.66.18',59232,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/7.txt'),(8,'172.23.198.220','42.99.128.162',59263,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/8.txt'),(9,'172.23.198.220','39.156.66.18',59232,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/9.txt'),(10,'172.23.198.220','42.99.128.162',59267,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/10.txt'),(12,'172.23.198.220','104.91.69.32',59272,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/12.txt'),(13,'172.23.198.220','39.156.66.18',59279,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/13.txt'),(14,'172.23.198.220','39.156.66.18',59232,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/14.txt'),(15,'172.23.198.220','39.156.66.18',59232,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/15.txt'),(16,'172.23.198.220','42.99.128.161',59280,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/16.txt'),(17,'172.23.198.220','39.156.66.18',59232,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/17.txt'),(19,'172.23.198.220','39.156.66.18',59279,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/19.txt'),(20,'172.23.198.220','42.99.128.162',59293,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/20.txt'),(21,'172.23.198.220','42.99.128.161',59299,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/21.txt'),(22,'172.23.198.220','61.149.9.164',59302,80,'serveraddr.service.kugou.com','serveraddr.service.kugou.com','none','none',247,'/home/ricardo/桌面/RequestPackets/22.txt'),(23,'172.23.198.220','39.156.66.18',59279,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/23.txt'),(26,'172.23.198.220','49.7.115.37',59305,80,'get.sogou.com','get.sogou.com','sogou_ime','none',1227,'/home/ricardo/桌面/RequestPackets/26.txt'),(27,'172.23.198.220','42.99.128.161',59306,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/27.txt'),(29,'172.23.198.220','42.99.128.162',59312,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/29.txt'),(32,'172.23.198.220','39.156.66.18',59279,80,'www.baidu.com','www.baidu.com','Mozilla/5.0','BAIDUID=74B6C733B1BDDDA6553CC0B57EE2B7F3:FG=1',307,'/home/ricardo/桌面/RequestPackets/32.txt'),(34,'172.23.198.220','42.99.128.162',59323,80,'www.msftconnecttest.com','www.msftconnecttest.com/connecttest.txt','Microsoft NCSI','none',203,'/home/ricardo/桌面/RequestPackets/34.txt'),(37,'172.23.198.220','110.249.194.78',59326,80,'shuc-pc-hunt.ksord.com','shuc-pc-hunt.ksord.com','none','none',236,'/home/ricardo/桌面/RequestPackets/37.txt');
/*!40000 ALTER TABLE `request` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `response`
--

DROP TABLE IF EXISTS `response`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `response` (
  `number` int(11) NOT NULL,
  `source_ip` varchar(20) COLLATE utf8_bin NOT NULL,
  `destination_ip` varchar(20) COLLATE utf8_bin NOT NULL,
  `source_port` int(11) NOT NULL,
  `destination_port` int(11) NOT NULL,
  `content_type` varchar(100) COLLATE utf8_bin NOT NULL,
  `frame_size` int(11) NOT NULL,
  `file_path` varchar(100) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`number`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `response`
--

LOCK TABLES `response` WRITE;
/*!40000 ALTER TABLE `response` DISABLE KEYS */;
INSERT INTO `response` VALUES (1,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/1.html'),(2,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/2.html'),(3,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/3.html'),(4,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/4.html'),(5,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/5.html'),(6,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/6.html'),(7,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/7.html'),(8,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/8.html'),(9,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/9.html'),(10,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/10.html'),(11,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/11.html'),(12,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/12.html'),(13,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/13.html'),(14,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/14.html'),(15,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/15.html'),(16,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/16.html'),(17,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/17.html'),(18,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/18.html'),(19,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/19.html'),(20,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/20.html'),(21,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/21.html'),(22,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/22.html'),(23,'111.13.142.195','172.24.60.217',80,50808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/23.html'),(24,'203.100.92.228','172.24.60.217',80,54051,'text/html',644,'/home/ricardo/桌面/ResponsePackets/24.html'),(25,'203.100.92.228','172.24.60.217',80,59071,'text/html',644,'/home/ricardo/桌面/ResponsePackets/25.html'),(26,'39.156.150.196','172.24.60.217',80,53812,'text/html',644,'/home/ricardo/桌面/ResponsePackets/26.html'),(28,'39.156.150.196','172.24.60.217',80,53812,'text/html',644,'/home/ricardo/桌面/ResponsePackets/28.html'),(30,'203.100.92.228','172.24.60.217',80,40964,'text/html',644,'/home/ricardo/桌面/ResponsePackets/30.html'),(33,'111.13.142.195','172.24.60.217',80,45594,'text/html',644,'/home/ricardo/桌面/ResponsePackets/33.html'),(34,'111.13.142.195','172.24.60.217',80,47733,'text/html',644,'/home/ricardo/桌面/ResponsePackets/34.html'),(37,'203.100.92.228','172.24.60.217',80,56351,'text/html',644,'/home/ricardo/桌面/ResponsePackets/37.html'),(38,'39.156.66.14','172.23.198.220',80,59919,'text/plain',367,'/home/ricardo/桌面/ResponsePackets/38.txt'),(39,'203.100.92.228','172.24.60.217',80,36808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/39.html'),(40,'203.100.92.228','172.24.60.217',80,36808,'text/html',644,'/home/ricardo/桌面/ResponsePackets/40.html'),(42,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/42.html'),(43,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/43.html'),(44,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/44.html'),(45,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/45.html'),(46,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/46.html'),(47,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/47.html'),(48,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/48.html'),(49,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/49.html'),(50,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/50.html'),(51,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/51.html'),(52,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/52.html'),(53,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/53.html'),(54,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/54.html'),(55,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/55.html'),(56,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/56.html'),(57,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/57.html'),(58,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/58.html'),(59,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/59.html'),(60,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/60.html'),(61,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/61.html'),(62,'111.13.142.142','172.24.60.217',80,48720,'text/html',644,'/home/ricardo/桌面/ResponsePackets/62.html');
/*!40000 ALTER TABLE `response` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `role`
--

DROP TABLE IF EXISTS `role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `role` (
  `id` int(11) NOT NULL,
  `name` varchar(10) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `role`
--

LOCK TABLES `role` WRITE;
/*!40000 ALTER TABLE `role` DISABLE KEYS */;
INSERT INTO `role` VALUES (1,'管理员'),(2,'普通用户');
/*!40000 ALTER TABLE `role` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `role_permissons`
--

DROP TABLE IF EXISTS `role_permissons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `role_permissons` (
  `id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  `permissons_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_role_permissons_1_idx` (`role_id`),
  KEY `fk_role_permissons_2_idx` (`permissons_id`),
  CONSTRAINT `fk_role_permissons_1` FOREIGN KEY (`role_id`) REFERENCES `role` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_role_permissons_2` FOREIGN KEY (`permissons_id`) REFERENCES `permissions` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `role_permissons`
--

LOCK TABLES `role_permissons` WRITE;
/*!40000 ALTER TABLE `role_permissons` DISABLE KEYS */;
INSERT INTO `role_permissons` VALUES (1,1,1),(2,1,2),(3,1,3),(4,1,4),(5,1,5),(6,1,6),(7,1,7),(8,1,8),(9,1,9),(10,1,10),(11,1,11),(12,1,12),(13,1,13),(14,1,14),(15,1,15),(16,1,16),(17,1,17),(18,1,18),(19,2,1),(20,2,2),(21,2,3),(22,2,4),(23,2,5),(24,2,6),(25,2,7),(26,2,8),(27,2,11),(28,2,14);
/*!40000 ALTER TABLE `role_permissons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `whois`
--

DROP TABLE IF EXISTS `whois`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `whois` (
  `domain_name` varchar(100) COLLATE utf8_bin NOT NULL,
  `registrant_organization` varchar(100) COLLATE utf8_bin DEFAULT NULL,
  `registrar` varchar(100) COLLATE utf8_bin DEFAULT NULL,
  `registrar_phone` varchar(50) COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`domain_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `whois`
--

LOCK TABLES `whois` WRITE;
/*!40000 ALTER TABLE `whois` DISABLE KEYS */;
INSERT INTO `whois` VALUES ('BAIDU.COM','Beijing Baidu Netcom Science Technology Co., Ltd.','MarkMonitor Inc.','+1.2083895740'),('MSFTCONNECTTEST.COM','Microsoft Corporation','Nom-iq Ltd. dba COM LAUDE','+442074218250'),('get.sogou.com','none','none','none'),('serveraddr.service.kugou.com','none','none','none'),('shuc-pc-hunt.ksord.com','none','none','none');
/*!40000 ALTER TABLE `whois` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2022-02-25 23:11:07
