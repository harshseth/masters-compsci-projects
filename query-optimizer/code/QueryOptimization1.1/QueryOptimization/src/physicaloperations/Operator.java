package physicaloperations;

import plan.Plan;


public class Operator {
	public static Operator INDEXSCAN=new ScanOperator();
	public static Operator MERGEJOIN =new MergeJoin();
	public static Operator INDEXJOIN =new IndexJoin();
	public static Operator SORTMERGEJOIN=new SortMergeJoin();
	public static Operator NESTEDLOOPJOIN=new NestedLoopJoin();
	public int getCost(Plan plan)
	{
		return 1;
	}

}
