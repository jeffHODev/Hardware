/*
Navicat MySQL Data Transfer

Source Server         : 昱星辰正式
Source Server Version : 50645
Source Host           : 123.57.185.1:3306
Source Database       : nivi

Target Server Type    : MYSQL
Target Server Version : 50645
File Encoding         : 65001

Date: 2020-03-13 10:00:59
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for analyze_result
-- ----------------------------
DROP TABLE IF EXISTS `analyze_result`;
CREATE TABLE `analyze_result` (
  `id` varchar(64) NOT NULL,
  `basis_measurement_id` varchar(64) NOT NULL COMMENT '测试号（基础测量表id）',
  `cg_pl_number` tinyint(4) NOT NULL COMMENT '左颈动脉选择号',
  `cg_pr_number` tinyint(4) NOT NULL COMMENT '右颈动脉选择号',
  `bp_type_eval_value` int(11) NOT NULL COMMENT '血压类型评价值',
  `sv` float NOT NULL COMMENT '心脏搏出量',
  `brt` float NOT NULL COMMENT '血流总阻力',
  `ps` float NOT NULL COMMENT '收缩压',
  `pd` float NOT NULL COMMENT '舒张压',
  `pm` float NOT NULL COMMENT '平均压',
  `pss` float NOT NULL COMMENT '拐点压',
  `coronl_pr` float NOT NULL COMMENT '冠状动脉（左）-脉压阻力',
  `coronl_c` float NOT NULL COMMENT '冠状动脉（左）-顺应性',
  `coronl_asc` float NOT NULL COMMENT '冠状动脉（左）-动脉硬化度',
  `coronl_q` float NOT NULL COMMENT '冠状动脉（左）-血流量',
  `coronr_pr` float NOT NULL COMMENT '冠状动脉（右）-脉压阻力',
  `coronr_c` float NOT NULL COMMENT '冠状动脉（右）-顺应性',
  `coronr_asc` float NOT NULL COMMENT '冠状动脉（右）-动脉硬化度',
  `coronr_q` float NOT NULL COMMENT '冠状动脉（右）-血流量',
  `coron_comp_rt` float NOT NULL COMMENT '（补偿）冠状动脉-血流总阻力',
  `coron_comp_c` float NOT NULL COMMENT '（补偿）冠状动脉-顺应性',
  `coron_eval` int(11) NOT NULL COMMENT '冠状动脉系总体评价',
  `create_date` datetime NOT NULL COMMENT '创建时间',
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for basis_measurement
-- ----------------------------
DROP TABLE IF EXISTS `basis_measurement`;
CREATE TABLE `basis_measurement` (
  `id` varchar(64) NOT NULL COMMENT '主键id（测试号）',
  `height` int(11) NOT NULL COMMENT '身高',
  `weight` int(11) NOT NULL COMMENT '体重',
  `diag_date` datetime NOT NULL COMMENT '检测日期',
  `ps` int(11) NOT NULL COMMENT '收缩压',
  `pd` int(11) NOT NULL COMMENT '舒张压',
  `br_ps` int(11) NOT NULL COMMENT '药前收缩压',
  `br_pd` int(11) NOT NULL COMMENT '药前舒张压',
  `al_ps` int(11) NOT NULL COMMENT '左下肢收缩压',
  `al_pd` int(11) NOT NULL COMMENT '右下肢收缩压',
  `ar_ps` int(11) NOT NULL COMMENT '冠脉药效等级',
  `ar_pd` int(11) NOT NULL,
  `pcmpt` int(11) NOT NULL COMMENT '收缩压',
  `pexpd` int(11) NOT NULL COMMENT '舒张压',
  `pavrg` int(11) NOT NULL COMMENT '平均值',
  `p_hr` int(11) NOT NULL COMMENT '脉搏',
  `patient_id` varchar(64) NOT NULL COMMENT '患者id',
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for hospital
-- ----------------------------
DROP TABLE IF EXISTS `hospital`;
CREATE TABLE `hospital` (
  `id` varchar(64) NOT NULL,
  `account` varchar(50) NOT NULL COMMENT '账号',
  `password` varchar(100) NOT NULL COMMENT '密码',
  `name` varchar(50) NOT NULL COMMENT '名称',
  `detect_count` int(11) NOT NULL COMMENT '剩余检测次数',
  `create_date` datetime NOT NULL COMMENT '创建时间',
  `create_user_id` varchar(64) DEFAULT NULL COMMENT '创建人',
  `update_user_id` varchar(64) DEFAULT '' COMMENT '更新的人',
  `update_date` datetime DEFAULT NULL COMMENT '更新时间',
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for original_waveform
-- ----------------------------
DROP TABLE IF EXISTS `original_waveform`;
CREATE TABLE `original_waveform` (
  `id` varchar(64) NOT NULL,
  `hypecgs` mediumblob NOT NULL COMMENT '超收缩压心电波',
  `hyppcgs` mediumblob NOT NULL COMMENT '超收缩压心音波',
  `hypapgs` mediumblob NOT NULL COMMENT '超收缩压脉搏波',
  `hypecgd` mediumblob NOT NULL COMMENT '低舒张压心电波',
  `hyppcgd` mediumblob NOT NULL COMMENT '低舒张压心音波',
  `hypapgd` mediumblob NOT NULL COMMENT '低舒张压脉搏波',
  `ecg_pl` mediumblob NOT NULL COMMENT '左颈动脉心电波',
  `pcg_pl` mediumblob NOT NULL COMMENT '左颈动脉心音波',
  `apg_l` mediumblob NOT NULL COMMENT '左颈动脉脉搏波',
  `ecg_pr` mediumblob NOT NULL COMMENT '右颈动脉心电波',
  `pcg_pr` mediumblob NOT NULL COMMENT '右颈动脉心音波',
  `arg_r` mediumblob NOT NULL COMMENT '右颈动脉脉搏波',
  `ecg_th` mediumblob NOT NULL COMMENT '股动脉心电波',
  `apg_th` mediumblob NOT NULL COMMENT '股动脉脉搏波',
  `create_date` datetime NOT NULL COMMENT '创建日期',
  `basis_measurement_id` varchar(64) NOT NULL COMMENT '测试号（基础测量表id）',
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for patient
-- ----------------------------
DROP TABLE IF EXISTS `patient`;
CREATE TABLE `patient` (
  `id` varchar(64) NOT NULL,
  `case_no` varchar(100) DEFAULT NULL COMMENT '病历号',
  `nationality` varchar(20) DEFAULT '' COMMENT '国籍',
  `address` varchar(50) DEFAULT '' COMMENT '详细地址',
  `phone` char(11) DEFAULT '' COMMENT '手机号',
  `birthday` date NOT NULL COMMENT '出生年月',
  `sex` tinyint(4) DEFAULT '0' COMMENT '性别（1男 2女）',
  `name` varchar(10) DEFAULT '' COMMENT '姓名',
  `unique_id` char(18) NOT NULL COMMENT '身份证号',
  `hospital_id` varchar(64) NOT NULL COMMENT '所属医院',
  `create_date` datetime NOT NULL COMMENT '创建时间',
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for update_event
-- ----------------------------
DROP TABLE IF EXISTS `update_event`;
CREATE TABLE `update_event` (
  `event_id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `version` int(11) NOT NULL COMMENT '版本号',
  `object_type` varchar(50) NOT NULL COMMENT '源数据的表名',
  `object_id` varchar(64) NOT NULL COMMENT '源数据表主键',
  `field_name` varchar(50) NOT NULL COMMENT '修改的源数据表的字段名',
  `field_old_value` varchar(80) DEFAULT NULL COMMENT '该字段原来的值',
  `field_new_value` varchar(80) DEFAULT NULL COMMENT '该字段最新的值',
  `update_time` datetime NOT NULL COMMENT '更新时间',
  `update_user` varchar(64) NOT NULL COMMENT '操作者',
  PRIMARY KEY (`event_id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;
