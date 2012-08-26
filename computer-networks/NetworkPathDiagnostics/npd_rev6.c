/* Network Path Diagnostics */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define FILE_LEN 500
#define FILE_LEN1 25

struct TrieNode{
char *data;
struct TrieNode *left;
struct TrieNode *right;
} ;


typedef struct Trie{
long int size;
struct TrieNode *root;
struct TrieNode *curRoot;
} BiTrie;

static FILE *fp1, *fp2, *fp3, *fp4, *fp5, *filePointer;
static char file1[FILE_LEN], file2[FILE_LEN], file3[FILE_LEN], file4[FILE_LEN1];
static char *concatString, *lastHopAddr, *lastHopHostName, *numberOfHops, *responseTime, *stringToken, *destToken, *lastHopToken, *fileInput;
static char **lastHopHostNames, **lastHopAddrs, **lastHopAddrs24,**lastHopBGPPrefixes,**lastHopBGPPrefixes24,**lastHopDomains;
static char *data, *nextHop, *tempStr,*charToStr, *bestMatch, *bgpPrefix, *destBGPPrefix, *lastHopBGPPrefix,*destBGPPrefix24, *lastHopBGPPrefix24;
static char *lastHop24, *dest24, *b,*destName,*destNameDomain,*lastHopHostNameDomain;
static int numberOfStarredHops = 0;
static float totalResTime = 0.0, totalTime = 0.0;
static BiTrie *trie ;
static struct TrieNode *node , *tempNode, *newnode;
static long int prefixCount = 0;
static u_int16_t prefixOctet1 = 0,prefixOctet2 = 0,prefixOctet3 = 0,prefixOctet4 = 0, prefixLength = 0,lookupOctet1 = 0,lookupOctet2 = 0,lookupOctet3 = 0,lookupOctet4 = 0, lookupLength = 32;
static u_int16_t destOctet1 = 0,destOctet2 = 0,destOctet3 = 0,destOctet4 = 0,destLength = 32;
static u_int16_t lastHopOctet1 = 0,lastHopOctet2 = 0,lastHopOctet3 = 0,lastHopOctet4 = 0,lastHopLength = 32;
static u_int16_t prefixFinal[32] = {}, k = 0, destFinal[32] = {}, lastHopFinal[32] = {};
char c = 0, d = 0, e = 0;
static char *initializer = "", *prohibited;
static int uniqueHostAddrs = 0, uniqueHostNames = 0, choice, nOfHops = 0;
int isUnReachable = 0;
static int uniqueBGPPrefixCount = 0, uniqueBGPPrefixCount24 = 0, uniqueDomainCount = 0;
int starCount = 0, dotCount = 0, timeCount = 0;
static char *string11, *string22 , *string33, *domainName;
static char domain1[100], domain2[100];

int TLD(char *string2, char domain[100])
{

char line[100], domain_t[100], linex[100];
char c=0, prepend[50], prepend_t[50],*string2_t , rest[50], *string2_t1 ;
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

				fp = fopen("effectiveTLD.txt","r+");		
				
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
							fpx = fopen("effectiveTLD.txt","r+");
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
							fclose(fpx);
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

int getDotCount(char* string){
	
	dotCount = 0;
	b = string;
	int i = 0;
	while( i++ < strlen(string)){

		if( (*b) == '.' )
			dotCount++;
	
		b = b + 1;
	}
	
	return dotCount;
}

int isOnlyNumeric(char *string){
	
	int i = 0;
	b = string;
	while( i++ < strlen(string)){
		
		if( ( (int)(*b) > 47 && (int)(*b) < 58 ) || (*b == '.' || *b == '\n') )
			b = b + 0;
		else
			return 0;
		
		b = b + 1;
	}
	
	return 1;
}

int getPrefix(u_int16_t preOct1, u_int16_t preOct2, u_int16_t preOct3, u_int16_t preOct4, int preLen, u_int16_t prefixFinal[]){
	
	u_int16_t j = 0, pos = 8, k = preOct1, l = preOct2, m = preOct3, n = preOct4;
	u_int16_t prefix[32] = {};
	while (j < 8){
		
		prefix[pos - j - 1] =  k & 0x01;
		prefix[2*pos - j - 1] = l & 0x01;
		prefix[3*pos - j - 1] = m & 0x01;
		prefix[4*pos - j - 1] = n & 0x01;
		
		k = k/2;
		l = l/2;
		n = n/2;
		m = m/2;
		j++;
	}


	for(j = 0 ; j< preLen; j++) 
		prefixFinal[j] = prefix[j];

	return 0;

}

char* getLongestPrefixMatch(u_int16_t lookupFin[]){
	
	int i = 0, j = 0;
	bestMatch = (char *)calloc(1,20*sizeof(char));
	tempNode = (struct TrieNode *)calloc(1,sizeof(struct TrieNode));
	
	strcpy(bgpPrefix,initializer);
	tempNode = trie->root;
	
	for(i=0;i<lookupLength;i++){
			
			if(lookupFin[i] == 0) {
				tempNode = tempNode->left;
					if(tempNode == NULL) break;
					
					else{
						if(tempNode->data != NULL)
							strcpy(bestMatch,tempNode->data);
						else
							continue;
					}
			}
			
			else{
				tempNode = tempNode->right;
					if(tempNode == NULL) break;
					
					else {
						if(tempNode->data != NULL)
							strcpy(bestMatch,tempNode->data);
						else
							continue;
					}
			}		
	}
	
	for(j = 0; j< i; j++ ){
	
		if(lookupFin[j] == 0)
			strcat(bgpPrefix,"0");
		else
			strcat(bgpPrefix,"1");
	}
	
//	printf("\nThe longest prefix match for this address is %s at length %d \n", bestMatch, i);
//	printf("The BGP prefix for this address is %s \n", bgpPrefix);
	
	return bgpPrefix;
}


void BiTrieInit(BiTrie *trie ) {

	trie->size = 0;
	
	if((trie->root = (struct TrieNode *)calloc(1,sizeof(struct TrieNode))) == NULL)
			perror("Memory could not be allocated using calloc to BiTrie");
	
	trie->root->left = NULL;
	trie->root->right = NULL;
			
	trie->curRoot = trie->root;
	return ;
	
}

struct TrieNode* newNode(BiTrie *trie, char* data){
	
	
	
	if((newnode = (struct TrieNode *)calloc(1,sizeof(struct TrieNode))) == NULL)
			perror("Memory could not be allocated using calloc to BiTrieNode");
				
				if(data != NULL){ 
					if((newnode->data = (char *)calloc(1,20*sizeof(char)))==NULL)
						perror("Memory could not be allocated using calloc to newdata");
					strcpy(newnode->data, (char *)data);
				}
				else 
					newnode->data = (char *)data;
				
				newnode->left = NULL;
				newnode->right = NULL;
				trie->size++;
				return newnode;


}

struct TrieNode* BiTrieInsert(BiTrie *trie, struct TrieNode *node, char *data , int flag ) {

	if(node == NULL) 
		node = newNode(trie,data);

	else {
		
		if( flag == 0) {
			if(node->left == NULL){
				node->left = BiTrieInsert(trie, node->left, data , flag);
				return node->left;
			}	
			else
				return node->left;
		}	 
		
		else { 
			if(node->right == NULL){
				node->right = BiTrieInsert(trie, node->right, data , flag);
				return node->right;
			}
			else
				return node->right;
		}
	}
	
	return node;
}

void populateVal(){
	
	int posCount = 1;
	responseTime = (char *)calloc(1,5*sizeof(char));
	totalResTime = 0.0;
	isUnReachable = numberOfStarredHops = 0;
	uniqueDomainCount = uniqueBGPPrefixCount = uniqueBGPPrefixCount24 = 0;
	strcpy(prohibited,"");
	fp3 = fopen("output.txt","r");
	
	while(fgets(file2,FILE_LEN,fp3)!= NULL){
		
		starCount = timeCount = 0;
		totalTime = 0.0;
		stringToken = strtok(file2," ()");
		
		posCount = 1;
		if(posCount == 1 && ! (strlen(stringToken) <= 3) )
			posCount++;
		
		while(stringToken != NULL){
		
			if(strcmp(stringToken,"*") == 0 || strcmp(stringToken,"*\n") == 0){
				starCount++;
				if(starCount == 3)
					numberOfStarredHops++;
			}
			
			if(strcmp(stringToken,"!X") == 0 || strcmp(stringToken,"!H") == 0 || strcmp(stringToken,"!N") == 0 || strcmp(stringToken,"!P") == 0){
				strcpy(prohibited,stringToken);
				isUnReachable = 1;
			}
			
			if(posCount == 1 && strlen(stringToken) <= 3){
				strcpy(numberOfHops,stringToken);
				nOfHops++;
			}
			else if( getDotCount(stringToken) >=2 && strlen(stringToken) >= 7 && isOnlyNumeric(stringToken) == 0){
				strcpy(lastHopHostName,stringToken);
				strcpy(lastHopHostNames[uniqueHostNames],stringToken);	
				uniqueHostNames++;
			}
			else if(getDotCount(stringToken) == 3 && isOnlyNumeric(stringToken) == 1){
				strcpy(lastHopAddr,stringToken);
				strcpy(lastHopAddrs[uniqueHostAddrs],stringToken);	
				uniqueHostAddrs++;
			}
			else if(getDotCount(stringToken) == 1  && isOnlyNumeric(stringToken) == 1){
				strcpy(responseTime,stringToken);
				totalTime += atof(responseTime);
				timeCount++;
				//printf(" @@ %s @@ ", responseTime);
			}
			stringToken = strtok(NULL," ()");
			posCount++;
		}
		if(timeCount == 0) timeCount = 1;
		totalResTime += totalTime / (float)timeCount;
	}
	
	fclose(fp3);
}

void executeTraceroute(const char *addr){
	
	strcpy(concatString,"traceroute ");
	strcat(concatString,addr);
	fp1 = popen(concatString,"r");
	fp2 = fopen("output.txt","a+");
	
	while(fgets(file1,FILE_LEN,fp1)!= NULL){
		printf("%s",file1);
		fputs(file1,fp2);
	}
	
	pclose(fp1);
	fclose(fp2);
	
	populateVal();
	
}

int main(int argc, char **argv ){
	
	filePointer = (FILE *) malloc(10 * sizeof(FILE)); 
	int nextHopOctet1 = 0,nextHopOctet2 = 0,nextHopOctet3 = 0,nextHopOctet4 = 0;
	int i = 0, j = 0;
	tempStr = (char *)calloc(1,2*sizeof(char));
	charToStr = (char *)calloc(1,sizeof(char));
	nextHop = (char *)calloc(1,15*sizeof(char));
	data = (char *)calloc(1,20*sizeof(char));
	trie = (BiTrie *)calloc(1,sizeof(BiTrie));
	node = (struct TrieNode *)calloc(1,sizeof(struct TrieNode));

	char *input = (char *)calloc(1,10*sizeof(char)), *a = (char *)calloc(1,sizeof(char)), *inputString = (char *)calloc(1,15*sizeof(char)), *isTok = (char *)calloc(1,15*sizeof(char)),*lhTok = (char *)calloc(1,15*sizeof(char));
	int isHostName = 0, isIpAddress = 0, destCount = 1,lastHopCount = 1;
	struct hostent *inputAddr = (struct hostent *)calloc(1,sizeof(struct hostent));
	struct in_addr **addrlist = (struct in_addr **)calloc(1,sizeof(struct in_addr *));
	concatString = (char *)calloc(1,40*sizeof(char));
	lastHopAddr = (char *)calloc(1,40*sizeof(char));
	lastHopHostName = (char *)calloc(1,50*sizeof(char));
	stringToken = (char *)calloc(1,50*sizeof(char));
	destToken = (char *)calloc(1,40*sizeof(char));
	lastHopToken = (char *)calloc(1,40*sizeof(char));
	numberOfHops= (char *)calloc(1,5*sizeof(char));
	bgpPrefix = (char *)calloc(1,40*sizeof(char));
	destBGPPrefix = (char *)calloc(1,40*sizeof(char));
	lastHopBGPPrefix = (char *)calloc(1,40*sizeof(char));
	destBGPPrefix24 = (char *)calloc(1,40*sizeof(char));
	lastHopBGPPrefix24 = (char *)calloc(1,40*sizeof(char));
	lastHopAddrs = (char **)calloc(64,sizeof(char *));
	lastHopHostNames = (char **)calloc(64,sizeof(char *));
	lastHopAddrs24 = (char **)calloc(64,sizeof(char *));
	fileInput = (char *)calloc(1,20*sizeof(char));
	prohibited = (char *)calloc(1,5*sizeof(char));
	lastHopBGPPrefixes = (char **)calloc(64,sizeof(char *));
	lastHopBGPPrefixes24 = (char **)calloc(64,sizeof(char *));
	lastHopDomains = (char **)calloc(64,sizeof(char *));
	lastHop24 = (char *)calloc(1,40*sizeof(char));
	dest24 = (char *)calloc(1,40*sizeof(char));
	b = (char *)calloc(1,sizeof(char));
	string22 = NULL;
	string33 = (char *)calloc(1,100*sizeof(char));
	string11 = (char *)calloc(1,100*sizeof(char));
	domainName = (char *)calloc(1,100*sizeof(char));
	destName = (char *)calloc(1,100*sizeof(char));
	destNameDomain = (char *)calloc(1,100*sizeof(char));
	lastHopHostNameDomain = (char *)calloc(1,100*sizeof(char));
	
	strcpy(concatString,"traceroute ");
	
	for(i = 0; i<64 ; i++) 
		if( (lastHopAddrs[i] = (char *)calloc(20,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated to lastHopHostAddrs");
	for(i = 0; i<64 ; i++) 
		if( (lastHopAddrs24[i] = (char *)calloc(32,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated to lastHopHostAddrs24");
	for(i = 0; i<64 ; i++) 
		if( (lastHopHostNames[i] = (char *)calloc(100,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated to lastHopHostNames");	
	for(i = 0; i<64 ; i++) 
		if( (lastHopBGPPrefixes[i] = (char *)calloc(100,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated to lastHopBGPPrefixes");
	for(i = 0; i<64 ; i++) 
		if( (lastHopBGPPrefixes24[i] = (char *)calloc(100,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated to lastHopBGPPrefixes24");
	for(i = 0; i<64 ; i++) 
		if( (lastHopDomains[i] = (char *)calloc(100,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated to lastHopDomains");

	
	i = 0;
	BiTrieInit(trie);

	filePointer = fopen("load.txt", "r+");
	
	
	if( filePointer == NULL) 
		perror("Error opening file");

	else 
	while( !feof(filePointer) ) {
		
		c = (char)getc(filePointer);
		sprintf(charToStr,"%c",c);
		strcat(tempStr,charToStr);
		
		if( c == '.' || c == ' ' || c == '/' || c == '\n') {

			i++;
			if(i == 1) {
				prefixOctet1 = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
			if(i == 2) {
				prefixOctet2 = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
			if(i == 3) {
				prefixOctet3 = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
			if(i == 4) {
				prefixOctet4 = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
		
			if(i == 5) {
				prefixLength = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
			if(i == 6) {
				nextHopOctet1 = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
			if(i == 7) {
				nextHopOctet2 = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
			if(i == 8) {
				nextHopOctet3 = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
			if(i == 9) {
				nextHopOctet4 = atoi(tempStr);
				strcpy(tempStr,initializer);
			}
		
		}
			if (c == '\n' ) {
				prefixCount++;
				i = 0;
				strcpy(nextHop,initializer);
				strcpy(data,initializer);
				
				//printf("\n\nIPPrefix %d.%d.%d.%d/%d  NextHop %d.%d.%d.%d", prefixOctet1,prefixOctet2, prefixOctet3, prefixOctet4, prefixLength, nextHopOctet1,nextHopOctet2,nextHopOctet3,nextHopOctet4);
				getPrefix(prefixOctet1, prefixOctet2,prefixOctet3,prefixOctet4 ,prefixLength, prefixFinal);
				
				sprintf(nextHop,"%d.%d.%d.%d",nextHopOctet1,nextHopOctet2,nextHopOctet3,nextHopOctet4);
				
				//printf("  prefixFinal : ");
				//for(j = 0 ; j< prefixLength; j++)
				//	printf("%d", prefixFinal[j]);
				
				for(j = 0 ; j< prefixLength; j++){
			
					data = NULL;
					
					if( j == 0 && prefixCount > 1) trie->curRoot = trie->root;
					
					if(j == prefixLength -1) {
						data = (char *)calloc(1,20*sizeof(char));
						strcpy(data,nextHop);
					}
					
					if(prefixFinal[j] == 0){	
						trie->curRoot = BiTrieInsert(trie,trie->curRoot,data,0);
						continue;
					}
					
					else {
						if(prefixFinal[j] == 1){
							 trie->curRoot = BiTrieInsert(trie,trie->curRoot,data,1);
						 	 continue;
						}						
					}
				}
			}
		}	
	
	
	//This is where the second prog starts
	printf("\nEnter \n1)for IP Address \n2)for Hostname \n3)for file containing IP Addresses \n4)for file containing Hostnames \n:");
	scanf("%d",&choice);
	
	if(choice == 1){
		
		i = 0;
		printf("\nEnter the IP Address: ");
		scanf("%s",input);
		
		//checking for '.' '-' ,letters and numbers
		a = input;
		while( i++ < strlen(input)){
	
			if( ((*a) == '.' )|| (*a == '-') || ((int)(*a) > 47 && (int)(*a) < 58) || ((int)(*a) > 64 && (int)(*a) < 91) || ((int)(*a) > 96 && (int)(*a) < 123)){
				if( ((*a) == '.') )
					dotCount++;
			}	
			else{
				printf("\nInvalid characters found\n");
				exit(0);
			}		
			a = a + 1;
		}
		
		strcpy(inputString,input);
		executeTraceroute(inputString);
		
	/*	//For calculating hostname of an ip addr
		inet_pton(AF_INET, inputString, &addrlist);
		inputAddr = gethostbyaddr(&addrlist, sizeof addrlist, AF_INET);
		strcpy(destName,inputAddr->h_name);
		//printf("\nHostname is %s",destName);
		
		//For calculating effective TLD of input ip addr and lasthophostname
		TLDINIT(destName,NULL,destNameDomain);
		TLDINIT(lastHopHostName,NULL,lastHopHostNameDomain);
	*/	

		//For tokenizing the input addr
		destCount = 1;
		strcpy(isTok,inputString);
		destToken = strtok(isTok,".");
	
		while(destToken != NULL){
	
			if(destCount == 1)	
				destOctet1 = atoi(destToken);
			else if(destCount == 2)	
				destOctet2 = atoi(destToken);
			else if(destCount == 3)	
				destOctet3 = atoi(destToken);
			else if(destCount == 4)	
				destOctet4 = atoi(destToken);
		
			destToken = strtok(NULL,".");
			destCount++;
		}
	
		//For tokenizing the lastHop addr
		lastHopCount = 1;
		strcpy(lhTok,lastHopAddr);
		lastHopToken = strtok(lhTok,".");
		
		while(lastHopToken != NULL){
	
			if(lastHopCount == 1)	
				lastHopOctet1 = atoi(lastHopToken);
			else if(lastHopCount == 2)	
				lastHopOctet2 = atoi(lastHopToken);
			else if(lastHopCount == 3)	
				lastHopOctet3 = atoi(lastHopToken);
			else if(lastHopCount == 4)	
				lastHopOctet4 = atoi(lastHopToken);
		
			lastHopToken = strtok(NULL,".");
			lastHopCount++;
		}
	
		printf("\n---------------------------------------------------------------");
		if(strcmp(inputString,lastHopAddr) == 0)
			printf("\nTraceroute is successful");
		else{
	
			printf("\nTraceroute is Unsuccessful");
			getPrefix(destOctet1, destOctet2,destOctet3,destOctet4 ,destLength, destFinal);
			getPrefix(lastHopOctet1, lastHopOctet2,lastHopOctet3,lastHopOctet4 ,lastHopLength, lastHopFinal);
			strcpy(destBGPPrefix,getLongestPrefixMatch(destFinal));
			strcpy(lastHopBGPPrefix,getLongestPrefixMatch(lastHopFinal));
			//printf("\nThe BGPPrefix of destination is %s", destBGPPrefix);
			//printf("\nThe BGPPrefix of last non-starred hop is %s", lastHopBGPPrefix);
			
			strcpy(destBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(i = 0; i < 32; i++){
					if(destFinal[i] == 0)
						strcat(destBGPPrefix24, "0");
					else if(destFinal[i] == 1)
						strcat(destBGPPrefix24,"1");
				}
			strncpy(dest24,destBGPPrefix24,24);
			dest24[24] = '\0';
			
			strcpy(lastHopBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(i = 0; i < 32; i++){
					if(lastHopFinal[i] == 0)
						strcat(lastHopBGPPrefix24, "0");
					else if(lastHopFinal[i] == 1)
						strcat(lastHopBGPPrefix24,"1");
				}
				
			strncpy(lastHop24,lastHopBGPPrefix24,24);
			lastHop24[24] = '\0';
			printf("\n---------------------------------------------------------------");
			
			// Reaches the BGP prefix and /24
			if(strcmp(destBGPPrefix,lastHopBGPPrefix) == 0)
				printf("\nThe packet reached the destination's BGP router");
			else
				printf("\nThe packet didn't reach the destination's BGP router");
			if(strcmp(dest24,lastHop24) == 0)
				printf("\nThe lastHop addr and the destination share a /24");
			else
				printf("\nThe lastHop addr and the destination do not share a /24");
			printf("\n---------------------------------------------------------------");	
			
		/*	// Reaches the domain
			if(strcmp(destNameDomain,lastHopHostNameDomain) == 0)
				printf("\nThe packet reached the destination's domain");
			else
				printf("\nThe packet didn't reach the destination's domain");
			printf("\n---------------------------------------------------------------"); */
			// If the maximum limit of 64 hops is reached
			if(strcmp(numberOfHops, "64") == 0)
				printf("\nThe maximum limit of 64 hops is reached");
			printf("\n---------------------------------------------------------------");
			
			//If one of !H, !X, !P and !N appear in the traceroute output
			if(isUnReachable == 1 && prohibited != NULL)
				if(strcmp(prohibited,"!H") == 0)
					printf("\nTraceroute is incomplete because Host is unreachable");
				else if(strcmp(prohibited,"!X") == 0)
					printf("\nTraceroute is incomplete because communication is administratively prohibited");	
				else if(strcmp(prohibited,"!N") == 0)
					printf("\nTraceroute is incomplete because Network is unreachable");
				else if(strcmp(prohibited,"!P") == 0)
					printf("\nTraceroute is incomplete because Protocol is unreachable");
			printf("\n---------------------------------------------------------------");
		}	
			//Number of Hops traversed
			printf("\nTotal number of Hops traversed is %s",numberOfHops);
			printf("\n---------------------------------------------------------------");
			
			//Number of Starred Hops traversed
			printf("\nTotal number of Starred Hops traversed is %d",numberOfStarredHops);
			printf("\n Percentage of hops that only returned stars is %f", (((float)(numberOfStarredHops))/atof(numberOfHops)));
			
			printf("\n---------------------------------------------------------------");
			// Number of Unique Networks traversed
			
			for(i = 0; i< uniqueHostAddrs; i++){
				
				lastHopCount = 1;
				strcpy(lhTok,lastHopAddrs[i]);
				lastHopToken = strtok(lhTok,".");
	
				while(lastHopToken != NULL){
	
					if(lastHopCount == 1)	
						lastHopOctet1 = atoi(lastHopToken);
					else if(lastHopCount == 2)	
						lastHopOctet2 = atoi(lastHopToken);
					else if(lastHopCount == 3)	
						lastHopOctet3 = atoi(lastHopToken);
					else if(lastHopCount == 4)	
						lastHopOctet4 = atoi(lastHopToken);
		
					lastHopToken = strtok(NULL,".");
					lastHopCount++;
				}
				
				getPrefix(lastHopOctet1, lastHopOctet2,lastHopOctet3,lastHopOctet4 ,lastHopLength, lastHopFinal);
				strcpy(lastHopBGPPrefix,getLongestPrefixMatch(lastHopFinal));
				strcpy(lastHopBGPPrefixes[i],lastHopBGPPrefix);
			
				strcpy(lastHopBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(j = 0; j < 32; j++){
					if(lastHopFinal[j] == 0)
						strcat(lastHopBGPPrefix24, "0");
					else if(lastHopFinal[j] == 1)
						strcat(lastHopBGPPrefix24,"1");
				}
				
				strncpy(lastHopBGPPrefixes24[i],lastHopBGPPrefix24,24);
				lastHopBGPPrefixes24[i] += '\0';
				
			}
			//calculating the actual number of BGP prefixes
			for(i = 0; i< uniqueHostAddrs; i++){
				for(j = i+1; j< uniqueHostAddrs; j++){
					
					if( strcmp(lastHopBGPPrefixes[i],initializer) != 0 ){
						if(strcmp(lastHopBGPPrefixes[i] ,lastHopBGPPrefixes[j]) == 0 ){
							//uniqueBGPPrefixCount++;
							strcpy(lastHopBGPPrefixes[j], initializer);
							if(j == uniqueHostAddrs-1){
								//printf("\n%s",lastHopBGPPrefixes[i]);
								//printf("\n%s",lastHopAddrs[i]);
								uniqueBGPPrefixCount++;
							}
						}
						else if(j == uniqueHostAddrs-1){
							//printf("\n%s",lastHopBGPPrefixes[i]);
							//printf("\n%s",lastHopAddrs[i]);
							uniqueBGPPrefixCount++;
						}
					}
				}
				
				if(i == uniqueHostAddrs - 1 && strcmp(lastHopBGPPrefixes[i],initializer) != 0 )
					uniqueBGPPrefixCount++;
			}
			
			//calculating the number of unique /24s
			for(i = 0; i< uniqueHostAddrs; i++){
				for(j = i+1; j< uniqueHostAddrs; j++){
					if(strcmp(lastHopBGPPrefixes24[i],initializer) != 0 ){
						if(strcmp(lastHopBGPPrefixes24[i],lastHopBGPPrefixes24[j]) == 0 ){
							//uniqueBGPPrefixCount24++;
							strcpy(lastHopBGPPrefixes24[j], initializer);
							if(j == uniqueHostAddrs-1){
								//printf("\n%s",lastHopBGPPrefixes24[i]);
								printf("\n%s",lastHopAddrs[i]);
								uniqueBGPPrefixCount24++;
							}
						}
						else if(j == uniqueHostAddrs-1){
							//printf("\n%s",lastHopBGPPrefixes24[i]);
							printf("\n%s",lastHopAddrs[i]);
							uniqueBGPPrefixCount24++;
						}
					}
				}
				if(i == uniqueHostAddrs - 1 && strcmp(lastHopBGPPrefixes24[i],initializer) != 0 ){
					uniqueBGPPrefixCount24++;
					printf("\n%s",lastHopAddrs[i]);
				}
			}
			
			printf("\nNumber of unique BGP Prefixes traversed is %d", uniqueBGPPrefixCount);
			printf("\nNumber of unique /24s traversed is %d", uniqueBGPPrefixCount24);
			printf("\n---------------------------------------------------------------");	
			
			//Total number of unique domains
				for(i = 0; i< uniqueHostNames; i++){
					TLDINIT(lastHopHostNames[i],NULL,domainName);
					strcpy(lastHopDomains[i],domainName);
				}
			
			//calculating the number of unique domains
			for(i = 0; i< uniqueHostNames; i++){
				for(j = i+1; j< uniqueHostNames; j++){
					if(strcmp(lastHopDomains[i],initializer) != 0 ){
						if(strcmp(lastHopDomains[i],lastHopDomains[j]) == 0 ){
							//uniqueBGPPrefixCount24++;
							strcpy(lastHopDomains[j], initializer);
							if(j == uniqueHostNames-1){
								//printf("\n%s",lastHopBGPPrefixes24[i]);
								printf("\n%s",lastHopDomains[i]);
								uniqueDomainCount++;
							}
						}
						else if(j == uniqueHostNames-1){
							//printf("\n%s",lastHopBGPPrefixes24[i]);
							printf("\n%s",lastHopDomains[i]);
							uniqueDomainCount++;
						}
					}
				}
				if(i == uniqueHostNames - 1 && strcmp(lastHopDomains[i],initializer) != 0 ){
					uniqueDomainCount++;
					printf("\n%s",lastHopDomains[i]);
				}
			}
			
			printf("\nNumber of unique domains traversed is %d", uniqueDomainCount);
			printf("\n---------------------------------------------------------------");	
			
				
			// Total time taken to finish traceroute
			printf("\nThe Total Response time is %f ms\n", totalResTime);
		
	}
	
	else if (choice == 2){
		i = 0;
		printf("\nEnter the Hostname: ");
		scanf("%s",input);
		inputAddr = gethostbyname(input);
		addrlist = (struct in_addr **)inputAddr->h_addr_list;
	
		strcpy(inputString,inet_ntoa(*addrlist[0]));
		printf("\nThe ip addr for the input string is %s", inputString);
		executeTraceroute(inputString);
		
	/*	//For calculating effective TLD of input ip addr and lasthophostname
		TLDINIT(input,NULL,destNameDomain);
		TLDINIT(lastHopHostName,NULL,lastHopHostNameDomain);
	*/	
		//For tokenizing the input addr
		destCount = 1;
		strcpy(isTok,inputString);
		destToken = strtok(isTok,".");
	
		while(destToken != NULL){
	
			if(destCount == 1)	
				destOctet1 = atoi(destToken);
			else if(destCount == 2)	
				destOctet2 = atoi(destToken);
			else if(destCount == 3)	
				destOctet3 = atoi(destToken);
			else if(destCount == 4)	
				destOctet4 = atoi(destToken);
		
			destToken = strtok(NULL,".");
			destCount++;
		}
	
		//For tokenizing the lastHop addr
		lastHopCount = 1;
		strcpy(lhTok,lastHopAddr);
		lastHopToken = strtok(lhTok,".");
	
		while(lastHopToken != NULL){
	
			if(lastHopCount == 1)	
				lastHopOctet1 = atoi(lastHopToken);
			else if(lastHopCount == 2)	
				lastHopOctet2 = atoi(lastHopToken);
			else if(lastHopCount == 3)	
				lastHopOctet3 = atoi(lastHopToken);
			else if(lastHopCount == 4)	
				lastHopOctet4 = atoi(lastHopToken);
		
			lastHopToken = strtok(NULL,".");
			lastHopCount++;
		}
	
	
		if(strcmp(inputString,lastHopAddr) == 0)
			printf("\nTraceroute is successful\n");
			else{
	
				printf("\nTraceroute is unsuccessful\n");
				getPrefix(destOctet1, destOctet2,destOctet3,destOctet4 ,destLength, destFinal);
				getPrefix(lastHopOctet1, lastHopOctet2,lastHopOctet3,lastHopOctet4 ,lastHopLength, lastHopFinal);
				strcpy(destBGPPrefix,getLongestPrefixMatch(destFinal));
				strcpy(lastHopBGPPrefix,getLongestPrefixMatch(lastHopFinal));
			//	printf("\nThe BGPPrefix of destination is %s\n", destBGPPrefix);
			//	printf("\nThe BGPPrefix of last non-starred hop is %s\n", lastHopBGPPrefix);
				
				strcpy(destBGPPrefix24, initializer);
				//converting lastHopFinal into a string 
				for(i = 0; i < 32; i++){
					if(destFinal[i] == 0)
						strcat(destBGPPrefix24, "0");
					else if(destFinal[i] == 1)
						strcat(destBGPPrefix24,"1");
				}
			strncpy(dest24,destBGPPrefix24,24);
			dest24[24] = '\0';
			
			strcpy(lastHopBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(i = 0; i < 32; i++){
					if(lastHopFinal[i] == 0)
						strcat(lastHopBGPPrefix24, "0");
					else if(lastHopFinal[i] == 1)
						strcat(lastHopBGPPrefix24,"1");
				}
				
				strncpy(lastHop24,lastHopBGPPrefix24,24);
				lastHop24[24] = '\0';
				printf("\n---------------------------------------------------------------");
			
				// Reaches the BGP prefix and /24
				if(strcmp(destBGPPrefix,lastHopBGPPrefix) == 0)
					printf("\nThe packet reached the destination's BGP router");
				else
					printf("\nThe packet didn't reach the destination's BGP router");
				if(strcmp(dest24,lastHop24) == 0)
					printf("\nThe lastHop addr and the destination share a /24");
				else
					printf("\nThe lastHop addr and the destination do not share a /24");
				printf("\n---------------------------------------------------------------");	
				
			/*	// Reaches the domain
				if(strcmp(destNameDomain,lastHopHostNameDomain) == 0)
					printf("\nThe packet reached the destination's domain");
				else
					printf("\nThe packet didn't reach the destination's domain");
				printf("\n---------------------------------------------------------------");
			*/	
				
				// If the maximum limit of 64 hops is reached
				if(strcmp(numberOfHops, "64") == 0)
					printf("\nThe maximum limit of 64 hops is reached");
				printf("\n---------------------------------------------------------------");
			
				//If one of !H, !X, !P and !N appear in the traceroute output
				if(isUnReachable == 1 && prohibited != NULL)
					if(strcmp(prohibited,"!H") == 0)
						printf("\nTraceroute is incomplete because Host is unreachable");
					else if(strcmp(prohibited,"!X") == 0)
						printf("\nTraceroute is incomplete because communication is administratively prohibited");	
					else if(strcmp(prohibited,"!N") == 0)
						printf("\nTraceroute is incomplete because Network is unreachable");
					else if(strcmp(prohibited,"!P") == 0)
						printf("\nTraceroute is incomplete because Protocol is unreachable");
				printf("\n---------------------------------------------------------------");
		}	
			//Number of Hops traversed
			printf("\nTotal number of Hops traversed is %s",numberOfHops);
			printf("\n---------------------------------------------------------------");
			
			//Number of Starred Hops traversed
			printf("\nTotal number of Starred Hops traversed is %d",numberOfStarredHops);
			printf("\n Percentage of hops that only returned stars is %f", (((float)(numberOfStarredHops))/atof(numberOfHops)));
			
			printf("\n---------------------------------------------------------------");
			// Number of Unique Networks traversed
			
			for(i = 0; i< uniqueHostAddrs; i++){
				
				lastHopCount = 1;
				strcpy(lhTok,lastHopAddrs[i]);
				lastHopToken = strtok(lhTok,".");
	
				while(lastHopToken != NULL){
	
					if(lastHopCount == 1)	
						lastHopOctet1 = atoi(lastHopToken);
					else if(lastHopCount == 2)	
						lastHopOctet2 = atoi(lastHopToken);
					else if(lastHopCount == 3)	
						lastHopOctet3 = atoi(lastHopToken);
					else if(lastHopCount == 4)	
						lastHopOctet4 = atoi(lastHopToken);
		
					lastHopToken = strtok(NULL,".");
					lastHopCount++;
				}
				
				getPrefix(lastHopOctet1, lastHopOctet2,lastHopOctet3,lastHopOctet4 ,lastHopLength, lastHopFinal);
				strcpy(lastHopBGPPrefix,getLongestPrefixMatch(lastHopFinal));
				strcpy(lastHopBGPPrefixes[i],lastHopBGPPrefix);
			
				strcpy(lastHopBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(j = 0; j < 32; j++){
					if(lastHopFinal[j] == 0)
						strcat(lastHopBGPPrefix24, "0");
					else if(lastHopFinal[j] == 1)
						strcat(lastHopBGPPrefix24,"1");
				}
				
				strncpy(lastHopBGPPrefixes24[i],lastHopBGPPrefix24,24);
				lastHopBGPPrefixes24[i] += '\0';
				
			}
				//calculating the actual number of BGP prefixes
			for(i = 0; i< uniqueHostAddrs; i++){
				for(j = i+1; j< uniqueHostAddrs; j++){
					
					if( strcmp(lastHopBGPPrefixes[i],initializer) != 0 ){
						if(strcmp(lastHopBGPPrefixes[i] ,lastHopBGPPrefixes[j]) == 0 ){
							//uniqueBGPPrefixCount++;
							strcpy(lastHopBGPPrefixes[j], initializer);
							if(j == uniqueHostAddrs-1){
								//printf("\n%s",lastHopBGPPrefixes[i]);
								//printf("\n%s",lastHopAddrs[i]);
								uniqueBGPPrefixCount++;
							}
						}
						else if(j == uniqueHostAddrs-1){
							//printf("\n%s",lastHopBGPPrefixes[i]);
							//printf("\n%s",lastHopAddrs[i]);
							uniqueBGPPrefixCount++;
						}
					}
				}
				
				if(i == uniqueHostAddrs - 1 && strcmp(lastHopBGPPrefixes[i],initializer) != 0 )
					uniqueBGPPrefixCount++;
			}
			
			//calculating the number of unique domains
			for(i = 0; i< uniqueHostAddrs; i++){
				for(j = i+1; j< uniqueHostAddrs; j++){
					if(strcmp(lastHopBGPPrefixes24[i],initializer) != 0 ){
						if(strcmp(lastHopBGPPrefixes24[i],lastHopBGPPrefixes24[j]) == 0 ){
							//uniqueBGPPrefixCount24++;
							strcpy(lastHopBGPPrefixes24[j], initializer);
							if(j == uniqueHostAddrs-1){
								//printf("\n%s",lastHopBGPPrefixes24[i]);
								printf("\n%s",lastHopAddrs[i]);
								uniqueBGPPrefixCount24++;
							}
						}
						else if(j == uniqueHostAddrs-1){
							//printf("\n%s",lastHopBGPPrefixes24[i]);
							printf("\n%s",lastHopAddrs[i]);
							uniqueBGPPrefixCount24++;
						}
					}
				}
				if(i == uniqueHostAddrs - 1 && strcmp(lastHopBGPPrefixes24[i],initializer) != 0 ){
					uniqueBGPPrefixCount24++;
					printf("\n%s",lastHopAddrs[i]);
				}
			}
			
			
			printf("\nNumber of unique BGP Prefixes traversed is %d", uniqueBGPPrefixCount);
			printf("\nNumber of unique /24s traversed is %d", uniqueBGPPrefixCount24);
			printf("\n---------------------------------------------------------------");	
			
			//Total number of unique domains
				for(i = 0; i< uniqueHostNames; i++){
					TLDINIT(lastHopHostNames[i],NULL,domainName);
					strcpy(lastHopDomains[i],domainName);
				}
			
			//calculating the number of unique domains
			for(i = 0; i< uniqueHostNames; i++){
				for(j = i+1; j< uniqueHostNames; j++){
					if(strcmp(lastHopDomains[i],initializer) != 0 ){
						if(strcmp(lastHopDomains[i],lastHopDomains[j]) == 0 ){
							//uniqueBGPPrefixCount24++;
							strcpy(lastHopDomains[j], initializer);
							if(j == uniqueHostNames-1){
								//printf("\n%s",lastHopBGPPrefixes24[i]);
								printf("\n%s",lastHopDomains[i]);
								uniqueDomainCount++;
							}
						}
						else if(j == uniqueHostNames-1){
							//printf("\n%s",lastHopBGPPrefixes24[i]);
							printf("\n%s",lastHopDomains[i]);
							uniqueDomainCount++;
						}
					}
				}
				if(i == uniqueHostNames - 1 && strcmp(lastHopDomains[i],initializer) != 0 ){
					uniqueDomainCount++;
					printf("\n%s",lastHopDomains[i]);
				}
			}
			
			printf("\nNumber of unique domains traversed is %d", uniqueDomainCount);
			printf("\n---------------------------------------------------------------");	
			
			
			// Total time taken to finish traceroute
			printf("\nThe Total Response time is %f ms\n", totalResTime);
		printf("\n***************************************************************");	
		printf("\n***************************************************************\n");		
		
			
	}
	
	else if (choice == 3){
		printf("\nEnter the file containing IP Addresses: ");
		scanf("%s",fileInput);
		
		fp4 = fopen(fileInput,"r");
		
		while(fgets(file4,FILE_LEN1,fp4)!= NULL){
			
			i = 0;
			//checking for '.' '-' ,letters and numbers
			a = file4;
			while( i++ < strlen(file4)){
	
				if( ((*a) == '.' )|| (*a == '-') || ((int)(*a) > 47 && (int)(*a) < 58) || ((int)(*a) > 64 && (int)(*a) < 91) || ((int)(*a) > 96 && (int)(*a) < 123) || (*a) == '\n' || (*a) == '\0'){
					if( ((*a) == '.') )
						dotCount++;
				}	
				else{
					printf("\nInvalid characters found\n");
					exit(0);
				}		
				a = a + 1;
			}
	
			//chomping the linefeed
			if(file4[strlen(file4) - 1] == '\n' )
				file4[strlen(file4) - 1] = '\0';
	
			
			strcpy(inputString,file4);
			executeTraceroute(inputString);
			
		/*	
			//For calculating hostname of an ip addr
			strcpy(destNameDomain, initializer);
			strcpy(lastHopHostNameDomain, initializer);
			strcpy(destName, initializer);
			inet_pton(AF_INET, inputString, &addrlist);
			inputAddr = gethostbyaddr(&addrlist, sizeof addrlist, AF_INET);
			strcpy(destName,inputAddr->h_name);
			//printf("\nHostname is %s",destName);
		
			//For calculating effective TLD of input ip addr and lasthophostname
			TLDINIT(destName,NULL,destNameDomain);
			TLDINIT(lastHopHostName, NULL,lastHopHostNameDomain);
		
		*/	
			if( remove( "output.txt" ) != 0 )
  			 	 perror( "Error deleting file" );

			//For tokenizing the input addr
			destCount = 1;
			strcpy(isTok,inputString);
			destToken = strtok(isTok,".");
	
			while(destToken != NULL){
	
				if(destCount == 1)	
					destOctet1 = atoi(destToken);
				else if(destCount == 2)	
					destOctet2 = atoi(destToken);
				else if(destCount == 3)	
					destOctet3 = atoi(destToken);
				else if(destCount == 4)	
					destOctet4 = atoi(destToken);
		
				destToken = strtok(NULL,".");
				destCount++;
			}
	
			//For tokenizing the lastHop addr
			lastHopCount = 1;
			strcpy(lhTok,lastHopAddr);
			lastHopToken = strtok(lhTok,".");
	
			while(lastHopToken != NULL){
	
				if(lastHopCount == 1)	
					lastHopOctet1 = atoi(lastHopToken);
				else if(lastHopCount == 2)	
					lastHopOctet2 = atoi(lastHopToken);
				else if(lastHopCount == 3)	
					lastHopOctet3 = atoi(lastHopToken);
				else if(lastHopCount == 4)	
					lastHopOctet4 = atoi(lastHopToken);
		
				lastHopToken = strtok(NULL,".");
				lastHopCount++;
			}
			
			//chomping the linefeed
			if(inputString[strlen(inputString) - 1] == '\n' )
				inputString[strlen(inputString) - 1] = '\0';
				
			if(strcmp(inputString,lastHopAddr) == 0)
				printf("\nTraceroute is successful\n");
			else{
	
				printf("\nTraceroute is unsuccessful\n");
				getPrefix(destOctet1, destOctet2,destOctet3,destOctet4 ,destLength, destFinal);
				getPrefix(lastHopOctet1, lastHopOctet2,lastHopOctet3,lastHopOctet4 ,lastHopLength, lastHopFinal);
				strcpy(destBGPPrefix,getLongestPrefixMatch(destFinal));
				strcpy(lastHopBGPPrefix,getLongestPrefixMatch(lastHopFinal));
			//	printf("\nThe BGPPrefix of destination is %s\n", destBGPPrefix);
			//	printf("\nThe BGPPrefix of last non-starred hop is %s\n", lastHopBGPPrefix);
				
				strcpy(destBGPPrefix24, initializer);
				//converting lastHopFinal into a string 
				for(i = 0; i < 32; i++){
					if(destFinal[i] == 0)
						strcat(destBGPPrefix24, "0");
					else if(destFinal[i] == 1)
						strcat(destBGPPrefix24,"1");
				}
			strncpy(dest24,destBGPPrefix24,24);
			dest24[24] = '\0';
			
			strcpy(lastHopBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(i = 0; i < 32; i++){
					if(lastHopFinal[i] == 0)
						strcat(lastHopBGPPrefix24, "0");
					else if(lastHopFinal[i] == 1)
						strcat(lastHopBGPPrefix24,"1");
				}
				
				strncpy(lastHop24,lastHopBGPPrefix24,24);
				lastHop24[24] = '\0';
				printf("\n---------------------------------------------------------------");
			
				// Reaches the BGP prefix and /24
				if(strcmp(destBGPPrefix,lastHopBGPPrefix) == 0)
					printf("\nThe packet reached the destination's BGP router");
				else
					printf("\nThe packet didn't reach the destination's BGP router");
				if(strcmp(dest24,lastHop24) == 0)
					printf("\nThe lastHop addr and the destination share a /24");
				else
					printf("\nThe lastHop addr and the destination do not share a /24");
				printf("\n---------------------------------------------------------------");	
				
		/*		// Reaches the domain
				if(strcmp(destNameDomain,lastHopHostNameDomain) == 0)
					printf("\nThe packet reached the destination's domain");
				else
					printf("\nThe packet didn't reach the destination's domain");
				printf("\n---------------------------------------------------------------");
		*/		
				// If the maximum limit of 64 hops is reached
				if(strcmp(numberOfHops, "64") == 0)
					printf("\nThe maximum limit of 64 hops is reached");
				printf("\n---------------------------------------------------------------");
			
				//If one of !H, !X, !P and !N appear in the traceroute output
				if(isUnReachable == 1 && prohibited != NULL)
					if(strcmp(prohibited,"!H") == 0)
						printf("\nTraceroute is incomplete because Host is unreachable");
					else if(strcmp(prohibited,"!X") == 0)
						printf("\nTraceroute is incomplete because communication is administratively prohibited");	
					else if(strcmp(prohibited,"!N") == 0)
						printf("\nTraceroute is incomplete because Network is unreachable");
					else if(strcmp(prohibited,"!P") == 0)
						printf("\nTraceroute is incomplete because Protocol is unreachable");
				printf("\n---------------------------------------------------------------");
		}	
			//Number of Hops traversed
			printf("\nTotal number of Hops traversed is %s",numberOfHops);
			printf("\n---------------------------------------------------------------");
			
			//Number of Starred Hops traversed
			printf("\nTotal number of Starred Hops traversed is %d",numberOfStarredHops);
			printf("\n Percentage of hops that only returned stars is %f", (((float)(numberOfStarredHops))/atof(numberOfHops)));
			
			printf("\n---------------------------------------------------------------");
			// Number of Unique Networks traversed
			
			for(i = 0; i< uniqueHostAddrs; i++){
				
				lastHopCount = 1;
				strcpy(lhTok,lastHopAddrs[i]);
				lastHopToken = strtok(lhTok,".");
	
				while(lastHopToken != NULL){
	
					if(lastHopCount == 1)	
						lastHopOctet1 = atoi(lastHopToken);
					else if(lastHopCount == 2)	
						lastHopOctet2 = atoi(lastHopToken);
					else if(lastHopCount == 3)	
						lastHopOctet3 = atoi(lastHopToken);
					else if(lastHopCount == 4)	
						lastHopOctet4 = atoi(lastHopToken);
		
					lastHopToken = strtok(NULL,".");
					lastHopCount++;
				}
				
				getPrefix(lastHopOctet1, lastHopOctet2,lastHopOctet3,lastHopOctet4 ,lastHopLength, lastHopFinal);
				strcpy(lastHopBGPPrefix,getLongestPrefixMatch(lastHopFinal));
				strcpy(lastHopBGPPrefixes[i],lastHopBGPPrefix);
			
				strcpy(lastHopBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(j = 0; j < 32; j++){
					if(lastHopFinal[j] == 0)
						strcat(lastHopBGPPrefix24, "0");
					else if(lastHopFinal[j] == 1)
						strcat(lastHopBGPPrefix24,"1");
				}
				
				strncpy(lastHopBGPPrefixes24[i],lastHopBGPPrefix24,24);
				lastHopBGPPrefixes24[i] += '\0';
				
			}
				//calculating the actual number of BGP prefixes
			for(i = 0; i< uniqueHostAddrs; i++){
				for(j = i+1; j< uniqueHostAddrs; j++){
					
					if( strcmp(lastHopBGPPrefixes[i],initializer) != 0 ){
						if(strcmp(lastHopBGPPrefixes[i] ,lastHopBGPPrefixes[j]) == 0 ){
							//uniqueBGPPrefixCount++;
							strcpy(lastHopBGPPrefixes[j], initializer);
							if(j == uniqueHostAddrs-1){
								//printf("\n%s",lastHopBGPPrefixes[i]);
								//printf("\n%s",lastHopAddrs[i]);
								uniqueBGPPrefixCount++;
							}
						}
						else if(j == uniqueHostAddrs-1){
							//printf("\n%s",lastHopBGPPrefixes[i]);
							//printf("\n%s",lastHopAddrs[i]);
							uniqueBGPPrefixCount++;
						}
					}
				}
				
				if(i == uniqueHostAddrs - 1 && strcmp(lastHopBGPPrefixes[i],initializer) != 0 )
					uniqueBGPPrefixCount++;
			}
			
			//calculating the number of unique domains
			for(i = 0; i< uniqueHostAddrs; i++){
				for(j = i+1; j< uniqueHostAddrs; j++){
					if(strcmp(lastHopBGPPrefixes24[i],initializer) != 0 ){
						if(strcmp(lastHopBGPPrefixes24[i],lastHopBGPPrefixes24[j]) == 0 ){
							//uniqueBGPPrefixCount24++;
							strcpy(lastHopBGPPrefixes24[j], initializer);
							if(j == uniqueHostAddrs-1){
								//printf("\n%s",lastHopBGPPrefixes24[i]);
								printf("\n%s",lastHopAddrs[i]);
								uniqueBGPPrefixCount24++;
							}
						}
						else if(j == uniqueHostAddrs-1){
							//printf("\n%s",lastHopBGPPrefixes24[i]);
							printf("\n%s",lastHopAddrs[i]);
							uniqueBGPPrefixCount24++;
						}
					}
				}
				if(i == uniqueHostAddrs - 1 && strcmp(lastHopBGPPrefixes24[i],initializer) != 0 ){
					uniqueBGPPrefixCount24++;
					printf("\n%s",lastHopAddrs[i]);
				}
			}
			
			
			printf("\nNumber of unique BGP Prefixes traversed is %d", uniqueBGPPrefixCount);
			printf("\nNumber of unique /24s traversed is %d", uniqueBGPPrefixCount24);
			printf("\n---------------------------------------------------------------");	
			
			//Total number of unique domains
				for(i = 0; i< uniqueHostNames; i++){
					strcpy(domainName, initializer);
					TLDINIT(lastHopHostNames[i],NULL,domainName);
					strcpy(lastHopDomains[i],domainName);
				}
			
			//calculating the number of unique domains
			for(i = 0; i< uniqueHostNames; i++){
				for(j = i+1; j< uniqueHostNames; j++){
					if(strcmp(lastHopDomains[i],initializer) != 0 ){
						if(strcmp(lastHopDomains[i],lastHopDomains[j]) == 0 ){
							//uniqueBGPPrefixCount24++;
							strcpy(lastHopDomains[j], initializer);
							if(j == uniqueHostNames-1){
								//printf("\n%s",lastHopBGPPrefixes24[i]);
								printf("\n%s",lastHopDomains[i]);
								uniqueDomainCount++;
							}
						}
						else if(j == uniqueHostNames-1){
							//printf("\n%s",lastHopBGPPrefixes24[i]);
							printf("\n%s",lastHopDomains[i]);
							uniqueDomainCount++;
						}
					}
				}
				if(i == uniqueHostNames - 1 && strcmp(lastHopDomains[i],initializer) != 0 ){
					uniqueDomainCount++;
					printf("\n%s",lastHopDomains[i]);
				}
			}
			
			printf("\nNumber of unique domains traversed is %d", uniqueDomainCount);
			printf("\n---------------------------------------------------------------");	
			
			
			// Total time taken to finish traceroute
			printf("\nThe Total Response time is %f ms\n", totalResTime);
		printf("\n***************************************************************");	
		printf("\n***************************************************************\n");		
		
			
		}
		fclose(fp4);
		
	}
	else if(choice == 4){
		printf("\nEnter the file containing Hostnames: ");
		scanf("%s",fileInput);
		
		fp5 = fopen(fileInput,"r");
		
		while(fgets(file3,FILE_LEN,fp5)!= NULL){
			
			i = 0;
			//checking for '.' '-' ,letters and numbers
			a = file3;
			while( i++ < strlen(file4)){
	
				if( ((*a) == '.' )|| (*a == '-') || ((int)(*a) > 47 && (int)(*a) < 58) || ((int)(*a) > 64 && (int)(*a) < 91) || ((int)(*a) > 96 && (int)(*a) < 123) || (*a) == '\n' || (*a) == '\0'){
					if( ((*a) == '.') )
						dotCount++;
				}	
				else{
					printf("\nInvalid characters found\n");
					exit(0);
				}		
				a = a + 1;
			}
			
				
			//chomping the linefeed
			if(file3[strlen(file3) - 1] == '\n' )
				file3[strlen(file3) - 1] = '\0';
	
			inputAddr = gethostbyname(file3);
			addrlist = (struct in_addr **)inputAddr->h_addr_list;
	
			strcpy(inputString,inet_ntoa(*addrlist[0]));
			printf("\nThe ip addr for the input string is %s", inputString);
			executeTraceroute(inputString);
			
			if( remove( "output.txt" ) != 0 )
  			 	 perror( "Error deleting file" );
			
		/*	//For calculating effective TLD of input ip addr and lasthophostname
			TLDINIT(file3,NULL,destNameDomain);
			TLDINIT(lastHopHostName,NULL,lastHopHostNameDomain);
		*/	
			//For tokenizing the input addr
			destCount = 1;
			strcpy(isTok,inputString);
			destToken = strtok(isTok,".");
	
			while(destToken != NULL){
	
				if(destCount == 1)	
					destOctet1 = atoi(destToken);
				else if(destCount == 2)	
					destOctet2 = atoi(destToken);
				else if(destCount == 3)	
					destOctet3 = atoi(destToken);
				else if(destCount == 4)	
					destOctet4 = atoi(destToken);
		
				destToken = strtok(NULL,".");
				destCount++;
			}
	
			//For tokenizing the lastHop addr
			lastHopCount = 1;
			strcpy(lhTok,lastHopAddr);
			lastHopToken = strtok(lhTok,".");
	
			while(lastHopToken != NULL){
	
				if(lastHopCount == 1)	
					lastHopOctet1 = atoi(lastHopToken);
				else if(lastHopCount == 2)	
					lastHopOctet2 = atoi(lastHopToken);
				else if(lastHopCount == 3)	
					lastHopOctet3 = atoi(lastHopToken);
				else if(lastHopCount == 4)	
					lastHopOctet4 = atoi(lastHopToken);
		
				lastHopToken = strtok(NULL,".");
				lastHopCount++;
			}
			
			//chomping the linefeed
			if(inputString[strlen(inputString) - 1] == '\n' )
				inputString[strlen(inputString) - 1] = '\0';
	
			if(strcmp(inputString,lastHopAddr) == 0)
				printf("\nTraceroute is successful\n");
			else{
	
				printf("\nTraceroute is unsuccessful\n");
				getPrefix(destOctet1, destOctet2,destOctet3,destOctet4 ,destLength, destFinal);
				getPrefix(lastHopOctet1, lastHopOctet2,lastHopOctet3,lastHopOctet4 ,lastHopLength, lastHopFinal);
				strcpy(destBGPPrefix,getLongestPrefixMatch(destFinal));
				strcpy(lastHopBGPPrefix,getLongestPrefixMatch(lastHopFinal));
			//	printf("\nThe BGPPrefix of destination is %s\n", destBGPPrefix);
			//	printf("\nThe BGPPrefix of last non-starred hop is %s\n", lastHopBGPPrefix);
				
				strcpy(destBGPPrefix24, initializer);
				//converting lastHopFinal into a string 
				for(i = 0; i < 32; i++){
					if(destFinal[i] == 0)
						strcat(destBGPPrefix24, "0");
					else if(destFinal[i] == 1)
						strcat(destBGPPrefix24,"1");
				}
			strncpy(dest24,destBGPPrefix24,24);
			dest24[24] = '\0';
			
			strcpy(lastHopBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(i = 0; i < 32; i++){
					if(lastHopFinal[i] == 0)
						strcat(lastHopBGPPrefix24, "0");
					else if(lastHopFinal[i] == 1)
						strcat(lastHopBGPPrefix24,"1");
				}
				
				strncpy(lastHop24,lastHopBGPPrefix24,24);
				lastHop24[24] = '\0';
				printf("\n---------------------------------------------------------------");
			
				// Reaches the BGP prefix and /24
				if(strcmp(destBGPPrefix,lastHopBGPPrefix) == 0)
					printf("\nThe packet reached the destination's BGP router");
				else
					printf("\nThe packet didn't reach the destination's BGP router");
				if(strcmp(dest24,lastHop24) == 0)
					printf("\nThe lastHop addr and the destination share a /24");
				else
					printf("\nThe lastHop addr and the destination do not share a /24");
				printf("\n---------------------------------------------------------------");	
				
			/*	// Reaches the domain
				if(strcmp(destNameDomain,lastHopHostNameDomain) == 0)
					printf("\nThe packet reached the destination's domain");
				else
					printf("\nThe packet didn't reach the destination's domain");
				printf("\n---------------------------------------------------------------");
			*/	
				
				// If the maximum limit of 64 hops is reached
				if(strcmp(numberOfHops, "64") == 0)
					printf("\nThe maximum limit of 64 hops is reached");
				printf("\n---------------------------------------------------------------");
			
				//If one of !H, !X, !P and !N appear in the traceroute output
				if(isUnReachable == 1 && prohibited != NULL)
					if(strcmp(prohibited,"!H") == 0)
						printf("\nTraceroute is incomplete because Host is unreachable");
					else if(strcmp(prohibited,"!X") == 0)
						printf("\nTraceroute is incomplete because communication is administratively prohibited");	
					else if(strcmp(prohibited,"!N") == 0)
						printf("\nTraceroute is incomplete because Network is unreachable");
					else if(strcmp(prohibited,"!P") == 0)
						printf("\nTraceroute is incomplete because Protocol is unreachable");
				printf("\n---------------------------------------------------------------");
		}	
			//Number of Hops traversed
			printf("\nTotal number of Hops traversed is %s",numberOfHops);
			printf("\n---------------------------------------------------------------");
			
			//Number of Starred Hops traversed
			printf("\nTotal number of Starred Hops traversed is %d",numberOfStarredHops);
			printf("\n Percentage of hops that only returned stars is %f", (((float)(numberOfStarredHops))/atof(numberOfHops)));
			
			printf("\n---------------------------------------------------------------");
			// Number of Unique Networks traversed
			
			for(i = 0; i< uniqueHostAddrs; i++){
				
				lastHopCount = 1;
				strcpy(lhTok,lastHopAddrs[i]);
				lastHopToken = strtok(lhTok,".");
	
				while(lastHopToken != NULL){
	
					if(lastHopCount == 1)	
						lastHopOctet1 = atoi(lastHopToken);
					else if(lastHopCount == 2)	
						lastHopOctet2 = atoi(lastHopToken);
					else if(lastHopCount == 3)	
						lastHopOctet3 = atoi(lastHopToken);
					else if(lastHopCount == 4)	
						lastHopOctet4 = atoi(lastHopToken);
		
					lastHopToken = strtok(NULL,".");
					lastHopCount++;
				}
				
				getPrefix(lastHopOctet1, lastHopOctet2,lastHopOctet3,lastHopOctet4 ,lastHopLength, lastHopFinal);
				strcpy(lastHopBGPPrefix,getLongestPrefixMatch(lastHopFinal));
				strcpy(lastHopBGPPrefixes[i],lastHopBGPPrefix);
			
				strcpy(lastHopBGPPrefix24, initializer);
				
				//converting lastHopFinal into a string 
				for(j = 0; j < 32; j++){
					if(lastHopFinal[j] == 0)
						strcat(lastHopBGPPrefix24, "0");
					else if(lastHopFinal[j] == 1)
						strcat(lastHopBGPPrefix24,"1");
				}
				
				strncpy(lastHopBGPPrefixes24[i],lastHopBGPPrefix24,24);
				lastHopBGPPrefixes24[i] += '\0';
				
			}
				//calculating the actual number of BGP prefixes
			for(i = 0; i< uniqueHostAddrs; i++){
				for(j = i+1; j< uniqueHostAddrs; j++){
					
					if( strcmp(lastHopBGPPrefixes[i],initializer) != 0 ){
						if(strcmp(lastHopBGPPrefixes[i] ,lastHopBGPPrefixes[j]) == 0 ){
							//uniqueBGPPrefixCount++;
							strcpy(lastHopBGPPrefixes[j], initializer);
							if(j == uniqueHostAddrs-1){
								//printf("\n%s",lastHopBGPPrefixes[i]);
								//printf("\n%s",lastHopAddrs[i]);
								uniqueBGPPrefixCount++;
							}
						}
						else if(j == uniqueHostAddrs-1){
							//printf("\n%s",lastHopBGPPrefixes[i]);
							//printf("\n%s",lastHopAddrs[i]);
							uniqueBGPPrefixCount++;
						}
					}
				}
				
				if(i == uniqueHostAddrs - 1 && strcmp(lastHopBGPPrefixes[i],initializer) != 0 )
					uniqueBGPPrefixCount++;
			}
			
			//calculating the number of unique domains
			for(i = 0; i< uniqueHostAddrs; i++){
				for(j = i+1; j< uniqueHostAddrs; j++){
					if(strcmp(lastHopBGPPrefixes24[i],initializer) != 0 ){
						if(strcmp(lastHopBGPPrefixes24[i],lastHopBGPPrefixes24[j]) == 0 ){
							//uniqueBGPPrefixCount24++;
							strcpy(lastHopBGPPrefixes24[j], initializer);
							if(j == uniqueHostAddrs-1){
								//printf("\n%s",lastHopBGPPrefixes24[i]);
								printf("\n%s",lastHopAddrs[i]);
								uniqueBGPPrefixCount24++;
							}
						}
						else if(j == uniqueHostAddrs-1){
							//printf("\n%s",lastHopBGPPrefixes24[i]);
							printf("\n%s",lastHopAddrs[i]);
							uniqueBGPPrefixCount24++;
						}
					}
				}
				if(i == uniqueHostAddrs - 1 && strcmp(lastHopBGPPrefixes24[i],initializer) != 0 ){
					uniqueBGPPrefixCount24++;
					printf("\n%s",lastHopAddrs[i]);
				}
			}
			
			
			printf("\nNumber of unique BGP Prefixes traversed is %d", uniqueBGPPrefixCount);
			printf("\nNumber of unique /24s traversed is %d", uniqueBGPPrefixCount24);
			printf("\n---------------------------------------------------------------");	
			
			//Total number of unique domains
				for(i = 0; i< uniqueHostNames; i++){
					TLDINIT(lastHopHostNames[i],NULL,domainName);
					strcpy(lastHopDomains[i],domainName);
				}
			
			//calculating the number of unique domains
			for(i = 0; i< uniqueHostNames; i++){
				for(j = i+1; j< uniqueHostNames; j++){
					if(strcmp(lastHopDomains[i],initializer) != 0 ){
						if(strcmp(lastHopDomains[i],lastHopDomains[j]) == 0 ){
							//uniqueBGPPrefixCount24++;
							strcpy(lastHopDomains[j], initializer);
							if(j == uniqueHostNames-1){
								//printf("\n%s",lastHopBGPPrefixes24[i]);
								printf("\n%s",lastHopDomains[i]);
								uniqueDomainCount++;
							}
						}
						else if(j == uniqueHostNames-1){
							//printf("\n%s",lastHopBGPPrefixes24[i]);
							printf("\n%s",lastHopDomains[i]);
							uniqueDomainCount++;
						}
					}
				}
				if(i == uniqueHostNames - 1 && strcmp(lastHopDomains[i],initializer) != 0 ){
					uniqueDomainCount++;
					printf("\n%s",lastHopDomains[i]);
				}
			}
			
			printf("\nNumber of unique domains traversed is %d", uniqueDomainCount);
			printf("\n---------------------------------------------------------------");	
			
			
			// Total time taken to finish traceroute
			printf("\nThe Total Response time is %f ms\n", totalResTime);
		printf("\n***************************************************************");	
		printf("\n***************************************************************\n");		
		
			
		}
			fclose(fp5);
	}
	else
		printf("\nPlease enter a correct choice");
	
	/*
	printf("\nThe IP addr of the lasthop of traceroute is %s\n", lastHopAddr);
	printf("\nThe Hostname of the lasthop of traceroute is %s\n", lastHopHostName);
	printf("\nThe Total Response time is %f ms\n", totalResTime);
	printf("\nThe Number of Hops traversed  is %d\n", nOfHops);
	printf("\nThe Number of Starred Hops traversed  is %d\n", numberOfStarredHops);
	printf("\nThe IP Addrs in the output are \n");
	*/
	for(i = 0; i< uniqueHostAddrs; i++)
		printf(" %d) %s \n",i, lastHopAddrs[i] );
		
	printf("\nThe HostNames in the output are \n");	
	
	for(i = 0; i< uniqueHostNames; i++){
		printf(" %d) %s ",i, lastHopHostNames[i] );
		TLDINIT(lastHopHostNames[i],NULL,domainName);
		printf("- %s \n", domainName);
	}

	
	if( remove( "output.txt" ) != 0 )
  		perror( "Error deleting file" );
	return 0;
}