package com.hospital.nivihospital.modules.doctor.dao;

import com.hospital.nivihospital.modules.doctor.entity.SysDoctor;

public interface SysDoctorMapper {
    int deleteByPrimaryKey(String id);

    int insert(SysDoctor record);

    int insertSelective(SysDoctor record);

    SysDoctor selectByPrimaryKey(String id);

    int updateByPrimaryKeySelective(SysDoctor record);

    int updateByPrimaryKey(SysDoctor record);
}