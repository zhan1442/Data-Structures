void quicksort(int * list, int d, int u,int min, int max){
	  int i=d;
	    int j=u;
		  int mid=((long int)min+max)/2;
		    do {
				    while ((list[i]<mid)&&(i<=u)) i++;
					    while ((list[j]>mid)&&(j>=d)) j--;
						    if (i<=j){int temp=list[i]; list[i]=list[j];list[j]=temp;
							    i++;j--;}
								  } while (i<=j);
								    if (i<u) quicksort(list,i,u,mid,max);
									  if (j>d) quicksort(list,d,j,min,mid);
}
