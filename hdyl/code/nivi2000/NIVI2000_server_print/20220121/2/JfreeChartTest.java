package com.hengde.nivi.modules.pdftest;

import com.hengde.nivi.common.response.Result;
import com.hengde.nivi.modules.create.dao.BasisMeasurementMapper;
import com.hengde.nivi.modules.create.service.IBasisMeasurementService;
import com.hengde.nivi.modules.detection.dao.OriginalWaveformMapper;
import com.hengde.nivi.modules.detection.entity.OriginalWaveform;
import com.hengde.nivi.modules.detection.entity.OriginalWaveformReq;
import com.hengde.nivi.modules.detection.service.IDetectionService;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.CategoryAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.labels.StandardCategoryItemLabelGenerator;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.renderer.category.LineAndShapeRenderer;
import org.jfree.chart.title.LegendTitle;
import org.jfree.chart.title.TextTitle;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.general.DatasetUtilities;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.font.FontRenderContext;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.List;
import java.util.*;

/**
 * @author dong
 * @title: JfreeChartTest
 * @description: TODO
 * @date 2020/6/4 18:05
 */
@RestController
public class JfreeChartTest {
    protected static Logger logger = LoggerFactory.getLogger(JfreeChartTest.class);

    @Autowired
    private OriginalWaveformMapper originalWaveformMapper;

    @Autowired
    private IDetectionService detectionService;

    @Autowired
    private BasisMeasurementMapper basisMeasurementMapper;

    @Autowired
    private IBasisMeasurementService basisMeasurementService;

    // 保存为文件
    public static void saveAsFile(JFreeChart chart, String outputPath,
                                  int weight, int height) {
        FileOutputStream out = null;
        try {
            File outFile = new File(outputPath);
            if (!outFile.getParentFile().exists()) {
                outFile.getParentFile().mkdirs();
            }
            out = new FileOutputStream(outputPath);
            // 保存为PNG
            // ChartUtilities.writeChartAsPNG(out, chart, 600, 400);
            // 保存为JPEG
            ChartUtilities.writeChartAsJPEG(out, chart, 600, 400);
            out.flush();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (out != null) {
                try {
                    out.close();
                } catch (IOException e) {
                    // do nothing
                }
            }
        }
    }

    // 根据CategoryDataset创建JFreeChart对象
    public static JFreeChart createChart(CategoryDataset categoryDataset) {
        // 创建JFreeChart对象：ChartFactory.createLineChart
        /*JFreeChart jfreechart = ChartFactory.createLineChart("不同类别按小时计算拆线图", // 标题
                "年分", // categoryAxisLabel （category轴，横轴，X轴标签）
                "数量", // valueAxisLabel（value轴，纵轴，Y轴的标签）
                categoryDataset, // dataset
                PlotOrientation.VERTICAL, true, // legend
                false, // tooltips
                false); // URLs*/
        JFreeChart jfreechart = ChartFactory.createLineChart("", // 标题
                null, // categoryAxisLabel （category轴，横轴，X轴标签）
                null, // valueAxisLabel（value轴，纵轴，Y轴的标签）
                categoryDataset, // dataset
                PlotOrientation.VERTICAL, true, // legend
                true, // tooltips
                false); // URLs

        /*Font titleFont = new Font("黑体", Font.BOLD, 20);
        TextTitle textTitle = jfreechart.getTitle();
        textTitle.setFont(titleFont);// 为标题设置上字体

        Font plotFont = new Font("宋体", Font.PLAIN, 16);
        PiePlot plotP = (PiePlot) jfreechart.getPlot();
        plotP.setLabelFont(plotFont); // 为饼图元素设置上字体

        Font LegendFont = new Font("楷体", Font.PLAIN, 18);
        LegendTitle legend = jfreechart.getLegend(0);
        legend.setItemFont(LegendFont);// 为图例说明设置字体*/

        //todo 自定义设置
        // 设置外层图片 无边框 无背景色 背景图片透明
//        jfreechart.setBorderVisible(false);
//        jfreechart.setBackgroundPaint(Color.white);
//        jfreechart.setBackgroundImageAlpha(0.0f);

        /***设置柱状图、折线图相关属性***/
        CategoryPlot catplot = jfreechart.getCategoryPlot();  //获取柱状图、折线图区域对象。
        CategoryAxis domainAxis=catplot.getDomainAxis();
        catplot.setNoDataMessage("无数据显示");//没有数据的时候显示的内容
        //列表标题(X轴)
        TextTitle txtTitle = jfreechart.getTitle();
        txtTitle.setFont(new Font("黑体", Font.BOLD,20));
        //水平底部列表
        domainAxis.setLabelFont(new Font("黑体",Font.BOLD,20));
        //水平底部标题
        domainAxis.setTickLabelFont(new Font("宋体",Font.BOLD,20));
        // 设置距离图片左端距离
        domainAxis.setLowerMargin(0);
        // 设置距离图片右端距离
        domainAxis.setUpperMargin(0);
        //坐标轴是否可见
        domainAxis.setVisible(false);
//        catplot.setDomainAxis(domainAxis);
        //垂直标题(Y轴)
        ValueAxis rangeAxis=catplot.getRangeAxis();//获取柱状
        rangeAxis.setLabelFont(new Font("楷体", Font.PLAIN, 20));
        jfreechart.getLegend().setItemFont(new Font("楷体", Font.PLAIN, 20));
        //设置最高的一个值与图片顶端的距离
        rangeAxis.setUpperMargin(0);
        //设置最低的一个值与图片底端的距离
        rangeAxis.setLowerMargin(0);
        //坐标轴是否可见
        rangeAxis.setVisible(false);
//        catplot.setRangeAxis(rangeAxis);

        // 使用CategoryPlot设置各种参数。以下设置可以省略。
//        CategoryPlot plot = (CategoryPlot)jfreechart.getPlot();
        CategoryPlot plot = jfreechart.getCategoryPlot();
        // 背景色 透明度
        plot.setBackgroundAlpha(0.0f);
        // 前景色 透明度
        plot.setForegroundAlpha(0.5f);
        // 去除背景边框线
        plot.setOutlinePaint(null);
        // 其他设置 参考 CategoryPlot类
        LineAndShapeRenderer renderer = (LineAndShapeRenderer)plot.getRenderer();
        renderer.setBaseShapesVisible(true); // series 点（即数据点）可见
        renderer.setBaseLinesVisible(true); // series 点（即数据点）间有连线可见
        renderer.setUseSeriesOffset(true); // 设置偏移量
        renderer.setBaseItemLabelGenerator(new StandardCategoryItemLabelGenerator());
        renderer.setBaseItemLabelsVisible(false); //拐点数据是否可见
        renderer.setSeriesStroke(0, new BasicStroke(2F));//设置折线大小
        renderer.setSeriesPaint(0, Color.BLACK);//设置折线为黑色
        renderer.setBaseShapesVisible(false); // 设置拐点是否可见/是否显示拐点

        //改变底部分类标签的位置
        LegendTitle legend = jfreechart.getLegend();
//        legend.setPosition(RectangleEdge.RIGHT); //将底部分类标签放置到图例右侧
        legend.setVisible(false); //去除底部分类标签

        return jfreechart;
    }

    /**
     * 创建CategoryDataset对象 -- 测试Demo
     *
     */
    public static CategoryDataset createDatasetDemo() {
        String[] rowKeys = {"波形"};
        String[] colKeys = {"0:00", "1:00", "2:00", "7:00", "8:00", "9:00",
                "10:00", "11:00", "12:00", "13:00", "16:00", "20:00", "21:00",
                "23:00"};
        double[][] data = {{4, 3, 1, 1, 1, 1, 2, 2, 2, 1, 9, 2, 1, 1}};
        // 或者使用类似以下代码
        // DefaultCategoryDataset categoryDataset = new
        // DefaultCategoryDataset();
        // categoryDataset.addValue(10, "rowKey", "colKey");
        return DatasetUtilities.createCategoryDataset(rowKeys, colKeys, data);
//        return DatasetUtilities.createCategoryDataset("","",data);
    }

    //生成波形封装数据公共调用
    public CategoryDataset createDataset(Map<String, double[]> resuMap, String obj) {
        String[] rowKeys = {"波形"};

        //定义二维double数组
        int length = resuMap.get(obj).length;
        double[][] data = new double[1][length];
        for (int iii = 0; iii < length; iii++){
            data[0][iii] = resuMap.get(obj)[iii];
        }

        String[] colKeys = new String[length];
        for (int yy = 0; yy < length; yy++){
            colKeys[yy] = String.valueOf(resuMap.get(obj)[yy]);
        }

        List list = Arrays.asList(colKeys);
        Set set = new HashSet(list); //去除重复元素
        while (set.size() < length){
            set.add(String.valueOf(((Math.random()*9+1)*1000)));
        }
        colKeys=(String [])set.toArray(new String[0]);

        // 或者使用类似以下代码
        // DefaultCategoryDataset categoryDataset = new
        // DefaultCategoryDataset();
        // categoryDataset.addValue(10, "rowKey", "colKey");
        return DatasetUtilities.createCategoryDataset(rowKeys, colKeys, data);
//        return DatasetUtilities.createCategoryDataset("","",data);
    }

    public void resMap(Map parMap, OriginalWaveformReq originalWaveformReq, OriginalWaveform originalWaveform){
        if(originalWaveformReq == null){
            parMap.put("hypecgs", originalWaveform.getHypecgs());
            parMap.put("hyppcgs", originalWaveform.getHyppcgs());
            parMap.put("hypapgs", originalWaveform.getHypapgs());
            parMap.put("hypecgd", originalWaveform.getHypecgd());
            parMap.put("hyppcgd", originalWaveform.getHyppcgd());
            parMap.put("hypapgd", originalWaveform.getHypapgd());
            parMap.put("ecgPl", originalWaveform.getEcgPl());
            parMap.put("pcgPl", originalWaveform.getPcgPl());
            parMap.put("apgL", originalWaveform.getApgL());
            parMap.put("ecgPr", originalWaveform.getEcgPr());
            parMap.put("pcgPr", originalWaveform.getPcgPr());
            parMap.put("apgR", originalWaveform.getApgR());
            parMap.put("ecgTh", originalWaveform.getEcgTh());
            parMap.put("apgTh", originalWaveform.getApgTh());
        }else{
            parMap.put("hypecgs", originalWaveformReq.getHypecgs());
            parMap.put("hyppcgs", originalWaveformReq.getHyppcgs());
            parMap.put("hypapgs", originalWaveformReq.getHypapgs());
            parMap.put("hypecgd", originalWaveformReq.getHypecgd());
            parMap.put("hyppcgd", originalWaveformReq.getHyppcgd());
            parMap.put("hypapgd", originalWaveformReq.getHypapgd());
            parMap.put("ecgPl", originalWaveformReq.getEcgPl());
            parMap.put("pcgPl", originalWaveformReq.getPcgPl());
            parMap.put("apgL", originalWaveformReq.getApgL());
            parMap.put("ecgPr", originalWaveformReq.getEcgPr());
            parMap.put("pcgPr", originalWaveformReq.getPcgPr());
            parMap.put("apgR", originalWaveformReq.getApgR());
            parMap.put("ecgTh", originalWaveformReq.getEcgTh());
            parMap.put("apgTh", originalWaveformReq.getApgTh());
        }
    }

    /**
     * 返回新的PDF模板
     */
    @GetMapping("/retStamper")
    public Result retStamper(@RequestParam String basisMeasurementId) throws Exception{
        logger.info("前端请求打印PDF文件，basisMeasurementId：【{}】", basisMeasurementId);
        //todo linux获取路径
        File directory = new File("");// 参数为空
        String courseFile = directory.getCanonicalPath(); // /home/nivitest

        OriginalWaveform originalWaveform = originalWaveformMapper.selectByBasisMeasurementId(basisMeasurementId);
        if(originalWaveform == null){
            return null;
        }
        Map<String, String> resMap = new HashMap();
        this.resMap(resMap, null, originalWaveform);

        Map<String, double[]> resuMap = new HashMap();
        for (Map.Entry<String, String> entry : resMap.entrySet()) {
            String[] s = entry.getValue().split(",");

            double[] newFbuf = new double[s.length];
            for (int i = 0; i < s.length; i++){
                newFbuf[i] = Double.parseDouble(s[i]);
            }
            resuMap.put(entry.getKey(), newFbuf);
        }

        CategoryDataset dataset = createDataset(resuMap, "ecgPr");
        saveFile(dataset, courseFile, "/ecgPr.jpg");

        CategoryDataset dataset2 = createDataset(resuMap, "pcgPr");
        saveFile(dataset2, courseFile, "/pcgPr.jpg");

        CategoryDataset dataset3 = createDataset(resuMap, "apgR");
        saveFile(dataset3, courseFile, "/apgR.jpg");

        CategoryDataset dataset4 = createDataset(resuMap, "apgTh");
        saveFile(dataset4, courseFile, "/apgTh.jpg");


        OriginalWaveformReq originalWaveformReq = new OriginalWaveformReq();
        originalWaveformReq.setBasisMeasurementId(basisMeasurementId);
        originalWaveformReq.setIDL((short)-1);
        originalWaveformReq.setIDR((short)-1);
        Integer model = basisMeasurementMapper.selectModel(originalWaveformReq.getBasisMeasurementId());
        originalWaveformReq.setModel(model);
        Map dataMap = detectionService.saveOriginalWaveform(originalWaveformReq, originalWaveformReq.getIDL(), originalWaveformReq.getIDR());
        //左
        short IDL = (short)dataMap.get("IDL"); //位置
        short[] RtopL = (short[])dataMap.get("RtopL"); //起始点索引
        //右
        short IDR = (short)dataMap.get("IDR");
        short[] RtopR = (short[])dataMap.get("RtopR");

        //获取左右颈动脉脉搏波的数据截取的索引位置
        short apgLStart = RtopL[IDL];
        short apgLEnd = RtopL[IDL + 1];
        short apgRStart = RtopR[IDR];
        short apgREnd = RtopR[IDR + 1];

        //获取截取数据
        double[] apgL = resuMap.get("apgL");
        double[] apgLNew = Arrays.copyOfRange(apgL, apgLStart, apgLEnd);
        resuMap.put("apgLNew", apgLNew);
        double[] apgR = resuMap.get("apgR");
        double[] apgRNew = Arrays.copyOfRange(apgR, apgRStart, apgREnd);
        resuMap.put("apgRNew", apgRNew);

        CategoryDataset dataset5 = createDataset(resuMap, "apgLNew");
        saveFile(dataset5, courseFile, "/apgLNew.jpg");

        CategoryDataset dataset6 = createDataset(resuMap, "apgRNew");
        saveFile(dataset6, courseFile, "/apgRNew.jpg");

        float[] buf = (float[])dataMap.get("buf");
        int bufLenth = buf.length;
        for (int i = bufLenth - 1; i > 0; i--){
            if(buf[i] != 0){
                bufLenth = i;
                break;
            }
        }
        float[] newBuf = Arrays.copyOfRange(buf, 0, bufLenth + 1);
        //转成double数组
        double[] bufD = new double[newBuf.length];
        for (int j = 0; j < newBuf.length; j++){
            bufD[j] = Double.valueOf(String.valueOf(newBuf[j]));
        }
        resuMap.put("buf", bufD);
        CategoryDataset dataset7 = createDataset(resuMap, "buf");
        saveFile(dataset7, courseFile, "/buf.jpg");

        createNewL(courseFile + "/L.png", courseFile + "/newL.png", (float)dataMap.get("prResult"), (float)dataMap.get("cResult"), (float)dataMap.get("prResultR"), (float)dataMap.get("cResultR")); //生成新的画点的图
        createNewR(courseFile + "/R.png", courseFile + "/newR.png", (float)dataMap.get("qResult"), (float)dataMap.get("ascResult"), (float)dataMap.get("qResultR"), (float)dataMap.get("ascResultR"));
        createNewBig(courseFile + "/big.png", courseFile + "/newBig.png", (int)dataMap.get("age"), (float)dataMap.get("PTV"));

        Result result = new Result();
        Map byteMap = new HashMap();
        PdfStamperTest pd = new PdfStamperTest();
        Entity en = basisMeasurementService.selectPDFInfoBybasisMeasurementId(basisMeasurementId);
        byteMap.put("PDFByte", pd.fillTemplate(courseFile, en));
        return result.ok().setData(byteMap);
    }

    //保存图片公共调用
    public void saveFile(CategoryDataset dataset, String courseFile, String img){
        JFreeChart freeChart = createChart(dataset);
        saveAsFile(freeChart, courseFile + img, 600, 400);
    }

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
    public void createNewL(String oldPath, String newPath, float prResult, float cResult, float prResultR, float cResultR){
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
            g2.drawImage(image, 30, 30, width - 60, height - 60, null); //这里减去60是为了防止字和图重合
            /** 设置生成图片的文字样式 * */
            Font font = new Font("黑体", Font.BOLD, 25);
            g2.setFont(font);
            g2.setPaint(Color.BLACK);

            /** 设置字体在图片中的位置 在这里是坐标位置* */
            FontRenderContext context = g2.getFontRenderContext();
            Rectangle2D bounds = font.getStringBounds("c", context);
            //double x = (width - bounds.getWidth()) / 2; //居中
            double x = width - bounds.getWidth();
            //double y = (height - bounds.getHeight()) / 2; //Y轴居中
            double y = (height - bounds.getHeight());
            double ascent = -bounds.getY();
            double baseY = y + ascent;

            /** 防止生成的文字带有锯齿 * */
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            /** 在图片上生成文字  在图片上根据自身业务需求标坐标点 * */
            g2.drawString("c", (int) x, (int) baseY - 10);
            g2.drawString("pr", 15, (int)ascent - 10);
//            g2.fillOval((int)(width / 2.2 * cResult), (int)(baseY - (baseY / 2.2 * prResult)), 10, 10); //填充一个圆形 圆的外切矩形 左上角坐标为(0, 0), 宽高为10
            BufferedImage bg = ImageIO.read(new File(new File("").getCanonicalPath() + "/bb.png"));
            int cx = (int)(width / 2.2 * cResult);
            if(cResult > 2.2){
                cx = (int) width;
            }
            int pry = (int)(baseY - (baseY / 2.2 * prResult));
            if(prResult > 2.2){
                pry = (int) 0;
            }
            g2.drawImage(bg.getScaledInstance(30, 30, Image.SCALE_DEFAULT), cx, pry, null);
            BufferedImage bg1 = ImageIO.read(new File(new File("").getCanonicalPath() + "/bbb.png"));
            int crx = (int)(width / 2.2 * cResultR);
            if(cResultR > 2.2){
                crx = (int) width;
            }
            int prry = (int)(baseY - (baseY / 2.2 * prResultR));
            if(prResultR > 2.2){
                prry = (int) 0;
            }
            g2.drawImage(bg1.getScaledInstance(30, 30, Image.SCALE_DEFAULT), crx, prry, null);
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

    public void createNewR(String oldPath, String newPath, float qResult, float ascResult, float qResultR, float ascResultR){
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
            Rectangle2D bounds = font.getStringBounds("ASC", context);
            //double x = (width - bounds.getWidth()) / 2; //居中
            double x = width - bounds.getWidth();
            //double y = (height - bounds.getHeight()) / 2; //Y轴居中
            double y = (height - bounds.getHeight());
            double ascent = -bounds.getY();
            double baseY = y + ascent;

            /** 防止生成的文字带有锯齿 * */
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            /** 在图片上生成文字  在图片上根据自身业务需求标坐标点 * */
            g2.drawString("ASC", (int) x - 60, (int) baseY);
            g2.drawString("Q", 15, (int)ascent - 5);
//            g2.fillOval((int)(width / 1.55 * ascResult), (int)(baseY - (baseY / 1.1 * qResult)), 10, 10); //填充一个圆形 圆的外切矩形 左上角坐标为(0, 0), 宽高为10
            BufferedImage bg = ImageIO.read(new File(new File("").getCanonicalPath() + "/bb.png"));
//            g2.drawImage(bg.getScaledInstance(30, 30, Image.SCALE_DEFAULT), (int)(width / 1.55 * ascResult), (int)(baseY - (baseY / 1.1 * qResult)), null);
            int ascx = (int)(width / 0.65 * (ascResult - 0.9));
            if(ascx < 0){
                ascx = (int) (width / 0.65 * 0);
            }
            if(ascResult > 1.55){
                ascx = (int) (width);
            }
            int qy = (int)(baseY - baseY / 0.6 * (qResult - 0.5));
            if(qy < 0){
                qy = (int) (baseY - baseY / 0.6 * 0);
            }
            if(qResult > 1.1){
                qy = (int) (0);
            }
            g2.drawImage(bg.getScaledInstance(30, 30, Image.SCALE_DEFAULT), ascx, qy, null);
            BufferedImage bg1 = ImageIO.read(new File(new File("").getCanonicalPath() + "/bbb.png"));
//            g2.drawImage(bg1.getScaledInstance(30, 30, Image.SCALE_DEFAULT), (int)(width / 1.55 * ascResultR), (int)(baseY - (baseY / 1.1 * qResultR)), null);
            int ascrx = (int)(width / 0.65 * (ascResultR - 0.9));
            if(ascrx < 0){
                ascrx = (int) (width / 0.65 * 0);
            }
            if(ascResultR > 1.55){
                ascrx = (int) (width);
            }
            int qry = (int)(baseY - baseY / 0.6 * (qResultR - 0.5));
            if(qry < 0){
                qry = (int) (baseY - baseY / 0.6 * 0);
            }
            if(qResultR > 1.1){
                qry = (int) (0);
            }
            g2.drawImage(bg1.getScaledInstance(30, 30, Image.SCALE_DEFAULT), ascrx, qry, null);
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

    public void createNewBig(String oldPath, String newPath, int age, float PTV){
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
            Rectangle2D bounds = font.getStringBounds("年龄", context);
            //double x = (width - bounds.getWidth()) / 2; //居中
            double x = width - bounds.getWidth();
            //double y = (height - bounds.getHeight()) / 2; //Y轴居中
            double y = (height - bounds.getHeight());
            double ascent = -bounds.getY();
            double baseY = y + ascent;

            /** 防止生成的文字带有锯齿 * */
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

            /** 在图片上生成文字  在图片上根据自身业务需求标坐标点 * */
            g2.drawString("年龄", (int) x - 60, (int) baseY);
            g2.drawString("m/s", 15, (int)ascent - 5);

            int xx = 0;
            float yy = 0;
            if(age<30){
                xx = 0;
            }else if(age >= 30 && age < 40){
                xx = width / 5;
            }else if(age >= 40 && age < 50){
                xx = width / 5 * 2;
            }else if(age >= 50 && age < 60){
                xx = width / 5 * 3;
            }else if(age >= 60 && age < 70){
                xx = width / 5 * 4;
            }else if(age >= 70){
                xx = width;
            }

            float dpy = (float) (baseY / (20 - 2.25));
            yy = (float) (PTV / 100 - 2.25) * dpy; // 2.25 y坐标在原点的值
            g2.setPaint(Color.BLUE);
            g2.fillOval(xx, (int)(baseY - yy), 10, 10); //填充一个圆形 圆的外切矩形 左上角坐标为(0, 0), 宽高为10
            g2.drawString("PWV = " + PTV / 100 + "M/S", (int) x - 180, (int)ascent - 5);

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

            ImageIO.write(bi, "jpg", file);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
