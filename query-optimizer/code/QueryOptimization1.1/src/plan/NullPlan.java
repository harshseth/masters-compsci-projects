package plan;

import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import optimizer.QueryOptimizer;
import relation.RelationNode;

public class NullPlan extends Plan {

	@Override
	public List<Plan> getNextPlans() {
		// TODO Auto-generated method stub
		List<Plan> planList=new LinkedList<Plan>();
		Iterator<RelationNode> relationIter=QueryOptimizer.getRelationList().iterator();
		while(relationIter.hasNext())
		{	
			
			addAllPlans(ScanPlan.generatePlanFromRelation(relationIter.next()), planList);		
		}
		return planList;
	}

	@Override
	public int compareTo(Plan o) {
		// TODO Auto-generated method stub
		return 0;
	}

	

}
