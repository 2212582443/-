import java.util.Arrays;
import java.util.Scanner;
import java.util.concurrent.*;

public class QuickSort extends RecursiveAction {

	private Integer[] array;
	private int begin;
	private int end;

	public QuickSort(Integer[] array,int begin, int end) {
		this.array = array;
		this.begin = begin;
		this.end = end;
	}

	protected int divide(int low,int high){
	      Integer k=array[low];
	      do{
	          while (low < high && array[high].compareTo(k) >= 0) -- high;
		  if (low < high) array[low ++] = array[high];
		  while (low < high && array[low].compareTo(k) <= 0) ++ low;
		  if (low < high) array[high --] = array[low];
		}while(low!=high);
		array[low]=k;
		return low;
	}	
	
	protected void compute() {
			int mid;
			if(begin>=end) return;
			mid=divide(begin,end);
                       //leftsort
			QuickSort leftTask = new QuickSort(array, begin, mid - 1);
			leftTask.fork();
			leftTask.join();
			//rightsort
			QuickSort rightTask = new QuickSort(array, mid+ 1, end);
			rightTask.fork();
			rightTask.join();
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
		QuickSort task = new QuickSort(array, 0, n-1);
		pool.invoke(task);
		
		//output array
		System.out.println("Array after sort:");
		System.out.println(Arrays.toString(array));
	}
}
