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
);


DROP TABLE IF EXISTS `app_account`;
CREATE TABLE `app_account` (
  `id` varchar(64) NOT NULL,
  `entity_id` varchar(64) NOT NULL COMMENT '对应实体',
  `account` varchar(50) NOT NULL COMMENT '登录账号',
  `password` varchar(100) NOT NULL COMMENT '登录密码',
  `create_date` datetime NOT NULL COMMENT '创建时间',
  `entity_type` tinyint(4) NOT NULL COMMENT '实体类型（1医院 2医生 3经销商）',
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `app_version`;
create table app_version
(
   `edition` varchar(200)  NOT NULL COMMENT '版本号',
   `url` varchar(64) NOT NULL COMMENT '网址',
   `create_date` datetime NOT NULL COMMENT '创建时间',
   `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）'
)

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
  `device_id` varchar(64) NOT NULL COMMENT '设备id',
  `model` tinyint(4) COMMENT '测量模式（1 同步；2 异步）',
  `doctor_id` varchar(64) COMMENT '诊断的医生',
  `evaluation_content` varchar(4000) COMMENT '诊断的内容',
  `additional_doctor_id` varchar(64) COMMENT '追加医生',
  `additional_evaluation_content` varchar(64) COMMENT '追加的内容',
  `additionalDate` datetime COMMENT '追加诊断时间',
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
);


DROP TABLE IF EXISTS `device`;
CREATE TABLE `device` (
  `id` varchar(64) NOT NULL, 
  `device_no` varchar(64) NOT NULL COMMENT '设备号',
  `password` varchar(100) NOT NULL COMMENT '登录密码',
  `create_date` datetime NOT NULL COMMENT '添加时间', 
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
);


DROP TABLE IF EXISTS `doctor_evaluation`;
CREATE TABLE `doctor_evaluation` (
  `id` varchar(64) NOT NULL, 
  `basis_measurement_id` varchar(64) NOT NULL COMMENT '评估对象', 
  `create_date` datetime NOT NULL COMMENT '评估时间',
  `content` varchar(255) NOT NULL COMMENT '诊断评语',
  `doctor_id` varchar(64) NOT NULL COMMENT '评估医生',
  `state` tinyint(4) NOT NULL COMMENT '评估状态（0首次评估 1追加评估）',
  `signature` varchar(255) NOT NULL COMMENT '医生签名', 
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `original_waveform`;
CREATE TABLE `original_waveform` (
  `id` varchar(64) NOT NULL,
  `hypecgs` blob NOT NULL COMMENT '超收缩压心电波',
  `hyppcgs` blob NOT NULL COMMENT '超收缩压心音波',
  `hypapgs` blob NOT NULL COMMENT '超收缩压脉搏波',
  `hypecgd` blob NOT NULL COMMENT '低舒张压心电波',
  `hyppcgd` blob NOT NULL COMMENT '低舒张压心音波',
  `hypapgd` blob NOT NULL COMMENT '低舒张压脉搏波',
  `ecg_pl` blob NOT NULL COMMENT '左颈动脉心电波',
  `pcg_pl` blob NOT NULL COMMENT '左颈动脉心音波',
  `apg_l` blob NOT NULL COMMENT '左颈动脉脉搏波',
  `ecg_pr` blob NOT NULL COMMENT '右颈动脉心电波',
  `pcg_pr` blob NOT NULL COMMENT '右颈动脉心音波',
  `arg_r` blob NOT NULL COMMENT '右颈动脉脉搏波',
  `ecg_th` blob NOT NULL COMMENT '股动脉心电波',
  `apg_th` blob NOT NULL COMMENT '股动脉脉搏波',
  `create_date` datetime NOT NULL COMMENT '创建日期',
  `basis_measurement_id` varchar(64) NOT NULL COMMENT '测试号（基础测量表id）',
  `del_flag` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除（0正常 1删除）',
  PRIMARY KEY (`id`)
);

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
);

DROP TABLE IF EXISTS `sys_dealers`;
create table sys_dealers
(
    `id`  varchar(64) NOT NULL COMMENT '主键',
    `name` varchar(255) NOT NULL COMMENT '经销商名称',
    `address` varchar(255)  COMMENT '经销商地址',
    `create_date` datetime  COMMENT '创建时间',
    `create_user_id` varchar(255)  COMMENT '创建人',
    `update_date`    datetime COMMENT '更新时间',
    `update_user_id` varchar(255) COMMENT '更新人',
    `del_flag` tinyint(4) default 0 NOT NULL COMMENT '是否删除（0正常 1删除）',
    PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `sys_device_hospital`;
create table `sys_device_hospital`
(
    `id`   varchar(64) NOT NULL COMMENT '主键',
    `hospital_id` varchar(64) NOT NULL COMMENT '医院id',
    `device_id` varchar(64)  NOT NULL COMMENT '设备id',
    `create_date`   datetime  NOT NULL COMMENT '添加时间',
    `create_user_id` varchar(255)  NOT NULL COMMENT '创建人',
    `update_date`    datetime  COMMENT '更新时间',
    `update_user_id` varchar(255) COMMENT '更新人',
    `device_addres`  varchar(255) COMMENT '对应街道地址',
    `detect_count`   int  COMMENT '剩余检测次数',
    `del_flag` tinyint(4) default 0 NOT NULL COMMENT '是否删除（0正常 1删除）',
    `dealer_id` varchar(64)  COMMENT  '经销商ID',
     PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `sys_doctor`;
create table `sys_doctor`
(
	id             varchar(64)       not null,
	name           varchar(255)      not null comment '姓名',
	birthday       datetime          not null comment '生日',
	sex            tinyint           not null comment '性别（1男 2女）',
	hospital_id    varchar(64)       not null comment '所属医院',
	create_date    datetime          not null comment '创建时间',
	create_user_id varchar(255)      not null comment '创建人',
	update_date    datetime comment '更新时间',
	update_user_id varchar(255) comment '更新人',
	`del_flag` tinyint(4) default 0 NOT NULL COMMENT '是否删除（0正常 1删除）',
	dealer_id      varchar(64) comment '经销商ID',
	PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `sys_hospital`;
create table `sys_hospital`
(
    id             varchar(64)       not null,
    hospital_id    varchar(64)       not null comment '对应的医院字典id',
    create_date    datetime          null comment '创建时间',
    create_user_id varchar(255)      null comment '创建人',
    update_date    datetime          null comment '更新时间',
    update_user_id varchar(255)      null comment '更新人',
    dealers_id     varchar(255)      not null comment '所属经销商',
    remark         varchar(255)      null comment '备注（客户信息）',
    `del_flag` tinyint(4) default 0 NOT NULL COMMENT '是否删除（0正常 1删除）',
    PRIMARY KEY (`id`)
);

DROP TABLE IF EXISTS `sys_hospital_dictionary`;
create table `sys_hospital_dictionary`
(
    id             varchar(64)       not null,
    name           varchar(255)      not null comment '医院名称',
    address        varchar(255)      not null comment '医院地址',
    level          tinyint(255)      not null comment '医院等级（1三甲 2地方）',
    create_date    datetime          not null comment '创建时间',
    create_user_id varchar(64)       not null comment '创建人',
    update_date    datetime          null comment '更新时间',
    update_user_id varchar(64)       null comment '更新人',
    `del_flag` tinyint(4) default 0 NOT NULL COMMENT '是否删除（0正常 1删除）',
    PRIMARY KEY (`id`)
);

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
)









