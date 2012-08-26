package relation;



public class RelationNode implements Comparable<RelationNode> {
	
	final static int NCOLUMNS =3;
	static int i=0;
	int id;
	Column[] columns=new Column[3]; 
	@Override
	
	public int compareTo(RelationNode rN) {
		// TODO Auto-generated method stub
		return id-rN.id;
	}

	
	
	public RelationNode(String[] s)
	{	for(int k=0 ;k<NCOLUMNS ;k++)
		{	columns[k] = new Column(s[k]);
		}
		id=i;
		i++;	
	}
	public void bindSubject(String[] values)
	{
		columns[0].setValue(values);
	}
	public void bindPredicate(String[] values)
	{
		columns[1].setValue(values);
	}
	public void bindObject(String[] values)
	{
		columns[2].setValue(values);
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
