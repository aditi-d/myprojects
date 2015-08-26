/***
 * Developed by: Aditi Deshpande
 * 
 * This class acts as the server for the testing of this research. 
 * This class receives files of different sizes from the client and sends the response.
 * 
 */

package jmsexample;

import org.apache.activemq.ActiveMQConnectionFactory;

import javax.jms.*;

public class JMSConsumer implements ExceptionListener{
	public void startConsumer(String queue_name){
		try {
			 
            // Create a ConnectionFactory           
			ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory();
            
			// Create a Connection
            Connection connection = connectionFactory.createConnection();
            connection.start();

            connection.setExceptionListener(this);

            // Create a Session
            Session session = connection.createSession(false,Session.AUTO_ACKNOWLEDGE);

            // Create the destination (Topic or Queue)
            Destination destination = session.createQueue(queue_name);

            // Create a MessageConsumer from the Session to the Topic or Queue
            MessageConsumer consumer = session.createConsumer(destination);

            Message message = null;
            int char_count = 0;
            
            // Wait for a message
            while(true){
            	message = consumer.receive();        
            	if (message instanceof TextMessage) {
            		TextMessage textMessage = (TextMessage) message;
            		String text = textMessage.getText();
            		System.out.println("Received: " + text);
            		if(text != null)
            			char_count = char_count + text.length();
            		else
            			break;     
            	} 
            }
            consumer.close();
            
            //send the response
            Destination response_destination = session.createQueue("resp_queue");
            MessageProducer producer = session.createProducer(response_destination);
            String text = Integer.toString(char_count);
            TextMessage response_message = session.createTextMessage(text);
            producer.send(response_message);
            
            producer.close();
            session.close();
            connection.close();
        } catch (Exception e) {
            System.out.println("Caught: " + e);
            e.printStackTrace();
        }
	}

	@Override
	public void onException(JMSException exception) {
		System.out.println("Received exception");
		
	}
	
	public static void main(String args[]){
		JMSConsumer consumer = new JMSConsumer();
		consumer.startConsumer("queue_5_3");
	}
}
