package socket.client;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
//import java.io.File;
//import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class ClientSocketTest {

	private Socket fileSizerSocket     = null;
	private PrintWriter p_out       = null;
	private BufferedReader br_in     = null;
	//private BufferedReader stdIn  = null;
	//private String file_name = null;
	final int MAX_CHUNK_SIZE = 2000;
	static String socket_result = "src\\socket_result_2.txt";
	int port_num = 0;
	static int int_val = 36;
	static FileWriter fw = null;
	static BufferedWriter buff_writer =  null;
	
	
	public ClientSocketTest(){
		 int_val = (int_val % 36) - 1;
		 port_num =  4444 + int_val;
		 try{
				fileSizerSocket = new Socket("localhost", port_num);
	            p_out = new PrintWriter(fileSizerSocket.getOutputStream(), true);
	            br_in = new BufferedReader(
	                    					new InputStreamReader(fileSizerSocket.getInputStream()));          
			}
			catch(UnknownHostException uhe){
				uhe.printStackTrace();
			}
			catch(IOException io){
				io.printStackTrace();
			}
	}
	
	/*public void runClient(){		
            String userInput;
            stdIn = new BufferedReader(new InputStreamReader(System.in));
            try{
            	while ((userInput = stdIn.readLine()) != null) {
                    p_out.println(userInput);
                    System.out.println("echo: " + br_in.readLine());
                }
            }
            catch(IOException io){
    			io.printStackTrace();
            }            
	}*/
	
	public void sendFileInStringFormat(String f_name) throws IOException{				
		String line = null;
		FileReader fr = null;
		BufferedReader bffreader = null;
		File file = null;
		try{
			
			fr = new FileReader(f_name);
			bffreader = new BufferedReader(fr);
			file = new File(f_name);
			
			long start = System.nanoTime();
			while ((line = bffreader.readLine()) != null) {
				//System.out.println("here::"+line);
				p_out.println(line);
			}					
			p_out.println(line);
			long elapsedTime = System.nanoTime() - start;
			//System.out.println("Chars read::" + br_in.readLine() + "elapsed time::" + elapsedTime);
			//(file_length,bytes_read,port_num,time);
			buff_writer.write(file.length() + "," + br_in.readLine() + "," + port_num + "," + elapsedTime);
			buff_writer.newLine();
		}
		finally{
			bffreader.close();
			fr.close();				
			if(p_out != null)
				p_out.close();
			if(fileSizerSocket != null)
				fileSizerSocket.close();			
		}		
	}
	
	public static void main(String args[]) throws IOException{	
		try {
			fw = new FileWriter(socket_result);
			buff_writer =  new BufferedWriter(fw);
			
			for(int i = 0; i < 12; i++){
				String fname = "src\\temp_" + i + ".txt";
				for(int j = 0; j < 3; j ++){					
					ClientSocketTest cst = new ClientSocketTest();
					cst.sendFileInStringFormat(fname);
					System.out.println("file "+ fname + " read.");
				}
			}		
			/*for(int j = 0; j < 3; j ++){
				ClientSocketTest cst = new ClientSocketTest();
				cst.sendFileInStringFormat("src\\temp_0");
				System.out.println("file "+ "src\\temp_0" + " read.");
			}*/
		} catch (FileNotFoundException e) {			
			e.printStackTrace();
		}		
		catch(IOException io){
			io.printStackTrace();
		}
		finally{
			buff_writer.close();
			fw.close();			
		}
	}
}
