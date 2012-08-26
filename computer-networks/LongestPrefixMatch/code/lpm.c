/* Program to construct a Binary trie of prefixes that stores the next hop addresses */

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <stdint.h>

#define rdtsc(x) __asm__ volatile ("rdtsc" : "=A" (x))


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

struct mTrieNode{
char *pdata, *sdata;
struct mTrieNode *first;
struct mTrieNode *second;
struct mTrieNode *third;
struct mTrieNode *fourth;
};

typedef struct mTrie{
long int size;
struct mTrieNode *root;
struct mTrieNode *curRoot;
} mTrie;



static BiTrie *trie ;
static mTrie *mtrie ;
static uint64_t a,b,y,z,p,q,r,s,unisum = 0,multisum = 0,u,u1,v,v1,g,g1,h,h1; 
//static unsigned long long max = 500000;
static unsigned int lpmuniarray[500000] ={} , updateuniarray[500000] = {} ,lpmmultiarray[500000] ={} , updatemultiarray[500000] = {} , arr;
static uint64_t ulpmcount = 0, mlpmcount = 0, uupdatecount = 0, mupdatecount = 0, len = 0;
static unsigned int ulpmmedian, mlpmmedian, uupdatemedian, mupdatemedian, median,min,max,mean;
static unsigned int ulpmmean, mlpmmean, uupdatemean, mupdatemean;
static unsigned int ulpmmin, mlpmmin, uupdatemin, mupdatemin;
static unsigned int ulpmmax, mlpmmax, uupdatemax, mupdatemax;
static struct TrieNode *node , *tempNode, *tempNode1,*tempNode2,*newnode, *newnode1;
static struct mTrieNode *mnode , *mtempNode,*mtempNode1, *mtempNode2,*mnewnode, *mnewnode1;
static long int prefixCount, lookupCount = 0,uprefixCount = 0, badprefixCount = 0;
static char *data, *mdata,*nextHop,*data1, *mdata1, *nextHop1, *tempStr,*charToStr,*tempStr2,*charToStr2,*tempStr3,*charToStr3, *bestMatch, *mbestMatch;
static FILE *filePointer, *filePointer2, *filePointer3;
static u_int16_t prefixOctet1 = 0,prefixOctet2 = 0,prefixOctet3 = 0,prefixOctet4 = 0, prefixLength = 0, mprefixLength = 0;
static u_int16_t lookupOctet1 = 0,lookupOctet2 = 0,lookupOctet3 = 0,lookupOctet4 = 0, lookupLength = 32, mlookupLength = 16;
static u_int16_t uprefixOctet1 = 0,uprefixOctet2 = 0,uprefixOctet3 = 0,uprefixOctet4 = 0, uprefixLength = 0, muprefixLength = 0;
static u_int16_t unextHopOctet1 = 0,unextHopOctet2 = 0,unextHopOctet3 = 0,unextHopOctet4 = 0;
static u_int16_t prefixFinal[32] = {}, lookupFinal[32] = {} , uprefixFinal[32] = {};
static char **mprefixFinal, **mlookupFinal, **muprefixFinal, *mflag;
static char c = 0, d = 0, e = 0, o;
static char *initializer = "", *lookupfile, *updatefile, *prefixfile;
static int newnodes = 0,mnewnodes = 0;
static int k = 0;

int getPrefix(u_int16_t preOct1, u_int16_t preOct2, u_int16_t preOct3, u_int16_t preOct4, int preLen, u_int16_t prefixFinal[],char **mprefixFinal){
	
	u_int16_t j = 0,w = 0, pos = 8, k = preOct1, l = preOct2, m = preOct3, n = preOct4;
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
	
	if(preLen % 2 == 0)
		mprefixLength = preLen / 2;
	else 
		mprefixLength = (preLen/2) + 1;
		
	//printf("\nprefix : ");
	
	//for(j = 0 ; j< 32; j++)
	//	printf("%d", prefix[j]);

	for(j = 0 ; j< preLen; j++) 
		prefixFinal[j] = prefix[j];

	for(j = 0 , w = 0; j< preLen, w < mprefixLength; j = j+2, w++) {
		
		if(preLen % 2 != 0 && j == preLen-1){
			strcpy(mprefixFinal[w],initializer);
			sprintf(mprefixFinal[w], "%d", prefixFinal[j]) ;
		}
		else{
			strcpy(mprefixFinal[w],initializer);
			sprintf(mprefixFinal[w], "%d%d", prefixFinal[j], prefixFinal[j+1]) ;
		}	
	}
	
	return 0;

}

void getLongestPrefixMatch(u_int16_t lookupFin[], char **mlookupFinal){
	
	int i = 0;
	a = b = y = z = 0;
	lookupCount++;
	bestMatch = (char *)calloc(1,20*sizeof(char));
	tempNode = (struct TrieNode *)calloc(1,sizeof(struct TrieNode));
	mbestMatch = (char *)calloc(1,20*sizeof(char));
	mtempNode = (struct mTrieNode *)calloc(1,sizeof(struct mTrieNode));
	
	tempNode = trie->root;
	rdtsc(a);
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
	rdtsc(b);
	
	if(strcmp(bestMatch,"")==0)
		printf(" cannot be routable from unibit trie\n");
	else	
	printf(" is routable through %s at %d from unibit trie\n", bestMatch, i);
	
	//printf("\nElapsed time is %llu cycles\n",(b-a));
	lpmuniarray[ulpmcount++] = (b-a);
	
	
	mtempNode = mtrie->root;
	rdtsc(y);
	for(i=0;i<mlookupLength;i++){
			
			if(strcmp(mlookupFinal[i], "00") == 0) {
				mtempNode = mtempNode->first;
					if(mtempNode == NULL) break;
					
					else{
						if(mtempNode->pdata != NULL)
							strcpy(mbestMatch,mtempNode->pdata);
						else if(mtempNode->sdata != NULL)
							strcpy(mbestMatch,mtempNode->sdata);
						else
							continue;
					}
			}
			
			
			else if(strcmp(mlookupFinal[i], "01") == 0) {
				mtempNode = mtempNode->second;
					if(mtempNode == NULL) break;
					
					else{
						if(mtempNode->pdata != NULL)
							strcpy(mbestMatch,mtempNode->pdata);
						else if(mtempNode->sdata != NULL)
							strcpy(mbestMatch,mtempNode->sdata);
						else
							continue;
					}
			}
			
			
			else if(strcmp(mlookupFinal[i], "10") == 0) {
				mtempNode = mtempNode->third;
					if(mtempNode == NULL) break;
					
					else{
						if(mtempNode->pdata != NULL)
							strcpy(mbestMatch,mtempNode->pdata);
						else if(mtempNode->sdata != NULL)
							strcpy(mbestMatch,mtempNode->sdata);
						else
							continue;
					}
			}
			
			
			else if(strcmp(mlookupFinal[i], "11") == 0) {
				mtempNode = mtempNode->fourth;
					if(mtempNode == NULL) break;
					
					else{
						if(mtempNode->pdata != NULL)
							strcpy(mbestMatch,mtempNode->pdata);
						else if(mtempNode->sdata != NULL)
							strcpy(mbestMatch,mtempNode->sdata);
						else
							continue;
					}
			}
	}
	rdtsc(z);
	
	if(strcmp(mbestMatch,"")==0)
		printf(" cannot be routable from multibit trie\n");
	else	
		printf(" is routable through %s at %d from multibit trie\n", mbestMatch, i);

	//printf("\nElapsed time is %llu cycles\n",(z-y));
	lpmmultiarray[mlpmcount++] = (z-y);


}

unsigned int getMedian(unsigned int arr[], uint64_t len){

	int i = 0, j =0;
	uint64_t length = 0, temp = 0;
	median = 0;
	length = len;

	for(i = 0; i< length; i++){
		for(j = 0; j< length; j++){
			
			if(arr[i] < arr[j]){
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			}
		}
	}
	
	
	if(length %2 == 0)
		median = (arr[length/2] + arr[(length/2) -1])/2;
	else
		median = arr[(length-1)/2];
	
	return median;

}

unsigned int getMean(unsigned int arr[], uint64_t len){

	int i = 0;
	uint64_t length = 0,sum = 0;
	mean = 0;
	length = len;

	for(i = 0; i< length; i++)
		sum += arr[i];
		
	mean = sum/length;
	return mean;

}

unsigned int getMin(unsigned int arr[], uint64_t len){

	int i = 0;
	uint64_t length = 0;
	min = arr[0];
	length = len;

	for(i = 0; i< length; i++)
		if(arr[i] < min)
			min = arr[i];

	return min;

}

unsigned int getMax(unsigned int arr[], uint64_t len){

	int i = 0, j =0;
	uint64_t length = 0;
	max = arr[0];
	length = len;

	for(i = 0; i< length; i++)
		if(arr[i] > max)
			max = arr[i];

	return max;

}

void BiTrieInit(BiTrie *trie ) {

	trie->size = 0;
	
	if((trie->root = (struct TrieNode *)calloc(1,sizeof(struct TrieNode))) == NULL)
			fprintf(stderr,"\nMemory cannot be allocated to trie->root\n");
	
	trie->root->left = NULL;
	trie->root->right = NULL;
	trie->curRoot = trie->root;
	return ;
	
}


void mTrieInit(mTrie *mtrie ) {

	mtrie->size = 0;
	
	if((mtrie->root = (struct mTrieNode *)calloc(1,sizeof(struct mTrieNode))) == NULL)
			fprintf(stderr,"\nMemory cannot be allocated to mtrie->root\n");

	mtrie->root->first = NULL;
	mtrie->root->second = NULL;
	mtrie->root->third = NULL;
	mtrie->root->fourth = NULL;
				
	mtrie->curRoot = mtrie->root;
	return ;
	
}

struct TrieNode* newNode(BiTrie *trie, char* data){
	
	if((newnode = (struct TrieNode *)calloc(1,sizeof(struct TrieNode))) == NULL)
			fprintf(stderr,"\nMemory cannot be allocated to newnode\n");
				
				if(data != NULL){ 
					if((newnode->data = (char *)calloc(1,20*sizeof(char)))==NULL)
						fprintf(stderr,"\nMemory cannot be allocated to newnode->data\n");
					strcpy(newnode->data, (char *)data);
				}
				else 
					newnode->data = (char *)data;
				
				newnode->left = NULL;
				newnode->right = NULL;
				trie->size++;
				return newnode;

}

struct mTrieNode* mnewNode(mTrie *mtrie, char* mdata, int id1){
	
	
	
	if((mnewnode = (struct mTrieNode *)calloc(1,sizeof(struct mTrieNode))) == NULL)
			fprintf(stderr,"\nMemory cannot be allocated to mnewnode\n");
				
				if(mdata != NULL){ 
					if(id1 == 1){
						if((mnewnode->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnewnode->data\n");
						strcpy(mnewnode->sdata, (char *)mdata);
						mnewnode->pdata = NULL;
					}
					else{
						if((mnewnode->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnewnode->data\n");
						strcpy(mnewnode->pdata, (char *)mdata);
						mnewnode->sdata = NULL;
					}
				}
				else {
					mnewnode->pdata = (char *)mdata;
					mnewnode->sdata = (char *)mdata;
				}
				mnewnode->first = NULL;
				mnewnode->second = NULL;
				mnewnode->third = NULL;
				mnewnode->fourth = NULL;
				mtrie->size++;
				return mnewnode;


}


struct TrieNode* updatenewNode(BiTrie *trie, char* data){
	
	newnodes++;
	if((newnode1 = (struct TrieNode *)calloc(1,sizeof(struct TrieNode))) == NULL)
			fprintf(stderr,"\nMemory cannot be allocated to newnode1\n");
				
				if(data != NULL){ 
				
					printf("\n New node created through update");
					printf("\nThe previous value at this address was %s", newnode1->data);
				
					if((newnode1->data = (char *)calloc(1,20*sizeof(char)))==NULL)
						fprintf(stderr,"\nMemory cannot be allocated to newnode1->data\n");
					
					strcpy(newnode1->data, (char *)data);
					printf("\nThe new value at this address is %s", newnode1->data);
				}
				else 
					newnode1->data = (char *)data;
				
				newnode1->left = NULL;
				newnode1->right = NULL;
				trie->size++;
				return newnode1;


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


struct mTrieNode* mTrieInsert(mTrie *mtrie, struct mTrieNode *mnode, char *mdata , char *mflag, int id ) {

	if(mnode == NULL) 
		mnode = mnewNode(mtrie,mdata, id);

	else {
		
		if( strcmp(mflag,"00") == 0) {
			if(mnode->first == NULL){
				mnode->first = mTrieInsert(mtrie, mnode->first, mdata , mflag,id);
				return mnode->first;
			}	
			else{
				if(mdata != NULL){
					if(id == 1){
						if((mnode->first->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnode->first->sdata\n");
						strcpy(mnode->first->sdata, (char *)mdata);
					}
					else{
						if((mnode->first->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnode->first->pdata\n");
						strcpy(mnode->first->pdata, (char *)mdata);
					}
						
				}
				return mnode->first;
			}	
		}	 
		
	
		else if( strcmp(mflag,"01") == 0) {
			if(mnode->second == NULL){
				mnode->second = mTrieInsert(mtrie, mnode->second, mdata , mflag, id);
				return mnode->second;
			}	
			else{
				if(mdata != NULL){
					if(id == 1){
						if((mnode->second->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnode->second->sdata\n");
						strcpy(mnode->second->sdata, (char *)mdata);
					}
					else{
						if((mnode->second->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnode->second->pdata\n");
						strcpy(mnode->second->pdata, (char *)mdata);
					}
						
				}
				return mnode->second;
			}
		}	 
		
		
		else if( strcmp(mflag,"10") == 0) {
			if(mnode->third == NULL){
				mnode->third = mTrieInsert(mtrie, mnode->third, mdata , mflag, id);
				return mnode->third;
			}	
			else{
				if(mdata != NULL){
					if(id == 1){
						if((mnode->third->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnode->first->sdata\n");
						strcpy(mnode->third->sdata, (char *)mdata);
					}
					else{
						if((mnode->third->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnode->first->pdata\n");
						strcpy(mnode->third->pdata, (char *)mdata);
					}
						
				}
				return mnode->third;
			}
		}	 
				
		else  { 
			if( strcmp(mflag,"11") == 0) {
				if(mnode->fourth == NULL){
					mnode->fourth = mTrieInsert(mtrie, mnode->fourth, mdata , mflag, id);
					return mnode->fourth;
				}	
				else{
				if(mdata != NULL){
					if(id == 1){
						if((mnode->fourth->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnode->first->sdata\n");
						strcpy(mnode->fourth->sdata, (char *)mdata);
					}
					else{
						if((mnode->fourth->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mnode->first->pdata\n");
						strcpy(mnode->fourth->pdata, (char *)mdata);
					}
						
				}
				return mnode->fourth;
			}
			}	 
		}
	}
	
	return mnode;
}

struct mTrieNode* mupdatenewNode(mTrie *mtrie, char* mdata1, int id2){
	
	mnewnodes++;
	if((mnewnode1 = (struct mTrieNode *)calloc(1,sizeof(struct mTrieNode))) == NULL)
			fprintf(stderr,"\nMemory cannot be allocated to mnewnode1\n");
				
				if(mdata1 != NULL){ 
					if(id2 == 1){
						printf("\n New node created through update");
						printf("\nThe previous value at this address was %s", mnewnode1->sdata);
				
							if((mnewnode1->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
									fprintf(stderr,"\nMemory cannot be allocated to mnewnode1->sdata\n");
								strcpy(mnewnode1->sdata, (char *)mdata1);
								mnewnode1->pdata = NULL;
							}
						else{
							printf("\n New node created through update");
							printf("\nThe previous value at this address was %s", mnewnode1->pdata);
				
							if((mnewnode1->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"\nMemory cannot be allocated to mnewnode1->pdata\n");
							strcpy(mnewnode1->pdata, (char *)mdata1);
							mnewnode1->sdata = NULL;
					}
				}
				else {
					mnewnode1->sdata = (char *)mdata1;
					mnewnode1->pdata = (char *)mdata1;
				}
				
				mnewnode1->first = NULL;
				mnewnode1->second = NULL;
				mnewnode1->third = NULL;
				mnewnode1->fourth = NULL;
				mtrie->size++;
				return mnewnode1;


}

struct TrieNode* updateNode(BiTrie* trie,struct TrieNode* tempNode2, char *data1, int flag1){

	if(tempNode2 == NULL) 
		tempNode2 = updatenewNode(trie,data1);

	else {
		
		if( flag1 == 0) {
			if(tempNode2->left == NULL){
				tempNode2->left = updateNode(trie, tempNode2->left, data1 , flag1);
				return tempNode2->left;
			}	
			else
				return tempNode2->left;
		}	 
		
		else { 
			if(tempNode2->right == NULL){
				tempNode2->right = updateNode(trie, tempNode2->right, data1 , flag1);
				return tempNode2->right;
			}
			else
				return tempNode2->right;
		}
	}
	
	return tempNode2;
}


struct mTrieNode* mupdateNode(mTrie* mtrie,struct mTrieNode* mtempNode2, char *mdata1, char *mflag1, int id2){

	if(mtempNode2 == NULL) 
		mtempNode2 = mupdatenewNode(mtrie,mdata1, id2);

	else {
		
		if( strcmp(mflag1,"00") == 0) {
			if(mtempNode2->first == NULL){
				mtempNode2->first = mupdateNode(mtrie, mtempNode2->first, mdata1 , mflag1, id2);
				return mtempNode2->first;
			}	
			else{
				if(mdata1 != NULL){
					if(id2 == 1){
						if((mtempNode2->first->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mtempNode2->first->sdata\n");
						strcpy(mtempNode2->first->sdata, (char *)mdata1);
					}
					else{
						if((mtempNode2->first->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mtempNode2->first->pdata\n");
						strcpy(mtempNode2->first->pdata, (char *)mdata1);
					}
						
				}
				return mtempNode2->first;
			}
		}	 
		
		else if( strcmp(mflag1,"01") == 0) {
			if(mtempNode2->second == NULL){
				mtempNode2->second = mupdateNode(mtrie, mtempNode2->second, mdata1 , mflag1, id2);
				return mtempNode2->second;
			}	
			else{
				if(mdata1 != NULL){
					if(id2 == 1){
						if((mtempNode2->second->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mtempNode2->second->sdata\n");
						strcpy(mtempNode2->second->sdata, (char *)mdata1);
					}
					else{
						if((mtempNode2->second->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mtempNode2->second->pdata\n");
						strcpy(mtempNode2->second->pdata, (char *)mdata1);
					}
						
				}
				return mtempNode2->second;
			}
		}	 
		
		else if( strcmp(mflag1,"10") == 0) {
			if(mtempNode2->third == NULL){
				mtempNode2->third = mupdateNode(mtrie, mtempNode2->third, mdata1 , mflag1, id2);
				return mtempNode2->third;
			}	
			else{
				if(mdata1 != NULL){
					if(id2 == 1){
						if((mtempNode2->third->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mtempNode2->third->sdata\n");
						strcpy(mtempNode2->third->sdata, (char *)mdata1);
					}
					else{
						if((mtempNode2->third->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mtempNode2->third->pdata\n");
						strcpy(mtempNode2->third->pdata, (char *)mdata1);
					}
						
				}
				return mtempNode2->third;
			}
		}	 
		else { 
			if(strcmp(mflag1,"11") == 0){
				if(mtempNode2->fourth == NULL){
					mtempNode2->fourth = mupdateNode(mtrie, mtempNode2->fourth, mdata1 , mflag1, id2);
					return mtempNode2->fourth;
				}
			else{
				if(mdata1 != NULL){
					if(id2 == 1){
						if((mtempNode2->fourth->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mtempNode2->fourth->sdata\n");
						strcpy(mtempNode2->fourth->sdata, (char *)mdata1);
					}
					else{
						if((mtempNode2->fourth->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
							fprintf(stderr,"\nMemory cannot be allocated to mtempNode2->fourth->pdata\n");
						strcpy(mtempNode2->fourth->pdata, (char *)mdata1);
					}
						
				}
				return mtempNode2->fourth;
			}
			}
		}
	}
	
	return mtempNode2;
}

void deleteNode(u_int16_t uprefixFinal[], struct TrieNode* tempNode1, u_int16_t uprefixLength){
	
	int i = 0;
	for(i=0;i<uprefixLength;i++){
			
			if(uprefixFinal[i] == 0) 
				tempNode1 = tempNode1->left;
					
			else
				tempNode1 = tempNode1->right;
					
	}
	printf("\nDeleting %s ", tempNode1->data);
	
		tempNode1->data = NULL;

	if( tempNode1->left == NULL && tempNode1->right == NULL){
		tempNode1 = NULL;
		trie->size--;
	}
}

void mdeleteNode(char **muprefixFinal, struct mTrieNode* mtempNode1, u_int16_t uprefixLength){
	
	int i = 0;
	muprefixLength = 0;
	
	if(uprefixLength % 2 == 0)
		muprefixLength = uprefixLength / 2;
	else 
		muprefixLength = (uprefixLength/2) + 1;
		
	for(i=0;i<muprefixLength;i++){
			
			if(strcmp(muprefixFinal[i],"00") == 0) 
				mtempNode1 = mtempNode1->first;
			else if(strcmp(muprefixFinal[i],"01") == 0) 
				mtempNode1 = mtempNode1->second;
			else if(strcmp(muprefixFinal[i],"10") == 0) 
				mtempNode1 = mtempNode1->third;		
			else if(strcmp(muprefixFinal[i],"11") == 0) 
				mtempNode1 = mtempNode1->fourth;
			else {
				if(strcmp(muprefixFinal[i],"0") == 0) {
					mtempNode1->first->sdata  = NULL;
					mtempNode1->second->sdata  = NULL;
					if( mtempNode1->first->first == NULL && mtempNode1->first->second == NULL && mtempNode1->first->third == NULL && mtempNode1->first->fourth == NULL &&  mtempNode1->first->pdata == NULL ){
						mtempNode1->first = NULL;
						mtrie->size--;
					}
					if( mtempNode1->second->first == NULL && mtempNode1->second->second == NULL && mtempNode1->second->third == NULL && mtempNode1->second->fourth == NULL && mtempNode1->second->pdata == NULL ){
						mtempNode1->second = NULL;
						mtrie->size--;
					}
					return;
				}
				else if(strcmp(muprefixFinal[i],"1") == 0) {
					mtempNode1->third->sdata  = NULL;
					mtempNode1->fourth->sdata  = NULL;
					if( mtempNode1->third->first == NULL && mtempNode1->third->second == NULL && mtempNode1->third->third == NULL && mtempNode1->third->fourth == NULL &&  mtempNode1->third->pdata == NULL){
						mtempNode1->third = NULL;
						mtrie->size--;
					}
					if( mtempNode1->fourth->first == NULL && mtempNode1->fourth->second == NULL && mtempNode1->fourth->third == NULL && mtempNode1->fourth->fourth == NULL && mtempNode1->fourth->pdata == NULL){
						mtempNode1->fourth = NULL;
						mtrie->size--;
					}
					return;
				}
			}
					
	}
	
//	printf("\nDeleting %s ", tempNode1->data);
	
		 mtempNode1->pdata = NULL;

	if( mtempNode1->first == NULL && mtempNode1->second == NULL && mtempNode1->third == NULL && mtempNode1->fourth == NULL && mtempNode1->sdata == NULL){
			mtempNode1 = NULL;
		mtrie->size--;
	}
	return;
}


void ParseLookup(char *lookupfile){
	
	ulpmcount = mlpmcount = ulpmmedian = mlpmmedian= lookupCount = 0;
	k = 0;
	tempStr2 = (char *)calloc(1,2*sizeof(char));
	charToStr2 = (char *)calloc(1,sizeof(char));
	filePointer2 = (FILE *) calloc(1,10 * sizeof(FILE)); 

	filePointer2 = fopen(lookupfile, "r+");
	//printf("\n\nInside ParseLookup() lookup.txt starts here\n\n");

	if( filePointer2 == NULL) 
		fprintf(stderr,"\nMemory cannot be allocated to filepointer2");
	
	else 
	while( !feof(filePointer2) ) {
		
		d = (char)getc(filePointer2);
		sprintf(charToStr2,"%c",d);
		strcat(tempStr2,charToStr2);
		
		if( d == '.' || d == '\n') {

			k++;
			if(k == 1) {
				lookupOctet1 = atoi(tempStr2);
				strcpy(tempStr2,initializer);
			}
			if(k == 2) {
				lookupOctet2 = atoi(tempStr2);
				strcpy(tempStr2,initializer);
			}
			if(k == 3) {
				lookupOctet3 = atoi(tempStr2);
				strcpy(tempStr2,initializer);
			}
			if(k == 4) {
				lookupOctet4 = atoi(tempStr2);
				strcpy(tempStr2,initializer);
			}
		
		
		}
			if (d == '\n' ) {		
				k = 0;
				printf("\nAddress %d.%d.%d.%d  ", lookupOctet1, lookupOctet2,lookupOctet3,lookupOctet4 );
				getPrefix(lookupOctet1, lookupOctet2,lookupOctet3,lookupOctet4 ,lookupLength, lookupFinal, mlookupFinal);
				getLongestPrefixMatch(lookupFinal, mlookupFinal);
			}
	}
	
	 
	ulpmmedian = getMedian(lpmuniarray,ulpmcount);
	mlpmmedian = getMedian(lpmmultiarray,mlpmcount);
	ulpmmean = getMean(lpmuniarray,ulpmcount);
	mlpmmean = getMean(lpmmultiarray,mlpmcount);
	ulpmmin = getMin(lpmuniarray,ulpmcount);
	mlpmmin = getMin(lpmmultiarray,mlpmcount);
	ulpmmax = getMax(lpmuniarray,ulpmcount);
	mlpmmax = getMax(lpmmultiarray,mlpmcount);
}


void ParseUpdate(char *updatefile){
	
	uupdatecount = mupdatecount = uupdatemedian  = mupdatemedian = uprefixCount =  0;
	char k=0;
	int j = 0;
	tempStr3 = (char *)calloc(1,2*sizeof(char));
	charToStr3 = (char *)calloc(1,sizeof(char));
	filePointer3 = (FILE *) calloc(1,10 * sizeof(FILE)); 
	
	filePointer3 = fopen(updatefile, "r+");
	
	if( filePointer3 == NULL) 
		fprintf(stderr,"\nMemory cannot be allocated to filepointer3");
	
	else 
		while( !feof(filePointer3) ) {
		
			e = (char)getc(filePointer3);
			
			if (e == 'W' || e == 'A') { 
				fseek(filePointer3,10,SEEK_CUR);
				continue;
			}
		
			else {
			
				sprintf(charToStr3,"%c",e);
				strcat(tempStr3,charToStr3);
		
				if( e == '.' || e == '\n' || e == '/' || e == ' ') {

					k++;
					if(k == 1) {
						uprefixOctet1 = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
				
					if(k == 2) {
						uprefixOctet2 = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
				
					if(k == 3) {
						uprefixOctet3 = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
				
					if(k == 4) {
						uprefixOctet4 = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
			
					if(k == 5) {
						uprefixLength = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
					if(k == 6) {
						unextHopOctet1 = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
					if(k == 7) {
						unextHopOctet2 = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
					if(k == 8) {
						unextHopOctet3 = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
					if(k == 9) {
						unextHopOctet4 = atoi(tempStr3);
						strcpy(tempStr3,initializer);
					}
		
				}
			}
		
		if (e == '\n' ) {
			k = 0;
			
			if(uprefixLength < 2 || uprefixLength > 30) 
				continue;
			
			u = u1 = v = v1 = g = g1 = h = h1 = 0;
				
			getPrefix(uprefixOctet1, uprefixOctet2,uprefixOctet3,uprefixOctet4 ,uprefixLength, uprefixFinal, muprefixFinal);
			uprefixCount++;
			
			if(unextHopOctet1 == 0 && unextHopOctet2 == 0 && unextHopOctet3 == 0 && unextHopOctet4 == 0 ){
				printf("\n\nDeleting nextHop at %d.%d.%d.%d/%d ", uprefixOctet1,uprefixOctet2, uprefixOctet3, uprefixOctet4, uprefixLength);
				//getPrefix(uprefixOctet1, uprefixOctet2,uprefixOctet3,uprefixOctet4 ,uprefixLength, uprefixFinal, muprefixFinal);
				rdtsc(u);
				deleteNode(uprefixFinal,trie->root,uprefixLength);
				rdtsc(u1);
				updateuniarray[uupdatecount++] = (u1 - u);
				rdtsc(v);
				mdeleteNode(muprefixFinal,mtrie->root,muprefixLength);
				rdtsc(v1);
				
				updatemultiarray[mupdatecount++] = (v1 - v);
			}
			else {
				printf("\n\nUpdating nextHop at %d.%d.%d.%d/%d ", uprefixOctet1,uprefixOctet2, uprefixOctet3, uprefixOctet4, uprefixLength, unextHopOctet1,unextHopOctet2,unextHopOctet3,unextHopOctet4);
				
				//getPrefix(uprefixOctet1, uprefixOctet2,uprefixOctet3,uprefixOctet4 ,uprefixLength, uprefixFinal, muprefixFinal);
				
				strcpy(nextHop1, initializer);
				sprintf(nextHop1,"%d.%d.%d.%d",unextHopOctet1,unextHopOctet2,unextHopOctet3,unextHopOctet4);
				
				if(uprefixLength % 2 == 0)
					muprefixLength = uprefixLength / 2;
				else 
					muprefixLength = (uprefixLength/2) + 1;
			
				rdtsc(g);
				for(j = 0 ; j< uprefixLength; j++){
			
					data1 = NULL;
					
					if( j == 0 ) tempNode2 = trie->root;
					
					if(j == uprefixLength -1) {
						data1 = (char *)calloc(1,20*sizeof(char));
						strcpy(data1,nextHop1);
					}
					
					if(uprefixFinal[j] == 0){	
						tempNode2 = updateNode(trie,tempNode2,data1,0);
						if(j == uprefixLength - 1){
							printf("\nReplacing nextHop");
							printf("\nThe previous value at this address was %s", tempNode2->data );
							if((tempNode2->data = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to newdata");
							strcpy(tempNode2->data,nextHop1);
							printf("\nThe new value at this address is %s", tempNode2->data );
						}
						continue;
					}
					
					else {
						if(uprefixFinal[j] == 1){
							 tempNode2 = updateNode(trie,tempNode2,data1,1);
							 if(j == uprefixLength - 1 ){
							 	printf("\nReplacing nextHop");
							 	printf("\nThe previous value at this address was %s", tempNode2->data );
								if((tempNode2->data = (char *)calloc(1,20*sizeof(char)))==NULL)
									fprintf(stderr,"Memory could not be allocated using calloc to newdata");
							strcpy(tempNode2->data,nextHop1);
							printf("\nThe new value at this address is %s", tempNode2->data );
							}
						 	 continue;
						}						
					}
				}
				
				rdtsc(g1);
				updateuniarray[uupdatecount++] = (g1 - g);
				rdtsc(h);
				for(j = 0 ; j< muprefixLength; j++){
			
					mdata1 = NULL;
					
					if( j == 0 ) mtempNode2 = mtrie->root;
					
					if(j == muprefixLength -1) {
						mdata1 = (char *)calloc(1,20*sizeof(char));
						strcpy(mdata1,nextHop1);
					}
					
					if(strcmp(muprefixFinal[j], "00") == 0){	
						mtempNode2 = mupdateNode(mtrie,mtempNode2,mdata1,"00",0);
						if(j == muprefixLength - 1){
							printf("\nReplacing nextHop");
							printf("\nThe previous value at this address was %s", mtempNode2->pdata );
							if((mtempNode2->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to mtempNode2->pdata");
							strcpy(mtempNode2->pdata,nextHop1);
							printf("\nThe new value at this address is %s", mtempNode2->pdata );
						}
						continue;
					}
					
					else if(strcmp(muprefixFinal[j], "01") == 0){	
						mtempNode2 = mupdateNode(mtrie,mtempNode2,mdata1,"01",0);
						if(j == muprefixLength - 1){
							printf("\nReplacing nextHop");
							printf("\nThe previous value at this address was %s", mtempNode2->pdata );
							if((mtempNode2->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to mtempNode2->pdata");
							strcpy(mtempNode2->pdata,nextHop1);
							printf("\nThe new value at this address is %s", mtempNode2->pdata );
						}
						continue;
					}
					
					else if(strcmp(muprefixFinal[j], "10") == 0){	
						mtempNode2 = mupdateNode(mtrie,mtempNode2,mdata1,"10",0);
						if(j == muprefixLength - 1){
							printf("\nReplacing nextHop");
							printf("\nThe previous value at this address was %s", mtempNode2->pdata );
							if((mtempNode2->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to mtempNode2->pdata");
							strcpy(mtempNode2->pdata,nextHop1);
							printf("\nThe new value at this address is %s", mtempNode2->pdata );
						}
						continue;
					}
					
					else if(strcmp(muprefixFinal[j], "11") == 0){	
						mtempNode2 = mupdateNode(mtrie,mtempNode2,mdata1,"11",0);
						if(j == muprefixLength - 1){
							printf("\nReplacing nextHop");
							printf("\nThe previous value at this address was %s", mtempNode2->pdata );
							if((mtempNode2->pdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to mtempNode2->pdata");
							strcpy(mtempNode2->pdata,nextHop1);
							printf("\nThe new value at this address is %s", mtempNode2->pdata );
						}
						continue;
					}
					else {
						if(strcmp(muprefixFinal[j], "0") == 0){	
							 mupdateNode(mtrie,mtempNode2,mdata1,"00",1);
							 mupdateNode(mtrie,mtempNode2,mdata1,"01",1);
						
						if(j == muprefixLength - 1){
							printf("\nReplacing nextHop");
							printf("\nThe previous value at this address was %s", mtempNode2->first->sdata );
							
							if((mtempNode2->first->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to mtempNode2->first->sdata");
							
							strcpy(mtempNode2->first->sdata,nextHop1);
							printf("\nThe new value at this address is %s", mtempNode2->first->sdata );
							
							printf("\nThe previous value at this address was %s", mtempNode2->second->sdata );
							
							if((mtempNode2->second->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to mtempNode2->first->sdata");
							
							strcpy(mtempNode2->second->sdata,nextHop1);
							printf("\nThe new value at this address is %s", mtempNode2->second->sdata );
						
						}
						continue;
					}	
					
						else if(strcmp(muprefixFinal[j], "1") == 0){	
							 mupdateNode(mtrie,mtempNode2,mdata1,"10",1);
							 mupdateNode(mtrie,mtempNode2,mdata1,"11",1);
						
						if(j == muprefixLength - 1){
							printf("\nReplacing nextHop");
							printf("\nThe previous value at this address was %s", mtempNode2->third->sdata );
							
							if((mtempNode2->third->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to mtempNode2->third->sdata");
							
							strcpy(mtempNode2->third->sdata,nextHop1);
							printf("\nThe new value at this address is %s", mtempNode2->third->sdata );
							
							printf("\nThe previous value at this address was %s", mtempNode2->fourth->sdata );
							
							if((mtempNode2->fourth->sdata = (char *)calloc(1,20*sizeof(char)))==NULL)
								fprintf(stderr,"Memory could not be allocated using calloc to mtempNode2->fourth->sdata");
							
							strcpy(mtempNode2->fourth->sdata,nextHop1);
							printf("\nThe new value at this address is %s", mtempNode2->fourth->sdata );
						
						}
						continue;
					}	
					
					}
				}
				rdtsc(h1);
				updatemultiarray[mupdatecount++] = (h1 - h);
				unextHopOctet1 = unextHopOctet2 = unextHopOctet3 = unextHopOctet4 = 0;
			}
				
				
		}		
	}
	
	uupdatemedian = getMedian(updateuniarray,uupdatecount);
	mupdatemedian = getMedian(updatemultiarray,mupdatecount);	
	uupdatemean = getMean(updateuniarray,uupdatecount);
	mupdatemean = getMean(updatemultiarray,mupdatecount);	
	uupdatemin = getMin(updateuniarray,uupdatecount);
	mupdatemin = getMin(updatemultiarray,mupdatecount);	
	uupdatemax = getMax(updateuniarray,uupdatecount);
	mupdatemax = getMax(updatemultiarray,mupdatecount);	
	
}

int main( int argc, char **argv){

	filePointer = (FILE *) malloc(10 * sizeof(FILE)); 
	int nextHopOctet1 = 0,nextHopOctet2 = 0,nextHopOctet3 = 0,nextHopOctet4 = 0;
	int i = 0, j = 0;
	tempStr = (char *)calloc(1,2*sizeof(char));
	charToStr = (char *)calloc(1,sizeof(char));
	nextHop = (char *)calloc(1,15*sizeof(char));
	data = (char *)calloc(1,20*sizeof(char));
	data1 = (char *)calloc(1,20*sizeof(char));
	mdata = (char *)calloc(1,20*sizeof(char));
	mdata1 = (char *)calloc(1,20*sizeof(char));
	trie = (BiTrie *)calloc(1,sizeof(BiTrie));
	mtrie = (mTrie *)calloc(1,sizeof(mTrie));
	node = (struct TrieNode *)calloc(1,sizeof(struct TrieNode));
	mnode = (struct mTrieNode *)calloc(1,sizeof(struct mTrieNode));
	mflag = (char *)calloc(1,2*sizeof(char));
	tempNode2 = (struct TrieNode *)calloc(1,sizeof(struct TrieNode));
	tempNode1 = (struct TrieNode *)calloc(1,sizeof(struct TrieNode));
	mtempNode2 = (struct mTrieNode *)calloc(1,sizeof(struct mTrieNode));
	mtempNode1 = (struct mTrieNode *)calloc(1,sizeof(struct mTrieNode));
	nextHop1 = (char *)calloc(1,20*sizeof(char));
	mprefixFinal = (char **)calloc(16,sizeof(char *));
	mlookupFinal = (char **)calloc(16,sizeof(char *));
	muprefixFinal = (char **)calloc(16,sizeof(char *));
	lookupfile = (char *)calloc(1,10*sizeof(char));
	updatefile = (char *)calloc(1,10*sizeof(char));
	prefixfile = (char *)calloc(1,10*sizeof(char));
	
	for(i = 0; i<16 ; i++) 
		if( (mprefixFinal[i] = (char *)calloc(3,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated");

	for(i = 0; i<16 ; i++) 
		if( (mlookupFinal[i] = (char *)calloc(3,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated");
	for(i = 0; i<16 ; i++) 
		if( (muprefixFinal[i] = (char *)calloc(3,sizeof(char))) == NULL )
			  fprintf(stderr, "\nMemory cannot be allocated");
	i = 0;

	BiTrieInit(trie);
	mTrieInit(mtrie);

	strcpy(prefixfile,argv[1]);
	filePointer = fopen(prefixfile, "r+");
	
	
	if( filePointer == NULL) 
		fprintf(stderr,"\nMemory cannot be allocated to filepointer");

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
				i = 0;
				strcpy(nextHop,initializer);
				strcpy(data,initializer);
				strcpy(mdata,initializer);
				
			//	printf("\n prefix %d.%d.%d.%d/%d  nextHop %d.%d.%d.%d", prefixOctet1,prefixOctet2, prefixOctet3, prefixOctet4, prefixLength, nextHopOctet1,nextHopOctet2,nextHopOctet3,nextHopOctet4);
				if(prefixLength <= 1 || prefixLength >= 31) {		
					badprefixCount++;
					continue;
				}
				getPrefix(prefixOctet1, prefixOctet2,prefixOctet3,prefixOctet4 ,prefixLength, prefixFinal, mprefixFinal);
				prefixCount++;
				sprintf(nextHop,"%d.%d.%d.%d",nextHopOctet1,nextHopOctet2,nextHopOctet3,nextHopOctet4);
				
				//printf(" prefixFinal : ");
				//for(j = 0 ; j< prefixLength; j++)
					//printf("%d", prefixFinal[j]);
				
				rdtsc(p);
				
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
				
				rdtsc(q);
				unisum += (q-p);
				rdtsc(r);
				for(j = 0 ; j< mprefixLength; j++){
			
					mdata = NULL;
					
					if( j == 0 && prefixCount > 1) mtrie->curRoot = mtrie->root;
					
					if(j == mprefixLength -1) {
						mdata = (char *)calloc(1,20*sizeof(char));
						strcpy(mdata,nextHop);
					}
					
					if(strcmp(mprefixFinal[j],"00") == 0){	
						mtrie->curRoot = mTrieInsert(mtrie,mtrie->curRoot,mdata,"00",0);
						continue;
					}
					
					if(strcmp(mprefixFinal[j],"01") == 0){	
						mtrie->curRoot = mTrieInsert(mtrie,mtrie->curRoot,mdata,"01",0);
						continue;
					}
					
					if(strcmp(mprefixFinal[j],"10") == 0){	
						mtrie->curRoot = mTrieInsert(mtrie,mtrie->curRoot,mdata,"10",0);
						continue;
					}
					
					if(strcmp(mprefixFinal[j],"11") == 0){	
						mtrie->curRoot = mTrieInsert(mtrie,mtrie->curRoot,mdata,"11",0);
						continue;
					}
					
					else {
						if(strcmp(mprefixFinal[j],"0") == 0){
							 mTrieInsert(mtrie,mtrie->curRoot,mdata,"00",1);
						 	 mTrieInsert(mtrie,mtrie->curRoot,mdata,"01",1);
						 	 
						 	 continue;
						}
						else if(strcmp(mprefixFinal[j],"1") == 0){
							 mTrieInsert(mtrie,mtrie->curRoot,mdata,"10",1);
						 	 mTrieInsert(mtrie,mtrie->curRoot,mdata,"11",1);
						 	 
						 	 continue;
						}
					}
				}
			
				rdtsc(s);
				multisum += (s-r);
			}
		}	
		
	printf("\n-------------------------------------------------------------");	
		
	printf("\nThe total number of nodes in the unibit trie before update is %ld", trie->size);
	printf("\nThe total number of nodes in the multibit trie before update is %ld", mtrie->size);
	printf("\n-------------------------------------------------------------");	
	printf("\nThe total number of prefixes outside specified lengths is %ld", badprefixCount);
	printf("\nThe total time taken to construct unibit trie is %llu cycles", unisum);
	printf("\nThe total time taken to construct multibit trie is %llu cycles", multisum);
	printf("\n-------------------------------------------------------------");	
	printf("\nTotal number of loaded prefixes is %ld", prefixCount);
	printf("\nTotal number of prefixes (loaded + bad) is %ld", prefixCount + badprefixCount);
	printf("\nThe size of the unibit trie is %ld bytes or %ld MB ",(trie->size)*(sizeof(struct TrieNode)), (trie->size)*(sizeof(struct TrieNode))/(1024*1024) );
	printf("\nThe size of the multibit trie is %ld bytes or %ld MB",(mtrie->size)*(sizeof(struct mTrieNode)),(mtrie->size)*(sizeof(struct mTrieNode))/(1024*1024));
	printf("\n-------------------------------------------------------------");	
	
	printf("\nPlease Enter: \n1.Longest Prefix Match \n2.Update \n3.Exit\n");
	
	while(o = getchar()) {
		
		if(o == '1'){
			//printf("\nLooking up...");
			printf("\n--------------");
			printf("\nPlease Enter the filename: ");
			scanf("%s",lookupfile);
			ParseLookup(lookupfile);
		}
		else if(o == '2'){
			//printf("\nUpdating...");
			printf("\n--------------");
			printf("\nPlease Enter the filename: ");
			scanf("%s", updatefile);
			ParseUpdate(updatefile);
		}
		else if(o == '3')
			return 0;
	
		else 
			continue;
	
	
	printf("\n-----------------------------------------------------------------------");
	printf("\n Longest Prefix Match stats");
	printf("\n-----------------------------------------------------------------------");
	printf("\nNumber of addresses looked up is %ld", lookupCount);
	printf("\nThe Median time taken to perform lpm for unibit tries is %u cycles", ulpmmedian);
	printf("\nThe Median time taken to perform lpm for multi bit tries is %u cycles", mlpmmedian);
	printf("\n-----------------------------------------------------------------------");
	printf("\nThe Mean time taken to perform lpm for unibit tries is %u cycles", ulpmmean);
	printf("\nThe Mean time taken to perform lpm for multi bit tries is %u cycles", mlpmmean);
	printf("\n-----------------------------------------------------------------------");
	printf("\nThe Min time taken to perform lpm for unibit tries is %u cycles", ulpmmin);
	printf("\nThe Min time taken to perform lpm for multi bit tries is %u cycles", mlpmmin);
	printf("\n-----------------------------------------------------------------------");
	printf("\nThe Max time taken to perform lpm for unibit tries is %u cycles", ulpmmax);
	printf("\nThe Max time taken to perform lpm for multi bit tries is %u cycles", mlpmmax);
	
	printf("\n-----------------------------------------------------------------------");
	printf("\n Update and Delete stats");
	printf("\n-----------------------------------------------------------------------");
	printf("\nThe Total number of values  updated/deleted is %ld", uprefixCount);
	printf("\nThe Total number of new unibit  nodes created is %d\n", newnodes);
	printf("\nThe Total number of new multibit nodes created is %d\n", mnewnodes);
	printf("\n-----------------------------------------------------------------------");
	printf("\nThe Median time taken to perform updates for unibit tries is %u cycles", uupdatemedian);
	printf("\nThe Median time taken to perform updates for multi bit tries is %u cycles", mupdatemedian);
	printf("\n-----------------------------------------------------------------------");
	printf("\nThe Mean time taken to perform updates for unibit tries is %u cycles", uupdatemean);
	printf("\nThe Mean time taken to perform updates for multi bit tries is %u cycles", mupdatemean);
	printf("\n-----------------------------------------------------------------------");
	printf("\nThe Min time taken to perform updates for unibit tries is %u cycles", uupdatemin);
	printf("\nThe Min time taken to perform updates for multi bit tries is %u cycles", mupdatemin);
	printf("\n-----------------------------------------------------------------------");	
	printf("\nThe Max time taken to perform updates for unibit tries is %u cycles", uupdatemax);
	printf("\nThe Max time taken to perform updates for multi bit tries is %u cycles", mupdatemax);
	printf("\n-------------------------------------------------------------");	
	

	printf("\n\nThe total number of nodes in the trie after update is %ld", trie->size);
	printf("\n\nThe total number of nodes in the multibit trie after update is %ld", mtrie->size);
	printf("\nThe size of the unibit trie is %ld bytes or %ld MB ",(trie->size)*(sizeof(struct TrieNode)), (trie->size)*(sizeof(struct TrieNode))/(1024*1024) );
	printf("\nThe size of the multibit trie is %ld bytes or %ld MB",(mtrie->size)*(sizeof(struct mTrieNode)),(mtrie->size)*(sizeof(struct mTrieNode))/(1024*1024));
	
	
	
	
	//printf("\nThe lengths of different arrays are %llu, %llu, %llu, %llu\n",ulpmcount, mlpmcount, uupdatecount, mupdatecount);
	
	printf("\n-------------------------------------------------------------");	
	
	printf("\nPlease Enter: \n1.Longest Prefix Match \n2.Update \n3.Exit\n");
	
	}
	
	return 0;
	
}

