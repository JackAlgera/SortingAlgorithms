#include <iostream>
#include <ctime>
#include <SFML\Graphics.hpp>
#include <thread>
#include <chrono>
#include <vector>

#define SCREEN_SIZE_X 800
#define SCREEN_SIZE_Y 600
#define WAIT_TIME 5 // µs

int N = 500;
int nbrMixes = 1500;
float recSizeX = (float)SCREEN_SIZE_X / N;
float * recSizeY = new float[N];

// Some utility functions
void DrawRectangle(sf::RenderWindow &window, int i, int value)
{
	sf::VertexArray rec(sf::Quads, 4);

	rec[0].position = sf::Vector2f(i * recSizeX, SCREEN_SIZE_Y - recSizeY[value]);
	rec[1].position = sf::Vector2f((i + 1) * recSizeX, SCREEN_SIZE_Y - recSizeY[value]);
	rec[2].position = sf::Vector2f((i + 1) * recSizeX, SCREEN_SIZE_Y);
	rec[3].position = sf::Vector2f(i * recSizeX, SCREEN_SIZE_Y);

	window.draw(rec);
}
void DrawRectangle(sf::RenderWindow &window, int i, int value, sf::Color color)
{
	sf::VertexArray rec(sf::Quads, 4);

	rec[0].position = sf::Vector2f(i * recSizeX, SCREEN_SIZE_Y - recSizeY[value]);
	rec[1].position = sf::Vector2f((i + 1) * recSizeX, SCREEN_SIZE_Y - recSizeY[value]);
	rec[2].position = sf::Vector2f((i + 1) * recSizeX, SCREEN_SIZE_Y);
	rec[3].position = sf::Vector2f(i * recSizeX, SCREEN_SIZE_Y);

	for (int i = 0; i < 4; i++)
	{
		rec[i].color = color;
	}

	window.draw(rec);
}
void DrawArray(sf::RenderWindow &window, int * arr, int N, int currentNode)
{
	window.clear();
	for (int i = 0; i < N; i++)
	{
		DrawRectangle(window, i, arr[i]);
	}
	DrawRectangle(window, currentNode, arr[currentNode], sf::Color::Red);
	window.display();
}
int getMax(int arr[], int n)
{
	int mx = arr[0];
	for (int i = 1; i < n; i++)
		if (arr[i] > mx)
			mx = arr[i];
	return mx;
}
void swap(int * a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
// Creates a random array of size N, going from 0 to N-1, then mixes it up
int* randomArray(int N, int nbrMixes)
{
	int * arr = new int[N];

	for (int i = 0; i < N; i++)
	{
		arr[i] = i;
	}

	for (int j = 0; j < nbrMixes; j++)
	{
		int a = std::rand() % N;
		int b = std::rand() % N;
		swap(&arr[a], &arr[b]);
	}

	return arr;
};
void printArr(int *arr, int N)
{
	for (int i = 0; i < N; i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << "\n";
}

// -----------------------------------------------------------------------------
// --------------------------- Quick Sort --------------------------------------
// -----------------------------------------------------------------------------

// Partition used for the Quick Sort algorithm
int partition(sf::RenderWindow &window ,int * arr, int low, int high)
{
	int pivot = arr[low]; // Pivot at left side
	int leftMost = low;

	for (int i = low + 1; i <= high; i++)
	{
		if (arr[i] < pivot)
		{
			leftMost++;
			swap(&arr[leftMost], &arr[i]);
		}
		DrawArray(window, arr, N, i);
		std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
	}
	swap(&arr[leftMost], &arr[low]);
	DrawArray(window, arr, N, low);
	std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
	return leftMost;
}
// Quick Search algorithm (recursive)
void QuickSearchRec(sf::RenderWindow &window, int * arr, int low, int high)
{
	if (low < high) //While low != high
	{
		// p=position of pivot in correct place
		int p = partition(window, arr, low, high);

		// Do quicksearch on left sub-array
		QuickSearchRec(window, arr, low, p - 1);
		// Do quicksearch on right sub-array
		QuickSearchRec(window, arr, p + 1, high);
	}
}
// Just a function to simplify the recursive calls and drawing each frame
void QuickSearch(sf::RenderWindow &window, int *arr, int N)
{
	QuickSearchRec(window, arr, 0, N-1);
}

// -----------------------------------------------------------------------------
// --------------------------- Bubble Sort -------------------------------------
// -----------------------------------------------------------------------------

void BubbleSort(sf::RenderWindow &window, int * arr, int N)
{
	bool sorted = false;
	while (!sorted)
	{
		bool foundWrongOrder = false;
		for (int i = 0; i < N-1; i++)
		{
			if (arr[i] > arr[i + 1])
			{
				swap(&arr[i], &arr[i + 1]);
				foundWrongOrder = true;
			}
			DrawArray(window, arr, N, i);
			std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
		}
		if (!foundWrongOrder)
		{
			sorted = true;
		}
	}
}

// -----------------------------------------------------------------------------
// --------------------------- Merge Sort --------------------------------------
// -----------------------------------------------------------------------------

// Just a function to simplify the recursive calls and drawing each frame

void merge(sf::RenderWindow &window ,int arr[], int l, int m, int r)
{
	int n1 = m - l + 1;	// Left arr size
	int n2 = r - m;		// Right arr size

	// Arrays to store left and right sub arrays
	int *lArr = new int[n1];
	int *rArr = new int[n2];

	for (int i = 0; i < n1; i++)
		lArr[i] = arr[l + i];
	for (int j = 0; j < n2; j++)
		rArr[j] = arr[m + 1 + j];


	int i = 0; // Initial index of first subarray 
	int j = 0; // Initial index of second subarray 
	int k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2)
	{
		if (lArr[i] <= rArr[j])
		{
			arr[k] = lArr[i];
			i++;
			DrawArray(window, arr, N, i);
			std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
		}
		else
		{
			arr[k] = rArr[j];
			j++;
			DrawArray(window, arr, N, i);
			std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
		}
		k++;
	}

	// Copy the remaining elements of lArr[]
	while (i < n1)
	{
		DrawArray(window, arr, N, i);
		std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
		arr[k] = lArr[i];
		i++;
		k++;
	}

	// Copy the remaining elements of rArr[]
	while (j < n2)
	{
		DrawArray(window, arr, N, j);
		std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
		arr[k] = rArr[j];
		j++;
		k++;
	}
}

void mergeSortRec(sf::RenderWindow &window,int arr[], int l, int r)
{
	if (l < r)
	{
		// Get middle of array
		int mid = (r + l) / 2;
		// Edge problem
		if (mid > r)
		{
			mid = r;
		}

		// Sort left sub array
		mergeSortRec(window, arr, l, mid);
		// Sort right sub array
		mergeSortRec(window, arr, mid + 1, r);
		// Concatinate arrays again
		merge(window, arr, l, mid, r);
	}
}

void MergeSort(sf::RenderWindow &window, int *arr, int N)
{
	mergeSortRec(window, arr, 0, N);
}

// -----------------------------------------------------------------------------
// --------------------------- Radix Sort --------------------------------------
// -----------------------------------------------------------------------------

void countSort(sf::RenderWindow &window, int arr[], int N, int exp)
{
	int *output = new int[N]; // output array 
	// Use 10 buckets to store each value from arr
	// in correct bucket depending on current digit
	int i, buckets[10] = { 0 };

	// Store count of occurrences in count
	for (i = 0; i < N; i++)
	{
		buckets[(arr[i] / exp) % 10] += 1;
		DrawArray(window, arr, N, i);
		std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
	}

	// Change buckets[i] so that buckets[i] now contains actual 
	// position of this digit in output
	for (i = 1; i < 10; i++)
		buckets[i] += buckets[i - 1];

	// Build the output array 
	for (i = N - 1; i >= 0; i--)
	{
		int newI = (arr[i] / exp) % 10;
		output[buckets[newI] - 1] = arr[i];
		buckets[newI] -= 1;
		DrawArray(window, arr, N, i);
		std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
	}

	// Arr now contains values sorted by current digit
	for (i = 0; i < N; i++)
	{
		arr[i] = output[i];
		DrawArray(window, arr, N, i);
		std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
	}
}

void RadixSort(sf::RenderWindow &window, int arr[], int N)
{
	// Get highest num in array
	int maxNum = getMax(arr, N);

	// Number of passes depends on the number of
	// digits maxNum has - (ex: 132 -> 3 passes)
	for (int pow = 1; maxNum / pow > 0; pow *= 10)
		countSort(window, arr, N, pow);
}

// -----------------------------------------------------------------------------
// --------------------------- Bucket Sort -------------------------------------
// -----------------------------------------------------------------------------

void BucketSort(sf::RenderWindow &window, int *arr, int N)
{
	// 1) Create N empty buckets 
	std::vector<std::vector<float>> buckets(N);

	// 2) Put array elements in different buckets 
	for (int i = 0; i < N; i++)
	{
		int bi = N * ((float)arr[i] / N); // Index in bucket 
		buckets[bi].push_back(arr[i]);
		DrawArray(window, arr, N, i);
		std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));
	}

	// 3) Sort individual buckets 
	for (int i = 0; i < N; i++)
		std::sort(buckets[i].begin(), buckets[i].end());

	// 4) Concatenate all buckets into arr[] 
	int index = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < buckets[i].size(); j++)
		{
			arr[index] = buckets[i][j];
			DrawArray(window, arr, N, index);
			std::this_thread::sleep_for(std::chrono::microseconds(WAIT_TIME));

			index++;
		}
	}
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

int main()
{
	std::srand(time(NULL));

	for (int i = 0; i < N; i++)
	{
		recSizeY[i] = ((float)i / N) * SCREEN_SIZE_Y;
	}
	
	int * arr = randomArray(N, nbrMixes);

	sf::RenderWindow window(sf::VideoMode(SCREEN_SIZE_X, SCREEN_SIZE_Y), "Sorting Algorithm");

	std::this_thread::sleep_for(std::chrono::seconds(2));
	
	arr = randomArray(N, nbrMixes);
	RadixSort(window, arr, N);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	arr = randomArray(N, nbrMixes);
	QuickSearch(window, arr, N);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	arr = randomArray(N, nbrMixes);
	MergeSort(window, arr, N);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	arr = randomArray(N, nbrMixes);
	BucketSort(window, arr, N);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	arr = randomArray(N, nbrMixes);
	BubbleSort(window, arr, N);

	std::this_thread::sleep_for(std::chrono::seconds(1));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}

	return 0;
}