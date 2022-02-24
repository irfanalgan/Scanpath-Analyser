//irfan Kaan ALGAN 232859
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct graphHead{
	int count;
	struct graphVertex *first;
};

struct graphVertex{
	struct graphVertex *next;
	char scanpath[50]; //scanpaths to save in a node/vertex
	float weight;
	int processed; //Can be used a flag, such as visited or unvisited in traversal
	struct graphArc *firstArc;
};

struct similarityRecord{
	struct similarityRecord *next;
	char scanpathOne[50]; //scanpath to save in a node/vertex
	char scanpathTwo[50]; //scanpath to save in a node/vertex
	float similarity; // holds the similarity value
};
struct similarityRecord *recordHead = NULL;

struct graphArc{
	struct graphVertex *destination;
	struct graphArc *next;
};

struct graphHead * createGraph(void){
	struct graphHead *head = (struct graphHead *)malloc(sizeof(struct graphHead));
	head->count = 0;
	head->first = NULL;
	return head;
}

void displayGraph(){
	
   struct similarityRecord *records = recordHead;

   while(records != NULL) {        
      printf("Scanpath %s and Scanpath %s - Similarity: %2.f\n",records->scanpathOne,records->scanpathTwo, records->similarity);
      records = records->next;
   }
	
}
//Displaying similar records
void sortSimilarities() { 
	struct similarityRecord *list = recordHead, *index = NULL;
	float tempWeight;
	char tempScanpath1[50];
	char tempScanpath2[50];
	if(recordHead == NULL){
		return;
	}
	else{
		while(list != NULL){
			index = list->next;
			while(index != NULL){
				if(list->similarity < index->similarity){
					tempWeight = list->similarity;
					strcpy(tempScanpath1, list->scanpathOne);
					strcpy(tempScanpath2, list->scanpathTwo);
					
					list->similarity = index->similarity;
					strcpy(list->scanpathOne, index->scanpathOne);
					strcpy(list->scanpathTwo, index->scanpathTwo);
					
					index->similarity = tempWeight;
					strcpy(index->scanpathOne, tempScanpath1);
					strcpy(index->scanpathTwo, tempScanpath2);
				}
				index = index->next;
			}
			list = list->next;
		}
	}
} 
// we are doing just sorting 

void createVertices(struct graphHead * head){

  char fileName[50];
  printf("Enter a file name > "),
  scanf("%s",fileName);	
  FILE *fileIn = fopen(fileName,"r");
  if(fileIn == NULL)
  {
      printf("FAILED!\n");
      exit(1);
  }

  struct graphVertex *vertices = (struct graphVertex *)malloc(sizeof(struct graphVertex));
  vertices->next = NULL;
  while (fscanf(fileIn, "%s\n", vertices->scanpath)!=EOF){
    vertices->firstArc = NULL;
    vertices->weight = 0;
    head->count++;
    if(head->first == NULL){
      head->first = vertices;
    }
    else{
      struct graphVertex *temp = head->first;
      while(temp->next != NULL){
        temp = temp->next;
      }
      temp->next = vertices;
    }
    vertices = (struct graphVertex *)malloc(sizeof(struct graphVertex));
  	vertices->next = NULL;
  } 
  fclose(fileIn);
  printf("Vertices are created!\n\n");
}

void fillSimilarityRecord(struct similarityRecord *records,char scan1[],char scan2[],float sim){
	
	records = (struct similarityRecord *)malloc(sizeof(struct similarityRecord)); //we allocate new space for new records
	strcpy(records->scanpathOne,scan1);// copy the similar strings  in our fillSimilarityRecord
	strcpy(records->scanpathTwo,scan2); // copy the similar strings in our fillSimilarityRecord
	records->similarity = sim;// copy the similar similartiy value in our fillSimilarityRecord 
	records->next = recordHead;
	recordHead = records;

}
// it holds the similar datas in it for display function

int createEdges(struct similarityRecord *records,struct graphHead *head, char fromKey[], char toKey[], float percentage){
	
	struct graphArc *arc = (struct graphArc *) malloc(sizeof(struct graphArc));
	struct graphVertex *fromVertex = NULL, *toVertex = NULL;
	
	struct graphArc *tempArc = NULL;
	fromVertex = head->first;
	
	while(fromVertex != NULL && fromVertex->scanpath != fromKey)
		fromVertex = fromVertex->next;
	
	if (fromVertex ==NULL)
		return -2; //fromKey Not found
	
	toVertex = head->first;
	while(toVertex != NULL && toVertex->scanpath != toKey)
		toVertex = toVertex->next;
	
	if (toVertex ==NULL)
		return -3; //toKey Not found
	
	fromVertex->weight = percentage;
	toVertex->weight = percentage;
	
	arc->destination = toVertex;
	
	tempArc = fromVertex->firstArc;
	if (tempArc == NULL){
		fromVertex->firstArc = arc;
		arc->next = NULL;
		fillSimilarityRecord(records, fromKey,toKey,percentage);// I send the records to save in similarTable
		return 1; //Success as the first arc
	}
	else{
		while(tempArc->next != NULL){
			tempArc = tempArc->next;
		}
		tempArc->next = arc;
		arc->next= NULL;
		fillSimilarityRecord(records, fromKey,toKey,percentage);// I send the records to save in similarTable
		return 1; //Success as the last arc
	}
}

int min(int num1, int num2, int num3){
	int smallest;
	if(num1<num2){//compare num1 and num2
        if(num1<num3){//compare num1 and num3
     		smallest = num1;
        }
        else{
            smallest = num3;
        }
    }
    else{
        if(num2<num3){//compare num2 and num1
            smallest = num2;
        }
        else{
            smallest = num3;
        }
    }
	return smallest;
}   
// finds min element
int editDist(char s1[], char s2[], int m, int n) {

	if (m == 0)
 		return n;
 	if (n == 0)
 		return m;
 	if (s1[m - 1] == s2[n - 1])
 		return editDist (s1, s2, m - 1, n - 1);
 	return 1 + min(editDist(s1, s2, m, n - 1), editDist(s1, s2, m - 1, n), editDist(s1, s2, m - 1, n - 1));
}
//find distance between two string
int findmax(int a, int b){
	if(a > b)
		return a;
	else
		return b;
	
}
//find max element

int findSimilarity (int d, int max){
	return (100 *(1-(float)d/max));
}
//finds similarity between two string

void scanVertices(struct graphHead * head){
	int m,n,d,max;
	float s;
	struct graphVertex *tempVertexFirst = NULL;
	struct graphVertex *tempVertexSecond = NULL;
	struct similarityRecord *records = recordHead;
	
	tempVertexFirst = head->first;					
	tempVertexSecond = head->first->next;
	
	while(tempVertexFirst->next != NULL){
		while(tempVertexSecond != NULL){// this loops for visiting every elements in our graph
			
			m = strlen(tempVertexFirst->scanpath);		//lenght of string 1
			n = strlen(tempVertexSecond->scanpath);		//lenght of string 2
			
			max = findmax(m,n);	
			d = editDist(tempVertexFirst->scanpath,tempVertexSecond->scanpath,m,n);	//we calculate the distance between strings
			s = findSimilarity(d,max);		//finds similarity percentage
			
			if(s > 50){ //checks similiarty between two scanpath 
				createEdges(records, head,tempVertexFirst->scanpath,tempVertexSecond->scanpath,s);//send vertices to create edges
			}
			tempVertexSecond = tempVertexSecond->next;
		}
		tempVertexFirst = tempVertexFirst->next;
		tempVertexSecond = tempVertexFirst->next;//We do these things so that they don't visit the same vertex again.
	}
}
//Testing purposes!
int main(){
	struct graphHead *head = createGraph();
	createVertices(head);
	scanVertices(head);//send vertices to calculate similarities
	sortSimilarities();	
	displayGraph();
	return 0;
}



