package quoridor;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

//import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener; 

import java.util.Enumeration; 
import java.util.TooManyListenersException; 



public class communication implements SerialPortEventListener { //SerialPortEventListener 监听器,我的理解是独立开辟一个线程监听串口数据 
static CommPortIdentifier portId; //串口通信管理类 

static Enumeration portList;   //已经连接上的端口的枚举 

InputStream inputStream; //从串口来的输入流 

OutputStream outputStream;//向串口输出的流 

SerialPort serialPort;     //串口的引用 


public communication() { 
try { 
portId = CommPortIdentifier.getPortIdentifier("COM13"); 
} catch (NoSuchPortException e1) { 
//没有这个串口 
e1.printStackTrace(); 
} 
   try { 
   serialPort = (SerialPort) portId.open("COM13", 2000);//打开串口名字为myapp,延迟为2毫秒 
   } catch (PortInUseException e) { 
   //串口正在使用 
   } 
   try { 
    inputStream = serialPort.getInputStream(); 
    outputStream = serialPort.getOutputStream(); 
   } catch (IOException e) { 
   } 
   
   try { 
   serialPort.addEventListener(this);      //给当前串口天加一个监听器 
   } catch (TooManyListenersException e) { 
   } 
   
   serialPort.notifyOnDataAvailable(true); //当有数据时通知 
   
   try { 
   serialPort.setSerialPortParams(115200, SerialPort.DATABITS_8,   //设置串口读写参数 
       SerialPort.STOPBITS_1, SerialPort.PARITY_NONE); 
   } catch (UnsupportedCommOperationException e) { 
   } 
} 
public void serialEvent(SerialPortEvent event) {//SerialPortEventListener 的方法,监听的时候会不断执行 
   switch (event.getEventType()) { 
   case SerialPortEvent.BI: 
   case SerialPortEvent.OE: 
   case SerialPortEvent.FE: 
   case SerialPortEvent.PE: 
   case SerialPortEvent.CD: 
   case SerialPortEvent.CTS: 
   case SerialPortEvent.DSR: 
   case SerialPortEvent.RI: 
   case SerialPortEvent.OUTPUT_BUFFER_EMPTY: 
    break; 
   case SerialPortEvent.DATA_AVAILABLE://当有可用数据时读取数据,并且给串口返回数据 
    byte[] readBuffer = new byte[1000]; 

    try { 
    while (inputStream.available() > 0) { 
    int numBytes = inputStream.read(readBuffer); 
    } 
    System.out.println(new String(readBuffer)); 
    char[] a= {0x16,0x16,0x20,0x10,0x69,0x0,0x03,0x20,0x01,0x22}; 
    byte[] b = new String(a).getBytes(); 
outputStream.write(b); 
    } catch (IOException e) { } 
    break; 
   } 
} 

public static void main(String[] args) { 
	communication reader = new communication(); //实例一个 
} 
} 