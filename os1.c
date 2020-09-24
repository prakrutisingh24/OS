#include<stdio.h>
#include<stdlib.h>

int main(){
	

	int a,b,c;

	printf("Enter the first number\n");
	scanf("%d", &a);
	printf("Enter the second number\n");
	scanf("%d", &b);

	int i;
	int *array;
	
	printf("Enter the size of the array!\n");
	scanf("%d", &c);
	if(c <0){
		printf("try a positive number\n");
		printf("Enter the size of the array!\n");
		scanf("%d", &c);
	}

	if(c > (b-a)){
		printf("yo,not possible");
		exit(0);
	}

	array = (int*) malloc(c * sizeof(int));

	for(i=0; i<c ; i++){
		array[i] = rand()%10000;
		
		for(int j=0;j<(c-i-1);j++)
			if(array[i]==array[j])
				array[i] = rand()%10000;
		printf(" %d ", array[i]);
	}
/*
	for(i=0;i<c;i++){
		int k = rand()%10000;

    	for(int j = 0; j < (c - i - 1); j++){
    		k = rand()%10000;
    		k = array[i];

    		if(array[i] == k)
    			//int t = array[i];
    			printf("%d is repeating\n", array[i]);

    		
    	}
    	printf(" %d ", array[i]);
    }

*/	


return 0;

}

/*
int binarySearch(int arr[], int l, int r, int x) 
{ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
  
        if (arr[mid] == x) 
            return mid; 
  
        if (arr[mid] > x) 
            return binarySearch(arr, l, mid - 1, x); 
  
        return binarySearch(arr, mid + 1, r, x); 
    } 
 
    return 0; 
} 
  */
