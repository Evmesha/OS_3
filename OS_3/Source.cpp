#include <windows.h>
#include <iostream>

using namespace std;
CRITICAL_SECTION cs, cs1;
HANDLE hOutEvent;
int k;
struct param {
    int size;
    float* arr;
    
};
 float sum = 0;
DWORD WINAPI SumOfElements(LPVOID perem){

    EnterCriticalSection(&cs1);
    WaitForSingleObject(hOutEvent, INFINITE);
   
    
    param* per = (param*)perem;
    std::cout << "Sum Started" << endl;

   
   

    for (int i = 0; i < k; i++)
    {
        sum += per->arr[i];
    }

    std::cout << "Sum of first k elements: " << sum << endl;
    
    LeaveCriticalSection(&cs1);
    std::cout << "Sum finished"  << endl;
   
    return 0;
}

DWORD WINAPI work(LPVOID perem)
{
    EnterCriticalSection(&cs);
    param* per = (param*)perem;
    std:: cout << "work started" << endl;
    int sec;
    std::cout << "miliseconds " << endl;
    cin >> sec;
    
    for (int i = 0; i < k; i++) {


        for (int j = 0; j < k - 1; j++) {
            if (per->arr[j] > per->arr[j + 1]) {
                float b = per->arr[j];
                per->arr[j] = per->arr[j + 1];
                per->arr[j + 1] = b;

            }
            Sleep(sec);
        }

    }

    std:: cout << "work finished" << endl;

    LeaveCriticalSection(&cs);
    return 0;
}

int main()
{
    InitializeCriticalSection(&cs);
    InitializeCriticalSection(&cs1);
    hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hOutEvent == NULL)
        return GetLastError();
    HANDLE  hThread;
    DWORD  IDThread;
    int size;
    std::cout << "Enter size of Arr" << endl;
    cin >> size;

    float* arr = new float[size];
    std::cout << "Enter elements" << endl;
    for (int i = 0; i < size; i++)
    {
        cin >> arr[i];
    }
    param* per = new param();
    per->size = size;
    per->arr = arr;
   
    std::cout << "Size of Array: " << size << endl;
    std::cout << "BAse Array: ";
    for (int i = 0; i < size; i++)
    {
        std::cout << arr[i] << " ";
    }

    std::cout << endl;
    HANDLE hThreadwork;
    DWORD IDThreadwork;
    DWORD dwCountwork;
    HANDLE hThreadSumElement;
    DWORD IDThreadwoSumElement;
    DWORD dwCountSumElement;
    
    hThreadwork = CreateThread(NULL, 0, work, (void*)per, CREATE_SUSPENDED, &IDThreadwork);
   
    std::cout << "Enter k: ";
    cin >> k;
    dwCountwork = ResumeThread(hThreadwork);
    hThreadSumElement = CreateThread(NULL, 0, SumOfElements, (LPVOID)per, NULL, &IDThreadwoSumElement);
    
    Sleep(50);
   
    EnterCriticalSection(&cs);
    cout << "First k elements after sort " << endl;
    for (int i = 0; i < k; i++) 
        std::cout << arr[i] << " ";
   
    cout << endl;
    LeaveCriticalSection(&cs);

  


    DeleteCriticalSection(&cs);
  
   
  
    //Sleep(50);
  
   

    SetEvent(hOutEvent);
    EnterCriticalSection(&cs1);

    cout << "Elements after k: " << endl;
    for (int i = k; i < size; i++)  {
        std::cout << arr[i] << " ";
     }
    cout << endl;
    cout << "Summ the second time: " << sum << endl;
    LeaveCriticalSection(&cs1);
   

    WaitForSingleObject(hThreadwork, INFINITE);

    CloseHandle(hThreadwork);

    WaitForSingleObject(hThreadSumElement, INFINITE);
    CloseHandle(hThreadSumElement);
   
    delete[] arr;


    return 0;
}