import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Name{
	public static void main(String[] args) {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in)); 
		String str = null; 
		System.out.println("What's your name? "); 
        		try {
			str = br.readLine();
			System.out.println("Hallo " + str + "!");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
}
