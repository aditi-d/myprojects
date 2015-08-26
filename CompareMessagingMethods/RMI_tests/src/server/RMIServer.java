/***
 * Developed by: Aditi Deshpande
 * 
 * This class acts as the server for the testing of this research. 
 * This class receives files of different sizes from the client and sends the response.
 * 
 */
package server;

import java.io.IOException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
//import java.rmi.server.RMISocketFactory;
import java.rmi.server.UnicastRemoteObject;

public class RMIServer implements IFileSizeCompute{

	private int file_size = 0;
	@Override
	public void send_file(String line) throws RemoteException {
		file_size += line.length();		
	}

	@Override
	public int get_file_size() throws RemoteException {
		return file_size;
	}
	 
	public static void main(String[] args) {
		IFileSizeCompute stub = null;
        try {
            String name = "FileSizeServer1";
            IFileSizeCompute engine = new RMIServer();        
            stub =
                (IFileSizeCompute) UnicastRemoteObject.exportObject(engine, 0);
            Registry registry = LocateRegistry.getRegistry();
            registry.rebind(name, stub);
            System.out.println("FileSizeServer bound");
        }
        catch(IOException io){
        	io.printStackTrace();
        }
        catch (Exception e) {
            System.err.println("FileSizeServer exception:");
            e.printStackTrace();
        }
    }
}
