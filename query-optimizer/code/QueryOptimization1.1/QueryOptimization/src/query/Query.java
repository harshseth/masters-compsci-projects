package query;

import java.util.Collection;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import relation.RelationNode;

public class Query {
	 TreeSet<RelationNode> relationSet=new TreeSet<RelationNode>();
	 public Query(int i)
	 {
		 
	 }
	 public boolean containsAllRelations(TreeSet<RelationNode> relations)
	 {
		return relationSet.containsAll(relations);
	 }
	 public void generateQuery(int i)
	 {
		 
	 }
	 public TreeSet<RelationNode> getRelationList()
	 {
		return relationSet; 
	 }
}
