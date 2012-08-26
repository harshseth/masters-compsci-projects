package relation;



public class RelationNode implements Comparable<RelationNode> {
	static String coltype[]={"s","p","o"};
	final static int NCOLUMNS =3;
	static int i=0;
	int id;
	boolean bound = false;
	Column[] columns=new Column[3]; 
	@Override
	
	public int compareTo(RelationNode rN) {
		// TODO Auto-generated method stub
		return id-rN.id;
	}

	
	
	public RelationNode(String[] s)
	{	for(int k=0 ;k<NCOLUMNS ;k++)
		{	columns[k] = new Column(s[k], coltype[k]);
		}
		id=i;
		i++;	
	}
	public void bindSubject(String values)
	{	bound = true;
		columns[0].setValue(values);
	}
	public void bindPredicate(String values)
	{	bound =true;
		columns[1].setValue(values);
	}
	public void bindObject(String values)
	{	bound = true;
		columns[2].setValue(values);
	}
	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return ""+id;
	}

public boolean isBound()
{
return bound;	
}

	public boolean isJoinable(RelationNode rN)
	{
		for(int i=0;i<NCOLUMNS;i++)
		{
			for(int k=0;k<NCOLUMNS;k++)
			{
				if(columns[i].equals(rN.columns[k]));
					return true;
			}
		}
		return false;
	}
	public int getID()
	{
		return id;
	}
	
	@Override
	public boolean equals(Object obj) {
		// TODO Auto-generated method stub
		RelationNode rN= (RelationNode) obj;
		return id==rN.id;
	}



	public Column[] getColumns() {
		// TODO Auto-generated method stub
		return columns;
		
	}
}
