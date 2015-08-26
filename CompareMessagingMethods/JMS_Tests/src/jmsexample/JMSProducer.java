/***
 * Developed by: Aditi Deshpande
 * 
 * This class acts the client for the testing of this research. 
 * This class send files of different sizes to the server and receives the response.
 * It records the round trip time all the file sizes in another file.
 */


package jmsexample;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import javax.jms.Connection;
import javax.jms.DeliveryMode;
import javax.jms.Destination;
import javax.jms.Message;
import javax.jms.MessageConsumer;
import javax.jms.MessageProducer;
import javax.jms.Session;
import javax.jms.TextMessage;

import org.apache.activemq.ActiveMQConnectionFactory;

public class JMSProducer {
	String retval = null;
	FileWriter fw = null;
	BufferedWriter buff_writer = null;
	public JMSProducer() throws IOException{
		fw = new FileWriter("src\\JMS_results.txt",true);
		buff_writer =  new BufferedWriter(fw);
	}
	public String startProducer(String queue_name, String file_name){
		System.out.println("producer started....");		
		long elapsedTime = 0;
		try {
			
            // Create a ConnectionFactory      
			ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory();
			System.out.println("connection factory created....");
            
            // Create a Connection
            Connection connection = connectionFactory.createConnection();
            connection.start();
            System.out.println("connection created....");
            
            // Create a Session
            Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
            System.out.println("session created....");

            // Create the destination Queue
            Destination destination = session.createQueue(queue_name);

            // Create a MessageProducer from the Session to the Queue
            MessageProducer producer = session.createProducer(destination);
            producer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);
            System.out.println("message producer created....");

            // Create messages
            String line = null;
			FileReader fr = new FileReader(file_name);
			BufferedReader bffreader = new BufferedReader(fr);
			File file = new File(file_name);
			
			//starting recording for round trip time		
			long start = System.nanoTime();
			
			//sending the data
			while ((line = bffreader.readLine()) != null) {				
				TextMessage message = session.createTextMessage(line);
				producer.send(message);
			}							
			TextMessage message = session.createTextMessage(line);
			producer.send(message);
			elapsedTime = System.nanoTime() - start;
			System.out.println("elapsed time::" + elapsedTime);
			
			//clean up for sending process
			bffreader.close();
			fr.close();            
            producer.close();
            
            //receive response from the server
            Destination response_destination = session.createQueue("resp_queue");
            MessageConsumer consumer = session.createConsumer(response_destination);
            Message message_received = consumer.receive();
                  
            retval = file.length() + "," + elapsedTime + "," + message_received ;
            
            // Clean up for receiving process
            consumer.close();
            session.close();
            connection.close();
        }
        catch (Exception e) {
            System.out.println("Caught: " + e);
            e.printStackTrace();
        }
		return retval;
    }
	
	public static void main(String args[]){
		System.out.println("producer starting....");
		String retval = null;
		JMSProducer producer = null;
		try{
			producer = new JMSProducer();
			retval = producer.startProducer("queue_5_3","src\\temp_5.txt");
			producer.buff_writer.write(retval);
			producer.buff_writer.newLine();
			producer.buff_writer.close();
			producer.fw.close();
		}
		catch(IOException io){
			io.printStackTrace();
		}		 
	}
}
		

