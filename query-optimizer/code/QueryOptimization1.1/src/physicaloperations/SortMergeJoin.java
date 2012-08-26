package physicaloperations;

import plan.CompositeJoinPlan;
import plan.Plan;

public class SortMergeJoin extends Operator {

	public long getCost(Plan p)
	{
		double cost = 0;
//		int innerCard, outerCard;
//		CompositeJoinPlan cjp = (CompositeJoinPlan)p;
//		double cost  = 0;
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
		
		
//		innerCard = cjp.getInner().getCardinality();
//		outerCard = cjp.getOuter().getCardinality();
//		
//		int noOfPagesPlan1 = innerCard/ PAGESIZE;
//		int noOfPagesPlan2 = outerCard/ PAGESIZE;
//
//		double IOcost =  (noOfPagesPlan1)*Math.log(noOfPagesPlan1) +  (noOfPagesPlan1)*Math.log(noOfPagesPlan1) + noOfPagesPlan1 + noOfPagesPlan2;
//		double CPUcost = innerCard + outerCard;
		
		double sortingCPU = innerCard*Math.log10(1+innerCard)/Math.log10(2)+ outerCard*Math.log10(1+outerCard)/Math.log10(2); 
		double sortingIO =  num_pages_inner*Math.log10(1+num_pages_inner)/Math.log10(BUFFERSIZE)+num_pages_outer*Math.log10(1+num_pages_outer)/Math.log10(BUFFERSIZE);
		
		cost = IOcost+sortingIO + CPUcost+sortingCPU;
		//System.out.println("sort"+cost);
		return (long)cost;
	}

}
