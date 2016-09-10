package com.zzmax.admin.ecganomalydetection;

import android.content.res.AssetManager;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.FileInputStream;
//import java.io.FileInputStreamReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import lecho.lib.hellocharts.animation.ChartAnimationListener;
import lecho.lib.hellocharts.gesture.ZoomType;
import lecho.lib.hellocharts.listener.LineChartOnValueSelectListener;
import lecho.lib.hellocharts.listener.ViewportChangeListener;
import lecho.lib.hellocharts.model.Axis;
import lecho.lib.hellocharts.model.Line;
import lecho.lib.hellocharts.model.LineChartData;
import lecho.lib.hellocharts.model.PointValue;
import lecho.lib.hellocharts.model.ValueShape;
import lecho.lib.hellocharts.model.Viewport;
import lecho.lib.hellocharts.util.ChartUtils;
import lecho.lib.hellocharts.view.Chart;
import lecho.lib.hellocharts.view.LineChartView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
        setContentView(R.layout.activity_signal_chart);
        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
        }
    }

    /**
     * A fragment containing a line chart.
     */
    public static class PlaceholderFragment extends Fragment {

        private LineChartView chart;//for signal orignal
        private LineChartData data;

        private LineChartView chart2;//for noise
        private LineChartData data2;

        private int numberOfLines = 1;
        private int maxNumberOfLines = 4;
        private int numberOfPoints = 12;

        float[][] randomNumbersTab = new float[maxNumberOfLines][numberOfPoints];

        private boolean hasAxes = true;
        private boolean hasAxesNames = true;
        private boolean hasLines = true;
        private boolean hasPoints = true;
        private ValueShape shape = ValueShape.CIRCLE;
        private boolean isFilled = false;
        private boolean hasLabels = false;
        private boolean isCubic = false;
        private boolean hasLabelForSelected = false;
        private boolean pointsHaveDifferentColor;

        public PlaceholderFragment() {
        }

        @Override
        public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
            setHasOptionsMenu(true);
            View rootView = inflater.inflate(R.layout.fragment_line_chart, container, false);

            chart = (LineChartView) rootView.findViewById(R.id.chart);
            
            chart2 = (LineChartView) rootView.findViewById(R.id.chart2);

            // Generate data for previewed chart and copy of that data for preview chart.
            generateDefaultData();

            chart.setLineChartData(data);
            // Disable zoom/scroll for previewed chart, visible chart ranges depends on preview chart viewport so
            // zoom/scroll is unnecessary.
            chart.setZoomEnabled(false);
            chart.setScrollEnabled(false);

            chart2.setLineChartData(data2);
            chart2.setViewportChangeListener(new ViewportListener());

            previewX(false);

            return rootView;
        }

        // MENU
        @Override
        public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
//            inflater.inflate(R.menu.preview_line_chart, menu);
        }

//        @Override
//        public boolean onOptionsItemSelected(MenuItem item) {
//            int id = item.getItemId();
//            if (id == R.id.action_reset) {
//                generateDefaultData();
//                chart.setLineChartData(data);
//                chart2.setLineChartData(data2);
//                previewX(true);
//                return true;
//            }
//            if (id == R.id.action_preview_both) {
//                previewXY();
//                chart2.setZoomType(ZoomType.HORIZONTAL_AND_VERTICAL);
//                return true;
//            }
//            if (id == R.id.action_preview_horizontal) {
//                previewX(true);
//                return true;
//            }
//            if (id == R.id.action_preview_vertical) {
//                previewY();
//                return true;
//            }
//            if (id == R.id.action_change_color) {
//                int color = ChartUtils.pickColor();
//                while (color == chart2.getPreviewColor()) {
//                    color = ChartUtils.pickColor();
//                }
//                chart2.setPreviewColor(color);
//                return true;
//            }
//            return super.onOptionsItemSelected(item);
//        }

        private void generateDefaultData() {
            AssetManager am = getContext().getAssets();
            InputStream is = null;
            try {
                is = am.open("heatbeatSample1.txt");

                BufferedReader reader = new BufferedReader(new InputStreamReader(
                        is));
//                //Skip 2 lines.
//                for(int i=1;i<=2;i++)
//                {
//                    reader.readLine();
//                }

                int numValues = 0;

                List<PointValue> values = new ArrayList<PointValue>();
                String aStr = null;
                List<double[]> heatMetricsArray = new ArrayList<double[]>();
                List<double[]> clustersMetricsArray = new ArrayList<double[]>();

                while (reader.ready())
                {
                    aStr = reader.readLine();
//                    String[] separated = aStr.split("\t");
//                    String[] separated2 = separated[0].split(":");

//                    values.add(new PointValue(Float.parseFloat(separated2[1]), Float.parseFloat(separated[1])));
                    String[] separated = aStr.split(",");
                    int i = 0;
                    double [] aHeatArray = new double[separated.length];
                    while (i < separated.length)
                    {
//                        values.add(new PointValue(numValues, Float.parseFloat(separated[i])));
                        aHeatArray[i] = Double.parseDouble(separated[i]);
                        i++;
                    }
                    heatMetricsArray.add(aHeatArray);
                    numValues++;

                }
                double[][] heatMetrics = new double[numValues][];

                for (int i=0; i<numValues; i++)
                {
                    heatMetrics[i] = heatMetricsArray.get(i);
                }

                is.close();
                //get clusters
                is = am.open("clusters.txt");
                BufferedReader reader2 = new BufferedReader(new InputStreamReader(
                        is));
                numValues = 0;
                while (reader2.ready())
                {
                    aStr = reader2.readLine();
//                    String[] separated = aStr.split("\t");
//                    String[] separated2 = separated[0].split(":");

//                    values.add(new PointValue(Float.parseFloat(separated2[1]), Float.parseFloat(separated[1])));
                    String[] separated = aStr.split(",");
                    int i = 0;
                    double [] aClusterArray = new double[separated.length];
                    while (i < separated.length)
                    {
//                        values.add(new PointValue(numValues, Float.parseFloat(separated[i])));
                        aClusterArray[i] = Double.parseDouble(separated[i]);
                        i++;
                    }
                    clustersMetricsArray.add(aClusterArray);
                    numValues++;

                }
                double[][] clustersMetrics = new double[numValues][];

                for (int i=0; i<numValues; i++)
                {
                    clustersMetrics[i] = clustersMetricsArray.get(i);
                }
                double[][] diffWindowsMetrics = new double[numValues][];

                noiseTransform.initialize();
                diffWindowsMetrics = noiseTransform.nearKmean(clustersMetrics,heatMetrics);
                heatMetrics = noiseTransform.asSignal(heatMetrics);

                for (int i = 0; i < heatMetrics.length; i++) {
                    for (int j=0; j < heatMetrics[i].length; j++)
                     values.add(new PointValue(j, (float) heatMetrics[i][j]));
                 }
//                 for (int i = 0; i < numValues; ++i) {
//                     reader.readLine();
////                    values.add(new PointValue(i, (float) Math.random() * 100f));
//                     values.add(new PointValue(reader.read(), (float)reader.read()));
//                 }

                Line line = new Line(values);
                line.setColor(ChartUtils.COLOR_GREEN);
                line.setHasPoints(false);// too many values so don't draw points.

                 List<Line> lines = new ArrayList<Line>();
                lines.add(line);

                is.close();


            data = new LineChartData(lines);
            data.setAxisXBottom(new Axis());
            data.setAxisYLeft(new Axis().setHasLines(true));

            // prepare preview data, is better to use separate deep copy for preview chart.
            // Set color to grey to make preview area more visible.
            data2 = new LineChartData(data);
            data2.getLines().get(0).setColor(ChartUtils.DEFAULT_DARKEN_COLOR);
            } catch (IOException e) {
                e.printStackTrace();
            }

        }

        private void previewY() {
            Viewport tempViewport = new Viewport(chart.getMaximumViewport());
            float dy = tempViewport.height() / 4;
            tempViewport.inset(0, dy);
            chart2.setCurrentViewportWithAnimation(tempViewport);
            chart2.setZoomType(ZoomType.VERTICAL);
        }

        private void previewX(boolean animate) {
            Viewport tempViewport = new Viewport(chart.getMaximumViewport());
            float dx = tempViewport.width() / 4;
            tempViewport.inset(dx, 0);
            if (animate) {
                chart2.setCurrentViewportWithAnimation(tempViewport);
            } else {
                chart2.setCurrentViewport(tempViewport);
            }
            chart2.setZoomType(ZoomType.HORIZONTAL);
        }

        private void previewXY() {
            // Better to not modify viewport of any chart directly so create a copy.
            Viewport tempViewport = new Viewport(chart.getMaximumViewport());
            // Make temp viewport smaller.
            float dx = tempViewport.width() / 4;
            float dy = tempViewport.height() / 4;
            tempViewport.inset(dx, dy);
            chart2.setCurrentViewportWithAnimation(tempViewport);
        }

        /**
         * Viewport listener for preview chart(lower one). in {@link #onViewportChanged(Viewport)} method change
         * viewport of upper chart.
         */
        private class ViewportListener implements ViewportChangeListener {

            @Override
            public void onViewportChanged(Viewport newViewport) {
                // don't use animation, it is unnecessary when using preview chart.
                chart.setCurrentViewport(newViewport);
            }

        }

    }

    public  static ArrayList<heatbeat> getHeatBeatListFromTextFiles(String filePath)
    {
        FileInputStream fis = null;
        InputStreamReader isr = null;
        BufferedReader bReader = null;
        ArrayList<heatbeat> listRes = new ArrayList<heatbeat>();
        try{
            fis = new FileInputStream(filePath);
            isr = new InputStreamReader(fis);
            bReader = new BufferedReader(isr);
            // get from text
            String line = null;
            //save to array
            String[] strHeatBeat = null;
            while (true)
            {
                line = bReader.readLine();
                if (line == null)
                {
                    break;
                }
                else
                {
                    strHeatBeat = line.split(",");
//                    listRes.add(new heatbeat())
                }
            }
        }catch (Exception e)
        {

        }
        return listRes;
    }
}
