package plan;

import java.util.LinkedList;
import java.util.List;
import java.util.TreeSet;

import physicaloperations.Operator;
import relation.Column;
import relation.RelationNode;

public class ScanPlan extends Plan {

	Operator op;
	/* This index type holds the information about which index is to be used depending on the particular index value set*/
	private byte[] indexType;
	
	public ScanPlan(TreeSet<RelationNode> rels, TreeSet<Column> cols,Column sortedCol, byte[] indexType)
	{
		super(rels, cols, sortedCol);
		op = Operator.INDEXSCAN;
		this.indexType = indexType;
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
	
	//TODO
	public static ScanPlan  generateSPO(RelationNode rN)
	{
		Column[] cols = rN.getColumns();
		Column sortedCol;
		TreeSet<RelationNode> rNTree = new TreeSet<RelationNode>();
		rNTree.add(rN);
		
		TreeSet<Column> columnTree = new TreeSet<Column>();
		columnTree.add(cols[0]);
		columnTree.add(cols[1]);
		columnTree.add(cols[2]);
		
		if(cols[0].getValue()== null)
			return null;
		else
		{
			byte indexType[] = new byte[] {1, 2, 3};
			if(cols[1].getValue() != null)
				sortedCol = cols[2];
			else
				sortedCol = cols[1];
			return new ScanPlan(rNTree, columnTree, sortedCol, indexType);
		}
	}
	
	public static ScanPlan  generateSOP(RelationNode rN)
	{
		Column[] cols = rN.getColumns();
		Column sortedCol;
		TreeSet<RelationNode> rNTree = new TreeSet<RelationNode>();
		rNTree.add(rN);
		
		TreeSet<Column> columnTree = new TreeSet<Column>();
		columnTree.add(cols[0]);
		columnTree.add(cols[1]);
		columnTree.add(cols[2]);
		
		if(cols[0].getValue()== null)
			return null;
		else
		{
			byte indexType[] = new byte[] {1, 3, 2};
			if(cols[2].getValue() != null)
				sortedCol = cols[1];
			else
				sortedCol = cols[2];
			return new ScanPlan(rNTree, columnTree, sortedCol, indexType);
		}
	}
	
	public static ScanPlan generatePSO(RelationNode rN)
	{
		Column[] cols = rN.getColumns();
		Column sortedCol;
		TreeSet<RelationNode> rNTree = new TreeSet<RelationNode>();
		rNTree.add(rN);
		
		TreeSet<Column> columnTree = new TreeSet<Column>();
		columnTree.add(cols[0]);
		columnTree.add(cols[1]);
		columnTree.add(cols[2]);
		
		if(cols[1].getValue()== null)
			return null;
		else
		{
			byte indexType[] = new byte[] {2, 1, 3};
			if(cols[0].getValue() != null)
				sortedCol = cols[2];
			else
				sortedCol = cols[0];
			return new ScanPlan(rNTree, columnTree, sortedCol, indexType);
		}
	}
	
	public static ScanPlan  generatePOS(RelationNode rN)
	{
		Column[] cols = rN.getColumns();
		Column sortedCol;
		TreeSet<RelationNode> rNTree = new TreeSet<RelationNode>();
		rNTree.add(rN);
		
		TreeSet<Column> columnTree = new TreeSet<Column>();
		columnTree.add(cols[0]);
		columnTree.add(cols[1]);
		columnTree.add(cols[2]);
		
		if(cols[1].getValue()== null)
			return null;
		else
		{
			byte indexType[] = new byte[] {3, 1, 2};
			if(cols[2].getValue() != null)
				sortedCol = cols[0];
			else
				sortedCol = cols[2];
			return new ScanPlan(rNTree, columnTree, sortedCol, indexType);
		}
	}
	
	public static ScanPlan  generateOSP(RelationNode rN)
	{
		Column[] cols = rN.getColumns();
		Column sortedCol;
		TreeSet<RelationNode> rNTree = new TreeSet<RelationNode>();
		rNTree.add(rN);
		
		TreeSet<Column> columnTree = new TreeSet<Column>();
		columnTree.add(cols[0]);
		columnTree.add(cols[1]);
		columnTree.add(cols[2]);
		
		if(cols[2].getValue()== null)
			return null;
		else
		{
			byte indexType[] = new byte[] {2, 3, 1};
			if(cols[0].getValue() != null)
				sortedCol = cols[1];
			else
				sortedCol = cols[0];
			return new ScanPlan(rNTree, columnTree, sortedCol, indexType);
		}
	}
	
	public static ScanPlan  generateOPS(RelationNode rN)
	{
		Column[] cols = rN.getColumns();
		Column sortedCol;
		TreeSet<RelationNode> rNTree = new TreeSet<RelationNode>();
		rNTree.add(rN);
		
		TreeSet<Column> columnTree = new TreeSet<Column>();
		columnTree.add(cols[0]);
		columnTree.add(cols[1]);
		columnTree.add(cols[2]);
		
		if(cols[2].getValue()== null)
			return null;
		else
		{
			byte indexType[] = new byte[] {3, 2, 1};
			if(cols[1].getValue() != null)
				sortedCol = cols[0];
			else
				sortedCol = cols[1];
			return new ScanPlan(rNTree, columnTree, sortedCol, indexType);
		}
	}
	
	public static List<Plan> generatePlanFromRelation(RelationNode rN)
	{
		List<Plan> planList= new LinkedList<Plan>();
		
		addPlanToListifGood(generateSPO(rN),planList);
		addPlanToListifGood(generateSOP(rN),planList);
		addPlanToListifGood(generatePSO(rN),planList);
		addPlanToListifGood(generatePOS(rN),planList);
		addPlanToListifGood(generateOSP(rN),planList);
		addPlanToListifGood(generateOPS(rN),planList);
		
		return planList;
	}

	public byte[] getIndexType() {
		return indexType;
	}
}
