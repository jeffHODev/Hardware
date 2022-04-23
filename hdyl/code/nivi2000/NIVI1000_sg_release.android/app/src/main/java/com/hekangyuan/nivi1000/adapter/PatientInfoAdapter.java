package com.hekangyuan.nivi1000.adapter;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;


import com.hekangyuan.nivi1000.R;
import com.hekangyuan.nivi1000.model.PatientInformationBean;

import java.util.List;
import java.util.Map;

/**
 * Created by liuchao on 2016/5/11.
 */
public class PatientInfoAdapter extends BaseAdapter {
   private List<PatientInformationBean>  patientInformationBean;
    private LayoutInflater layoutinfater;
    private Button bt_goumai;
    private Activity activity;


    public PatientInfoAdapter(Activity activity, List<PatientInformationBean>  patientInformationBean) {
        this.patientInformationBean = patientInformationBean;
        this.activity = activity;
        this.layoutinfater = LayoutInflater.from(activity);
    }

    @Override
    public int getCount() {
        return patientInformationBean.size();

    }

    @Override
    public Object getItem(int position) {
        // TODO Auto-generated method stub
        return patientInformationBean.get(position);
    }

    @Override
    public long getItemId(int position) {
        // TODO Auto-generated method stub
        return patientInformationBean.size();
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        PatientInformationBean patientinfo = patientInformationBean.get(position);
        convertView = layoutinfater.inflate(R.layout.list_item, null);
        TextView tv_idcard = (TextView) convertView.findViewById(R.id.tv_idcard);
        TextView tv_name = (TextView) convertView.findViewById(R.id.tv_name);
        TextView  tv_createdate = (TextView) convertView.findViewById(R.id.tv_createdate);
        tv_idcard.setText(patientinfo.getUniqueId());
        tv_name.setText(patientinfo.getName());
        tv_createdate.setText(patientinfo.getCreateDate());
        return convertView;
    }


//    public interface update_list {
//        public void update_view(List<Map<String, String>> orderList);
//    }
}
