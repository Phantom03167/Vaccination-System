/*
 Navicat Premium Data Transfer

 Source Server         : 郭是聿
 Source Server Type    : MySQL
 Source Server Version : 80023
 Source Host           : 127.0.0.1:3306
 Source Schema         : 新冠疫苗接种

 Target Server Type    : MySQL
 Target Server Version : 80023
 File Encoding         : 65001

 Date: 03/12/2021 00:03:08
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for 居民
-- ----------------------------
DROP TABLE IF EXISTS `居民`;
CREATE TABLE `居民`  (
  `姓名` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `性别` char(1) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `年龄` tinyint NOT NULL,
  `身份证号` char(5) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `联系方式` char(11) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `现居地址` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `备注` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '无',
  PRIMARY KEY (`身份证号`) USING BTREE,
  UNIQUE INDEX `resident_id`(`身份证号`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of 居民
-- ----------------------------
INSERT INTO `居民` VALUES ('张三', '男', 43, '12345', '15639682095', '西青区', '无');
INSERT INTO `居民` VALUES ('赵四', '女', 20, '29385', '18520397348', '河东区', '无');
INSERT INTO `居民` VALUES ('郭是聿', '女', 26, '66666', '17720473957', '东丽区', '无');
INSERT INTO `居民` VALUES ('李四', '女', 25, '67890', '13864758234', '河北区', '无');

-- ----------------------------
-- Table structure for 接种点
-- ----------------------------
DROP TABLE IF EXISTS `接种点`;
CREATE TABLE `接种点`  (
  `接种点编号` smallint UNSIGNED NOT NULL AUTO_INCREMENT,
  `接种点名称` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `接种点地址` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `接种开始时间` time NOT NULL,
  `接种结束时间` time NOT NULL,
  PRIMARY KEY (`接种点编号`) USING BTREE,
  UNIQUE INDEX `vaccination_site_id`(`接种点编号`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of 接种点
-- ----------------------------
INSERT INTO `接种点` VALUES (1, '社区第一卫生院', '和平区', '08:00:00', '18:00:00');
INSERT INTO `接种点` VALUES (2, '社区第二卫生院', '河西区', '07:00:00', '14:00:00');
INSERT INTO `接种点` VALUES (3, '社区第三卫生院', '南开区', '08:00:00', '21:00:00');

-- ----------------------------
-- Table structure for 接种记录
-- ----------------------------
DROP TABLE IF EXISTS `接种记录`;
CREATE TABLE `接种记录`  (
  `接种编号` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `预约编号` bigint UNSIGNED NOT NULL,
  `疫苗追溯码` char(5) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`接种编号`, `预约编号`, `疫苗追溯码`) USING BTREE,
  UNIQUE INDEX `vaccination_appointment_id`(`预约编号`) USING BTREE,
  UNIQUE INDEX `vaccination_id`(`接种编号`) USING BTREE,
  UNIQUE INDEX `vaccination_vaccine_trace_code`(`疫苗追溯码`) USING BTREE,
  CONSTRAINT `vaccination_appointment_id` FOREIGN KEY (`预约编号`) REFERENCES `预约记录` (`预约编号`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB AUTO_INCREMENT = 9 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of 接种记录
-- ----------------------------
INSERT INTO `接种记录` VALUES (1, 1, 'aaaaa');
INSERT INTO `接种记录` VALUES (2, 2, 'ASDEF');
INSERT INTO `接种记录` VALUES (3, 3, 'DUJEW');
INSERT INTO `接种记录` VALUES (4, 4, 'ASDFG');

-- ----------------------------
-- Table structure for 疫苗
-- ----------------------------
DROP TABLE IF EXISTS `疫苗`;
CREATE TABLE `疫苗`  (
  `疫苗编号` tinyint UNSIGNED NOT NULL AUTO_INCREMENT,
  `疫苗名称` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `疫苗类型` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `生产企业` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `需接种剂次` tinyint UNSIGNED NOT NULL,
  `疫苗说明` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`疫苗编号`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of 疫苗
-- ----------------------------
INSERT INTO `疫苗` VALUES (1, '国药中国生物新型冠状病毒灭活疫苗（Vero细胞）', '灭活疫苗', '国药集团中国北京生物制品有限责任公司', 2, '存储条件 使用说明 有效期 适用人群 症状 注意事项');
INSERT INTO `疫苗` VALUES (2, '北京科兴中维新型冠状病毒灭活疫苗（Vero细胞）', '灭活疫苗', '北京科兴中维生物制品有限责任公司', 2, '存储条件 使用说明 有效期 适用人群 症状 注意事项');
INSERT INTO `疫苗` VALUES (3, '武汉生物新型冠状病毒灭活疫苗（Vero细胞）', '灭活疫苗', '中国生物武汉生物制品研究所有限责任公司', 2, '存储条件 使用说明 有效期 适用人群 症状 注意事项');
INSERT INTO `疫苗` VALUES (4, '康希诺重组新型冠状病毒疫苗（5型腺病毒载体）', '腺病毒载体疫苗', '康希诺生物股份公司', 1, '存储条件 使用说明 有效期 适用人群 症状 注意事项');
INSERT INTO `疫苗` VALUES (5, '安徽智飞龙科马重组新型冠状病毒疫苗（CHO细胞）', '重组亚单位疫苗', '安徽智飞龙科马生物制药有限公司', 3, '存储条件 使用说明 有效期 适用人群 症状 注意事项');

-- ----------------------------
-- Table structure for 预约记录
-- ----------------------------
DROP TABLE IF EXISTS `预约记录`;
CREATE TABLE `预约记录`  (
  `预约编号` bigint UNSIGNED NOT NULL AUTO_INCREMENT,
  `身份证号` char(5) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `预约疫苗编号` tinyint UNSIGNED NOT NULL,
  `预约接种点编号` smallint UNSIGNED NOT NULL,
  `预约剂次` tinyint UNSIGNED NOT NULL,
  `预约接种时间` datetime NOT NULL,
  `预约状态` varchar(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL DEFAULT '预约成功',
  PRIMARY KEY (`预约编号`) USING BTREE,
  UNIQUE INDEX `appointment_id`(`预约编号`) USING BTREE,
  INDEX `appointment_vaccination_site_id`(`预约接种点编号`) USING BTREE,
  INDEX `appointment_resident_id`(`身份证号`) USING BTREE,
  INDEX `appointment_vaccine_id`(`预约疫苗编号`) USING BTREE,
  CONSTRAINT `appointment_resident_id` FOREIGN KEY (`身份证号`) REFERENCES `居民` (`身份证号`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `appointment_vaccination_site_id` FOREIGN KEY (`预约接种点编号`) REFERENCES `接种点` (`接种点编号`) ON DELETE RESTRICT ON UPDATE CASCADE,
  CONSTRAINT `appointment_vaccine_id` FOREIGN KEY (`预约疫苗编号`) REFERENCES `疫苗` (`疫苗编号`) ON DELETE RESTRICT ON UPDATE CASCADE
) ENGINE = InnoDB AUTO_INCREMENT = 5 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of 预约记录
-- ----------------------------
INSERT INTO `预约记录` VALUES (1, '12345', 1, 1, 1, '2021-10-01 13:00:00', '接种成功');
INSERT INTO `预约记录` VALUES (2, '12345', 1, 2, 2, '2021-11-12 14:00:00', '接种成功');
INSERT INTO `预约记录` VALUES (3, '67890', 2, 3, 1, '2021-11-21 15:00:00', '接种成功');
INSERT INTO `预约记录` VALUES (4, '66666', 5, 3, 1, '2021-11-30 14:00:00', '预约成功');

-- ----------------------------
-- View structure for 居民接种信息
-- ----------------------------
DROP VIEW IF EXISTS `居民接种信息`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `居民接种信息` AS select `居民`.`姓名` AS `姓名`,`居民`.`性别` AS `性别`,`居民`.`年龄` AS `年龄`,`居民`.`身份证号` AS `身份证号`,`居民`.`联系方式` AS `联系方式`,`居民`.`现居地址` AS `现居地址`,`居民`.`备注` AS `备注`,if((`a`.`接种日期` is null),'未接种',convert(`a`.`接种日期` using utf8mb4)) AS `第一剂接种日期`,if(`a`.`接种疫苗编号` in (select `疫苗`.`疫苗编号` from `疫苗` where (`疫苗`.`需接种剂次` >= 2)) is false,'无需接种',if((`b`.`接种日期` is null),'未接种',convert(`b`.`接种日期` using utf8mb4))) AS `第二剂接种日期`,if(`a`.`接种疫苗编号` in (select `疫苗`.`疫苗编号` from `疫苗` where (`疫苗`.`需接种剂次` >= 3)) is false,'无需接种',if((`c`.`接种日期` is null),'未接种',convert(`c`.`接种日期` using utf8mb4))) AS `第三剂接种日期` from (`居民` left join ((`接种信息` `a` left join `接种信息` `b` on(((`a`.`身份证号` = `b`.`身份证号`) and (`b`.`接种剂次` = 2)))) left join `接种信息` `c` on(((`b`.`身份证号` = `c`.`身份证号`) and (`c`.`接种剂次` = 3)))) on(((`居民`.`身份证号` = `a`.`身份证号`) and (`a`.`接种剂次` = 1))));

-- ----------------------------
-- View structure for 居民管理
-- ----------------------------
DROP VIEW IF EXISTS `居民管理`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `居民管理` AS select `居民`.`姓名` AS `姓名`,`居民`.`性别` AS `性别`,`居民`.`年龄` AS `年龄`,`居民`.`身份证号` AS `身份证号`,`居民`.`联系方式` AS `联系方式`,`居民`.`现居地址` AS `现居地址`,`居民`.`备注` AS `备注` from `居民`;

-- ----------------------------
-- View structure for 接种信息
-- ----------------------------
DROP VIEW IF EXISTS `接种信息`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `接种信息` AS select `接种记录`.`接种编号` AS `接种编号`,`居民`.`姓名` AS `姓名`,`预约记录`.`身份证号` AS `身份证号`,`接种记录`.`预约编号` AS `预约编号`,`预约记录`.`预约疫苗编号` AS `接种疫苗编号`,`疫苗`.`疫苗名称` AS `接种疫苗名称`,`预约记录`.`预约剂次` AS `接种剂次`,`接种记录`.`疫苗追溯码` AS `疫苗追溯码`,`预约记录`.`预约接种点编号` AS `接种点编号`,`接种点`.`接种点名称` AS `接种点名称`,date_format(`预约记录`.`预约接种时间`,'%Y-%m-%d') AS `接种日期` from ((((`接种记录` join `居民`) join `疫苗`) join `接种点`) join `预约记录`) where ((`预约记录`.`身份证号` = `居民`.`身份证号`) and (`预约记录`.`预约疫苗编号` = `疫苗`.`疫苗编号`) and (`预约记录`.`预约接种点编号` = `接种点`.`接种点编号`) and (`接种记录`.`预约编号` = `预约记录`.`预约编号`));

-- ----------------------------
-- View structure for 接种点管理
-- ----------------------------
DROP VIEW IF EXISTS `接种点管理`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `接种点管理` AS select `接种点`.`接种点编号` AS `接种点编号`,`接种点`.`接种点名称` AS `接种点名称`,`接种点`.`接种点地址` AS `接种点地址`,`接种点`.`接种开始时间` AS `接种开始时间`,`接种点`.`接种结束时间` AS `接种结束时间` from `接种点`;

-- ----------------------------
-- View structure for 用户管理
-- ----------------------------
DROP VIEW IF EXISTS `用户管理`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `用户管理` AS select `mysql`.`role_edges`.`TO_USER` AS `用户`,`mysql`.`role_edges`.`FROM_USER` AS `用户权限角色` from `mysql`.`role_edges` where ((`mysql`.`role_edges`.`FROM_USER` = 'community_maanger') or (`mysql`.`role_edges`.`FROM_USER` = 'medical_staff') or (`mysql`.`role_edges`.`FROM_USER` = 'vaccination_system_admin'));

-- ----------------------------
-- View structure for 疫苗管理
-- ----------------------------
DROP VIEW IF EXISTS `疫苗管理`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `疫苗管理` AS select `疫苗`.`疫苗编号` AS `疫苗编号`,`疫苗`.`疫苗名称` AS `疫苗名称`,`疫苗`.`疫苗类型` AS `疫苗类型`,`疫苗`.`生产企业` AS `生产企业`,`疫苗`.`需接种剂次` AS `需接种剂次`,`疫苗`.`疫苗说明` AS `疫苗说明` from `疫苗`;

-- ----------------------------
-- View structure for 预约管理
-- ----------------------------
DROP VIEW IF EXISTS `预约管理`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `预约管理` AS select `预约记录`.`预约编号` AS `预约编号`,`居民`.`姓名` AS `姓名`,`预约记录`.`预约剂次` AS `预约剂次`,`预约记录`.`身份证号` AS `身份证号`,`预约记录`.`预约疫苗编号` AS `预约疫苗编号`,`疫苗`.`疫苗名称` AS `预约疫苗名称`,`疫苗`.`疫苗类型` AS `预约疫苗类型`,`预约记录`.`预约接种点编号` AS `预约接种点编号`,`接种点`.`接种点名称` AS `预约接种点名称`,`接种点`.`接种点地址` AS `预约接种点地址`,`预约记录`.`预约接种时间` AS `预约接种时间`,`预约记录`.`预约状态` AS `预约状态` from (((`居民` join `预约记录`) join `接种点`) join `疫苗`) where ((`居民`.`身份证号` = `预约记录`.`身份证号`) and (`预约记录`.`预约接种点编号` = `接种点`.`接种点编号`) and (`预约记录`.`预约疫苗编号` = `疫苗`.`疫苗编号`));

-- ----------------------------
-- View structure for 预约记录管理
-- ----------------------------
DROP VIEW IF EXISTS `预约记录管理`;
CREATE ALGORITHM = UNDEFINED SQL SECURITY DEFINER VIEW `预约记录管理` AS select `预约记录`.`身份证号` AS `身份证号`,`预约记录`.`预约疫苗编号` AS `预约疫苗编号`,`预约记录`.`预约接种点编号` AS `预约接种点编号`,`预约记录`.`预约接种时间` AS `预约接种时间` from `预约记录`;

-- ----------------------------
-- Event structure for appointment status update
-- ----------------------------
DROP EVENT IF EXISTS `appointment status update`;
delimiter ;;
CREATE EVENT `appointment status update`
ON SCHEDULE
EVERY '1' HOUR STARTS '2021-11-20 00:00:00'
DO BEGIN
DECLARE currenttime CHAR(19) CHARACTER SET utf8;
SET currenttime = NOW();
UPDATE 	`预约记录` SET `预约状态`='未完成' WHERE `预约状态`='预约成功' AND ADDDATE(`预约时间`,INTERVAL 1 HOUR)<=currenttime;
END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 居民
-- ----------------------------
DROP TRIGGER IF EXISTS `resident_isValueNull_insert`;
delimiter ;;
CREATE TRIGGER `resident_isValueNull_insert` BEFORE INSERT ON `居民` FOR EACH ROW BEGIN

IF NEW.`姓名` = '' THEN
SET NEW.`姓名` = NULL;

ELSEIF NEW.`性别` = '' THEN
SET NEW.`性别` = NULL;

ELSEIF NEW.`身份证号` = '' THEN
SET NEW.`身份证号` = NULL;

ELSEIF NEW.`联系方式` = '' THEN
SET NEW.`联系方式` = NULL;

ELSEIF NEW.`现居地址` = '' THEN
SET NEW.`现居地址` = NULL;

ELSEIF NEW.`备注` = '' OR NEW.`备注` = NULL THEN
SET NEW.`备注` = '无';

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 居民
-- ----------------------------
DROP TRIGGER IF EXISTS `resident_isValueNull_update`;
delimiter ;;
CREATE TRIGGER `resident_isValueNull_update` BEFORE UPDATE ON `居民` FOR EACH ROW BEGIN

IF NEW.`姓名` = '' THEN
SET NEW.`姓名` = NULL;

ELSEIF NEW.`性别` = '' THEN
SET NEW.`性别` = NULL;

ELSEIF NEW.`身份证号` = '' THEN
SET NEW.`身份证号` = NULL;

ELSEIF NEW.`联系方式` = '' THEN
SET NEW.`联系方式` = NULL;

ELSEIF NEW.`现居地址` = '' THEN
SET NEW.`现居地址` = NULL;

ELSEIF NEW.`备注` = '' OR NEW.`备注` = NULL THEN
SET NEW.`备注` = '无';

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 接种点
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccination_site_isValueNull_insert`;
delimiter ;;
CREATE TRIGGER `vaccination_site_isValueNull_insert` BEFORE INSERT ON `接种点` FOR EACH ROW BEGIN

IF NEW.`接种点名称` = '' THEN
SET NEW.`接种点名称` = NULL;

ELSEIF NEW.`接种点地址` = '' THEN
SET NEW.`接种点地址` = NULL;

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 接种点
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccination_site_insert`;
delimiter ;;
CREATE TRIGGER `vaccination_site_insert` BEFORE INSERT ON `接种点` FOR EACH ROW BEGIN

IF NEW.`接种开始时间`>=NEW.`接种结束时间` THEN
SIGNAL SQLSTATE '23503' SET MESSAGE_TEXT = '接种时间范围无效';

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 接种点
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccination_site_isValueNull_update`;
delimiter ;;
CREATE TRIGGER `vaccination_site_isValueNull_update` BEFORE UPDATE ON `接种点` FOR EACH ROW BEGIN

IF NEW.`接种点名称` = '' THEN
SET NEW.`接种点名称` = NULL;

ELSEIF NEW.`接种点地址` = '' THEN
SET NEW.`接种点地址` = NULL;

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 接种点
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccination_site_update`;
delimiter ;;
CREATE TRIGGER `vaccination_site_update` BEFORE UPDATE ON `接种点` FOR EACH ROW BEGIN

IF NEW.`接种开始时间`>=NEW.`接种结束时间` THEN
SIGNAL SQLSTATE '23503' SET MESSAGE_TEXT = '接种时间范围无效';

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 接种记录
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccination_update`;
delimiter ;;
CREATE TRIGGER `vaccination_update` BEFORE UPDATE ON `接种记录` FOR EACH ROW BEGIN

SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '该对象不允许更新';

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 接种记录
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccination_isValueNull_insert`;
delimiter ;;
CREATE TRIGGER `vaccination_isValueNull_insert` BEFORE INSERT ON `接种记录` FOR EACH ROW BEGIN

IF NEW.`疫苗追溯码` = '' THEN
SET NEW.`疫苗追溯码` = NULL;

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 接种记录
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccination_insert`;
delimiter ;;
CREATE TRIGGER `vaccination_insert` BEFORE INSERT ON `接种记录` FOR EACH ROW BEGIN

IF (SELECT `接种编号` FROM `接种记录` WHERE `接种记录`.`预约编号`=NEW.`预约编号`)!=NULL THEN
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '接种记录已存在';

ELSEIF (SELECT `疫苗追溯码` FROM `接种记录` WHERE `接种记录`.`疫苗追溯码`=NEW.`疫苗追溯码`)!=NULL THEN
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '接种记录已存在';

END IF;
END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 接种记录
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccination_delete`;
delimiter ;;
CREATE TRIGGER `vaccination_delete` BEFORE DELETE ON `接种记录` FOR EACH ROW BEGIN

SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '该对象不允许删除';

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 疫苗
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccine_isValueNull_insert`;
delimiter ;;
CREATE TRIGGER `vaccine_isValueNull_insert` BEFORE INSERT ON `疫苗` FOR EACH ROW BEGIN

IF NEW.`疫苗名称` = '' THEN
SET NEW.`疫苗名称` = NULL;

ELSEIF NEW.`疫苗类型` = '' THEN
SET NEW.`疫苗类型` = NULL;

ELSEIF NEW.`生产企业` = '' THEN
SET NEW.`生产企业` = NULL;

ELSEIF NEW.`疫苗说明` = '' THEN
SET NEW.`疫苗说明` = NULL;

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 疫苗
-- ----------------------------
DROP TRIGGER IF EXISTS `vaccine_isValueNull_update`;
delimiter ;;
CREATE TRIGGER `vaccine_isValueNull_update` BEFORE UPDATE ON `疫苗` FOR EACH ROW BEGIN

IF NEW.`疫苗名称` = '' THEN
SET NEW.`疫苗名称` = NULL;

ELSEIF NEW.`疫苗类型` = '' THEN
SET NEW.`疫苗类型` = NULL;

ELSEIF NEW.`生产企业` = '' THEN
SET NEW.`生产企业` = NULL;

ELSEIF NEW.`疫苗说明` = '' THEN
SET NEW.`疫苗说明` = NULL;

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 预约记录
-- ----------------------------
DROP TRIGGER IF EXISTS `appointment_delete`;
delimiter ;;
CREATE TRIGGER `appointment_delete` BEFORE DELETE ON `预约记录` FOR EACH ROW BEGIN

SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '该对象不允许删除';

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 预约记录
-- ----------------------------
DROP TRIGGER IF EXISTS `appointment_isValueNull_insert`;
delimiter ;;
CREATE TRIGGER `appointment_isValueNull_insert` BEFORE INSERT ON `预约记录` FOR EACH ROW BEGIN

IF NEW.`身份证号` = '' THEN
SET NEW.`身份证号` = NULL;

END IF;

SET NEW.`预约状态` = '预约成功';

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 预约记录
-- ----------------------------
DROP TRIGGER IF EXISTS `appointment_isValueNull_update`;
delimiter ;;
CREATE TRIGGER `appointment_isValueNull_update` BEFORE UPDATE ON `预约记录` FOR EACH ROW BEGIN

IF NEW.`身份证号` = '' THEN
SET NEW.`身份证号` = NULL;

ELSEIF NEW.`预约剂次` = '' THEN
SET NEW.`预约剂次` = NULL;

ELSEIF NEW.`预约状态` = '' THEN
SET NEW.`预约状态` = NULL;

END IF;

END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 预约记录
-- ----------------------------
DROP TRIGGER IF EXISTS `appointment_insert`;
delimiter ;;
CREATE TRIGGER `appointment_insert` BEFORE INSERT ON `预约记录` FOR EACH ROW BEGIN

DECLARE vaccine VARCHAR(50) CHARACTER SET utf8;
DECLARE times TINYINT;


IF (SELECT `预约编号` FROM `预约记录` WHERE `预约记录`.`身份证号`=NEW.`身份证号` AND `预约状态`='预约成功')IS NOT NULL THEN
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '您已预约，无需再次预约';

END IF;

SET vaccine=(SELECT `预约疫苗编号` FROM `预约记录` WHERE `预约记录`.`身份证号`=NEW.`身份证号` LIMIT 1);

IF vaccine IS NOT NULL AND vaccine != NEW.`预约疫苗编号` THEN
SET vaccine = CONCAT('您无法预约接种此疫苗，您只可预约接种',vaccine,'号疫苗');
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = vaccine;

END IF;

SET times=(SELECT `预约剂次` FROM `预约记录` WHERE `预约记录`.`身份证号`=NEW.`身份证号` AND `预约记录`.`预约状态`='接种成功' ORDER BY `预约剂次` DESC LIMIT 1);

IF times>=(SELECT `需接种剂次` FROM `疫苗` WHERE NEW.`预约疫苗编号`=`疫苗`.`疫苗编号`) THEN
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '您已接种所有剂次，无需预约';

END IF;

IF times IS NULL THEN
SET times=0;

END IF;

SET NEW.`预约剂次`=times+1;



IF SUBDATE(NEW.`预约接种时间`,28)<=(SELECT `预约接种时间` FROM `预约记录` WHERE `预约记录`.`身份证号`=NEW.`身份证号` ORDER BY `预约接种时间` DESC LIMIT 1) THEN
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '您未到可接种时间（接种间隔4周），无法预约';

END IF;

IF DATE_FORMAT(NEW.`预约接种时间`,'%H:%i') NOT BETWEEN (SELECT `接种开始时间` FROM `接种点` WHERE `接种点`.`接种点编号`=NEW.`预约接种点编号`) AND (SELECT `接种结束时间` FROM `接种点` WHERE `接种点`.`接种点编号`=NEW.`预约接种点编号`) THEN
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '您预约的时间无法接种';

END IF;
END
;;
delimiter ;

-- ----------------------------
-- Triggers structure for table 预约记录
-- ----------------------------
DROP TRIGGER IF EXISTS `appointment_update`;
delimiter ;;
CREATE TRIGGER `appointment_update` BEFORE UPDATE ON `预约记录` FOR EACH ROW BEGIN

IF OLD.`预约状态` != '预约成功' THEN
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '该对象不允许更新';

ELSEIF OLD.`预约接种时间`>NOW() THEN
SIGNAL SQLSTATE '42807' SET MESSAGE_TEXT = '未到预约时间无法接种';

END IF;

END
;;
delimiter ;

SET FOREIGN_KEY_CHECKS = 1;
