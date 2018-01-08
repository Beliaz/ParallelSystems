#include <iostream>
#include <chrono>

void printArray(int* array, int problemSize) {
    for (int i = 0; i < problemSize; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

//Tests if a newly placed queen can be part of a solution. This method is described by the backtracking algorithm.
bool validateNewQueen(int* array, int position, int newQueen) {
    for (int i = 0; i < position; i++) {
        int distance = position - i;
        int queensDistance = abs(array[i] - newQueen);
        if (distance == queensDistance || array[i] == newQueen)
            return false;
    }
    array[position] = newQueen;
    return true;
}

//Place new queens at position and finalize the iteration
//return the number of found solutions
int placeNewQueen(int* array, int position, int problemSize) {
    int foundPermutations = 0;
    for (int i = 0; i < problemSize; i++) {

        //If array get's initialized (needed for single threaded and openmp), there are no checks needed
        if (position == 0) {
            array[position] = i;
            foundPermutations += placeNewQueen(array, position + 1, problemSize);
            continue;
        }

        //If new Queen is not possible, try next position.
        if (!validateNewQueen(array, position, i))
                continue;

        //decide whether to count the found solution or to go on.
        if (position < problemSize-1) {
            foundPermutations += placeNewQueen(array, position+1, problemSize);
        }
        else {
            foundPermutations++;
        }
    }
    return foundPermutations;
}

int main(int argc, char** argv) {
    //Test if problem size was specified
    if (argc != 2) {
        printf("Please give exactly one problem size");
        return EXIT_FAILURE;
    }
    int problemSize = atoi(argv[1]);
    int foundPermutations = 0;

    auto start = std::chrono::high_resolution_clock::now();
#if defined(_OPENMP)
#pragma omp parallel
    {
        auto *array = new int[problemSize];
#pragma omp for reduction(+:foundPermutations)
        for (int i = 0; i < problemSize; i++) {
            array[0] = i;
            foundPermutations += placeNewQueen(array, 1, problemSize);
        }
    }
#else
    auto *array = new int[problemSize];
    foundPermutations = placeNewQueen(array, 0, problemSize);
#endif
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time = end - start;
    std::cout << "found " << foundPermutations << " options within " << time.count() << "s\n" << std::endl;
}