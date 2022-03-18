package com.hekangyuan.nivi1000.view.graphViewForLive;


public class GraphPoint {
    float x, y;
    String colore = "";

    public GraphPoint(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public GraphPoint(float x, float y, String colore) {
        this.x = x;
        this.y = y;
        this.colore = colore;
    }


    public void setX(float x) {
        this.x = x;
    }

    public void setY(float y) {
        this.y = y;
    }

    public float getX() {
        return x;
    }

    public float getY() {
        return y;
    }

    public String getColore() {
        return colore;
    }

    public void setColore(String colore) {
        this.colore = colore;
    }

}
