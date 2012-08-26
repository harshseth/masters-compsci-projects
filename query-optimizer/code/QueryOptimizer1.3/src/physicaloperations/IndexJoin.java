package physicaloperations;

import plan.IndexJoinPlan;
import plan.Plan;

public class IndexJoin extends Operator 
{
	
	public long getCost(Plan p)
	{
		IndexJoinPlan ijp = (IndexJoinPlan)p;
		double cost = 0;
		//double size_of_tuple1 = ijp.getUnderlyingPlan().getColumns().size()*SIZE_OF_VARCHAR;
		double size_of_tuple2 = ijp.getRelation().getColumns().length*SIZE_OF_VARCHAR;
		double num_of_tup_per_page2=1+ PAGESIZE/size_of_tuple2;
		//System.out.println("index"+num_of_tup_per_page2);
		double innerCard= ijp.getUnderlyingPlan().getCardinality();
				
		//double noOfPagesPlan1 = innerCard/ PAGESIZE;
		
		
		double IOcost =  innerCard/num_of_tup_per_page2;
		double CPUcost = innerCard;
		cost = IOcost + CPUcost;
		//System.out.println("inde"+cost);
		return (long)cost;
	}

	
}
