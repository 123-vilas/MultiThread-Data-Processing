#include <iostream>
#include <windows.h>
#include <vector>

struct ThreadData {
    int threadId;
    HANDLE dataMutex;
    HANDLE dataReady;
};

DWORD WINAPI processDataThread(LPVOID lpParam);
void simulateDataInput(ThreadData* threadData);

int main() {
    const int numThreads = 2;

    // Create mutex and event objects
    HANDLE dataMutex = CreateMutex(NULL, FALSE, NULL);
    HANDLE dataReady = CreateEvent(NULL, FALSE, FALSE, NULL);

    // Initialize data processing threads
    std::vector<HANDLE> threads(numThreads);
    std::vector<ThreadData> threadData(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        threadData[i] = { i, dataMutex, dataReady };
        threads[i] = CreateThread(NULL, 0, processDataThread, &threadData[i], 0, NULL);
    }

    // Simulate data input
    simulateDataInput(&threadData[0]);

    // Wait for all threads to finish
    WaitForMultipleObjects(numThreads, threads.data(), TRUE, INFINITE);

    // Close handles
    CloseHandle(dataMutex);
    CloseHandle(dataReady);
    for (int i = 0; i < numThreads; ++i) {
        CloseHandle(threads[i]);
    }

    return 0;
}

DWORD WINAPI processDataThread(LPVOID lpParam) {
    ThreadData* threadData = static_cast<ThreadData*>(lpParam);
    int threadId = threadData->threadId;
    HANDLE dataMutex = threadData->dataMutex;
    HANDLE dataReady = threadData->dataReady;

    while (true) {
        WaitForSingleObject(dataReady, INFINITE);
        WaitForSingleObject(dataMutex, INFINITE);

        // Simulate data processing
        std::cout << "Thread " << threadId << " processing data." << std::endl;
        Sleep(1000);  // Simulate data processing time

        // Reset the flag
        ResetEvent(dataReady);
        ReleaseMutex(dataMutex);
    }

    return 0;
}

void simulateDataInput(ThreadData* threadData) {
    // Simulate data input from multiple sources
    // Modify this function based on your specific use case

    // Example: Read data from files, sensors, or network streams
    // Notify processing threads that data is ready

    while (true) {
        SetEvent(threadData->dataReady);
        std::cout << "Data ready. Signaling threads." << std::endl;
        Sleep(2000);  // Simulate time between data arrivals
    }
}
