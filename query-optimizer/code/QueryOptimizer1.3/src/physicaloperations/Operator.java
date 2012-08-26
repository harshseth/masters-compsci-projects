package physicaloperations;

import plan.Plan;


public abstract class Operator {
	public static Operator INDEXSCAN=new ScanOperator();
	public static Operator MERGEJOIN =new MergeJoin();
	public static Operator INDEXJOIN =new IndexJoin();
	public static Operator SORTMERGEJOIN=new SortMergeJoin();
	public static Operator NESTEDLOOPJOIN=new NestedLoopJoin();
	public final static int SIZE_OF_VARCHAR = 75;
	public static final int PAGESIZE = 4096;
	public static final int BUFFERSIZE = 10;
	public abstract long getCost(Plan p);

}
