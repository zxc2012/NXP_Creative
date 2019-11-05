package quoridor;													
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.UnsupportedCommOperationException;
import gnu.io.CommPort;
import java.io.IOException;
import java.io.InputStream;
import gnu.io.SerialPort;
import java.io.BufferedReader;
import java.io.InputStreamReader;
/**
 *  Handles all the game that are based around console input, as an extension of the Game class. 
 */
public class ConsoleGame extends Game {
	public static void main(String[] args) {
		new ConsoleGame();
	}

	/**
	 *Constructor
	 *Will only return once game has been set up and is ready to play 
	 */
	public ConsoleGame() {
		System.out.println("Welcome to Quoridor!");
		while (!setUp());
	}

	/**
	 * Determines the type of players needed for this game (AI, human)
	 * Sets the board/players according to the user's specification
	 * @return true once set up is complete.
	 */
	private Boolean setUp() {
		while(!this.menu());
		return true;
	}	

	/**
	 * Displays the game menu and allows players to select the type
	 * of game they wish to play. A board will be constructed based on this input
	 * 
	 * @return true if a valid board was constructed, false otherwise
	 */
	protected boolean menu() {
		int selection = 0;
		while(selection != 3) {
			System.out.println();
			System.out.print("Please select your choice:\n" +
					"1 - 对方先行\n" +
					"2 - 我方先行\n" +
					"3 - Quit\n" );
			try {
				selection = Integer.parseInt(getFromUser("Enter selection: "));
			}
			catch (Exception e) {
				System.out.println("Invalid Input - type the number corresponding to your selection");
				return false;
			}
			switch(selection){
				case 1: case 2: board = Factory.instance().makeBoard(selection); break;
				case 3: quit(); break;
				default: System.out.println("Invalid Input");
			}
			if(selection >= 1 && selection <= 4 && board != null){
				return true;
			}
		}
		return false;
	}

	
	/**
	 * This function controls the flow of game play.
	 */
	public boolean playGame() {
		boolean movePlayed = false;
		while (!gameOver) {
			System.out.println(board);
			movePlayed = false;
			while(!movePlayed) {
				try {
					playNextTurn();
					movePlayed = true;
				}
				catch (RuntimeException e) {
					System.out.println("Error: " + e.getMessage());
				}
			}
		}
		this.checkWin = board.checkWin();
		return true;
	}

	/* (non-Javadoc)
	 * @see quoridor.Game#playNextTurn()
	 */
	protected void playNextTurn() {
		System.out.println(board.whoseTurn()+"'s Turn");

		Move move = board.currentPlayer().getMove(board);
		if (move != null) board.makeMove(move);
		else {
			String userInput= getFromUser("Enter Move: ");
			if(userInput.equals("save")) {
					String filename = getFromUser("what should I call this game? : ");
					save(filename);
			}
			else if (userInput.equals("quit")) {
				quit();
			}
			else if (userInput.equals("moves")) {
				System.out.println("List of valid moves:\n" + board.currentPlayer().validMoves(board));
			}
			else {
				board.makeMoveFromInput(userInput);
			}
		}
		if (board.checkWin() != 0) {
			gameOver = true;
			System.out.println(board);
			if (board.checkWin() == 1) System.out.println(board.players._1().getName() + " wins!");
			else System.out.println(board.players._2().getName() + " wins!");
		}
	}
		/**
	 * 用于打开串口
	 * 
	 * @param portName
	 *            串口名
	 * @param baudrate
	 *            波特率
	 * @return 返回串口
	 */
	public static final SerialPort portParameterOpen(String portName, int baudrate) {
		SerialPort serialPort = null;
		try { // 通过端口名识别串口
			CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
			// 打开端口并设置端口名字 serialPort和超时时间 2000ms
			CommPort commPort = portIdentifier.open(portName, 1000);
			// 进一步判断comm端口是否是串口 instanceof
			if (commPort instanceof SerialPort) {
				System.out.println("SerialPortNumber：" + portName);
				// 进一步强制类型转换
				serialPort = (SerialPort) commPort;
				// 设置baudrate 此处需要注意:波特率只能允许是int型 对于57600足够
				// 8位数据位
				// 1位停止位
				// 无奇偶校验
				serialPort.setSerialPortParams(baudrate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
						SerialPort.PARITY_NONE);
				// 串口配制完成 log
				System.out.println("串口参数设置已完成，波特率为" + baudrate + ",数据位8bits,停止位1位,无奇偶校验");
			} else { // 不是串口
				System.out.println("该com端口不是串口,请检查设备!");
				// 将com端口设置为null 默认是null不需要操作
			}

		} catch (NoSuchPortException e) {
			e.printStackTrace();
		} catch (PortInUseException e) {
			e.printStackTrace();
		} catch (UnsupportedCommOperationException e) {
			e.printStackTrace();
		}

		return serialPort;
	}
	/*
	 * 上位机接收数据 串口对象seriesPort 接收数据buffer 返回一个byte数组
	 */
	public static byte[] uartReceiveDatafromSingleChipMachine(SerialPort serialPort) {
		byte[] receiveDataPackage = null;
		InputStream in = null;
		try {
			in = serialPort.getInputStream();
			// 获取data buffer数据长度
			int bufferLength = in.available();
			while (bufferLength != 0) {
				receiveDataPackage = new byte[bufferLength];
				in.read(receiveDataPackage);
				bufferLength = in.available();

			}
		} catch (IOException e) {
			e.printStackTrace();
		}
		return receiveDataPackage;
	}
	/**
	 * Read one line from user input
	 * @return the read line as a String
	 */
	protected String getFromUser(String message) {
		try {
			while (true) {
				BufferedReader userReader = new BufferedReader(new InputStreamReader(System.in));
				System.out.print(message);
				String fromUser = userReader.readLine().toLowerCase();
				if (fromUser.trim().length() > 0){
					return fromUser;	
				} else {
					System.out.println("Sorry empty strings are not valid input");
					System.out.print("Please try again: ");
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
	}
}
