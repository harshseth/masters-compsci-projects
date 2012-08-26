package plan;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import optimizer.QueryOptimizer;
import relation.Column;
import relation.RelationNode;
import Database.jdbcSetup;
import RelationalQueryGenerator.planCard;


public abstract class Plan implements Comparable<Plan> {
	
	int cardinality;
	Column sortedColumn= null;
	TreeSet<RelationNode> relations=new TreeSet<RelationNode>();
	TreeSet<Column> columns=new TreeSet<Column>();
	long cost;
	int estimatedCost;
	public Plan()
	{}
	public Plan(TreeSet<RelationNode> rels, TreeSet<Column> cols,Column sortedCol) {
		// TODO Auto-generated constructor stub
		relations=rels;
		columns=cols;
		sortedColumn=sortedCol;
		
	}
	
	public int getEstimatedCost()
	{
		return 100*(QueryOptimizer.getRelationList().size()-relations.size());
		
	}
	public boolean isFullPlan()
	{		return QueryOptimizer.containsAllRelations(relations);
	}
	public List<Plan> getNextPlans()
	{	List <Plan> planList= new LinkedList<Plan>();
		addAllPlans(CompositeJoinPlan.getCompositePlan(this),planList);
		addAllPlans(getIndexJoinPlans(), planList);
		return planList;
		
	}
	private Collection<? extends Plan> getIndexJoinPlans() {
		Set<RelationNode> complementRelationSet = new TreeSet<RelationNode>(QueryOptimizer.getRelationList());
		complementRelationSet.removeAll(relations);
		Iterator <RelationNode> rnIter = complementRelationSet.iterator();
		List<Plan> planList=new LinkedList<Plan>();
		while(rnIter.hasNext())
		{	RelationNode rN=rnIter.next();
			addAllPlans(IndexJoinPlan.getIndexedJoinPlan(this,rN),planList);
		}
		return planList;
	}
	
	public int generateCardinality()
	{
		int cardinality = 0; 
		
		String sql1 = "Select Cardinality from "+planCard.getRelCard()+" where Plan = '"+this.getPlanString()+"'";
		try
		{
			ResultSet rs = jdbcSetup.getConnection().executeQuery(sql1);
			rs.next();
			cardinality = rs.getInt(1);
		}
		catch(SQLException e)
		{	//System.out.println("gencard");
		e.printStackTrace();
		}
		return cardinality;	
	}
	
	public String getPlanString()
	{
		String str = "q1";
		Iterator<RelationNode> it = this.relations.iterator();
		while(it.hasNext())
		{
			int id = it.next().getID();
			str = str  + id; 
		}
		return str;
	}
	public static void addAllPlans(Collection<? extends Plan> plansToAdd,
			List<Plan> planList) {
		// TODO Auto-generated method stub
		
		Iterator <? extends Plan> planIter = plansToAdd.iterator();
		while(planIter.hasNext())
		{
			addPlanToSortedListIfNotInferior(planIter.next(), planList);
		}
	}
	
	
	public TreeSet<RelationNode> getRelations() {
		return relations;
	}
	
	public TreeSet<Column> getColumns() {
		return columns;
	}
	
	
	public static void addPlanToListifGood(Plan plan, List<Plan> planList) {
		// TODO Auto-generated method stub
		addPlanToSortedListIfNotInferior(plan,planList);
	}
	
	public static void addPlanToSortedListIfNotInferior(Plan newPlan,
			List<Plan> sortedList) {
		//System.out.println(sortedList);
		if (newPlan==null)
			return;
		//sortedList.add(newPlan);
		int insertposition = sortedList.size();
		boolean positionComputed = false;
		Plan planInList=null;
		Set <RelationNode> rNSetNewPlan = newPlan.getRelations();
		for(int i=0;i<sortedList.size();i++)
		{	planInList=sortedList.get(i);
			Set <RelationNode> rNSetPlanInList = planInList.getRelations();
			if(newPlan.getScore() < planInList.getScore())
			{	if(!positionComputed)
				{	insertposition = i;
					positionComputed=true;
				}
					
//				if(rNSetNewPlan.containsAll(rNSetPlanInList))
//				{	
//					if(planInList.sortedColumn==null||planInList.sortedColumn.equals(newPlan.sortedColumn))
//					{
//						sortedList.remove(i);
//						i--;
//						continue;
//					}
//				}
			}
			else
			{	
//				if(rNSetPlanInList.containsAll(rNSetNewPlan))
//				{	
//						if(newPlan.sortedColumn==null||newPlan.sortedColumn.equals(planInList.sortedColumn))
//						{
//							return;
//						}
//					
//				}	
			}
			
		}
		sortedList.add(insertposition, newPlan); 
		// TODO Auto-generated method stub
		
	}
	
	public long getScore() {
		// TODO Auto-generated method stub
		return cost + estimatedCost;
	}
	@Override
	public int compareTo(Plan plan) {
		// TODO Auto-generated method stub
		return (int)(cost + estimatedCost -((Plan) plan).cost - ((Plan)plan).estimatedCost);
	}
	public int getCardinality()
	{
		return cardinality;
	}

		
}
