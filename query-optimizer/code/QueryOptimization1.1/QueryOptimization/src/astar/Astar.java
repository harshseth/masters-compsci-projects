package astar;
import java.util.Iterator;
import java.util.List;
import java.util.SortedSet;
import java.util.TreeSet;

import plan.Plan;

public class Astar 
{	public static SortedSet<Plan> closedSet = new TreeSet<Plan>();
	public static SortedSet<Plan> openSet = new TreeSet<Plan>();
	
	
	public static void AstarSearch(Plan initialPlan)
	{
		//declaring the open and the closed set for the algorithm
		Iterator<Plan> openItr = openSet.iterator();	
		boolean tentativeBetter;
		Plan currentPlan;
		List<Plan> neighborPlan;
		Plan y, rep;
		Iterator<Plan> neighborItr = neighborPlan.iterator();
		//adding the initial plan to the open list
		openSet.add(initialPlan);
		
		while(true)
		{
			openItr = openSet.iterator();
			
			//gets the plan with lowest f_score value among all the plans
			if(openItr.hasNext() == true)
				currentPlan = openItr.next();
			else
				break;
			
			if(currentPlan.isFullPlan() == true)
			{
				//get the path code... still to write
			}
			
			openItr.remove();
			closedSet.add(currentPlan);
			
			//generate the next level plans from the current plan
			neighborPlan = currentPlan.getNextPlans();
			
			while(neighborItr.hasNext() == true)
			{
				y = neighborItr.next();
				if(closedSet.contains(y) == true)
					continue;
				tentativeBetter = false;
				
				while(openItr.hasNext())
				{
					rep = openItr.next();
					if(rep.equals(y) == true)
					{
						if(rep.getCost() > y.getCost())
						{
							openItr.remove();
							openSet.add(y);
							tentativeBetter = true;
						}
					}
				}
				
				if(tentativeBetter == false)
				{
					openSet.add(y);
					tentativeBetter = true;
				}
								
			}
		}//end of while loop
		
	}//end of function AstarSearch
	
}//end of Astar class


