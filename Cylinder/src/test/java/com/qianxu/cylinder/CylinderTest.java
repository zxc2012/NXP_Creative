package com.qianxu.cylinder;

import java.io.FileDescriptor;
import java.io.FileOutputStream;
import java.io.PrintStream;

import org.junit.Test;



/**
 * Unit test for simple App.
 */
public class CylinderTest implements VolumeTest {

    private Cylinder cylinder;

    public void Init() {
        System.setOut(new PrintStream(new FileOutputStream(FileDescriptor.out))); // 重定向
        cylinder = new Cylinder(100, 2);
    }
    public static void main(String args[]){
        CylinderTest cylinderTest = new CylinderTest();
        cylinderTest.Init();
        System.out.println("半径：" + cylinderTest.getRadiusTest() + "\n高度：" + cylinderTest.getHighTest() + "\n底面积："
                + cylinderTest.getBottomAreaTest() + "\n体积：" + cylinderTest.getVolumeTest());
        cylinderTest.setHighTest(20);
        cylinderTest.setRadiusTest(5);
        System.out.println("改变半径，高度");
        System.out.println("半径：" + cylinderTest.getRadiusTest() + "\n高度：" + cylinderTest.getHighTest() + "\n底面积："
                + cylinderTest.getBottomAreaTest() + "\n体积：" + cylinderTest.getVolumeTest());
    }
    /**
     * Rigorous Test :-)
     */
    @Test
    public void Test() {
        CylinderTest cylinderTest = new CylinderTest();
        cylinderTest.Init();
        System.out.println("半径：" + cylinderTest.getRadiusTest() + "\n高度：" + cylinderTest.getHighTest() + "\n底面积："
                + cylinderTest.getBottomAreaTest() + "\n体积：" + cylinderTest.getVolumeTest());
        cylinderTest.setHighTest(20);
        cylinderTest.setRadiusTest(5);
        System.out.println("改变半径，高度");
        System.out.println("半径：" + cylinderTest.getRadiusTest() + "\n高度：" + cylinderTest.getHighTest() + "\n底面积："
                + cylinderTest.getBottomAreaTest() + "\n体积：" + cylinderTest.getVolumeTest());

    }

    @Override
    public double getVolumeTest() {
        return cylinder.getVolume();
    }

    @Override
    public void setHighTest(double High) {
        cylinder.setHigh(High);
    }

    @Override
    public double getHighTest() {
        return cylinder.getHigh();
    }

    @Override
    public double getRadiusTest() {
        return cylinder.getRadius();
    }

    @Override
    public void setRadiusTest(double Radius) {
        cylinder.setRadius(Radius);
    }

    @Override
    public double getBottomAreaTest() {
        return cylinder.getBottomArea();
    }

}


interface VolumeTest {
    public void setHighTest(double High);

    public void setRadiusTest(double Radius);

    public double getRadiusTest();

    public double getHighTest();

    public double getVolumeTest();

    public double getBottomAreaTest();
}