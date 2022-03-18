package util;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * Created by Administrator on 2018/6/15.
 */

public class ByteUtil {

    /**
     * 字符串转化成为16进制字符串
     *
     * @param s
     * @return
     */
    public static String strTo16(String s) {
        String str = "";
        for (int i = 0; i < s.length(); i++) {
            int ch = (int) s.charAt(i);
            String s4 = Integer.toHexString(ch);
            str = str + s4;
        }
        return str;
    }

    /**
     * 16进制转换成为string类型字符串
     *
     * @param s
     * @return
     */
    public static String hexStringToString(String s) {
        if (s == null || s.equals("")) {
            return null;
        }
        s = s.replace(" ", "");
        byte[] baKeyword = new byte[s.length() / 2];
        for (int i = 0; i < baKeyword.length; i++) {
            try {
                baKeyword[i] = (byte) (0xff & Integer.parseInt(s.substring(i * 2, i * 2 + 2), 16));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        try {
            s = new String(baKeyword, "UTF-8");
            new String();
        } catch (Exception e1) {
            e1.printStackTrace();
        }
        return s;
    }

    /**
     * 向串口发送数据转为字节数组
     */
    public static byte[] hex2byte(String hex) {
        String digital = "0123456789ABCDEF";
        String hex1 = hex.replace(" ", "");
        char[] hex2char = hex1.toCharArray();
        byte[] bytes = new byte[hex1.length() / 2];
        byte temp;
        for (int p = 0; p < bytes.length; p++) {
            temp = (byte) (digital.indexOf(hex2char[2 * p]) * 16);
            temp += digital.indexOf(hex2char[2 * p + 1]);
            bytes[p] = (byte) (temp & 0xff);
        }
        return bytes;
    }

    // 判断奇数或偶数，位运算，最后一位是1则为奇数，为0是偶数
    static public int isOdd(int num)
    {
        return num & 0x1;
    }
    //-------------------------------------------------------
    static public int HexToInt(String inHex)//Hex字符串转int
    {
        return Integer.parseInt(inHex, 16);
    }
    //-------------------------------------------------------
    static public byte HexToByte(String inHex)//Hex字符串转byte
    {
        return (byte)Integer.parseInt(inHex,16);
    }

    static public byte HexToByte1(String inHex)//Hex字符串转byte
    {
        return (byte)Integer.parseInt(inHex,10);
    }


    static public byte[] HexToByteArr(String inHex)//hex字符串转字节数组
    {
        int hexlen = inHex.length();
        byte[] result;
        if (isOdd(hexlen)==1)
        {//奇数
            hexlen++;
            result = new byte[(hexlen/2)];
            inHex="0"+inHex;
        }else {//偶数
            result = new byte[(hexlen/2)];
        }
        int j=0;
        for (int i = 0; i < hexlen; i+=2)
        {
            result[j]=HexToByte(inHex.substring(i,i+2));
            j++;
        }
        return result;
    }

    /**
     * 接收到的字节数组转换16进制字符串
     */
    public static String bytes2HexString(byte[] b, int size) {
        String ret = "";
        for (int i = 0; i < size; i++) {
            String hex = Integer.toHexString(b[i] & 0xFF);
            if (hex.length() == 1) {
                hex = '0' + hex;
            }
            ret += hex.toUpperCase();
        }
        return ret;
    }

    public static String bytesToHexString(byte[] src) {
        StringBuilder stringBuilder = new StringBuilder("");
        if (src == null || src.length <= 0) {
            return null;
        }
        for (int i = 0; i < src.length; i++) {
            int v = src[i] & 0xFF;
            String hv = Integer.toHexString(v);
            if (hv.length() < 2) {
                stringBuilder.append(0);
            }
            stringBuilder.append(hv);
        }
        return stringBuilder.toString();
    }

    /**
     * 接收到的字节数组转换16进制字符串
     */
    public static String byteToStr(byte[] b, int size) {
        String ret = "";
        for (int i = 0; i < size; i++) {
            String hex = Integer.toHexString(b[i] & 0xFF);
            if (hex.length() == 1) {
                hex = '0' + hex;
            }
            ret += hex.toUpperCase();
        }
        return ret;

    }

    /**
     * 计算CRC16校验码
     * 逐个求和
     *
     * @param bytes 字节数组
     * @return {@link String} 校验码
     * @since 1.0
     */
    public static String getCRC_16(byte[] bytes) {
        int CRC = 0x0000ffff;
        int POLYNOMIAL = 0x0000a001;
        int i, j;
        for (i = 0; i < bytes.length; i++) {
            CRC ^= ((int) bytes[i] & 0x000000ff);
            for (j = 0; j < 8; j++) {
                if ((CRC & 0x00000001) != 0) {
                    CRC >>= 1;
                    CRC ^= POLYNOMIAL;
                } else {
                    CRC >>= 1;
                }
            }
        }
        if (Integer.toHexString(CRC).toUpperCase().length() == 2) {
            return byteToStr(bytes, bytes.length) + "00" + Integer.toHexString(CRC).toUpperCase();
        } else if (Integer.toHexString(CRC).toUpperCase().length() == 3) {
            return byteToStr(bytes, bytes.length) + "0" + Integer.toHexString(CRC).toUpperCase();
        }
        return byteToStr(bytes, bytes.length) + Integer.toHexString(CRC).toUpperCase();
    }

    /**
     * 指令校验和,并取出后两位字节
     * */
    public static String getSum16(byte[] msg, int length) {
        long mSum = 0;
        byte[] mByte = new byte[length];

        /** 逐Byte添加位数和 */
        for (byte byteMsg : msg) {
            long mNum = ((long) byteMsg >= 0) ? (long) byteMsg : ((long) byteMsg + 256);
            mSum += mNum;
        } /** end of for (byte byteMsg : msg) */

        /** 位数和转化为Byte数组 */
        for (int liv_Count = 0; liv_Count < length; liv_Count++) {
            mByte[length - liv_Count - 1] = (byte) (mSum >> (liv_Count * 8) & 0xff);
        } /** end of for (int liv_Count = 0; liv_Count < length; liv_Count++) */
        return byteToStr(msg, length) + byteToStr(mByte, mByte.length).substring(byteToStr(mByte, mByte.length).length() - 4, byteToStr(mByte, mByte.length).length());
    }
    public static  byte aucCRCHi[] = {
            (byte) 0x00, (byte) 0xC1, (byte)0x81,(byte) 0x40,(byte) 0x01, (byte)0xC0,(byte) 0x80, (byte)0x41,(byte) 0x01,(byte) 0xC0, (byte)0x80, (byte)0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40,(byte) 0x01, (byte)0xC0, (byte)0x80, (byte)0x41,(byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40,(byte) 0x01, (byte)0xC0, (byte)0x80, (byte)0x41, (byte)0x01,(byte) 0xC0,(byte) 0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x00, (byte)0xC1, (byte)0x81, (byte)0x40, (byte)0x01,(byte) 0xC0,(byte) 0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x01, (byte)0xC0, (byte)0x80, (byte)0x41, (byte)0x01,(byte) 0xC0,(byte) 0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x01, (byte)0xC0, (byte)0x80, (byte)0x41, (byte)0x00, (byte)0xC1, (byte)0x81,(byte) 0x40,
            (byte) 0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x01, (byte)0xC0, (byte)0x80, (byte)0x41, (byte)0x00,(byte) 0xC1, (byte)0x81, (byte)0x40,
            (byte)0x01, (byte)0xC0, (byte)0x80, (byte)0x41,(byte) 0x01, (byte)0xC0, (byte)0x80, (byte)0x41, (byte)0x00,(byte) 0xC1, (byte)0x81, (byte)0x40,
            (byte)0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x01, (byte)0xC0, (byte)0x80, (byte)0x41, (byte)0x01, (byte)0xC0, (byte)0x80, (byte)0x41,
            (byte)0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x01,(byte) 0xC0,(byte) 0x80, (byte)0x41, (byte)0x00,(byte) 0xC1, (byte)0x81,(byte) 0x40,
            (byte)0x00, (byte)0xC1, (byte)0x81,(byte) 0x40,(byte) 0x01,(byte) 0xC0,(byte) 0x80, (byte)0x41, (byte)0x01, (byte)0xC0, (byte)0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x00,(byte) 0xC1, (byte)0x81, (byte)0x40, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x01,(byte) 0xC0, (byte)0x80, (byte)0x41, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81, (byte)0x40,(byte) 0x00,(byte) 0xC1, (byte)0x81, (byte)0x40, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte) 0x41,
            (byte) 0x01, (byte)0xC0, (byte)0x80, (byte)0x41,(byte) 0x00,(byte) 0xC1, (byte)0x81,(byte) 0x40, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40, (byte)0x00,(byte) 0xC1, (byte)0x81,(byte) 0x40, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte)0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte) 0x41, (byte)0x01,(byte) 0xC0, (byte)0x80, (byte)0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte) 0x41,(byte) 0x00,(byte) 0xC1, (byte)0x81, (byte)0x40,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte) 0x41,(byte) 0x01,(byte) 0xC0, (byte)0x80, (byte)0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40, (byte)0x00,(byte) 0xC1, (byte)0x81,(byte) 0x40,(byte) 0x01,(byte) 0xC0,(byte) 0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40, (byte)0x01,(byte) 0xC0, (byte)0x80,(byte) 0x41,(byte) 0x01, (byte)0xC0, (byte)0x80,(byte) 0x41,
            (byte) 0x00, (byte)0xC1, (byte)0x81,(byte) 0x40
    };

    public static  byte aucCRCLo[] = {
            (byte)0x00, (byte)0xC0, (byte)0xC1, (byte)0x01, (byte)0xC3, (byte)0x03, (byte)0x02, (byte)0xC2, (byte)0xC6, (byte)0x06, (byte)0x07, (byte)0xC7,
            (byte)0x05, (byte)0xC5, (byte)0xC4, (byte)0x04, (byte)0xCC, (byte)0x0C, (byte)0x0D, (byte)0xCD, (byte)0x0F, (byte)0xCF, (byte)0xCE, (byte)0x0E,
            (byte) 0x0A, (byte)0xCA, (byte)0xCB, (byte)0x0B, (byte)0xC9, (byte)0x09, (byte)0x08, (byte)0xC8, (byte)0xD8, (byte)0x18, (byte)0x19, (byte)0xD9,
            (byte) 0x1B, (byte)0xDB, (byte)0xDA, (byte)0x1A, (byte)0x1E, (byte)0xDE, (byte)0xDF, (byte)0x1F, (byte)0xDD, (byte)0x1D, (byte)0x1C, (byte)0xDC,
            (byte) 0x14, (byte)0xD4, (byte)0xD5, (byte)0x15, (byte)0xD7, (byte)0x17, (byte)0x16, (byte)0xD6, (byte)0xD2, (byte)0x12, (byte)0x13, (byte)0xD3,
            (byte) 0x11, (byte)0xD1, (byte)0xD0, (byte)0x10, (byte)0xF0, (byte)0x30, (byte)0x31, (byte)0xF1, (byte)0x33, (byte)0xF3, (byte)0xF2, (byte)0x32,
            (byte) 0x36, (byte)0xF6, (byte)0xF7, (byte)0x37, (byte)0xF5, (byte)0x35, (byte)0x34, (byte)0xF4, (byte)0x3C, (byte)0xFC, (byte)0xFD, (byte)0x3D,
            (byte) 0xFF, (byte)0x3F, (byte)0x3E, (byte)0xFE, (byte)0xFA, (byte)0x3A, (byte)0x3B, (byte)0xFB, (byte)0x39, (byte)0xF9, (byte)0xF8, (byte)0x38,
            (byte) 0x28, (byte)0xE8, (byte)0xE9, (byte)0x29, (byte)0xEB, (byte)0x2B, (byte)0x2A, (byte)0xEA, (byte)0xEE, (byte)0x2E, (byte)0x2F, (byte)0xEF,
            (byte) 0x2D, (byte)0xED, (byte)0xEC, (byte)0x2C, (byte)0xE4, (byte)0x24, (byte)0x25, (byte)0xE5, (byte)0x27, (byte)0xE7, (byte)0xE6, (byte)0x26,
            (byte)  0x22, (byte)0xE2, (byte)0xE3, (byte)0x23, (byte)0xE1, (byte)0x21, (byte)0x20, (byte)0xE0, (byte)0xA0, (byte)0x60, (byte)0x61, (byte)0xA1,
            (byte) 0x63, (byte)0xA3, (byte)0xA2, (byte)0x62, (byte)0x66, (byte)0xA6, (byte)0xA7, (byte)0x67, (byte)0xA5, (byte)0x65, (byte)0x64, (byte)0xA4,
            (byte)0x6C, (byte)0xAC, (byte)0xAD, (byte)0x6D, (byte)0xAF, (byte)0x6F, (byte)0x6E, (byte)0xAE, (byte)0xAA, (byte)0x6A, (byte)0x6B, (byte)0xAB,
            (byte)0x69, (byte)0xA9, (byte)0xA8, (byte)0x68, (byte)0x78, (byte)0xB8, (byte)0xB9, (byte)0x79, (byte)0xBB, (byte)0x7B, (byte)0x7A, (byte)0xBA,
            (byte)0xBE, (byte)0x7E, (byte)0x7F, (byte)0xBF, (byte)0x7D, (byte)0xBD, (byte)0xBC, (byte)0x7C, (byte)0xB4, (byte)0x74, (byte)0x75, (byte)0xB5,
            (byte)0x77, (byte)0xB7, (byte)0xB6, (byte)0x76, (byte)0x72, (byte)0xB2, (byte)0xB3, (byte)0x73, (byte)0xB1, (byte)0x71, (byte)0x70, (byte)0xB0,
            (byte)0x50, (byte)0x90, (byte)0x91, (byte)0x51, (byte)0x93, (byte)0x53, (byte)0x52, (byte)0x92, (byte)0x96, (byte)0x56, (byte)0x57, (byte)0x97,
            (byte)0x55, (byte)0x95, (byte)0x94, (byte)0x54, (byte)0x9C, (byte)0x5C, (byte)0x5D, (byte)0x9D, (byte)0x5F, (byte)0x9F, (byte)0x9E, (byte)0x5E,
            (byte)0x5A, (byte)0x9A, (byte)0x9B, (byte)0x5B, (byte)0x99, (byte)0x59, (byte)0x58, (byte)0x98, (byte)0x88, (byte)0x48, (byte)0x49, (byte)0x89,
            (byte)0x4B, (byte)0x8B, (byte)0x8A, (byte)0x4A, (byte)0x4E, (byte)0x8E, (byte)0x8F, (byte)0x4F, (byte)0x8D, (byte)0x4D, (byte)0x4C, (byte)0x8C,
            (byte)0x44, (byte)0x84, (byte)0x85, (byte)0x45, (byte)0x87, (byte)0x47, (byte)0x46, (byte)0x86, (byte)0x82, (byte)0x42, (byte)0x43, (byte)0x83,
            (byte)0x41,(byte) 0x81, (byte)0x80, (byte)0x40
    };


}