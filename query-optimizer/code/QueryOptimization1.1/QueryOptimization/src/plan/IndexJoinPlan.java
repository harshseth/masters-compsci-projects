package plan;

import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import physicaloperations.Operator;

import relation.Column;
import relation.RelationNode;

public class IndexJoinPlan extends Plan {

	//variable declaration
	Plan p;
	Operator op;
	RelationNode relation;
	Set<Column> joinColumn;
	Column indexColumn;
	//end of variable declaration
	
	public IndexJoinPlan(TreeSet<RelationNode> rels, TreeSet<Column> cols,Column sortedCol, Plan p, RelationNode rel, Set<Column> joinCol, Column indexCol)
	{
		super(rels,cols,sortedCol);
		this.p = p;
		op = Operator.INDEXJOIN;
		relation = rel;
		joinColumn = joinCol;
		indexColumn = indexCol;
	}
	
	@Override
	public List<Plan> getNextPlans() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public int compareTo(Plan o) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public int getCost() {
		// TODO Auto-generated method stub
		return 0;
	}

	public static Collection<? extends Plan> getIndexedJoinPlan(Plan plan, RelationNode rN) 
	{
		// TODO Auto-generated method stub
		TreeSet<RelationNode> relation = new TreeSet<RelationNode>();
		relation.addAll(plan.relations);
		TreeSet<Column> finalColSet = new TreeSet<Column>(plan.getColumns());
		
		TreeSet<Column> relationCols = new TreeSet<Column>(Arrays.asList(rN.getColumns()));
		TreeSet<Column> intersectionCols = new TreeSet<Column>(finalColSet);
		intersectionCols.retainAll(relationCols);
		
		List<Plan> planList = new LinkedList<Plan>();
		Plan p;
		
		if(!intersectionCols.isEmpty())
		{
			finalColSet.addAll(relationCols);
			relation.add(rN);
			for (Iterator iterator = intersectionCols.iterator(); iterator.hasNext();) 
			{
				Column column = (Column) iterator.next();
				p = new IndexJoinPlan(relation, finalColSet, plan.sortedColumn, plan, rN, intersectionCols, column);
				addPlanToListifGood(p, planList);
			}
			return planList;
		}
		return null;
	}
	

}
