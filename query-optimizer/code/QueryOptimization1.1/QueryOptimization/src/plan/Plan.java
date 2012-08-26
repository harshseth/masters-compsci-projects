package plan;

import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import optimizer.QueryOptimizer;
import relation.Column;
import relation.RelationNode;


public abstract class Plan implements Comparable<Plan> {
	
	int cardinality;
	Column sortedColumn= null;
	TreeSet<RelationNode> relations=new TreeSet<RelationNode>();
	TreeSet<Column> columns=new TreeSet<Column>();
	int cost;
	public Plan()
	{}
	public Plan(TreeSet<RelationNode> rels, TreeSet<Column> cols,Column sortedCol) {
		// TODO Auto-generated constructor stub
		relations=rels;
		columns=cols;
		sortedColumn=sortedCol;
	}
	public boolean isFullPlan()
	{		return QueryOptimizer.containsAllRelations(relations);
	}
	public List<Plan> getNextPlans()
	{	List <Plan> planList= new LinkedList<Plan>();
		planList.addAll(getIndexJoinPlans());
		planList.addAll(CompositeJoinPlan.getCompositePlan(this));
		return planList;
		
	}
	private Collection<? extends Plan> getIndexJoinPlans() {
		Set<RelationNode> complementRelationSet = new TreeSet<RelationNode>(QueryOptimizer.getRelationList());
		complementRelationSet.removeAll(relations);
		Iterator <RelationNode> rnIter = complementRelationSet.iterator();
		List<Plan> planList=new LinkedList<Plan>();
		while(rnIter.hasNext())
		{	RelationNode rN=rnIter.next();
			planList.addAll(IndexJoinPlan.getIndexedJoinPlan(this,rN));
		}
		return planList;
	}
	public int getCost()
	{
		return cost;
	}
	
	public TreeSet<RelationNode> getRelations() {
		return relations;
	}
	public TreeSet<Column> getColumns() {
		return columns;
	}
	
	
	public void generateCost()
	{
		cost=1;
	}
	
	public static void addPlanToListifGood(Plan plan, List<Plan> planList) {
		// TODO Auto-generated method stub
		
	}
}
