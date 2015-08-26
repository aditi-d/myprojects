package server;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface IFileSizeCompute extends Remote{
	void send_file(String line) throws RemoteException;
	int get_file_size() throws RemoteException;
}
