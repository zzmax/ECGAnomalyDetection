package com.zzmax.admin.ecganomalydetection;

import android.content.res.AssetManager;
import android.net.Uri;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;

import java.io.BufferedReader;
import java.io.FileInputStream;
//import java.io.FileInputStreamReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
//import java.lang.object;
import org.apache.commons.math3.stat.descriptive.*;

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

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
        setContentView(R.layout.activity_signal_chart);
        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
        }
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Main Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app URL is correct.
                Uri.parse("android-app://com.zzmax.admin.ecganomalydetection/http/host/path")
        );
        AppIndex.AppIndexApi.start(client, viewAction);
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        Action viewAction = Action.newAction(
                Action.TYPE_VIEW, // TODO: choose an action type.
                "Main Page", // TODO: Define a title for the content shown.
                // TODO: If you have web page content that matches this app activity's content,
                // make sure this auto-generated web page URL is correct.
                // Otherwise, set the URL to null.
                Uri.parse("http://host/path"),
                // TODO: Make sure this auto-generated app URL is correct.
                Uri.parse("android-app://com.zzmax.admin.ecganomalydetection/http/host/path")
        );
        AppIndex.AppIndexApi.end(client, viewAction);
        client.disconnect();
    }

    /**
     * A fragment containing a line chart.
     */
    public static class PlaceholderFragment extends Fragment {

        private LineChartView chart;//for signal orignal
        private LineChartData data;

        private TextView outlierView;
        private TextView sigma1View;
        private TextView sigma2View;
        private TextView sigma3View;
        private TextView changesMView;
        private TextView changesSView;
        private TextView dataLenView;


        private LineChartView chart2;//for noise
        private LineChartData data2;

        private int numberOfLines = 2;
        private int maxNumberOfLines = 4;
        private int numberOfPoints = 12;

        private double[] anomalies = new double[6];
        private double dataLen;

        private String [] dataFileNames = {"normalHeatbeat.txt","arrhyHeatbeat.txt","cardiacDeathHeatbeat.txt"};

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
            outlierView = (TextView) rootView.findViewById(R.id.outlierNum);
             sigma1View = (TextView) rootView.findViewById(R.id.sigma1Num);
             sigma2View = (TextView) rootView.findViewById(R.id.sigma2Num);
             sigma3View = (TextView) rootView.findViewById(R.id.sigma3Num);
             changesMView = (TextView) rootView.findViewById(R.id.changesMNum);
             changesSView = (TextView) rootView.findViewById(R.id.changesSNum);
            dataLenView = (TextView) rootView.findViewById(R.id.dataLengthNum);

//            chart2 = (LineChartView) rootView.findViewById(R.id.chart2);

            // Generate data for previewed chart and copy of that data for preview chart.
            generateDefaultData(0);
            setTextViewsDate();
            chart.setLineChartData(data);

            // Disable zoom/scroll for previewed chart, visible chart ranges depends on preview chart viewport so
            // zoom/scroll is unnecessary.
            chart.setZoomEnabled(true);
            chart.setScrollEnabled(true);




//            chart2.setLineChartData(data2);
//            chart2.setViewportChangeListener(new ViewportListener());
//
//            previewX(false);

            return rootView;
        }

        // MENU
        @Override
        public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
            inflater.inflate(R.menu.line_chart, menu);
        }


        @Override
        public boolean onOptionsItemSelected(MenuItem item) {
            int id = item.getItemId();
            if (id == R.id.normal_heatbeat) {
                generateDefaultData(0);
                setTextViewsDate();
                chart.setLineChartData(data);
//                chart2.setLineChartData(data2);
//                previewX(true);
                return true;
            }
            if (id == R.id.arrhythmia_heatbeat) {
                generateDefaultData(1);
                setTextViewsDate();
                chart.setLineChartData(data);
                return true;
            }
            if (id == R.id.cardiac_death_heatbeat) {
                generateDefaultData(2);
                setTextViewsDate();
                chart.setLineChartData(data);
                return true;
            }

            return super.onOptionsItemSelected(item);
        }

        private void setTextViewsDate(){
            outlierView.setText( String.valueOf(anomalies[0]));
            sigma1View.setText( String.valueOf(anomalies[1]));
            sigma2View.setText( String.valueOf(anomalies[2]));
            sigma3View.setText( String.valueOf(anomalies[3]));
            changesMView.setText( String.valueOf(anomalies[4]/dataLen));
            changesSView.setText( String.valueOf(anomalies[5]/dataLen));
            dataLenView.setText( String.valueOf(dataLen));
        }

        private void generateDefaultData(int dataFileIndice) {
            AssetManager am = getContext().getAssets();
            InputStream is = null;
            try {
                /*
                is = am.open("heatbeatSample1.txt");

                BufferedReader reader = new BufferedReader(new InputStreamReader(
                        is));

                int numHeatMetricsRow = 0;


                String aStr = null;
                List<double[]> heatMetricsArray = new ArrayList<double[]>();


                while (reader.ready()) {
                    aStr = reader.readLine();
                    String[] separated = aStr.split(",");
                    int i = 0;
                    double[] aHeatArray = new double[separated.length];
                    while (i < separated.length) {
                        aHeatArray[i] = Double.parseDouble(separated[i]);
                        i++;
                    }
                    heatMetricsArray.add(aHeatArray);
                    numHeatMetricsRow++;

                }
                double[][] heatMetrics = new double[numHeatMetricsRow][];

                for (int i = 0; i < numHeatMetricsRow; i++) {
                    heatMetrics[i] = heatMetricsArray.get(i);
                }

                is.close();


                double[][] clusterIndiceMetrics = new double[numHeatMetricsRow][1];

                noiseTransform.initialize();
                clusterIndiceMetrics = noiseTransform.nearKmean(clustersMetrics, heatMetrics);

                double[][] diffWindowsMetrics = new double[numHeatMetricsRow][];
                diffWindowsMetrics = noiseTransform.diffKmean(clusterIndiceMetrics, clustersMetrics, heatMetrics);
                double[][] reconstructDelta  = noiseTransform.asSignal(diffWindowsMetrics);


                */

/************************************************
                DescriptiveStatistics stats = new DescriptiveStatistics();
                // Add the data from the array
                for( int i = 0; i < reconstructDelta[0].length; i++) {
                    stats.addValue(reconstructDelta[0][i]);
                }

                // Compute some statistics
                double mean = stats.getMean();
                double std = stats.getStandardDeviation();*/

/***************************************************/

                //get clusters
                is = am.open("clusters.txt");
                BufferedReader reader2 = new BufferedReader(new InputStreamReader(
                        is));
                String aStr = null;
                List<double[]> clustersMetricsArray = new ArrayList<double[]>();
                int numClusterMetricsRow = 0;
                while (reader2.ready()) {
                    aStr = reader2.readLine();
                    String[] separated = aStr.split(",");
                    int i = 0;
                    double[] aClusterArray = new double[separated.length];
                    while (i < separated.length) {
                        aClusterArray[i] = Double.parseDouble(separated[i]);
                        i++;
                    }
                    clustersMetricsArray.add(aClusterArray);
                    numClusterMetricsRow++;

                }
                double[][] clustersMetrics = new double[numClusterMetricsRow][];

                for (int i = 0; i < numClusterMetricsRow; i++) {
                    clustersMetrics[i] = clustersMetricsArray.get(i);
                }

                is = am.open(dataFileNames[dataFileIndice]);

                BufferedReader reader3 = new BufferedReader(new InputStreamReader(
                        is));

                int numSigHeatMetricsRow = 0;
                aStr = null;
                List<double[]> sigHeatMetricsArray = new ArrayList<double[]>();

                while (reader3.ready()) {
                    aStr = reader3.readLine();
                    String[] separated = aStr.split(",");
                    int i = 0;
                    double[] aSigHeatArray = new double[separated.length];
                    while (i < separated.length) {
                        aSigHeatArray[i] = Double.parseDouble(separated[i]);
                        i++;
                    }
                    sigHeatMetricsArray.add(aSigHeatArray);
                    numSigHeatMetricsRow++;

                }
                double[][] sigHeatMetrics = new double[numSigHeatMetricsRow][];

                for (int i = 0; i < numSigHeatMetricsRow; i++) {
                    sigHeatMetrics[i] = sigHeatMetricsArray.get(i);
                }
                double[][] clusterIndiceMetrics = new double[numSigHeatMetricsRow][1];
                double[][] diffWindowsMetrics = new double[numSigHeatMetricsRow][];
                clusterIndiceMetrics = noiseTransform.nearKmean(clustersMetrics, sigHeatMetrics);

                diffWindowsMetrics = new double[numSigHeatMetricsRow][];
                diffWindowsMetrics = noiseTransform.diffKmean(clusterIndiceMetrics, clustersMetrics, sigHeatMetrics);
                double[][] reconstructDelta = noiseTransform.asSignal(diffWindowsMetrics);
                is.close();

                double[][] clustersForSignalMetrics = new double[numSigHeatMetricsRow][];
                for (int i = 0; i < numSigHeatMetricsRow; i++){
                    clustersForSignalMetrics[i] = clustersMetricsArray.get((int)(clusterIndiceMetrics[i][0]) - 1 );
//                    int t = (int)(clusterIndiceMetrics[i][0]);
//                    clustersForSignalMetrics[i] = clustersMetrics[t];
                }
                
                is = am.open("meanAndStd.txt");

                BufferedReader reader4 = new BufferedReader(new InputStreamReader(
                        is));

                aStr = null;
                double mean = 0;
                double std = 0;
                while (reader4.ready()) {
                    aStr = reader4.readLine();
                    String[] separated = aStr.split(",");
                    int i = 0;
                    while (i < separated.length) {
                        switch (i)
                        {
                            case 0: mean = Double.parseDouble(separated[i]);
                            break;
                            case 1: std = Double.parseDouble(separated[i]);
                                break;
                        }


                        i++;
                    }
                }

                anomalies =noiseTransform.detectAnomaly(reconstructDelta,mean,std);
                dataLen = reconstructDelta[0].length;
/*********************************************************************/
                /****
                 * Draw charts for signal(1:200) and clustersForSignalMetrics(1:200)
                 */
                List<PointValue> valueSig = new ArrayList<PointValue>();
                List<PointValue> valueCluster = new ArrayList<PointValue>();
                int len = sigHeatMetrics[0].length;
                for (int k=0; k < 200/len; k++) {
                    for (int j = 0; j < len; j++)
                    {
                        valueSig.add(new PointValue(k*len+j, (float) sigHeatMetrics[k][j]));
                        valueCluster.add(new PointValue(k*len+j, (float) clustersForSignalMetrics[k][j]));
                    }

                }

                Line line = new Line(valueSig);
                line.setColor(ChartUtils.COLOR_GREEN);
                line.setStrokeWidth(1);
                line.setHasPoints(false);// too many values so don't draw points.
                Line lineCluster = new Line(valueCluster);
                lineCluster.setColor(ChartUtils.COLOR_ORANGE);
                lineCluster.setHasPoints(false);// too many v
                lineCluster.setStrokeWidth(1);

                List<Line> lines = new ArrayList<Line>();
                lines.add(line);
                lines.add(lineCluster);

                is.close();


                data = new LineChartData(lines);
                data.setAxisXBottom(new Axis());
                data.setAxisYLeft(new Axis().setHasLines(true));
//



                // prepare preview data, is better to use separate deep copy for preview chart.
                // Set color to grey to make preview area more visible.
//                data2 = new LineChartData(data);
//                data2.getLines().get(0).setColor(ChartUtils.DEFAULT_DARKEN_COLOR);
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

    public static ArrayList<heatbeat> getHeatBeatListFromTextFiles(String filePath) {
        FileInputStream fis = null;
        InputStreamReader isr = null;
        BufferedReader bReader = null;
        ArrayList<heatbeat> listRes = new ArrayList<heatbeat>();
        try {
            fis = new FileInputStream(filePath);
            isr = new InputStreamReader(fis);
            bReader = new BufferedReader(isr);
            // get from text
            String line = null;
            //save to array
            String[] strHeatBeat = null;
            while (true) {
                line = bReader.readLine();
                if (line == null) {
                    break;
                } else {
                    strHeatBeat = line.split(",");
//                    listRes.add(new heatbeat())
                }
            }
        } catch (Exception e) {

        }
        return listRes;
    }
}
