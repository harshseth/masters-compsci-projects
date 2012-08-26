package physicaloperations;

import plan.CompositeJoinPlan;
import plan.Plan;

public class NestedLoopJoin extends Operator {
	public long getCost(Plan p)
	{
		double cost = 0;
		double innerCard, outerCard;
		CompositeJoinPlan cjp = (CompositeJoinPlan)p;
		
		innerCard = cjp.getInner().getCardinality();
		outerCard = cjp.getOuter().getCardinality();
		//System.out.println("nested "+innerCard +" "+ outerCard);
		double size_of_tuple_inner = cjp.getInner().getColumns().size()*SIZE_OF_VARCHAR;
		double num_pages_inner= 1+ innerCard/(PAGESIZE/size_of_tuple_inner);
		double size_of_tuple_outer = cjp.getOuter().getColumns().size()*SIZE_OF_VARCHAR;
		double num_pages_outer= 1+outerCard/(PAGESIZE/size_of_tuple_outer);
		double IOcost =  (num_pages_outer) +  (outerCard)*(num_pages_inner);
		double CPUcost = innerCard * outerCard;
		
		cost = IOcost + CPUcost;
		//System.out.println(cjp.toString() + " "+cost);
		return (long)cost;
	}

}
