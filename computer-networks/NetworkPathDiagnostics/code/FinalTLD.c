# include<stdlib.h>
# include<stdio.h>
# include<string.h>

char domain1[100], domain2[100];
char *string11, *string22, *string33;


int TLD(char *string2, char domain[])
{
char line[100]="", domain_t[100], linex[100];
char c=0, prepend[100], prepend_t[100],*string2_t, rest[100], *string2_t1;
int length,j=0,t=1,x,x1,m=0,check=0,z=0,n=0,restl,linelen,longest_found=0,lno=1;
string2_t=string2;
int lens,is,zs,checks=0;
int checkx=0,zx,ix,finally=0,xx;
FILE* fpx; 
FILE* fp; 


	
	length=strlen(string2);
	while(j<length)
	{
		j++;
		
		if(*string2 == '.')
		{
			lno=0;
			longest_found=0;
			for(n=0;n<j-t;n++)
			{
				prepend[n]=*string2_t;string2_t++;}
				prepend[n]='\0';
				string2_t=string2_t+1;
				t=j+1;
				string2_t1=string2_t;
		
				for(m=0;m<length-j;m++)
				{
					rest[m]=*string2_t1;
					string2_t1++;
				}

				rest[m]='\0';

				fp = fopen("effectiveTLD.txt","r");		
				
				while(fgets(line,sizeof(line),fp) != NULL)
				{
					if(line[0] == '\n' || line[0]=='/')
					{
					}
					else if(line[0]=='*')
					{
						zs=0;checks=0;

						while(rest[zs]!='.')
						{
							zs++;
						}
						zs++;
						lens=strlen(rest);
						
						for(is=2;is<lens-1;is++)
						{
							if(line[is]==rest[zs])
							{
								checks=checks+0;
							}
							else{
								checks++;
								break;
							}
							zs++;

						}
						if(checks==0)
						{


							finally=0;
							fpx = fopen("effectiveTLD.txt","r");
							while(fgets(linex,sizeof(linex),fpx) != NULL)
							{
		
								if(linex[0]=='!')
								{
									checkx=0;
									xx=strlen(linex);
									for(zx=1;zx<xx-1;zx++)
									{
										if(linex[zx]==rest[zx-1]){
											checkx=checkx+0;
										}
										else{
											checkx++;
											break;
										}
									}
									if(checkx==0){
										finally++;
										break;
									}

								}
							}

							if(finally==0){
								strcpy(domain,prepend);
								strcat(domain,".");
								strcat(domain,rest);
							//	printf("\nDomain:%s\n\n",domain);
								return 0;
							}

						}
					}
					else{
						check=0;
						x1=strlen(line);
						x=strlen(rest);
						
						for(z=0;z<x;z++){	
							if(line[z]==rest[z]){
								check=check+0;
							}
							else{
								check++;
								break;
							}

						}

						if(check==0)
						{
							longest_found=1;
							strcpy(domain,prepend);
							strcat(domain,".");
							strcat(domain,rest);
							//printf("\nDomain:%s\n\n",domain);

							return 1;
						}
						else{
						}
					}

				}

			}
		string2++;				
	}
fclose(fp);
return 0;
}


int TLDINIT(char *string1i, char *string2i, char *string3i)
{

	int d1_length, d2_length,imt=0,checkmt=0;
	if(string2i==NULL)
	{
		TLD(string1i,domain1);
		printf("\nDomain:%s \n",domain1);
		strcpy(string3i,domain1);
	}
	else
	{
		TLD(string1i,domain1);
		TLD(string2i,domain2);
		d1_length=strlen(domain1);
		d2_length=strlen(domain2);
		if(d1_length==d2_length)
		{
			for(imt=0;imt<d1_length;imt++)
			{
				if(domain1[imt]==domain2[imt]){checkmt=checkmt+0;}
				else{checkmt++;}
			}
		}
		else	
		{
			//printf("Domain Not Matched");
			return 0;
		}
	}
	if(checkmt==0)
	{
		//printf("Domain:%s Matched",domain1);
		return 0;
	}
	else
	{
		//printf("Domain Not Matched");
		return 1;
	}
}



int main()
{
	int c;
	
	string11= "145.122.early-registration.of.surfnet.invalid";
	string22= NULL;
	string33= (char *)calloc(1,100*sizeof(char));

	c=TLDINIT(string11,string22,string33);
	printf("%s\n",string33);
	//printf("\nC:%d",c);
	return 0;
}
