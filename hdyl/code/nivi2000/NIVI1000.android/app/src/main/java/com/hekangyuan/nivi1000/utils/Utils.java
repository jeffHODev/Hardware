package com.hekangyuan.nivi1000.utils;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.PorterDuff.Mode;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.hardware.Camera;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.telephony.TelephonyManager;
import android.text.Editable;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.WindowManager.LayoutParams;
import android.view.animation.AnimationUtils;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.hekangyuan.nivi1000.R;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Enumeration;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

public class Utils {

    /**
     * 手机号校验
     *
     * @param phone
     * @return
     */
    public static boolean isPhone(String phone) {
        String regex = "^((13[0-9])|(14[5,7,9])|(15([0-3]|[5-9]))|(166)|(17[0,1,3,5,6,7,8])|(18[0-9])|(19[8|9]))\\d{8}$";
        if (phone.length() != 11) {
//            showToast("手机号应为11位数");
            return false;
        } else {
            Pattern p = Pattern.compile(regex);
            Matcher m = p.matcher(phone);
            boolean isMatch = m.matches();
            Log.i("isPhone", "isPhone: 是否正则匹配"+isMatch);
            return isMatch;
        }
    }

    /**
     * 身份证号校验
     *
     * @param idCard
     * @return
     */
    public static boolean isIdCardNum(String idCard) {
        String reg = "^\\d{15}$|^\\d{17}[0-9Xx]$";
        if (!idCard.matches(reg)) {
            System.out.println("Format Error!");
            return false;
        }
        return true;
    }

    /**
     * 判断字符串是否为空字符串
     * 
     * @param input
     * @return
     */
    public static boolean isNotEmptyString(String input) {
        if (input != null && !input.equals("null")) {
            if (input.trim().length() > 0) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }
        
    
    public static int px2sp(Context context, float pxValue) {
        final float fontScale = context.getResources().getDisplayMetrics().scaledDensity;
        return (int) (pxValue / fontScale + 0.5f);
    }

    public static int dp2px(Context context, int dp) {
        DisplayMetrics displayMetrics = context.getResources().getDisplayMetrics();
        return (int) ((dp * displayMetrics.density) + 0.5);
    }

    public static boolean stringFilter(String str) throws PatternSyntaxException {
        if(str.replaceAll("[\u4e00-\u9fa5]*[a-z]*[A-Z]*\\d*-*_*\\s*","").length()==0){ 
            //不包含特殊字符 
            return false; 
        } 
        return true;
    }  
    
    /**
     * 不让空格输入
     * @param editText
     */
    public static void replaceSpace(EditText editText){
        String inputStr = editText.getText().toString();
        if(inputStr.contains(" ")){
            editText.getText().delete(editText.getSelectionStart()-1, editText.getSelectionStart());
        }
    }
    
    /**
     * 不让空格输入
     * @param
     */
    public static String replaceSpace(String input){
        if(input == null || input.equals("null") || "".equals(input)){
            return "";
        }
        return input.replaceAll(" ", "");
    }
    
    public static Bitmap stringtoBitmap(String string) {
        //将字符串转换成Bitmap类型
        Bitmap bitmap = null;
        try {
            byte[] bitmapArray;
            bitmapArray = Base64.decode(string, Base64.DEFAULT);
            bitmap = BitmapFactory.decodeByteArray(bitmapArray, 0,
                bitmapArray.length);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return bitmap;
    }

    public static String getDateString(long l) {
        Date currentTime = new Date(l);
        SimpleDateFormat formatter = new SimpleDateFormat(
            "yyyy-MM-dd  HH:mm:ss");
        String dateString = formatter.format(currentTime);
        return dateString;
    }

    public static Point getScreenSize(Activity activity) {
        Point outSize = new Point();
        activity.getWindowManager().getDefaultDisplay().getSize(outSize);
        return outSize;
    }


    /**
     * @param context
     */
    public static void hiddenKeyBoard(Context context) {

        try {
            if (context == null) {
                return;
            }
            // 取消弹出的对话框
            InputMethodManager manager = (InputMethodManager) context
                .getSystemService(Context.INPUT_METHOD_SERVICE);
            manager.hideSoftInputFromWindow(((Activity) context)
                .getCurrentFocus().getWindowToken(),
                InputMethodManager.HIDE_NOT_ALWAYS);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static String convertNickname(String nickName, String compareNickName) {
        if (nickName == null || compareNickName == null) {
            return nickName;
        }
        if (nickName.equals(compareNickName.trim())) {
            return "我";
        }
        return nickName;
    }

    // 判断是否10富
    public static boolean isRich(long wealth) {
        if (wealth >= 5742000) {
            return true;
        } else {
            return false;
        }
    }

    public static final float[] credits = { 0, 50, 200, 500, 1000, 2000, 4000,
        7500, 13000, 21000, 33000, 48000, 68000, 98000, 148000, 228000, 348000,
        528000, 778000, 1108000, 1518000, 2018000, 2618000, 3318000, 4118000,
        5018000, 6018000, 7118000, 8368000, 9868000 };

    /**
     * 读取字节流，根据网络地址
     * 
     * @param
     *        ：根据网络地址返回的流
     * @return：返回数组
     * @throws IOException
     *         ：流异常
     */
    public static String read(InputStream is) throws IOException {
        byte[] data;
        ByteArrayOutputStream bout = new ByteArrayOutputStream();
        byte[] buf = new byte[1024];
        int len = 0;
        while ((len = is.read(buf)) != -1) {
            bout.write(buf, 0, len);
        }
        data = bout.toByteArray();
        bout.close();
        bout = null;
        return new String(data, "UTF-8");
    }

    /**
     * 转换图片成圆形
     * 
     * @param bitmap
     *        传入Bitmap对象
     * @return
     */
    public static Bitmap toRoundBitmap(Bitmap bitmap, int width, int height,
                                       Context context) {
        width = Utils.dip2px(context, width);
        height = Utils.dip2px(context, height);

        Bitmap output = Bitmap.createBitmap(width, height, Config.ARGB_8888);
        Canvas canvas = new Canvas(output);

        final int color = 0xff424242;

        final Paint paint = new Paint();
        final Rect rect = new Rect(0, 0, bitmap.getWidth(), bitmap.getHeight());
        final Rect mRect = new Rect(0, 0, width, height);

        final RectF rectF = new RectF(mRect);
        final float roundPx = width / 2;

        paint.setAntiAlias(true);
        canvas.drawARGB(0, 0, 0, 0);
        paint.setColor(color);
        canvas.drawRoundRect(rectF, roundPx, roundPx, paint);
        paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
        canvas.drawBitmap(bitmap, rect, mRect, paint);

        return output;
    }

    /**
     * 根据手机的分辨率从 dp 的单位 转成为 px(像素)
     */
    public static int dip2px(Context context, float dpValue) {
        final float scale = context.getResources().getDisplayMetrics().density;
        return (int) (dpValue * scale + 0.5f);
    }

    public static String getLineString(InputStream in) {
        try {
            StringBuffer sb = new StringBuffer();
            BufferedReader read = new BufferedReader(new InputStreamReader(in));
            String temp;
            try {
                while ((temp = read.readLine()) != null) {
                    sb.append(temp);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
            return sb.toString();

        } catch (Exception e) {
            e.printStackTrace();
            return "";
        } catch (Error e) {
            return "";
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public interface DialogListenner {
        public void confirm();
    }

    public interface DialogInputListenner {
        public void confirm(String input);
    }

    /**
     * bitmap转为base64
     * 
     * @param bitmap
     * @return
     */
    public static String bitmapToBase64(Bitmap bitmap) {

        String result = null;
        ByteArrayOutputStream baos = null;
        try {
            if (bitmap != null) {
                baos = new ByteArrayOutputStream();
                bitmap.compress(Bitmap.CompressFormat.JPEG, 100, baos);

                baos.flush();
                baos.close();

                byte[] bitmapBytes = baos.toByteArray();
                result = Base64.encodeToString(bitmapBytes, Base64.DEFAULT);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (baos != null) {
                    baos.flush();
                    baos.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return result;
    }

    /**
     * base64转为bitmap
     * 
     * @param base64Data
     * @return
     */
    public static Bitmap base64ToBitmap(String base64Data) {
        byte[] bytes = Base64.decode(base64Data, Base64.DEFAULT);
        return BitmapFactory.decodeByteArray(bytes, 0, bytes.length);
    }

    /**
     * 转换图片成圆形
     * 
     * @param bitmap
     *        传入Bitmap对象
     * @return
     */
    public static Bitmap toRoundBitmap(Bitmap bitmap) {
        try {

            Bitmap output = Bitmap.createBitmap(bitmap.getWidth(),
                bitmap.getHeight(), Config.ARGB_8888);
            Canvas canvas = new Canvas(output);

            final int color = 0xff424242;

            final Paint paint = new Paint();
            final Rect rect = new Rect(0, 0, bitmap.getWidth(),
                bitmap.getHeight());

            final RectF rectF = new RectF(rect);
            final float roundPx = bitmap.getWidth() / 2;

            paint.setAntiAlias(true);
            canvas.drawARGB(0, 0, 0, 0);
            paint.setColor(color);
            canvas.drawRoundRect(rectF, roundPx, roundPx, paint);
            paint.setXfermode(new PorterDuffXfermode(Mode.SRC_IN));
            canvas.drawBitmap(bitmap, rect, rect, paint);
            return output;

        } catch (Exception e) {
            e.printStackTrace();
        } catch (Error e) {
            e.printStackTrace();
            System.gc();
        }
        return null;
    }

    /**
     * 数据加密 不能有特殊字符
     * 
     * @param text
     * @return
     * @author by_wsc
     * @email wscnydx@gmail.com
     * @date 日期：2014-1-15 下午3:15:58
     */
    public static String stringEncode(String text) {
        if (TextUtils.isEmpty(text)) {
            return "";
        }
        String encode = Base64.encodeToString(text.getBytes(), Base64.DEFAULT);
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < encode.length(); i++) {
            char oldChar = encode.charAt(i);
            char newChar = (char) (oldChar * 2 + 1);
            sb.append(newChar);
        }
        return sb.toString();
    }

    /**
     * 数据解密 不能有特殊字符
     * 
     * @param text
     * @return
     * @author by_wsc
     * @email wscnydx@gmail.com
     * @date 日期：2014-1-15 下午3:18:56
     */
    public static String stringDecode(String text) {
        if (TextUtils.isEmpty(text)) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < text.toString().length(); i++) {
            char oldChar = text.toString().charAt(i);
            char newChar = (char) ((oldChar - 1) / 2);
            sb.append(newChar);
        }

        return new String(Base64.decode(sb.toString(), Base64.DEFAULT));

    }

    /**
     * 判断网络是否可用
     * 
     * @param context
     * @return
     */
    public static boolean isNetwokAvailable(Context context) {
            if(context == null){
                return false;
            }

            ConnectivityManager connectivity = (ConnectivityManager) context
                    .getSystemService(Context.CONNECTIVITY_SERVICE);
            if (connectivity != null) {  
                NetworkInfo info = connectivity.getActiveNetworkInfo();
                if (info != null && info.isConnected())   
                {  
                    // 当前网络是连接的  
                    if (info.getState() == NetworkInfo.State.CONNECTED)
                    {  
                        // 当前所连接的网络可用  
                        return true;  
                    }  
                }  
            }  
            return false;  
    }

    public static void dial(String number, Context context) {
        if(context == null){
            return;
        }
        Class<TelephonyManager> c = TelephonyManager.class;
        Method getITelephonyMethod = null;
        try {
            getITelephonyMethod = c.getDeclaredMethod("getITelephony",
                (Class[]) null);
            getITelephonyMethod.setAccessible(true);
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

        try {
            TelephonyManager tManager = (TelephonyManager) context
                .getSystemService(Context.TELEPHONY_SERVICE);
            Object iTelephony;
            if (getITelephonyMethod == null) {
                return;
            }
            iTelephony = getITelephonyMethod.invoke(tManager, (Object[]) null);
            Method dial = iTelephony.getClass().getDeclaredMethod("dial",
                String.class);
            dial.invoke(iTelephony, number);
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
    }

    public static void call(String number, Context context) {
        if(context == null){
            return;
        }
        Class<TelephonyManager> c = TelephonyManager.class;
        Method getITelephonyMethod = null;
        try {
            getITelephonyMethod = c.getDeclaredMethod("getITelephony",
                (Class[]) null);
            getITelephonyMethod.setAccessible(true);
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

        try {
            TelephonyManager tManager = (TelephonyManager) context
                .getSystemService(Context.TELEPHONY_SERVICE);
            Object iTelephony;
            if (getITelephonyMethod == null) {
                return;
            }
            iTelephony = getITelephonyMethod.invoke(tManager, (Object[]) null);
            Method dial = iTelephony.getClass().getDeclaredMethod("call",
                String.class);
            dial.invoke(iTelephony, number);
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
    }

    public static boolean isDigital(String number) {
        char[] ar = number.toCharArray();
        for (int i = 0; i < ar.length; i++) {
            if (!Character.isDigit(ar[i])) {
                return false;
            }
        }
        return true;
    }

//    public final static String MD5(String s) {
//        return new MD5Util().getMD5ofStr(s);
//    }
//
    /**
     * 进入下一个页面动画 左右滑动
     */
    public static void enterActivityAnimation(Activity act, boolean finish) {
        if (act != null) {
            act.overridePendingTransition(R.anim.act_open_enter,
                R.anim.act_open_exit);
            if (finish) {
                act.finish();
            }
        }
    }

    /**
     * 退出当前页面动画 左右滑动
     */
    public static void exitActivityAnimation(Activity act) {
        if (act != null) {
            act.overridePendingTransition(R.anim.act_close_enter,
                R.anim.act_close_exit);
            act.finish();
        }
    }

//    public static void customDialog(Context context, String content,
//                                    final DialogListenner dialogListenner, String...title) {
//        Activity activity = (Activity) context;
//        if(activity == null || activity.isFinishing()){
//            return;
//        }
//        final AlertDialog dialog = new AlertDialog.Builder(context).create();
//        dialog.show();
//        Window window = dialog.getWindow();
//        View view = LayoutInflater.from(context).inflate(R.layout.alert_dialog,
//            null);
//        window.setContentView(view);
//        WindowManager windowManager = ((Activity) context).getWindowManager();
//        Display display = windowManager.getDefaultDisplay();
//        LayoutParams params = window.getAttributes();
//        Point point = new Point();
//        display.getSize(point);
//        params.width = (int) (point.x * 0.8);
//        window.setAttributes(params);
//
//        TextView titleTextView = (TextView) window.findViewById(R.id.title);
//        TextView contentTextView = (TextView) window.findViewById(R.id.content);
//        Button confirm = (Button) window.findViewById(R.id.confirm);
//        Button cancel = (Button) window.findViewById(R.id.cancel);
//
//        titleTextView.setText("温馨提示");
//        if(title!=null && title.length>0 && isNotEmptyString(title[0])){
//            if("cancel".equals(title[0])){
//                cancel.setVisibility(View.GONE);
//            }else{
//                titleTextView.setText(title[0]);
//            }
//        }
//        contentTextView.setText(content);
//        confirm.setText("确认");
//        confirm.setOnClickListener(new OnClickListener() {
//
//            @Override
//            public void onClick(View arg0) {
//                if (dialog != null) {
//                    dialog.dismiss();
//                }
//                dialogListenner.confirm();
//            }
//        });
//        cancel.setText("取消");
//        cancel.setOnClickListener(new OnClickListener() {
//
//            @Override
//            public void onClick(View arg0) {
//                dialog.cancel();
//            }
//        });
//    }
    
//    public static void dialDialog(Context context, String content,
//                                  final DialogListenner dialogListenner) {
//        Activity activity = (Activity) context;
//        if(activity == null || activity.isFinishing()){
//            return;
//        }
//        final AlertDialog dialog = new AlertDialog.Builder(context).create();
//        dialog.show();
//        Window window = dialog.getWindow();
//        View view = LayoutInflater.from(context).inflate(R.layout.alert_dialog,
//            null);
//        window.setContentView(view);
//        WindowManager windowManager = ((Activity) context).getWindowManager();
//        Display display = windowManager.getDefaultDisplay();
//        LayoutParams params = window.getAttributes();
//        Point point = new Point();
//        display.getSize(point);
//        params.width = (int) (point.x * 0.8);
//        window.setAttributes(params);
//
//        TextView titleTextView = (TextView) window.findViewById(R.id.title);
//        TextView contentTextView = (TextView) window.findViewById(R.id.content);
//        Button confirm = (Button) window.findViewById(R.id.confirm);
//        Button cancel = (Button) window.findViewById(R.id.cancel);
//
//        titleTextView.setText("温馨提示");
//
//        contentTextView.setText(content);
//        confirm.setText("呼叫");
//        confirm.setOnClickListener(new OnClickListener() {
//
//            @Override
//            public void onClick(View arg0) {
//                if (dialog != null) {
//                    dialog.dismiss();
//                }
//                dialogListenner.confirm();
//            }
//        });
//        cancel.setText("取消");
//        cancel.setOnClickListener(new OnClickListener() {
//
//            @Override
//            public void onClick(View arg0) {
//                dialog.cancel();
//            }
//        });
//    }

//    public static void customInputDialog(Context context, String title,
//                                         final DialogInputListenner dialogInputListenner) {
//        if(context == null){
//            return;
//        }
//        final Dialog dialog = new Dialog(context, R.style.input_dialog);
//        dialog.show();
//        Window window = dialog.getWindow();
//        View view = LayoutInflater.from(context).inflate(
//            R.layout.alert_input_dialog, null);
//        window.setContentView(view);
//        WindowManager windowManager = ((Activity) context).getWindowManager();
//        Display display = windowManager.getDefaultDisplay();
//        LayoutParams params = window.getAttributes();
//        Point point = new Point();
//        display.getSize(point);
//        params.width = (int) (point.x * 0.8);
//        window.setAttributes(params);
//
//        TextView mainContent = (TextView) window.findViewById(R.id.title);
//        final EditText input = (EditText) window.findViewById(R.id.input);
//        input.setFocusable(true);
//        Button confirm = (Button) window.findViewById(R.id.confirm);
//        Button cancel = (Button) window.findViewById(R.id.cancel);
//
//        mainContent.setText(title);
//        confirm.setText("确认");
//        confirm.setOnClickListener(new OnClickListener() {
//
//            @Override
//            public void onClick(View arg0) {
//                if (dialog != null) {
//                    dialog.dismiss();
//                }
//                dialogInputListenner.confirm(input.getText().toString());
//            }
//        });
//        cancel.setText("取消");
//        cancel.setOnClickListener(new OnClickListener() {
//
//            @Override
//            public void onClick(View arg0) {
//                dialog.cancel();
//            }
//        });
//    }
    
//    public static void customImageDialog(Context context, String title, int resId, Spanned spanned,
//                                         final DialogListenner dialogListenner) {
//        if(context == null){
//            return;
//        }
//        final Dialog dialog = new Dialog(context, R.style.input_dialog);
//        dialog.show();
//        Window window = dialog.getWindow();
//        View view = LayoutInflater.from(context).inflate(
//            R.layout.custom_image_dialog, null);
//        window.setContentView(view);
//        WindowManager windowManager = ((Activity) context).getWindowManager();
//        Display display = windowManager.getDefaultDisplay();
//        LayoutParams params = window.getAttributes();
//        Point point = new Point();
//        display.getSize(point);
//        params.width = (int) (point.x * 0.8);
//        window.setAttributes(params);
//
//        TextView mainContent = (TextView) window.findViewById(R.id.title);
//        TextView tip = (TextView) window.findViewById(R.id.notice_tip);
//        ImageView imageView = (ImageView) window.findViewById(R.id.image_notice);
//        imageView.setBackgroundResource(resId);
//        tip.setText(spanned);
//
//        Button confirm = (Button) window.findViewById(R.id.confirm);
//
//        mainContent.setText(title);
//        confirm.setText("去拍照");
//        confirm.setOnClickListener(new OnClickListener() {
//
//            @Override
//            public void onClick(View arg0) {
//                if (dialog != null) {
//                    dialog.dismiss();
//                }
//                dialogListenner.confirm();
//            }
//        });
//    }

    public static Toast toast;

    public static void MakeToast(Context context, String message) {
        if (context != null) {
            LayoutInflater inflate = (LayoutInflater) context
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
            View view = inflate.inflate(R.layout.custom_toast, null);
            if (Utils.toast == null) {
                Utils.toast = new Toast(context);
                Utils.toast.setView(view);
                Utils.toast.setText(message);
                Utils.toast.setDuration(Toast.LENGTH_SHORT);
                Utils.toast.setGravity(Gravity.CENTER, 0, 200);
            } else {
                Utils.toast.setView(view);
                Utils.toast.setText(message);
            }
            Utils.toast.show();
        }
    }

//    /**
//     * 生成对话框
//     *
//     * @param context
//     * @return
//     */
//    public static Dialog showProgressDialog(Context context, String message,
//                                            boolean cancelableOnTouch) {
//        Dialog dialog = new Dialog(context, R.style.loading_dialog);
//        dialog.setCanceledOnTouchOutside(false);
//        dialog.setCancelable(false);
//        View view = LayoutInflater.from(context).inflate(R.layout.ns_loading,
//            null);
//        dialog.setContentView(view);
//        TextView tv = (TextView) view.findViewById(R.id.tipTextView);
//        tv.setText(message);
//        ImageView imageView = (ImageView) view.findViewById(R.id.img);
//        imageView.startAnimation(AnimationUtils.loadAnimation(context,
//            R.anim.loading_animation));
//        return dialog;
//    }

//    public static View showImageViewProgress(Context context, String message) {
//        View view = LayoutInflater.from(context).inflate(R.layout.ns_loading,
//            null);
//        TextView tv = (TextView) view.findViewById(R.id.tipTextView);
//        tv.setText(message);
//        ImageView imageView = (ImageView) view.findViewById(R.id.img);
//        imageView.startAnimation(AnimationUtils.loadAnimation(context,
//            R.anim.loading_animation));
//        return view;
//    }

    public static RelativeLayout.LayoutParams getLayoutParams() {
        RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(
            ViewGroup.LayoutParams.WRAP_CONTENT,
            ViewGroup.LayoutParams.WRAP_CONTENT);
        layoutParams.addRule(RelativeLayout.CENTER_IN_PARENT,
            RelativeLayout.TRUE);
        return layoutParams;
    }


    public static boolean isLetterDigit(String str) {
        boolean isDigit = false;
        boolean isLetter = false;
        for (int i = 0; i < str.length(); i++) {
            if (Character.isDigit(str.charAt(i))) {
                isDigit = true;
            }
            if (Character.isLetter(str.charAt(i))) {
                isLetter = true;
            }
        }
        String regex = "^[a-zA-Z0-9]+$";
        boolean isRight = isDigit && isLetter && str.matches(regex);
        return isRight;

    }

    public static boolean isPasswordContinuous(String pwd) {
        int count = 0;//正序次数  
        int reverseCount = 0;//反序次数  
        String[] strArr = pwd.split("");
        for (int i = 1; i < strArr.length - 1; i++) {//从1开始是因为划分数组时，第一个为空  
            if (isPositiveContinuous(strArr[i], strArr[i + 1])) {
                count++;
            } else {
                count = 0;
            }
            if (isReverseContinuous(strArr[i], strArr[i + 1])) {
                reverseCount++;
            } else {
                reverseCount = 0;
            }
            if (count > 4 || reverseCount > 4)
                break;
        }
        if (count > 4 || reverseCount > 4)
            return true;
        return false;
    }

    public static boolean isPasswordSame(String pwd) {
        int count = 0;
        String[] strArr = pwd.split("");
        for (int i = 1; i < strArr.length - 1; i++) {//从1开始是因为划分数组时，第一个为空  
            if ((strArr[i].hashCode() - strArr[i + 1].hashCode()) == 0) {
                count++;
            } else {
                count = 0;
            }
            if (count > 4)
                break;
        }
        if (count > 4)
            return true;
        return false;
    }

    private static boolean isPositiveContinuous(String str1, String str2) {
        if (str2.hashCode() - str1.hashCode() == 1)
            return true;
        return false;
    }

    private static boolean isReverseContinuous(String str1, String str2) {
        if (str2.hashCode() - str1.hashCode() == -1)
            return true;
        return false;
    }

    /**
     * 验证邮箱
     * 
     * @param email
     * @return
     */
    public static boolean checkEmail(String email) {
        Pattern pattern = Pattern
            .compile("^[A-Za-z0-9][\\w\\._]*+@[A-Za-z0-9-_]+\\.([A-Za-z]{2,4})");
        Matcher mc = pattern.matcher(email);
        return mc.matches();
    }

    /**
     * 验证手机号码
     * 
     * @param
     * @return
     */
    public static boolean checkMobileNumber(String mobileNumber) {
        boolean flag = false;
        try {
            Pattern regex = Pattern
                .compile("^(((13[0-9])|(15([0-3]|[5-9]))|(18[0,5-9]))\\d{8})|(0\\d{2}-\\d{8})|(0\\d{3}-\\d{7})$");
            Matcher matcher = regex.matcher(mobileNumber);
            flag = matcher.matches();
        } catch (Exception e) {
            flag = false;
        }
        return flag;
    }

    /**
     * 设置TextView某几个字符点击效果
     * 
     * @param msg
     *        要显示的内容
     * @param textView
     *        某个特定的textView
     * @param start
     *        从哪个位置开始，0是第一个
     * @param end
     *        结束位置
     * @param context
     *        上下文
     * @param
     *
     */
    public static void setTextViewClick(String msg, TextView textView,
                                        int start, int end, final Context context,
                                        final TextViewClickListener listener, final int color, final boolean isUderLine) {
        SpannableString ssb = new SpannableString(msg);
        ssb.setSpan(new ClickableSpan() {

            @Override
            public void onClick(View view) {
                listener.onClick(view);
            }

            @Override
            public void updateDrawState(TextPaint ds) {
                ds.setTypeface(Typeface.DEFAULT_BOLD);
                if(context != null){
                    ds.setColor(context.getResources().getColor(
                        color));
                }
                ds.setUnderlineText(false);
            }

        }, start, end, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        if(context != null){
            textView.setHighlightColor(context.getResources().getColor(
                android.R.color.transparent));//方法重新设置文字背景为透明色。
        }
        textView.append(ssb);
        textView.setMovementMethod(LinkMovementMethod.getInstance());
    }

    public interface TextViewClickListener {
        public void onClick(View view);
    }

    /**
     * 半角转换为全角
     * 
     * @param input
     * @return
     */
    public static String ToDBC(String input) {
        char[] c = input.toCharArray();
        for (int i = 0; i < c.length; i++) {
            if (c[i] == 12288) {
                c[i] = (char) 32;
                continue;
            }
            if (c[i] > 65280 && c[i] < 65375)
                c[i] = (char) (c[i] - 65248);
        }
        return new String(c);
    }

    /**
     * 　　* 获取版本号
     * 　　* @return 当前应用的版本号
     * 　　
     */
    public static String getVersion(Context context) {
        if(context == null){
            return "1.0";
        }
        PackageManager manager = context.getPackageManager();
        try {
            PackageInfo info;
            info = manager.getPackageInfo(context.getPackageName(), 0);
            String version = info.versionName;
            return version;
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            return "1.0";
        }
    }

    public static String getProtectedMobile(String phoneNumber) {
        if (TextUtils.isEmpty(phoneNumber) || phoneNumber.length() < 11) {
            return "";
        }
        StringBuilder builder = new StringBuilder();
        builder.append(phoneNumber.subSequence(0, 3));
        builder.append("****");
        builder.append(phoneNumber.subSequence(7, 11));
        return builder.toString();
    }


    private static final int MIN_CLICK_DELAY_TIME = 5000;
    private static long lastClickTime = 0l;
    private static long lastNextClickTime = 01;

    public static boolean isFastClick() {
        boolean flag = false;
        long curClickTime = System.currentTimeMillis();
        if ((curClickTime - lastClickTime) < MIN_CLICK_DELAY_TIME) {
            flag = true;
        }
        lastClickTime = curClickTime;
        return flag;
    }

    public static boolean nextFastClick() {
        boolean flag = false;
        long curClickTime = System.currentTimeMillis();
        if ((curClickTime - lastNextClickTime) < 2000) {
            flag = true;
        }
        lastNextClickTime = curClickTime;
        return flag;
    }


    public static float[] unWrap(Float[] source){
        float[] result = new float[source.length];
        for(int i=0; i<source.length; i++){
            result[i] = source[i].floatValue();
        }
        return result;
    }

    public static Float[] wrap(float[] source){
        Float[] result = new Float[source.length];
        for(int i=0; i<source.length; i++){
            result[i] = Float.valueOf(source[i]);
        }
        return result;
    }

    public static float[] toArray(List<Float> source){
        return Utils.unWrap(source.toArray(new Float[source.size()]));
    }



}
