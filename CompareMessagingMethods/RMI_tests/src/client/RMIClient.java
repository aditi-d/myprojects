/***
 * Developed by: Aditi Deshpande
 * 
 * This class acts the client for the testing of this research. 
 * This class send files of different sizes to the server and receives the response.
 * It records the round trip time all the file sizes in another file.
 */

package client;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
//import java.rmi.server.RMISocketFactory;

import server.IFileSizeCompute;
//import server.FixedPortRMISocketFactory;

public class RMIClient {
	
	FileWriter fw = null;
	BufferedWriter buff_writer = null;
	
	public RMIClient() throws IOException{
		fw = new FileWriter("src\\RMI_results.txt",true);
		buff_writer =  new BufferedWriter(fw);
	}
	
	public String runRMIClient(String host){
		String retval = null;
		int chars_read = 0;
		try {
			String name = "FileSizeServer1";
			// RMISocketFactory.setSocketFactory(new FixedPortRMISocketFactory());
			Registry registry = LocateRegistry.getRegistry(host);
			String line;
			FileReader fr = new FileReader("src\\temp_11.txt");
			BufferedReader bffreader = new BufferedReader(fr);
			long start = System.nanoTime();
			IFileSizeCompute comp = (IFileSizeCompute) registry.lookup(name);
			
			while ((line = bffreader.readLine()) != null) {
				comp.send_file(line);
			}					
			chars_read = comp.get_file_size();
			long elapsedTime = System.nanoTime() - start;
			retval = chars_read + "," + elapsedTime;
			bffreader.close();
			fr.close();	          	           

		} catch (Exception e) {
			System.err.println("FileSizeServer exception:");
			e.printStackTrace();
		}
		return retval;
	}
	
	public static void main(String args[]) {  
		RMIClient client = null; 
		try{
		String retval = null;
		client = new RMIClient();
		retval = client.runRMIClient(args[0]);
		client.buff_writer.write(retval);
		client.buff_writer.newLine();
		client.buff_writer.close();
		client.fw.close();
		}catch(Exception e){
			e.printStackTrace();
		}
	}    
}
