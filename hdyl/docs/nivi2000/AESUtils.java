package com.hengde.nivi.common.util;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

@SuppressWarnings("restriction")
public class AESUtils
{

	// 密匙
	private static final String KEY = "b1767b1b3aef8ed8";

	// 偏移量
	private static final String OFFSET = "ced376e1ff11811d";

	// 编码
	private static final String ENCODING = "UTF-8";

	// 算法
	private static final String ALGORITHM = "AES";

	// 默认的加密算法
	private static final String CIPHER_ALGORITHM = "AES/CBC/PKCS5Padding";

	/**
	 * 加密
	 * 
	 * @param data
	 * @return
	 * @throws Exception
	 */

	public static String encrypt(String data) throws Exception
	{
		Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM);
		SecretKeySpec skeySpec = new SecretKeySpec(KEY.getBytes("ASCII"), ALGORITHM);
		// 使用CBC模式，需要一个向量iv，可增加加密算法的强度
		IvParameterSpec iv = new IvParameterSpec(OFFSET.getBytes());
		cipher.init(Cipher.ENCRYPT_MODE, skeySpec, iv);
		byte[] encrypted = cipher.doFinal(data.getBytes(ENCODING));
		// 此处使用BASE64做转码。
		return new BASE64Encoder().encode(encrypted);
	}

	

	/**
	 * 解密
	 * 
	 * @param data
	 * @return
	 * @throws Exception
	 */
	public static String decrypt(String data) throws Exception
	{
		Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM);
		SecretKeySpec skeySpec = new SecretKeySpec(KEY.getBytes("ASCII"), ALGORITHM);
		// 使用CBC模式，需要一个向量iv，可增加加密算法的强度
		IvParameterSpec iv = new IvParameterSpec(OFFSET.getBytes());
		cipher.init(Cipher.DECRYPT_MODE, skeySpec, iv);
		byte[] buffer = new BASE64Decoder().decodeBuffer(data);
		byte[] encrypted = cipher.doFinal(buffer);
		// 此处使用BASE64做转码。
		return new String(encrypted, ENCODING);
	}
	
	
	
	public static void main(String[] args)
	{
		try
		{
			System.out.println(encrypt("{\r\n" + 
					"    \"loginName\":\"admin\",\r\n" + 
					"    \"password\":\"123\"\r\n" + 
					"}"));
			System.out.println(encrypt("{\"basisMeasurementId\":\"0150fc9fcaf34beca263e4c7665d8147\"}")); 
			System.out.println("================");
			System.out.println(encrypt("{\"pageNo\":1,\"pageSize\":15}"));
			System.out.println("================");
			System.out.println(decrypt("AUMkhgNAZjykp8RywniVEgj6tJfgVGVGN3bSamMFyDkdEicx/C7ngusuD+JK4Js5AnZP0Sv95c2+\r\n" + 
					"ZgVakv2KRPLgxobssrwoFje8pfi1hs/vXlx3tw+TVVZ4xDf0ZrajxVqHZYYosL4bj1DFjpGwIfM4\r\n" + 
					"0VDn7Cm2E3qUQO/0vwUVd4mZ3WNvVQXS9DQskv2K+z/HR+zD0+UlQakHnKRs7Q=="));
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

}
