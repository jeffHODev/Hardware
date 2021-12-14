package com.hekangyuan.nivi1000.utils;

import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;

/**
 * 验证身份证号码 身份证号码, 可以解析身份证号码的各个字段，
 * 以及验证身份证号码是否有效; 身份证号码构成：6位地址编码+8位生日+3位顺序码+1位校验码
 *
 * @ClassName: CheckIdCard
 * @Description: TODO
 * @author miemie
 *
 */
public class CheckIdCard {

    private final static String BIRTH_DATE_FORMAT = "yyyyMMdd"; // 身份证号码中的出生日期的格式

    private final static Date MINIMAL_BIRTH_DATE = new Date(-2209017600000L); // 身份证的最小出生日期,1900年1月1日

    private final static int NEW_CARD_NUMBER_LENGTH = 18;

    private final static int OLD_CARD_NUMBER_LENGTH = 15;

    private final static char[] VERIFY_CODE = { '1', '0', 'X', '9', '8', '7',
            '6', '5', '4', '3', '2' }; // 18位身份证中最后一位校验码

    private final static int[] VERIFY_CODE_WEIGHT = { 7, 9, 10, 5, 8, 4, 2, 1,
            6, 3, 7, 9, 10, 5, 8, 4, 2 };// 18位身份证中，各个数字的生成校验码时的权值


    /**
     * 如果是15位身份证号码，则自动转换为18位
     *
     * @param cardNumber
     * @return
     */
    public static boolean check(String cardNumber){
        if (null != cardNumber){
            cardNumber = cardNumber.trim();
            if (OLD_CARD_NUMBER_LENGTH == cardNumber.length()){
                cardNumber = contertToNewCardNumber(cardNumber);
            }
            return validate(cardNumber);
        }
        return false;
    }

    public static boolean validate(String cardNumber){
        boolean result = true;
        result = result && (null != cardNumber); // 身份证号不能为空
        result = result && NEW_CARD_NUMBER_LENGTH == cardNumber.length(); // 身份证号长度是18(新证)
        // 身份证号的前17位必须是阿拉伯数字
        for (int i = 0; result && i < NEW_CARD_NUMBER_LENGTH - 1; i++){
            char ch = cardNumber.charAt(i);
            result = result && ch >= '0' && ch <= '9';
        }
        // 身份证号的第18位校验正确
        result = result
                && (calculateVerifyCode(cardNumber) == cardNumber
                .charAt(NEW_CARD_NUMBER_LENGTH - 1));
        // 出生日期不能晚于当前时间，并且不能早于1900年
        try{
            Date birthDate = new SimpleDateFormat(BIRTH_DATE_FORMAT)
                    .parse(getBirthDayPart(cardNumber));
            result = result && null != birthDate;
            result = result && birthDate.before(new Date());
            result = result && birthDate.after(MINIMAL_BIRTH_DATE);
            /**
             * 出生日期中的年、月、日必须正确,比如月份范围是[1,12],
             * 日期范围是[1,31]，还需要校验闰年、大月、小月的情况时，
             * 月份和日期相符合
             */
            String birthdayPart = getBirthDayPart(cardNumber);
            String realBirthdayPart = new SimpleDateFormat(BIRTH_DATE_FORMAT)
                    .format(birthDate);
            result = result && (birthdayPart.equals(realBirthdayPart));
        }catch(Exception e){
            result = false;
        }
        return result;
    }

    private static String getBirthDayPart(String cardNumber){
        return cardNumber.substring(6, 14);
    }

    /**
     * 校验码（第十八位数）：
     *
     * 十七位数字本体码加权求和公式 S = Sum(Ai * Wi), i = 0...16 ，先对前17位数字的权求和；
     * Ai:表示第i位置上的身份证号码数字值 Wi:表示第i位置上的加权因子 Wi: 7 9 10 5 8 4 2 1 6 3 7 9 10 5 8 4
     * 2; 计算模 Y = mod(S, 11)< 通过模得到对应的校验码 Y: 0 1 2 3 4 5 6 7 8 9 10 校验码: 1 0 X 9
     * 8 7 6 5 4 3 2
     *
     * @param cardNumber
     * @return
     */
    private static char calculateVerifyCode(CharSequence cardNumber){
        int sum = 0;
        for (int i = 0; i < NEW_CARD_NUMBER_LENGTH - 1; i++){
            char ch = cardNumber.charAt(i);
            sum += ((int) (ch - '0')) * VERIFY_CODE_WEIGHT[i];
        }
        return VERIFY_CODE[sum % 11];
    }

    /**
     * 把15位身份证号码转换到18位身份证号码<br>
     * 15位身份证号码与18位身份证号码的区别为：<br>
     * 1、15位身份证号码中，"出生年份"字段是2位，转换时需要补入"19"，表示20世纪<br>
     * 2、15位身份证无最后一位校验码。18位身份证中，校验码根据根据前17位生成
     *
     * @param cardNumber
     * @return
     */
    private static String contertToNewCardNumber(String oldCardNumber){
        StringBuilder buf = new StringBuilder(NEW_CARD_NUMBER_LENGTH);
        buf.append(oldCardNumber.substring(0, 6));
        buf.append("19");
        buf.append(oldCardNumber.substring(6));
        buf.append(CheckIdCard.calculateVerifyCode(buf));
        return buf.toString();
    }

    /**
     * 获取出生日期  yyyy年MM月dd日
     * @param IDCard
     * @return
     */
    public static String getBirthday(String IDCard){
        String birthday="";
        String year="";
        String month="";
        String day="";
        if (null != IDCard && IDCard != ""){
            //15位身份证号
            if (IDCard.length() == OLD_CARD_NUMBER_LENGTH){
                // 身份证上的年份(15位身份证为1980年前的)
                year = "19" + IDCard.substring(6, 8);
                //身份证上的月份
                month = IDCard.substring(8, 10);
                //身份证上的日期
                day= IDCard.substring(10, 12);
                //18位身份证号
            }else if(IDCard.length() == NEW_CARD_NUMBER_LENGTH){
                // 身份证上的年份
                year = IDCard.substring(6).substring(0, 4);
                // 身份证上的月份
                month = IDCard.substring(10).substring(0, 2);
                //身份证上的日期
                day=IDCard.substring(12).substring(0,2);
            }
            birthday=year+"-"+month+"-"+day;
        }
        return birthday;
    }

    /**
     * 获取出生日期  yyyyMMdd
     * @param IDCard
     * @return
     */
    public static String getBirthday1(String IDCard){
        String birthday="";
        String year="";
        String month="";
        String day="";
        if (null != IDCard && IDCard != ""){
            //15位身份证号
            if (IDCard.length() == OLD_CARD_NUMBER_LENGTH){
                // 身份证上的年份(15位身份证为1980年前的)
                year = "19" + IDCard.substring(6, 8);
                //身份证上的月份
                month = IDCard.substring(8, 10);
                //身份证上的日期
                day= IDCard.substring(10, 12);
                //18位身份证号
            }else if(IDCard.length() == NEW_CARD_NUMBER_LENGTH){
                // 身份证上的年份
                year = IDCard.substring(6).substring(0, 4);
                // 身份证上的月份
                month = IDCard.substring(10).substring(0, 2);
                //身份证上的日期
                day=IDCard.substring(12).substring(0,2);
            }
            birthday=year+month+day;
        }
        return birthday;
    }

    /**
     * 日期合法check
     *
     * @param date 需要check的日期
     * @return 日期是否合法
     */
    public static boolean chkDateFormat(String date) {
        String[] dateFields = date.split("-");

        int year = 0;
        int month = 0;
        int day = 0;
        try {
            year = Integer.parseInt(dateFields[0]);
            month = Integer.parseInt(dateFields[1]);
            day = Integer.parseInt(dateFields[2]);
        } catch (Exception e) {
            return false;
        }

        List<Integer> bigMonthList = Arrays.asList(1, 3, 5, 7, 8, 10, 12);
        List<Integer> smallMonthList = Arrays.asList(4, 6, 9, 11);

        if (month == 2) {
            if (year % 4 == 0) {// 闰年2月
                if (day > 0 && day < 30) {
                    return true;
                }
            } else {// 平年二月
                if (day > 0 && day < 29) {
                    return true;
                }
            }
        }

        if (bigMonthList.contains(month)) {// 大月
            if (day > 0 && day < 32) {
                return true;
            }
        } else if (smallMonthList.contains(month)) {// 小月
            if (day > 0 && day < 31) {
                return true;
            }
        } else {// 非法
            return false;
        }

        return false;
    }
}
