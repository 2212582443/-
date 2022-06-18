import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.*;

public class MergeSort extends RecursiveAction {
	private Integer[] array;
	private int begin;
	private int end;

	public MergeSort(Integer[] array, int begin, int end) {
		this.array = array;
		this.begin = begin;
		this.end = end;
	}
	
	protected void merge(int left, int mid, int right)
	{
	       Integer[] tmp = new Integer [right-left + 1];
	       
	       int i=left,j=mid,k=0;
	       
	       while(i<mid&&j<=right)
	            if(array[i].compareTo(array[j])<0) tmp[k++]=array[i++];
	            else tmp[k++]=array[j++];
	       
	       while(i<mid) tmp[k++]=array[i++];
	       while(j<=right) tmp[k++]=array[j++];
	       
	       for(i=0,k=left;k<=right;) array[k++]=tmp[i++];
	}
	
	     

	protected void compute() {
	      
	     int left=begin,right=end;
	     int mid=(left+right)/2;
	     
	     if(left==right) return;
	     
	     MergeSort leftTask = new MergeSort(array, left, mid);
	     leftTask.fork();
	     leftTask.join();
	     
	     MergeSort rightTask = new MergeSort(array, mid + 1, right);
	     rightTask.fork();
	     rightTask.join();
	     
	     merge(left,mid+1,right);
		
	}
	

	public static void main(String[] args) {
		ForkJoinPool pool = new ForkJoinPool();
		Scanner sc = new Scanner(System.in);

		
		//input array
		System.out.println("Please input the length of your unsorted array:");
		int n=sc.nextInt();
		System.out.println("Please input the elements to sort:");
		Integer[] array = new Integer[n];
		for (int i = 0; i < n; ++ i)
		{
			array[i]=sc.nextInt();
		}
		
		// use fork-join parallelism to sum the array
		MergeSort task = new MergeSort(array,0, n-1);
		pool.invoke(task);
		
		//output array
		System.out.println("Array after sort:");
		System.out.println(Arrays.toString(array));
	}
}
