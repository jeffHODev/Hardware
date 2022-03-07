package android_serialport_api;

public class SerialBuffer {

    public static StringBuffer stringBuffer = new StringBuffer();

    public void put(String string){
        stringBuffer.append(string);
    }

    public static StringBuffer getStringBuffer() {
        return stringBuffer;
    }

    public static void setStringBuffer(StringBuffer stringBuffer) {
        SerialBuffer.stringBuffer = stringBuffer;
    }

    public static void clearBuffer(){
        stringBuffer = new StringBuffer();
    }
}
