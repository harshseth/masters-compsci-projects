package physicaloperations;

import plan.Plan;

public class ScanOperator extends Operator {
	static int fanout = 200;
	public long getCost(Plan p)
	{   
		long costIO = 1+p.getCardinality()/(PAGESIZE/(p.getColumns().size()*SIZE_OF_VARCHAR));
		//System.out.println("scan "+costIO + p.getRelations()+p.getCardinality()+PAGESIZE/(p.getColumns().size()*SIZE_OF_VARCHAR));
		return costIO;
	}

}
