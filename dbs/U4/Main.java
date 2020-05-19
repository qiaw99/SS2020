import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

public class Main {

    public static void main(String[] args) {

        Connection conn = null;
        Statement statement = null;

        try {
            // make the connection
            conn = DriverManager.getConnection("jdbc:postgresql://localhost:5432/testdb", "testuser", "password");
            System.out.println("Opened database successfully");

            // statements execute queries
            statement = conn.createStatement();
            ResultSet results = statement.executeQuery("SELECT * FROM People WHERE age = 42");

            // get the first result & unpack the data
            results.next();
            String content = results.getString("name");
            System.out.println(content);

            // close all active objects
            results.close();
            statement.close();
            conn.close();

        } catch (Exception e) {
            e.printStackTrace();
            System.err.println(e.getClass().getName() + ": " + e.getMessage());
            System.exit(0);
        }
    }
}
