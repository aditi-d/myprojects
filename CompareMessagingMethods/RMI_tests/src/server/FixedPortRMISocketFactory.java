package server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.rmi.server.RMISocketFactory;

public class FixedPortRMISocketFactory extends RMISocketFactory{

//	@Override
	public ServerSocket createServerSocket(int port) throws IOException {
	//	ServerSocket socket = null;
		if(port == 0){
			return new ServerSocket(4440);
		}		
		return createServerSocket(port); 
	}

	@Override
	public Socket createSocket(String host, int port) throws IOException {
		Socket socket = new Socket("localhost", 4440);
		return socket;
	}
	
}