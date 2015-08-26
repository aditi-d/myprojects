package jmsexample;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileReader;
import java.io.BufferedReader;
import java.util.Arrays;

public class TestFile {
	private String file_name;
	final int MAX_CHUNK_SIZE = 2000;
	public void readFile(String f_name) throws IOException{
		file_name = f_name;		
	}
	
	public void openFile() throws IOException{

		/*//reads as strings:

		String line;
		FileReader fr = new FileReader(file_name);
		BufferedReader bffreader = new BufferedReader(fr);

		while ((line = bffreader.readLine()) != null) {
			System.out.println(line);
		}		
		bffreader.close();
		fr.close();*/
		
		//reads as bytes
		File file = new File(file_name);
		FileInputStream fsin = null;
		byte[] chunk = new byte[MAX_CHUNK_SIZE]; 
		int offset = 0;
		long bytesRemaining = 0;
		try{
			fsin = new FileInputStream(file);
			bytesRemaining = file.length();
			while(bytesRemaining > 0){
				fsin.read(chunk, offset, MAX_CHUNK_SIZE);
				bytesRemaining -= MAX_CHUNK_SIZE;
				System.out.println("Read::" + Arrays.toString(chunk));
			}			
		}
		finally{
			if(fsin != null){
				fsin.close();
			}
		}		
	}
	public void writeFile() throws IOException{
		/*
		 * 
		 * //writes as strings
		FileWriter fw = new FileWriter(file_name);
		BufferedWriter bffwriter = new BufferedWriter(fw);
		int i = 0;
		
		while(i < 10){
			bffwriter.write('a');
		}
		bffwriter.close();
		fw.close();
		*/
	}
	public static void main(String args[]){
		/*while(true){
			System.out.println("test");
		}*/
		
		try{
			TestFile tf =  new TestFile();
			tf.readFile("src\\temp.txt");
			tf.openFile();
		}
		catch(IOException io){
			io.printStackTrace();
		}
	}
}

