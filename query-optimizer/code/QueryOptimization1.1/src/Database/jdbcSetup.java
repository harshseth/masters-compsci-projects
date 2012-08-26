package Database;


import java.sql.*;

public class jdbcSetup{

static Connection Conn;
static Statement stmt;
static String url;

public jdbcSetup() { };

static {
		try{
		Class.forName("com.mysql.jdbc.Driver").newInstance();
		}
			 catch (InstantiationException e) {
				e.printStackTrace();
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			} catch (IllegalAccessException e) {
				e.printStackTrace();
			}
try{
		
	String url = "jdbc:mysql://localhost:3306/test";
	Connection conn = DriverManager.getConnection(url,"root","root");
	stmt = conn.createStatement();
	}
			catch(Exception e){
			System.out.println("Cannot open Connection");
			System.exit(0);
			}		
}

public static Statement getConnection(){
	return stmt;

}
}
