package com.hengde.nivi.modules.pdftest;

import com.hengde.nivi.common.util.GetAge;
import com.hengde.nivi.modules.create.dao.BasisMeasurementMapper;
import com.hengde.nivi.modules.create.service.IBasisMeasurementService;
import com.itextpdf.text.Document;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.Image;
import com.itextpdf.text.Rectangle;
import com.itextpdf.text.pdf.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Map;

/**
 * @author dong
 * @title: PdfStamperTest
 * @description: TODO
 * @date 2020/6/4 19:23
 */
@RestController
public class PdfStamperTest {
    @Autowired
    private IBasisMeasurementService basisMeasurementService;

    public byte[] fillTemplate(String url){//利用模板生成pdf
        //模板路径
        String templatePath = url + "/moban.pdf";
        //生成的新文件路径
        String newPDFPath = url + "/newmoban.pdf";
        PdfReader reader;
//        FileOutputStream out;
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        PdfStamper stamper;
        try {
//            out = new FileOutputStream(newPDFPath);//输出流
            reader = new PdfReader(templatePath);//读取pdf模板
            stamper = new PdfStamper(reader, bos);
            AcroFields form = stamper.getAcroFields();
            form.addSubstitutionFont(BaseFont.createFont("STSong-Light","UniGB-UCS2-H", BaseFont.NOT_EMBEDDED));
//            form.addSubstitutionFont(BaseFont.createFont("/usr/share/fonts/dejavu/simsun.ttc,0", BaseFont.IDENTITY_H, BaseFont.NOT_EMBEDDED));

//            String[] str = {"Ms Wang", "age", "70", "doctore", "name", "cc"};
//            String[] str = {en.getPatientName(), GetAge.getAge(date)+"", en.getWeight(), en.getDoctoreName(), "name", "cc"};
//            int i = 0;
//            java.util.Iterator<String> it = form.getFields().keySet().iterator();
//            while(it.hasNext()){
//                String name = it.next().toString();
//                System.out.println(name);
//                form.setField(name, str[i++]);
//            }

//            form.setField("a", str[0]);
//            form.setField("b", str[1]);
//            form.setField("c", str[2]);
//            form.setField("d", str[3]);

            //todo 开始放入图片--start
            // 通过域名获取所在页和坐标，左下角为起点

            File fileHeight = new File(url + "/patientHeight.jpg");//所属文件夹的路径
            if(fileHeight.exists()){
                addImage(form, "height", url, "/patientHeight.jpg", stamper);
            }
            File fileSex = new File(url + "/patientSex.jpg");//所属文件夹的路径
            if(fileSex.exists()){
                addImage(form, "sex", url, "/patientSex.jpg", stamper);
            }
            File fileTime = new File(url + "/Time.jpg");//所属文件夹的路径
            if(fileTime.exists()){
                addImage(form, "time", url, "/Time.jpg", stamper);
            }
            File filea = new File(url + "/patientName.jpg");//所属文件夹的路径
            if(filea.exists()){
                addImage(form, "a", url, "/patientName.jpg", stamper);
            }
            File fileb = new File(url + "/patientAge.jpg");//所属文件夹的路径
            if(fileb.exists()){
                addImage(form, "b", url, "/patientAge.jpg", stamper);
            }
            File filec = new File(url + "/patientWeight.jpg");//所属文件夹的路径
            if(filec.exists()){
                addImage(form, "c", url, "/patientWeight.jpg", stamper);
            }
            File filed = new File(url + "/doctoreName.jpg");//所属文件夹的路径
            if(filed.exists()){
                addImage(form, "d", url, "/doctoreName.jpg", stamper);
            }
            addImage(form, "e", url, "/newBig.png", stamper);
            addImage(form, "g", url, "/ecgPr.jpg", stamper); //ecg_pr 右颈动脉心电波
            addImage(form, "h", url, "/pcgPr.jpg", stamper); //pcg_pr 右颈动脉心音波
            addImage(form, "i", url, "/apgR.jpg", stamper); //apg_r 右颈动脉脉搏波
            addImage(form, "j", url, "/apgTh.jpg", stamper); //apg_th 股动脉脉搏波
            addImage(form, "f", url, "/apgLNew.jpg", stamper); //apg_l 左颈动脉脉搏波
            addImage(form, "k", url, "/apgRNew.jpg", stamper); //apg_r 右颈动脉脉搏波
            addImage(form, "l", url, "/buf.jpg", stamper); //buf[]
            addImage(form, "m", url, "/newL.png", stamper);
            addImage(form, "n", url, "/newR.png", stamper);

            stamper.setFormFlattening(true); //如果为false那么生成的PDF文件还能编辑，一定要设为true
            stamper.close();
            reader.close();

//            Document doc = new Document();
//            PdfCopy copy = new PdfCopy(doc, out);
//            doc.open();
//            PdfImportedPage importPage = copy.getImportedPage(
//                    new PdfReader(bos.toByteArray()), 1);
//            copy.addPage(importPage);
//
//            doc.close();

            File file = new File(newPDFPath);
            file.createNewFile();
            FileOutputStream fos = new FileOutputStream(file);//创建文件输出流
            fos.write(bos.toByteArray());//写入数据
            fileHeight.delete();
            fileSex.delete();
            fileTime.delete();
            filea.delete();
            fileb.delete();
            filec.delete();
            filed.delete();
            fos.close();

        } catch (IOException e) {
            System.out.println(1);
            e.printStackTrace();
        } catch (DocumentException e) {
            System.out.println(2);
        }

        return bos.toByteArray();
    }

    //往PDF文件中放入图片公共调用
    public void addImage(AcroFields form, String field, String url, String ima, PdfStamper stamper) {
        try {
            int pageNo = form.getFieldPositions(field).get(0).page;
            Rectangle signRect = form.getFieldPositions(field).get(0).position;
            float x = signRect.getLeft();
            float y = signRect.getBottom();

            // 读图片
            Image image = Image.getInstance(url + ima);
            // 获取操作的页面
            PdfContentByte under = stamper.getOverContent(pageNo);
            image.scaleAbsolute(signRect.getWidth(), signRect.getHeight());
            image.setAbsolutePosition(x, y);

            under.addImage(image);

        } catch (IOException e) {
            System.out.println(1);
            e.printStackTrace();
        } catch (DocumentException e) {
            System.out.println(2);
        }
    }
}
