#ifndef _MYSORTING_H_
#define _MYSORTING_H_

#include <list>
#include <vector>

class Sorting
{
public:
	virtual ~Sorting() {};

	template <typename T>
	static void QuickSort(std::vector<T>& arr, int b, int e, bool(*cmp)(const T&, const T&))
	{
		int pivot;

		if (b < e)
		{
			pivot = b;
			pivot = Partition(arr, b, e, pivot, cmp);
			if (b < (pivot - 1))
				QuickSort(arr, b, (pivot - 1), cmp);
			if ((pivot + 1) < e)
				QuickSort(arr, (pivot + 1), e, cmp);
		}
	}

	template <typename T>
	static void QuickSort(std::vector<T*>& arr, int b, int e, bool(*cmp)(const T*, const T*))
	{
		int pivot;

		if (b < e)
		{
			pivot = b;
			pivot = Partition(arr, b, e, pivot, cmp);
			if (b < (pivot - 1))
				QuickSort(arr, b, (pivot - 1), cmp);
			if ((pivot + 1) < e)
				QuickSort(arr, (pivot + 1), e, cmp);
		}
	}

	template <typename T>
	static void QuickSort(std::list<T*>& arr, int b, int e, bool(*cmp)(const T*, const T*))
	{
		int pivot;

		if (b < e)
		{
			pivot = b;
			pivot = Partition(arr, b, e, pivot, cmp);
			if (b < (pivot - 1))
				QuickSort(arr, b, (pivot - 1), cmp);
			if ((pivot + 1) < e)
				QuickSort(arr, (pivot + 1), e, cmp);
		}
	}

private:

	template <typename T>
	static int Partition(std::vector<T>& arr, int b, int e, int pivot, bool(*cmp)(const T&, const T&))
	{
		T		tmp;
		int j = b;

		tmp = arr[pivot];
		arr[pivot] = arr[e];
		arr[e] = tmp;
		for (size_t i = b; i < e; i++)
		{
			if (cmp(arr[i], arr[e]))
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
				j++;
			}
		}
		tmp = arr[e];
		arr[e] = arr[j];
		arr[j] = tmp;
		return (j);
	}

	template <typename T>
	static int Partition(std::vector<T*>& arr, int b, int e, int pivot, bool(*cmp)(const T*, const T*))
	{
		T		*tmp;
		int j = b;

		tmp = arr[pivot];
		arr[pivot] = arr[e];
		arr[e] = tmp;
		for (size_t i = b; i < e; i++)
		{
			if (cmp(arr[i], arr[e]))
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
				j++;
			}
		}
		tmp = arr[e];
		arr[e] = arr[j];
		arr[j] = tmp;
		return (j);
	}

	template <typename T>
	static int Partition(std::list<T*>& arr, int b, int e, int pivot, bool(*cmp)(const T*, const T*))
	{
		T		*tmp;
		int j = b;

		tmp = arr[pivot];
		arr[pivot] = arr[e];
		arr[e] = tmp;
		for (size_t i = b; i < e; i++)
		{
			if (cmp(arr[i], arr[e]))
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
				j++;
			}
		}
		tmp = arr[e];
		arr[e] = arr[j];
		arr[j] = tmp;
		return (j);
	}
};





#endif /* !_MYSORTING_H_ */
