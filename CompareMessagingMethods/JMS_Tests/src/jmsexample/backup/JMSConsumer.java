package jmsexample;

import org.apache.activemq.ActiveMQConnectionFactory;

import javax.jms.*;

public class JMSConsumer implements ExceptionListener{
	public void startConsumer(){
		try {
			 
            // Create a ConnectionFactory
            //ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory("vm://localhost");
			ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory();
            // Create a Connection
            Connection connection = connectionFactory.createConnection();
            connection.start();

            connection.setExceptionListener(this);

            // Create a Session
            Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);

            // Create the destination (Topic or Queue)
            Destination destination = session.createQueue("test.queue");

            // Create a MessageConsumer from the Session to the Topic or Queue
            MessageConsumer consumer = session.createConsumer(destination);

            
            // Wait for a message
            while(true){
            	Message message = consumer.receive();

            	if (message instanceof TextMessage) {
            		TextMessage textMessage = (TextMessage) message;
            		String text = textMessage.getText();
            		System.out.println("Received: " + text);
            	} else {
            		System.out.println("Received: " + message);
            	}
            }
            //consumer.close();
            //session.close();
            //connection.close();
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
		consumer.startConsumer();
	}
}
