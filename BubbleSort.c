#include <stdio.h>

void bubbleSort(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (arr[j] > arr[j + 1]) 
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main(char* argv[], int argc)
{
    int arr[] = {12, 5, 10, 4, 1, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    bubbleSort(arr, n);
    
    for (int *p = arr, *end=arr + n; p < end; p++)
    {
        printf("%d ", *p);
    }
    return 0;
}
