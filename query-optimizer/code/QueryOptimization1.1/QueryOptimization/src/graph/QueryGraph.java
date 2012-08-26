package graph;

import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import relation.RelationNode;

public class QueryGraph {
	public static QueryGraph graph;
	boolean[][]matrix;
	public QueryGraph(List<RelationNode> relations)
	{
		matrix = new boolean [relations.size()][relations.size()];
		for(int i=0;i<relations.size();i++)
		{	
			for(int k=i+1;k<relations.size();k++)
			{	int idi=relations.get(i).getID();
				int idk=relations.get(k).getID();
				matrix[idi][idk]=matrix[idk][idi]=relations.get(i).isJoinable(relations.get(k));
			}
		}
	}
	public boolean isJoinable(int i, int j)
	{
		return matrix[i][j];
	}
	public Set<Integer> getJoinableRelation(Set<RelationNode> rNS) {
		// TODO Auto-generated method stub
		Set<Integer> sRID = new HashSet<Integer>();
		Set<Integer> joinableRID = new HashSet<Integer>();
		Iterator<RelationNode> r=rNS.iterator();
		while(r.hasNext())
		{
			sRID.add(r.next().getID());
		}
		Iterator<Integer> iterID=sRID.iterator();
		
		while(iterID.hasNext())
		{	int index = iterID.next();
			for(int i=0;i<matrix.length;i++)
			{
				if(matrix[index][i])
					joinableRID.add(i);
			}
		}
		joinableRID.removeAll(sRID);
		return joinableRID;
	}
	
}
