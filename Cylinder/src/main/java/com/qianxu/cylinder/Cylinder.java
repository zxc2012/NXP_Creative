package com.qianxu.cylinder;

public class Cylinder implements area {

    private double BottomArea; // 底面积
    private double High; // 高度
    private double Radius;

    public static void main(String[] args) {
        Cylinder a=new Cylinder(5, 4);
        System.out.println(a.getVolume());
    }

    //构造函数
    public Cylinder(double High, double Radius) {
        this.High = High;
        this.Radius=Radius;
        this.BottomArea = Math.pow(this.Radius, 2) * Math.PI;
    }

    //获取体积
    @Override
    public double getBottomArea() {
        return BottomArea;
    }

    //设置高度
    public void setHigh(double High) {
        this.High = High;
    }

    //获取高度
    public double getHigh() {
        return High;
    }

    //设置半径
    public void setRadius(double Radius) {
        this.Radius = Radius;
        this.BottomArea=Math.pow(Radius, 2) * Math.PI;
    }
    //获取半径
    public double getRadius() {
        return Radius;
    }

    //获取体积
    double getVolume() {
        return BottomArea * High;
    }


}

interface area {
    public double getBottomArea();
}