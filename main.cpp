/*
	Furkan Pala
	
	How to compile:
	g++ -std=c++11 main.cpp
	
	How to run:
	./a.out N
	
	---------------------------IMPORTANT NOTE---------------------------------------------
	For (d) section of Part 2, since algorithm works on worst case, i.e., O(n^2),
	there are too many function calls so stack overflow exception occurs when N is large. That's why
	I have compiled my code with the following command in which I increase the allocated
	stack size to 256mb
	
	g++ -std=c++11 -Wl,--stack,268435456 main.cpp
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

struct Sale
{
	string sCountry, sItemType, sOrderId;
	int unitsSold = 0;
	double profit = 0.0;
};

int Partition(vector<Sale> &data, int beginIndex, int endIndex)
{
	Sale pivot = data[endIndex]; // Last element is pivot
	string sPivotCountry = pivot.sCountry;
	double pivotProfit = pivot.profit;

	int left = beginIndex;	// Left comparator
	int right = beginIndex; // Right comparator

	// Iterate "right" until the end of the array
	for (; right <= endIndex; right++)
	{

		// If "right" sees endIndex (Pivot), swap it with "left"
		if (right == endIndex)
		{
			data[endIndex] = data[left];
			data[left] = pivot;
		}

		// If "right" is alphabetically before than pivot,
		// OR
		// if "right" and pivot are the same country AND right has greater profit than pivot
		// (because they should be in descending order by profit,
		// which means larger profit will be on the left side of pivot)
		// SWAP and increase "left" by one
		else if (
			data[right].sCountry.compare(sPivotCountry) < 0 ||
			(data[right].sCountry.compare(sPivotCountry) == 0 && data[right].profit > pivotProfit))
		{
			if (right != left)
			{
				Sale temp = data[left];
				data[left] = data[right];
				data[right] = temp;
			}
			left++;
		}
	}

	return left; // Return the pivot position
}

void QuickSort(vector<Sale> &data, int beginIndex, int endIndex)
{
	if (beginIndex == endIndex || beginIndex > endIndex)
		return;

	int pivotIndex = Partition(data, beginIndex, endIndex);

	QuickSort(data, beginIndex, pivotIndex - 1);
	QuickSort(data, pivotIndex + 1, endIndex);
}

int main(int argc, char *argv[])
{
	ifstream file;
	file.open("sales.txt");

	if (!file)
	{
		cerr << "File cannot be opened!" << endl;
		exit(1);
	}

	if (argc < 2)
	{
		cerr << "Number is required" << endl;
		exit(1);
	}

	int N = atoi(argv[1]);
	string line;
	Sale sale;
	vector<Sale> sales;
	sales.reserve(N);

	getline(file, line); //this is the header line

	for (int i = 0; i < N; i++)
	{
		getline(file, line, '\t'); //country (string)
		sale.sCountry = line;
		getline(file, line, '\t'); //item type (string)
		sale.sItemType = line;
		getline(file, line, '\t'); //order id (string)
		sale.sOrderId = line;
		file >> sale.unitsSold; //units sold (integer)
		file >> sale.profit;	//total profit (float)
		sales.emplace_back(sale);
		getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
	}
	file.close();

	auto start_timer = chrono::high_resolution_clock::now();
	QuickSort(sales, 0, N - 1);
	auto stop_timer = chrono::high_resolution_clock::now();

	ofstream sortedFile("sorted.txt");

	sortedFile << "Country" << '\t';
	sortedFile << "Item Type" << '\t';
	sortedFile << "Order ID" << '\t';
	sortedFile << "Units Sold" << '\t';
	sortedFile << "Total Profit" << endl;

	for (auto &i : sales)
	{
		sortedFile << i.sCountry << '\t';
		sortedFile << i.sItemType << '\t';
		sortedFile << i.sOrderId << '\t';
		sortedFile << i.unitsSold << '\t';
		sortedFile << fixed << i.profit << endl;
	}

	sortedFile.close();

	auto duration = chrono::duration_cast<chrono::microseconds>(stop_timer - start_timer);

	cout << "N: " << N << endl;
	cout << "Sorting took " << duration.count() / 1000.f << " ms" << endl;

	return 0;
}
