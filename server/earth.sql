DROP TABLE IF EXISTS `log`;
CREATE TABLE `log` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `server` varchar(20)   NOT NULL,
  `port` int(11) NOT NULL,
  `name` varchar(100)   NOT NULL,
  `operation` varchar(100)   NOT NULL,
  `content` varchar(1024)   DEFAULT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `marker`;
CREATE TABLE `marker` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `group` varchar(100) NOT NULL,
  `name` varchar(100) NOT NULL,
  `lat` double NOT NULL,
  `lng` double NOT NULL,
  PRIMARY KEY (`id`)
) ;

DROP TABLE IF EXISTS `server`;
CREATE TABLE `server` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `ip` varchar(20) NOT NULL,
  `port` int(11) NOT NULL,
  `user` varchar(50) NOT NULL DEFAULT '',
  `web` int(11) NOT NULL DEFAULT '1',
  `earth` int(11) NOT NULL DEFAULT '1',
  `merge` int(11) NOT NULL DEFAULT '1',
  `activetime` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
);

INSERT INTO `server` VALUES (1,'127.0.0.1',8825,'',1,1,1,'2020-12-07 04:16:27');

DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `password` varchar(50) NOT NULL,
  `web` int(11) NOT NULL DEFAULT '1',
  `earth` int(11) NOT NULL DEFAULT '1',
  `merge` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ;

INSERT INTO `user` VALUES (1,'admin','123',1,0,0);

DROP TABLE IF EXISTS `version`;
CREATE TABLE `version` (
  `version` varchar(10) NOT NULL,
  `url` varchar(100) NOT NULL,
  PRIMARY KEY (`version`)
) ;
