package com.zzmax.admin.ecganomalydetection;

/**
 * Created by admin on 16/9/9.
 */
public class noiseTransform {

    /* loading the C DLL */
    static { System.loadLibrary("transformNoise"); }
    /* functions prototypes */
    public static native void initialize();
    public static native double[][] nearKmean(double[][] clusters,double[][] samples);
    public static native double[][] diffKmean(double[][] clusterIndices,double[][] clusters,double[][] samples);
    public static native double[][] asSignal(double[][] diffWindows);
    public static native double[] detectAnomaly(double[][] reconstruct, double usualMean,
                                                double usualStd );
}
