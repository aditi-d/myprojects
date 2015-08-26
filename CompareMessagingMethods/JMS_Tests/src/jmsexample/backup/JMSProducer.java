package jmsexample;

import org.apache.activemq.ActiveMQConnectionFactory;
import javax.jms.*;
import java.lang.instrument.*;

public class JMSProducer {
	private static Instrumentation instrumentation;
	/*static{
		instrumentation = new 
	}*/
	public void startProducer(){
		System.out.println("producer started....");
		try {
			
            // Create a ConnectionFactory
            //ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory("vm://localhost");
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
            Destination destination = session.createQueue("test.queue");

            // Create a MessageProducer from the Session to the Queue
            MessageProducer producer = session.createProducer(destination);
            producer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);
            System.out.println("message producer created....");

            // Create a messages
            int i =0;
            while(i < 100){
            String text = "test message";//"Hello world! From: " + Thread.currentThread().getName() + " : " + this.hashCode();
            TextMessage message = session.createTextMessage(text);
            //BytesMessage message = session.createBytesMessage();
            //message.writeBytes(chunk);
            //message.reset();
            System.out.println("byte message created...." +" timestamp::" +message.getJMSTimestamp());
            		//+ "bytes sent::" +  instrumentation.getObjectSize(message));
            /*while(message.getPropertyNames().hasMoreElements()) {
            	System.out.println(message.getPropertyNames().nextElement());
            	System.out.println(message.);
            }*/
            
            // Tell the producer to send the message
            System.out.println("Sent message: ");//+ message.hashCode() + " : " + Thread.currentThread().getName());
            producer.send(message);
            i++;
            }
            // Clean up
            session.close();
            connection.close();
        }
        catch (Exception e) {
            System.out.println("Caught: " + e);
            e.printStackTrace();
        }
    }
	
	public static void main(String args[]){
		System.out.println("producer starting....");
		JMSProducer producer = new JMSProducer();
		producer.startProducer();
	}
}
		

