/*
Navicat MySQL Data Transfer

Source Server         : cobox_bd2
Source Server Version : 80018
Source Host           : localhost:3306
Source Database       : cobox_data

Target Server Type    : MYSQL
Target Server Version : 80018
File Encoding         : 65001

Date: 2019-12-17 20:02:45
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for mesures
-- ----------------------------
DROP TABLE IF EXISTS `mesures`;
CREATE TABLE `mesures` (
  `id` int(11) NOT NULL,
  `id_device` int(11) DEFAULT NULL,
  `temp` double DEFAULT NULL,
  `hum` double DEFAULT NULL,
  `timestamp` int(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of mesures
-- ----------------------------
INSERT INTO `mesures` VALUES ('1', '2', '19', '72', '1576506486');
INSERT INTO `mesures` VALUES ('2', '2', '19', '71.7', '1576506496');
INSERT INTO `mesures` VALUES ('3', '2', '19', '71.7', '1576506506');
INSERT INTO `mesures` VALUES ('4', '2', '19', '71.7', '1576506516');
INSERT INTO `mesures` VALUES ('5', '2', '19.5', '62.7', '1576608474');
INSERT INTO `mesures` VALUES ('6', '2', '19.4', '63.9', '1576608484');
