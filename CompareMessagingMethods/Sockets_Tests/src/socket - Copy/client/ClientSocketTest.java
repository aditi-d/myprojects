package socket.client;

import java.io.BufferedReader;
//import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class ClientSocketTest {

	private Socket fileSizerSocket     = null;
	private PrintWriter p_out       = null;
	private BufferedReader br_in     = null;
	private BufferedReader stdIn  = null;
	//private String file_name = null;
	final int MAX_CHUNK_SIZE = 2000;
	public void initConnections(){
		try{
			fileSizerSocket = new Socket("localhost", 4444);
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
	
	public void runClient(){		
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
	}
	
	public void sendFileInStringFormat(String f_name) throws IOException{				
		FileInputStream fsin = null;
		/*byte[] chunk = new byte[MAX_CHUNK_SIZE]; 
		int offset = 0;
		long bytesRemaining = 0;
		File file = new File(f_name);*/
		try{
			/*fsin = new FileInputStream(file);
			bytesRemaining = file.length();
			long start = System.nanoTime();
			while(bytesRemaining > 0){
				fsin.read(chunk, offset, MAX_CHUNK_SIZE);
				bytesRemaining -= MAX_CHUNK_SIZE;
				System.out.println("Read::" + Arrays.toString(chunk));
				out.print(chunk);
			}*/
			String line;
			FileReader fr = new FileReader(f_name);
			BufferedReader bffreader = new BufferedReader(fr);
			long start = System.nanoTime();
			while ((line = bffreader.readLine()) != null) {
				System.out.println("data::" + line);
				p_out.println(line);
			}					
			p_out.println("data::"+line);
			long elapsedTime = System.nanoTime() - start;
			System.out.println("Chars read::" + br_in.readLine() + "elapsed time::" + elapsedTime);
			bffreader.close();
			fr.close();
		}
		finally{
			if(fsin != null){
				fsin.close();
			}
			if(fileSizerSocket != null)
				fileSizerSocket.close();
		}		
	}
	
	public static void main(String args[]){
		String socket_result = "";
		FileReader fr;
		try {
			fr = new FileReader(socket_result);
			BufferedReader bffreader = new BufferedReader(fr);
			
			ClientSocketTest cst = new ClientSocketTest();
			cst.initConnections();
			cst.sendFileInStringFormat("src\\temp0.txt");
			
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
		catch(IOException io){
			io.printStackTrace();
		}
		//cst.initConnections();
		//cst.runClient();
	}
}
