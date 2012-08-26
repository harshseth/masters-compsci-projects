package relation;

public class Column implements Comparable<Column> {

String name;
String[] values=null;

public Column(String lname)
{
	name=lname;
}
public void setValue(String[] lvalues)
{
	values=lvalues;
}
public String[] getValue()
{
	return values;
}

//public void setCardinality(int card)
//{	
//	cardinality=card;
//}
//public boolean isSorted() {
//	return sorted;
//}
//public void setSorted(boolean sorted) {
//	this.sorted = sorted;
//}
//public boolean isBound() {
//	return bound;
//}
//public void setBound(boolean bound) {
//	this.bound = bound;
//}
//public int getCardinality() {
//	return cardinality;
//}
@Override
public int compareTo(Column col) {
	// TODO Auto-generated method stub
	return name.compareTo(col.name);
}
@Override
public boolean equals(Object o) {
	// TODO Auto-generated method stub
	Column col = (Column)o;
	return name.equals(col.name);
}
}
