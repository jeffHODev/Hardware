package com.hengde.nivi.modules.pdftest;

import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import javax.imageio.ImageIO;

/**
 * @author dong
 * @title: GraphicsTest
 * @description: TODO
 * @date 2020/12/16 9:42
 */
public class GraphicsTest {
    /**
     * @param str
     *            生产的图片文字
     * @param oldPath
     *            原图片保存路径
     * @param newPath
     *            新图片保存路径
     * @return
     * @throws IOException
     */
    public void createNewL(String str, String oldPath, String newPath){
        try {
            File oldFile = new File(oldPath);
            Image image = ImageIO.read(oldFile);
            BufferedImage sourceImg =ImageIO.read(new FileInputStream(oldFile));
            int width = sourceImg.getWidth() + 60;
            int height = sourceImg.getHeight() + 60;

            File file = new File(newPath);
            BufferedImage bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
            Graphics2D g2 = bi.createGraphics();
            g2.setBackground(Color.WHITE);
            g2.clearRect(0, 0, width, height);
            g2.drawImage(image, 30, 30, width - 60, height - 60, null); //这里减去25是为了防止字和图重合
            /** 设置生成图片的文字样式 * */
            Font font = new Font("黑体", Font.BOLD, 25);
            g2.setFont(font);
            g2.setPaint(Color.BLACK);

            /** 设置字体在图片中的位置 在这里是坐标位置* */
            FontRenderContext context = g2.getFontRenderContext();
            Rectangle2D bounds = font.getStringBounds(str, context);
            //double x = (width - bounds.getWidth()) / 2; //居中
            double x = width - bounds.getWidth();
            //double y = (height - bounds.getHeight()) / 2; //Y轴居中
            double y = (height - bounds.getHeight());
            double ascent = -bounds.getY();
            double baseY = y + ascent;

            /** 防止生成的文字带有锯齿 * */
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            /** 在图片上生成文字  在图片上根据自身业务需求标坐标点 * */
            g2.drawString(str, (int) x, (int) baseY - 10);
            g2.drawString("pr", 15, (int)ascent - 10);
            g2.fillOval(120, 100, 8, 8); //填充一个圆形 圆的外切矩形 左上角坐标为(0, 0), 宽高为10
            /** 再次设置生成图片的文字样式 * */
            Font font1 = new Font("黑体", Font.BOLD, 15);
            g2.setFont(font1);
            g2.setPaint(Color.BLACK);
            g2.drawString("0", 25, (int) baseY - 13);
            g2.drawString("2.2", (int) x - 30, (int) baseY - 10);
            g2.drawString("2.2", 3, (int)ascent + 10);

            ImageIO.write(bi, "png", file);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void createNewR(String str, String oldPath, String newPath){
        try {
            File oldFile = new File(oldPath);
            Image image = ImageIO.read(oldFile);
            BufferedImage sourceImg =ImageIO.read(new FileInputStream(oldFile));
            int width = sourceImg.getWidth() + 60;
            int height = sourceImg.getHeight() + 60;

            File file = new File(newPath);
            BufferedImage bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
            Graphics2D g2 = bi.createGraphics();
            g2.setBackground(Color.WHITE);
            g2.clearRect(0, 0, width, height);
            g2.drawImage(image, 30, 30, width - 60, height - 60, null); //这里减去25是为了防止字和图重合
            /** 设置生成图片的文字样式 * */
            Font font = new Font("黑体", Font.BOLD, 25);
            g2.setFont(font);
            g2.setPaint(Color.BLACK);

            /** 设置字体在图片中的位置 在这里是坐标位置* */
            FontRenderContext context = g2.getFontRenderContext();
            Rectangle2D bounds = font.getStringBounds(str, context);
            //double x = (width - bounds.getWidth()) / 2; //居中
            double x = width - bounds.getWidth();
            //double y = (height - bounds.getHeight()) / 2; //Y轴居中
            double y = (height - bounds.getHeight());
            double ascent = -bounds.getY();
            double baseY = y + ascent;

            /** 防止生成的文字带有锯齿 * */
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            /** 在图片上生成文字  在图片上根据自身业务需求标坐标点 * */
            g2.drawString(str, (int) x - 60, (int) baseY);
            g2.drawString("Q", 15, (int)ascent - 5);
            g2.fillOval(width, (int)baseY, 8, 8); //填充一个圆形 圆的外切矩形 左上角坐标为(0, 0), 宽高为10
            /** 再次设置生成图片的文字样式 * */
            Font font1 = new Font("黑体", Font.BOLD, 15);
            g2.setFont(font1);
            g2.setPaint(Color.BLACK);
            g2.drawString("0.9", 30, (int) baseY - 13);
            g2.drawString("0.5", 5, (int) baseY - 25);
            g2.drawString("1.55", (int) x - 10, (int) baseY - 10);
            g2.drawString("1.1", 0, (int)ascent + 10);

            ImageIO.write(bi, "png", file);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void createNewBig(String str, String oldPath, String newPath){
        try {
            File oldFile = new File(oldPath);
            Image image = ImageIO.read(oldFile);
            BufferedImage sourceImg =ImageIO.read(new FileInputStream(oldFile));
            int width = sourceImg.getWidth() + 60;
            int height = sourceImg.getHeight() + 60;

            File file = new File(newPath);
            BufferedImage bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
            Graphics2D g2 = bi.createGraphics();
            g2.setBackground(Color.WHITE);
            g2.clearRect(0, 0, width, height);
            g2.drawImage(image, 30, 30, width - 60, height - 60, null); //这里减去25是为了防止字和图重合
            /** 设置生成图片的文字样式 * */
            Font font = new Font("黑体", Font.BOLD, 25);
            g2.setFont(font);
            g2.setPaint(Color.BLACK);

            /** 设置字体在图片中的位置 在这里是坐标位置* */
            FontRenderContext context = g2.getFontRenderContext();
            Rectangle2D bounds = font.getStringBounds(str, context);
            //double x = (width - bounds.getWidth()) / 2; //居中
            double x = width - bounds.getWidth();
            //double y = (height - bounds.getHeight()) / 2; //Y轴居中
            double y = (height - bounds.getHeight());
            double ascent = -bounds.getY();
            double baseY = y + ascent;

            /** 防止生成的文字带有锯齿 * */
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            /** 在图片上生成文字  在图片上根据自身业务需求标坐标点 * */
            g2.drawString(str, (int) x - 60, (int) baseY);
            g2.drawString("m/s", 15, (int)ascent - 5);
            g2.fillOval(width, (int)baseY, 8, 8); //填充一个圆形 圆的外切矩形 左上角坐标为(0, 0), 宽高为10
            /** 再次设置生成图片的文字样式 * */
            Font font1 = new Font("黑体", Font.BOLD, 15);
            g2.setFont(font1);
            g2.setPaint(Color.BLACK);
            g2.drawString("<30", 15, (int) baseY - 10);
            g2.drawString("30-39", width / 5 - 5, (int) baseY - 10);
            g2.drawString("40-49", width / 5 * 2 - 15, (int) baseY - 10);
            g2.drawString("50-59", width / 5 * 3 - 28, (int) baseY - 10);
            g2.drawString("60-69", width / 5 * 4 - 40, (int) baseY - 10);
            g2.drawString(">=70", (int) x, (int) baseY - 10);
            g2.drawString("20", 0, (int)ascent + 10);
            g2.drawString("15", 0, (int)ascent + 110);
            g2.drawString("10", 0, (int)ascent + 205);
            g2.drawString("5", 0, (int)ascent + 303);

            ImageIO.write(bi, "png", file);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {

        try {
            GraphicsTest img = new GraphicsTest();
            img.createNewBig("年龄", "D:\\work\\big.jpg", "D:\\work\\newBig.jpg");
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}
