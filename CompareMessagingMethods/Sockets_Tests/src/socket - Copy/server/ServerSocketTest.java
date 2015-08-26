package socket.server;

import java.io.*;
import java.net.*;

public class ServerSocketTest {
	private static int portNumber = 4444;
	private ServerSocket serverSocket = null;
	private Socket clientSocket = null;
	private PrintWriter p_out = null;
	private BufferedReader br_in = null;
	
	public void initConnections(){
		try{
		    serverSocket =
                new ServerSocket(portNumber);
            clientSocket = serverSocket.accept();     
            p_out =
                new PrintWriter(clientSocket.getOutputStream(), true);                   
            br_in = new BufferedReader(
                new InputStreamReader(clientSocket.getInputStream()));
		}
		catch(IOException io){
			io.printStackTrace();
		}
	}
	
	public void runServer() throws IOException{		
		String inputLine;
		while ((inputLine = br_in.readLine()) != null) {
			System.out.println(inputLine);
			p_out.println("Server says echo::"+inputLine);
		}
		serverSocket.close();	
	}
	
	public void runServerForFileData() throws IOException{
		 String inputLine = null;
		 int byteCount = 0;
		 System.out.println("in");
         while (!(inputLine = br_in.readLine()).equals("null")) {
		    //inputLine = br_in.readLine();
         	System.out.println("data" + inputLine);
         	byteCount += inputLine.length();
            System.out.println("Server says echo::"+inputLine);
         }
         System.out.println("out");
         p_out.println("Bytes received::"+byteCount);         
         serverSocket.close();
	}
	
	public static void main(String args[]){
		ServerSocketTest sst = new ServerSocketTest();
		sst.initConnections();
		try{
			//sst.runServer();
			sst.runServerForFileData();
		}
		catch(IOException io){
			io.printStackTrace();
		}
	}
}
