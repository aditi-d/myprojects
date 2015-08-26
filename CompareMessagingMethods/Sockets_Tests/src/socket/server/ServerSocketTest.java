/***
 * Developed by: Aditi Deshpande
 * 
 * This class acts as the server for the testing of this research. 
 * This class receives files of different sizes from the client and sends the response.
 * 
 */

package socket.server;

import java.io.*;
import java.net.*;

public class ServerSocketTest {
	private static int portNumber = 4444;
	private ServerSocket serverSocket = null;
	private Socket clientSocket = null;
	private PrintWriter p_out = null;
	private BufferedReader br_in = null;
	int port_num = 4444;
	static int int_val = 36;
	
	public ServerSocketTest(){
		 int_val = (int_val % 36) - 1;
		 port_num =  portNumber + int_val;
		try{
		    serverSocket =
                new ServerSocket(port_num);
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
	
	public void runServerForFileData() throws IOException{
		 String inputLine = null;
		 int byteCount = 0;
         while (!(inputLine = br_in.readLine()).equals("null")) {		 
         	byteCount += inputLine.length();
         }         
         p_out.println(byteCount);         
         serverSocket.close();
         clientSocket.close();
         p_out.close();
         br_in.close();
	}
	
	public static void main(String args[]){
		for(int i = 0; i < 12; i++){
			for(int j = 0;j < 3; j++){
				ServerSocketTest sst = new ServerSocketTest();
				try{				
					sst.runServerForFileData();
				}
				catch(IOException io){
					io.printStackTrace();
				}
			}
		}
	}
}
