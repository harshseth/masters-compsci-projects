package plan;


import java.util.Collection;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import physicaloperations.Operator;
import relation.Column;
import relation.RelationNode;
import astar.Astar;

public class CompositeJoinPlan extends Plan {
	Plan inner;
	Operator operator;
	Plan outer;
	Set<Column> joinColumn;
	
	public CompositeJoinPlan()
	{}
	public CompositeJoinPlan(Plan p1, Plan p2, Operator op, Set<Column> joinCol,Set<RelationNode> rNSet,Set<Column> cols,Column sortedCol)
	{	super((TreeSet<RelationNode>)rNSet,(TreeSet<Column>)cols,sortedCol);
		inner=p1;
		outer=p2;
		operator=op;
		cost= p1.cost+p2.cost+operator.getCost(this);
		
	}
	@Override
	public List<Plan> getNextPlans() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean equals(Object p) {
		// TODO Auto-generated method stub
		Plan plan = (Plan) p;
		return relations.equals(plan.relations);
	}

	@Override
	public int compareTo(Plan plan) {
		// TODO Auto-generated method stub
		return cost -((Plan) plan).cost;
	}
	private int cost()
	{//TODO
		return 0;
	}

	@Override
	
	public boolean isFullPlan() {
		// TODO Auto-generated method stub
		return false;
	}
	@Override
	public int getCost() {
		// TODO Auto-generated method stub
		return 0;
	}
	public static Collection<? extends Plan> getCompositePlan(Plan plan) {
		// TODO Auto-generated method stub
		Iterator<Plan> planIter= Astar.openSet.iterator();
		List<Plan> planList= new LinkedList<Plan>();
		while(planIter.hasNext())
		{
			List <Plan> compositePlans= joinTwoPlans(plan , planIter.next());
			if(compositePlans!=null)
			{ Iterator<Plan> planIter2= compositePlans.iterator();
				while(planIter2.hasNext())
				{
					addPlanToListifGood(planIter2.next(), planList);
				}
			}
		}
		return planList;
		
		
	}
	private static List<Plan> joinTwoPlans(Plan plan1, Plan plan2) {
		// TODO Auto-generated method stub
		Set<RelationNode> interRNSet=new TreeSet<RelationNode>(plan1.relations); 
		interRNSet.retainAll(plan2.relations);
		if(interRNSet.isEmpty())
		{	TreeSet<RelationNode> unionRNSet=new TreeSet<RelationNode>(plan1.relations);
			unionRNSet.addAll(plan2.relations);
			TreeSet<Column> intersectionCols = new TreeSet<Column>(plan1.getColumns());
			TreeSet<Column> unionCols;
			intersectionCols.retainAll(plan2.getColumns());
			if(!intersectionCols.isEmpty())
			{	unionCols= new TreeSet<Column>(plan1.getColumns());
				unionCols.addAll(plan2.getColumns());
				return applyPossibleOperator(plan1,plan2,intersectionCols,unionRNSet,unionCols);
					
			
			}
			return null;
		}
			return null;
	}
	private static List<Plan> applyPossibleOperator(Plan plan1, Plan plan2,TreeSet<Column> intersectionCols, TreeSet<RelationNode> unionRNSet, TreeSet<Column> unionCols) {
		// TODO Auto-generated method stub
		List <Plan> planList = new LinkedList<Plan>();
		Plan plan = new CompositeJoinPlan(plan1, plan2, Operator.NESTEDLOOPJOIN, intersectionCols,unionRNSet,unionCols,plan1.sortedColumn);
		planList.add(plan);
		plan = new CompositeJoinPlan(plan2, plan1, Operator.NESTEDLOOPJOIN, intersectionCols,unionRNSet,unionCols,plan2.sortedColumn);
		addPlanToListifGood(plan,planList);
		
		
		if(plan1.sortedColumn.equals(plan2.sortedColumn))
		{	
			plan=new CompositeJoinPlan(plan1, plan2, Operator.MERGEJOIN, intersectionCols,unionRNSet,unionCols,plan1.sortedColumn);
			addPlanToListifGood(plan, planList);
			plan=new CompositeJoinPlan(plan2, plan1, Operator.MERGEJOIN, intersectionCols,unionRNSet,unionCols,plan1.sortedColumn);
			addPlanToListifGood(plan, planList);
		}
		Iterator<Column> colIter = intersectionCols.iterator();
		while(colIter.hasNext())
		{
			Column col=colIter.next();
			if(col.equals(plan1.sortedColumn)&&col.equals(plan2.sortedColumn))
				continue;
			plan=new CompositeJoinPlan(plan1, plan2, Operator.SORTMERGEJOIN, intersectionCols,unionRNSet,unionCols,col);
			addPlanToListifGood(plan, planList);
			plan=new CompositeJoinPlan(plan2, plan1, Operator.SORTMERGEJOIN, intersectionCols,unionRNSet,unionCols,col);
			addPlanToListifGood(plan, planList);
		}
		
		return planList;
	}
	
	
	
}
