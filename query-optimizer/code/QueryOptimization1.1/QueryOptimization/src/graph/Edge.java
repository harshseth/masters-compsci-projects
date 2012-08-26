package graph;

import java.util.Dictionary;
import java.util.HashMap;

public class Edge {
	boolean edge = false;
	byte[] bA;
	public Edge()
	{	bA=new byte[3];
		for(int i=0;i<bA.length;i++)
		{
			bA[i]=-1;
		}
	}
	public void setTrue()
	{	
		edge=true;	
		
	}
	public void setjoinVariable(byte i, byte j)
	{
		bA[i]=j;
	}
}
