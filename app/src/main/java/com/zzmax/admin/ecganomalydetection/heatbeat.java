package com.zzmax.admin.ecganomalydetection;

/**
 * Created by admin on 16/9/9.
 */
public class heatbeat {
    public heatbeat(float heatBeatVolt) {
        this.heatBeatVolt = heatBeatVolt;
    }

    private float heatBeatVolt;

    public float getHeatBeatVolt() {
        return heatBeatVolt;
    }

    public void setHeatBeatVolt(float heatBeatVolt) {
        this.heatBeatVolt = heatBeatVolt;
    }
}
