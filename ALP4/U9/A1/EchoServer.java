package assignments;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.ServerSocket;
import java.net.Socket;

public class EchoServer {
	public static void main(String args[]) throws IOException {
		int PORT = 12345; 
		ServerSocket listen = new ServerSocket(PORT); // set port

		while (true) { // non-terminating server

			Socket socket = listen.accept();
			//BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

			PrintStream out = new PrintStream(socket.getOutputStream());

			out.println(new String("Hello World."));
			
			//in.close();
			out.close();
			socket.close(); // close connection
			System.out.println("Socket closed.");
		}
	}
}
