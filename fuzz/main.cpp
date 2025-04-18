#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include <windows.h>

// Function to randomly flip between 1 and 10 bytes in the seed input
void mutateInput(const char *seed, char *output, size_t len)
{
    memcpy(output, seed, len);
    int mutations = rand() % 10 + 1;
    for(int i = 0; i < mutations; i++)
    {
        int pos = rand() % len;
        output[pos] ^= rand() % 256; // Random byte flip
    }
}

double getTime()
{
    LARGE_INTEGER freq, t;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / freq.QuadPart;
}

void fuzz(const char *benchmark, const char *seed, const int numImputs)
{
    printf("Fuzzing executable: %s (%d inputs)\n", benchmark, numImputs);
    const double startTime = getTime();
    const size_t seedLength = strlen(seed);
    char *input = (char *)malloc(seedLength + 1);

    for (int i = 0; i < numImputs; i++)
    {
        mutateInput(seed, input, seedLength);
        char* tempFile = (char*)malloc(64);
        snprintf(tempFile, 64, "tempfile%s%d.txt", benchmark, i);
        FILE *file = fopen(tempFile, "wb");
        if (!file)
        {
            perror("Failed to open temp file");
            exit(1);
        }
        fwrite(input, 1, seedLength, file);
        fclose(file);

        STARTUPINFO si = {sizeof(si)};
        PROCESS_INFORMATION pi;
        char cmd[256];
        snprintf(cmd, sizeof(cmd), "%s %s", benchmark, tempFile);
        if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            fprintf(stderr, "CreateProcess failed (%lu)\n", GetLastError());
            exit(1);
        }
        WaitForSingleObject(pi.hProcess, 1000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        free(tempFile);
    }

    double elapsedTime = getTime() - startTime;
    double rate = numImputs / elapsedTime;
    printf(" Total process execution time: %.2f\n", elapsedTime);
    printf(" Rate of inputs tested       : %.2f inputs/sec\n", rate);
    free(input);
}

int main()
{
    // Constants for fuzzing
    const char* benchmarks[5] = {"sublime_text.exe",
                                 "notepad.exe",
                                 "notepad++.exe",
                                 "atom.exe",
                                 "gvim.exe"};
    const char* fileText = "Hello World!\nThis is input text which will be\n\tused to test each"
                           "of the test text editors \n\nwith random byte flips.";
    const int numImputs = 500;

    for (int programIndex = 0; programIndex < 5; programIndex++)
    {
        fuzz(benchmarks[programIndex], fileText, numImputs);
    }
    return 0;
}
// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.