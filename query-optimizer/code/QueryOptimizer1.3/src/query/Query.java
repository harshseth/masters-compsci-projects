package query;

import java.lang.reflect.Array;
import java.util.TreeSet;

import relation.RelationNode;

public class Query {
	 TreeSet<RelationNode> relationSet=new TreeSet<RelationNode>();
	 public Query(int i)
	 {		
//		 	String[] s1={"s1","p1","o1"};
//	 		String[] s2={"p1","p2","o2"};
//	 		String[] s3={"o2","p3","o3"};
//	 		String[] s4={"s4","s1","o4"};
//	 		RelationNode R1=new RelationNode(s1);
//	 		RelationNode R2=new RelationNode(s2);
//	 		RelationNode R3=new RelationNode(s3);
//	 		RelationNode R4=new RelationNode(s4);
//		 	relationSet.add(R1);
//		 	relationSet.add(R2);
//		 	relationSet.add(R3);
//		 	relationSet.add(R4);
//		 	R1.bindObject("value1");
//		 	R3.bindObject("value2");
//		 	String[] s1={"s1","p1","o1"};
//	 		String[] s2={"v2","p2","o5"};
//	 		String[] s3={"s1","v3","o3"};
//	 		String[] s4={"s1","p4","v4"};
//	 		String[] s5={"p1","v5","o5"};
//	 		RelationNode R1=new RelationNode(s1);
//	 		RelationNode R2=new RelationNode(s2);
//	 		RelationNode R3=new RelationNode(s3);
//	 		RelationNode R4=new RelationNode(s4);
//	 		RelationNode R5=new RelationNode(s5);
//		 	relationSet.add(R1);
//		 	relationSet.add(R2);
//		 	relationSet.add(R3);
//		 	relationSet.add(R4);
//		 	relationSet.add(R5);
//		 	R2.bindSubject("v2");
//		 	R3.bindPredicate("v3");
//		 	R5.bindPredicate("v5");
//		 	R4.bindObject("v4");
		 if(i == 1)
		 {
			String[] s1={"s1","p1","o1"};
	 		String[] s2={"s1","p2","o2"};
	 		String[] s3={"s2","p2","o3"};
	 		
	 		RelationNode R1=new RelationNode(s1);
	 		RelationNode R2=new RelationNode(s2);
	 		RelationNode R3=new RelationNode(s3);
	 		
	 		//RelationNode R5=new RelationNode(s5);
		 	relationSet.add(R1);
		 	relationSet.add(R2);
		 	relationSet.add(R3);
		 	
		 	//relationSet.add(R5);
		 	R1.bindPredicate("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>");
		 	R1.bindObject("<http://simile.mit.edu/2006/01/ontologies/mods3#Text>");
		 }
		 else if(i == 2)
		 {
			String[] s1={"s1","p1","o1"};
	 		String[] s2={"s1","p2","o2"};
	 		String[] s3={"o2","p3","o3"};
	 		
	 		RelationNode R1=new RelationNode(s1);
	 		RelationNode R2=new RelationNode(s2);
	 		RelationNode R3=new RelationNode(s3);
	 		
	 		//RelationNode R5=new RelationNode(s5);
		 	relationSet.add(R1);
		 	relationSet.add(R2);
		 	relationSet.add(R3);
		 	
		 	//relationSet.add(R5);
		 	R1.bindPredicate("<http://simile.mit.edu/2006/01/ontologies/mods3#origin>");
		 	R1.bindObject("<info:marcorg/DLC>");
		 	R2.bindPredicate("http://simile.mit.edu/2006/01/ontologies/mods3#records>");
		 	R3.bindPredicate("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>");
		 	R3.bindObject("<http://simile.mit.edu/2006/01/ontologies/mods3#text>");
		 }
		 else if(i == 3)
		 {
			String[] s1={"s1","p1","o1"};
	 		String[] s2={"s1","p2","o2"};
	 		String[] s3={"s1","p3","o3"};
	 		
	 		RelationNode R1=new RelationNode(s1);
	 		RelationNode R2=new RelationNode(s2);
	 		RelationNode R3=new RelationNode(s3);
	 		
	 		//RelationNode R5=new RelationNode(s5);
		 	relationSet.add(R1);
		 	relationSet.add(R2);
		 	relationSet.add(R3);
		 	
		 	//relationSet.add(R5);
		 	R1.bindPredicate("<http://simile.mit.edu/2006/01/ontologies/mods3#Point>");
		 	R1.bindObject("\"end\"");
		 	R2.bindPredicate("http://simile.mit.edu/2006/01/ontologies/mods3#Encoding>");
		 	R3.bindPredicate("<http://www.w3.org/1999/02/22-rdf-syntax-ns#Type>");
		 }
		 else if(i == 4)
		 {
			String[] s1={"s1","p1","o1"};
	 		String[] s2={"s1","p2","o2"};
	 		String[] s3={"s2","p2","o2"};
	 		String[] s4={"s1","p3","o3"};
	 		
	 		RelationNode R1=new RelationNode(s1);
	 		RelationNode R2=new RelationNode(s2);
	 		RelationNode R3=new RelationNode(s3);
	 		RelationNode R4=new RelationNode(s4);
	 		//RelationNode R5=new RelationNode(s5);
		 	relationSet.add(R1);
		 	relationSet.add(R2);
		 	relationSet.add(R3);
		 	relationSet.add(R4);
		 	//relationSet.add(R5);
		 	R1.bindPredicate("<http://www.w3.org/1999/02/22-rdf-syntax-ns#type>");
		 	R1.bindObject("<http://simile.mit.edu/2006/01/ontologies/mods3#Text>");
		 	R4.bindObject("<http://simile.mit.edu/2006/01/language/iso639-2b/fre>");
		 	R4.bindPredicate("<http://simile.mit.edu/2006/01/ontologies/mods3#language>"); 
		 }
		 else if(i == 5)
		 {
			String[] s1={"s1","p1","o1"};
	 		String[] s2={"s2","p2","o2"};
	 		String[] s3={"s3","p3","o3"};
	 		String[] s4={"s3","p5","o6"};
	 		String[] s5={"s1","p1","o6"};
	 		String[] s6={"s3","p2","o3"};
	 		String[] s7={"s4","p5","o3"};
	 		String[] s8={"s1","p1","o4"};
	 			 		
	 		RelationNode R1=new RelationNode(s1);
	 		RelationNode R2=new RelationNode(s2);
	 		RelationNode R3=new RelationNode(s3);
	 		RelationNode R4=new RelationNode(s4);
	 		RelationNode R5=new RelationNode(s5);
	 		RelationNode R6=new RelationNode(s6);
	 		RelationNode R7=new RelationNode(s7);
	 		RelationNode R8=new RelationNode(s8);
	 		
	 		relationSet.add(R1);
		 	relationSet.add(R2);
		 	relationSet.add(R3);
		 	relationSet.add(R4);
		 	relationSet.add(R5);
		 	relationSet.add(R6);
		 	relationSet.add(R7);
		 	relationSet.add(R8);
		 	
		 	R1.bindPredicate("<http://simile.mit.edu/2006/01/ontologies/mods3#authority>");
		 	R5.bindPredicate("<http://simile.mit.edu/2006/01/ontologies/mods3#authority>");
		 	R8.bindPredicate("<http://simile.mit.edu/2006/01/ontologies/mods3#authority>");
		 	R3.bindObject("<http://simile.mit.edu/2006/01/ontologies/mods3#Date>");
		 	R3.bindSubject("<http://libraries.mit.edu/barton/MCM/000002459>");
		 	R4.bindSubject("<http://libraries.mit.edu/barton/MCM/000002459>");
		 	R6.bindSubject("<http://libraries.mit.edu/barton/MCM/000002459>");
		 	R6.bindObject("<http://simile.mit.edu/2006/01/ontologies/mods3#Date>");
		 	R7.bindObject("<http://simile.mit.edu/2006/01/ontologies/mods3#Date>");
		 }
	 }
	 public boolean containsAllRelations(TreeSet<RelationNode> relations)
	 {	
		return relations.containsAll(relationSet);
	 }
	 public void generateQuery(int i)
	 {
		 
	 }
	 public TreeSet<RelationNode> getRelationList()
	 {
		return relationSet; 
	 }
}
