package physicaloperations;

import plan.CompositeJoinPlan;
import plan.Plan;

public class MergeJoin extends Operator {
	public long getCost(Plan p)
	{
		double cost  = 0;
		double innerCard, outerCard;
		CompositeJoinPlan cjp = (CompositeJoinPlan)p;
		
		innerCard = cjp.getInner().getCardinality();
		outerCard = cjp.getOuter().getCardinality();
		double size_of_tuple_inner = cjp.getInner().getColumns().size()*SIZE_OF_VARCHAR;
		double num_pages_inner= 1+innerCard/(PAGESIZE/size_of_tuple_inner);
		double size_of_tuple_outer = cjp.getOuter().getColumns().size()*SIZE_OF_VARCHAR;
		double num_pages_outer= 1+outerCard/(PAGESIZE/size_of_tuple_outer);
		double IOcost =  num_pages_inner + num_pages_outer ;
		double CPUcost = innerCard + outerCard;
		
		cost = IOcost + CPUcost;		
		//System.out.println(p.toString() + " hi"+cost);
		return (long)cost;
	}

	
}
