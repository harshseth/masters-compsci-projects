package astar;
import java.util.LinkedList;
import java.util.List;

import plan.Plan;

public class Astar 
{	public static List<Plan> openList = new LinkedList<Plan>();
	public static Plan AstarSearch(Plan initialPlan)
	{
		
		Plan currentPlan;
		//adding the initial plan to the open list
		openList.add(initialPlan);
		
		while(openList.size()>0)
		{	//System.out.println(openList);
			currentPlan=openList.remove(0);
			//System.out.println(openList);
			if(currentPlan.isFullPlan())
			{	//System.out.println(currentPlan);
				return currentPlan;
			}
			//gets the plan with lowest f_score value among all the plans
			List <Plan> newPlans = currentPlan.getNextPlans();
			
			for(Plan newPlan: newPlans)
			{	
				
					//System.out.println("\n"+newPlan.toString());
			
				Plan.addPlanToSortedListIfNotInferior(newPlan, openList);	
			}
		}//end of while loop
		return null;
	}//end of function AstarSearch
	
}//end of Astar class


