package com.hekangyuan.nivi1000.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

import java.util.Map;
import java.util.Map.Entry;


public class SharePreferenceTools {
    private Context context;
    private SharedPreferences mSharedPreferences;

    public SharePreferenceTools(Context context) {
        this.context = context;
    }

    /**
     * 写入SharedPreference
     *
     * @param fileName
     * @param map
     * @return
     */
    public boolean writeSharedPreference(String fileName,
                                         Map<String, Object> map) {
        boolean flag = false;
        mSharedPreferences = context.getSharedPreferences(fileName,
                Context.MODE_PRIVATE);
        Editor editor = mSharedPreferences.edit();
        for (Entry<String, Object> entry : map.entrySet()) {
            String key = entry.getKey();
            Object object = entry.getValue();
            if (object instanceof Boolean) {
                editor.putBoolean(key, (Boolean) object);
            }
            if (object instanceof Float) {
                editor.putFloat(key, (Float) object);
            }
            if (object instanceof Integer) {
                editor.putInt(key, (Integer) object);
            }
            if (object instanceof Long) {
                editor.putLong(key, (Long) object);
            }
            if (object instanceof String) {
                editor.putString(key, (String) object);
            }
        }
        flag = editor.commit();
        return flag;
    }

    public boolean clearSharedPreference(String fileName) {
        mSharedPreferences = context.getSharedPreferences(fileName,
                Context.MODE_PRIVATE);
        Editor editor = mSharedPreferences.edit();
        editor.clear();
        return editor.commit();

    }

    public Map<String, ?> readSharedPreference(String fileName) {
        Map<String, ?> map = null;
        mSharedPreferences = context.getSharedPreferences(fileName,
                Context.MODE_PRIVATE);
        map = mSharedPreferences.getAll();
        return map;
    }
}
