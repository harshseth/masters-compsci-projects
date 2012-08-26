package RelationalQueryGenerator;

//
//  planCard.java
//  Created by Konidena, Bhargava Deepak on 4/22/09.
//

/* importing util, relation and plan */
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Set;
import java.util.TreeSet;

import plan.CompositeJoinPlan;
import plan.IndexJoinPlan;
import plan.NullPlan;
import plan.Plan;
import plan.ScanPlan;
import relation.Column;
import relation.RelationNode;
import Database.jdbcSetup;


public class planCard {
	
	static String tripleStore = "spo";
	static String relCard = "planCard";
	static Statement stmt ;
	
	//constructor
	static{
					//Creating the table with Hardcoded cardinalities
				String sql0 = "CREATE TABLE "+ relCard +" ( Plan VARCHAR(30), Cardinality INTEGER);";
				//System.out.println(sql0);
				stmt = jdbcSetup.getConnection();
				//System.out.println(stmt);
				try
				{	
					 stmt.executeUpdate(sql0);
				
				}catch(Exception e)
				{System.out.println("sql error in the static initializer");
				e.printStackTrace();}
				
	}
	
	public static void  genCardinality(Plan planObj) {
		
				
			//If the next plan is a comp join plan call this method
			// downcasting ??
			try
			{
				if(planObj instanceof CompositeJoinPlan)
			
				InsertCardCJP((CompositeJoinPlan) planObj);
			//If the next plan is a index join plan call this method
			if(planObj instanceof IndexJoinPlan)
				InsertCardIJP((IndexJoinPlan)planObj);
			//If the next plan is a comp scan plan call this method
			if(planObj instanceof ScanPlan)
				InsertCardSP((ScanPlan) planObj);
			//If the next plan is a null plan do nothing
			if(planObj instanceof NullPlan) {};
			}catch(SQLException sqe)
			{	sqe.printStackTrace();
				System.out.println("sqlerror");
			}
			
	
		
				
			
	}
	
	//getter functions to retrieve the relation name
	public static String getRelCard()
	{
		return relCard;
	}
	
		
	public static void InsertCardCJP(CompositeJoinPlan planCJP) throws SQLException{
		
	//	Retrieve the relation nodes of Inner and Outer Plans
	
			TreeSet<RelationNode> rNSet = planCJP.getRelations();
		
		String newPlanTableName = getRelationString(rNSet);
		if(doesTableExists(newPlanTableName))
			return;
		
		
		
		String strInner = getRelationString(planCJP.getInner().getRelations());
		
		String strOuter = getRelationString(planCJP.getOuter().getRelations());
		Set<Column> outerCol = new TreeSet<Column>(planCJP.getOuter().getColumns());
		outerCol.removeAll(planCJP.getJoinColumn());
		String strAsPart1 , strWherePart ,strAsPart2;
		 
		strAsPart1 = getJoinQuerySelectPart(strInner, planCJP.getInner().getColumns());
		strWherePart = getJoinQueryWherePart(strOuter,strInner,planCJP.getJoinColumn());
		strAsPart2 = getJoinQuerySelectPart(strOuter, outerCol);
		StringBuilder sqlQuery = new StringBuilder("Create table "+ newPlanTableName+ " As ");
			sqlQuery.append(" Select "+strAsPart1+" ");
			if(!strAsPart2.equals(" "))
				sqlQuery.append(","+strAsPart2);
			sqlQuery.append(" from " + strOuter+" , " +strInner+" Where "+ strWherePart);
		
			//System.out.println("composite "+sqlQuery.toString());
			stmt.executeUpdate(sqlQuery.toString());
			//cardinality of the table
			sqlQuery=new StringBuilder("Select count(*) as count from " + newPlanTableName );
			ResultSet rs4 = stmt.executeQuery(sqlQuery.toString());
			rs4.next();
		     int count = rs4.getInt("count");
		     			
			//Inserting the Cardinality into the table
			sqlQuery = new StringBuilder("INSERT INTO "+ relCard+" values ('"+newPlanTableName+"','"+count+"')");
			stmt.executeUpdate(sqlQuery.toString());	
		
	}
	
	/**
	 * @param planSP
	 * @throws SQLException
	 */
public static void InsertCardSP(ScanPlan planSP) throws SQLException{
		Set<RelationNode>  rNSet = planSP.getRelations();
		Set<Column>  columnSet = planSP.getColumns();
		
		StringBuilder sqlQuery = new StringBuilder("");
		String newRelStr=getRelationString(rNSet);
		//check if table already present
		if(doesTableExists(newRelStr))
			return;
		//Performing the join and calculating the cardinality
		
		sqlQuery.append("CREATE TABLE " + newRelStr +" AS ");
		sqlQuery.append("SELECT "+getSelectTableColumnRenameString(columnSet) +" FROM "+ tripleStore +" WHERE "+ getSelectTableColumnEqualValueString(columnSet));
    	//System.out.println(sqlQuery.toString());
		//System.out.println(sqlQuery);
		stmt.executeUpdate(sqlQuery.toString());
		//cardinality of the table
		sqlQuery=new StringBuilder("Select count(*) as count from " + newRelStr );
		ResultSet rs4 = stmt.executeQuery(sqlQuery.toString());
		rs4.next();
	     int count = rs4.getInt("count");
	     			
		//Inserting the Cardinality into the table
	     sqlQuery=new StringBuilder( "INSERT INTO "+ relCard+" values ('"+newRelStr+"','"+count+"')");
		stmt.executeUpdate(sqlQuery.toString());	
	}
	


	public static void InsertCardIJP(IndexJoinPlan planIJP) throws SQLException {
		
	//	Retrieve the relation nodes of Inner and Outer Plans
	
		TreeSet<RelationNode> rNSet = planIJP.getRelations();
		
		String newPlanTableName = getRelationString(rNSet);
		if(doesTableExists(newPlanTableName))
			return;
		
		
		RelationNode rN= planIJP.getRelation();
		Set<RelationNode> setSingleRN = new TreeSet <RelationNode>();
		setSingleRN.add(rN);
		String strSingleRN = getRelationString(setSingleRN);
		
		String strPlan = getRelationString(planIJP.getUnderlyingPlan().getRelations());
		String strAsPart1 , strAsPart2;
		
		
		Set<Column> setRelCol = new TreeSet<Column>(Arrays.asList(rN.getColumns()));
		setRelCol.removeAll(planIJP.getJoinColumn());
		if(!doesTableExists(strSingleRN))
		{	strSingleRN=tripleStore;
			Set<Column> setjoinCol = new TreeSet<Column>(Arrays.asList(rN.getColumns()));
			setjoinCol.retainAll(planIJP.getJoinColumn());
			strAsPart1 = getJoinQuerySelectPartWhenUnbound(strSingleRN, setRelCol);
			
			strAsPart2 = getJoinQueryWherePartWhenUnbound(strPlan,strSingleRN,setjoinCol);
		}
		else
		{
			strAsPart1 = getJoinQuerySelectPart(strSingleRN,setRelCol);
			strAsPart2 = getJoinQueryWherePart(strPlan,strSingleRN,planIJP.getJoinColumn());
		}
		StringBuilder sqlQuery = new StringBuilder("Create table "+ newPlanTableName+ " As ");
			sqlQuery.append(" Select "+getJoinQuerySelectPart(strPlan, planIJP.getUnderlyingPlan().getColumns())+" ");
			if(!strAsPart1.equals(" "))
			{	System.out.println("debug" +strAsPart1+"h");
				sqlQuery.append(","+strAsPart1);}
			sqlQuery.append(" from " + strPlan+" , " +strSingleRN+" Where "+ strAsPart2);
			//System.out.println(sqlQuery.toString());
			
			stmt.executeUpdate(sqlQuery.toString());
			//cardinality of the table
			sqlQuery=new StringBuilder("Select count(*) as count from " + newPlanTableName );
			ResultSet rs4 = stmt.executeQuery(sqlQuery.toString());
			rs4.next();
		     int count = rs4.getInt("count");
		     			
			//Inserting the Cardinality into the table
			sqlQuery = new StringBuilder("INSERT INTO "+ relCard+" values ('"+newPlanTableName+"','"+count+"')");
			stmt.executeUpdate(sqlQuery.toString());	
			}
	
	
	private static String getJoinQueryWherePartWhenUnbound(String tableName1,
			String tableName2, Set<Column> joinColumn) {
		StringBuilder strB = new StringBuilder("");
		Iterator<Column> it = joinColumn.iterator();
		while(it.hasNext())
		{   Column col =it.next();
			String name = col.getName();
			String type =col.getType();
			strB.append(tableName1 +"."+name+" = "+ tableName2 +"."+type+" and ");
		}
		strB.delete(strB.length()-" and ".length(),strB.length());
		return strB.append(" ").toString();

		
	}

	private static String getJoinQuerySelectPartWhenUnbound(String tabName,
			Set<Column> colSet) {
		StringBuilder strB = new StringBuilder("");
		Iterator<Column> it = colSet.iterator();
		while(it.hasNext())
		{   Column col =it.next();
			String type = col.getType();
			String name =col.getName(); 
			strB.append(tabName +"."+type+ " as "+ name+",");
		}
		if(!strB.toString().equals(""))
			strB.delete(strB.length()-",".length(),strB.length());
		return strB.append(" ").toString();

		
	}

	private static String getJoinQuerySelectPart(String tabName,
			Set<Column> colSet) {
		// TODO Auto-generated method stub
		StringBuilder strB = new StringBuilder("");
		Iterator<Column> it = colSet.iterator();
		while(it.hasNext())
		{   Column col =it.next();
			String name = col.getName();
			strB.append(tabName +"."+name+",");
		}
		
		if(!strB.toString().equals(""))
			strB.delete(strB.length()-",".length(),strB.length());
		return strB.append(" ").toString();
	}

	private static String getJoinQueryWherePart(String tableName1,
			String tableName2, Set<Column> joinColumn) {
		// TODO Auto-generated method stub
		StringBuilder strB = new StringBuilder("");
		Iterator<Column> it = joinColumn.iterator();
		while(it.hasNext())
		{   Column col =it.next();
			String name = col.getName();
			strB.append(tableName1 +"."+name+" = "+ tableName2 +"."+name+" and ");
		}
		strB.delete(strB.length()-" and ".length(),strB.length());
		return strB.append(" ").toString();

	}

		
	

	public static String getSelectTableColumnRenameString(Set<Column> rNCol )
	{	
		StringBuilder strB = new StringBuilder("");
		Iterator<Column> it = rNCol.iterator();
		while(it.hasNext())
		{   Column col =it.next();
			String colType = col.getType();
			String name = col.getName();
			strB.append(colType +" As "+ name +",");
		}
		if(!strB.toString().equals(""))
			strB.delete(strB.length()-",".length(),strB.length());
		return strB.toString();

	}
	public static String getSelectTableColumnEqualValueString(Set<Column> rNCol )
	{	
		StringBuilder strB = new StringBuilder("");
		Iterator<Column> it = rNCol.iterator();
		while(it.hasNext())
		{   Column col =it.next();
			String colType = col.getType();
			String value = col.getValue();
			if(value != null)
				strB.append(colType +" = '"+ value +"' and ");
		}
		strB.delete(strB.length()-"and ".length(),strB.length());
		return strB.toString();

	}

	public static String getCreateTableColumnString(Set<Column> rNCol )
	{	String dataType = "VARCHAR(100)";
		StringBuilder strB = new StringBuilder("(");
		Iterator<Column> it = rNCol.iterator();
		while(it.hasNext())
		{
			String colName = it.next().getName();
			strB.append(colName +" "+ dataType +",");
		}
		strB.deleteCharAt(strB.length()-1);
		strB.append(')');
		
		return strB.toString();

	}
	public static String getRelationString(Set<RelationNode> rNSet )
	{
		String str = "q1";
		Iterator<RelationNode> it = rNSet.iterator();
		while(it.hasNext())
		{
			int id = it.next().getID();
			str = str  + id; 
		}
		return str;
	}
    public static boolean doesTableExists(String planStr)throws SQLException
    {
    	String sql2 = "SELECT * FROM "+relCard+" WHERE Plan = '"+planStr+"'";
		//System.out.println(sql2);
		ResultSet rs2 = stmt.executeQuery(sql2);
		//if the value is already present in the table return
		//rs2.next();
		rs2.last();
		//System.out.println(rs2.getRow());
		if (rs2.getRow() != 0)
		{	//System.out.println("exists");
				return true;
		}
		else
			return false;
    }
}