package optimizer;

import java.util.TreeSet;

import astar.Astar;

import plan.NullPlan;
import plan.Plan;
import query.Query;
import relation.RelationNode;

public class QueryOptimizer {
	static Query query;
	//static QueryGraph queryGraph;
	static Plan plan;
	public static void main(String[] str)
	{//System.out.println(new Integer(1).compareTo(new Integer(2)));
		   
	        

		query=new Query(5);
		//queryGraph=new QueryGraph(query.getRelationList());
		plan=new NullPlan();
		
		System.out.println("\n"+Astar.AstarSearch(plan));
		
	}
	
	public static boolean containsAllRelations(TreeSet <RelationNode> rNS)
	{
		return query.containsAllRelations(rNS);
	}

	public static TreeSet<RelationNode> getRelationList() {
		// TODO Auto-generated method stub
		return query.getRelationList();
	}
	
	
//	public static Set<RelationNode> getJoinableRelation(Set <RelationNode> rNS)
//	{	Set<RelationNode> joinableRNS=new HashSet<RelationNode>();
//		Set<Integer> sRID=queryGraph.getJoinableRelation(rNS);
//		Iterator <RelationNode> rNIter=query.getRelationList().iterator();
//		if(rNIter.hasNext())
//		{
//			if(sRID.contains(rNIter.next().getID()))
//				
//		}
//		
//	
//	}
	

}
