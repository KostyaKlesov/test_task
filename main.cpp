#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <numeric>
#include <chrono>
#include <thread>
using namespace std;
using namespace chrono;


int getRandomIndex(int N) {
   return rand() % N;
}


void generateIndices(std::vector<int>& index, int start, int end, int N) {
    for (int i = start; i < end; i++) {
        index[i] = getRandomIndex(N);
    }
}


std::vector<int> Index_Elements(int K) {
    size_t N = 100000000;
    std::vector<int> index(K);

    int numThreads = std::thread::hardware_concurrency();
    int chunkSize = K / numThreads;

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; i++) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? K : (i + 1) * chunkSize;
        threads.emplace_back(generateIndices, std::ref(index), start, end, N);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return index;
}


std::vector<double> Summ_Elements(const std::vector<double>& A, int K, std::vector<int>& index) {
   if (K > static_cast<int>(A.size())) {
       throw std::invalid_argument("Количество элементов для суммирования превышает количество элементов в массиве");
   }
   std::vector<double> result(K);
   for (int i = 0; i < K; ++i) {
       //size_t randomIndex = getRandomIndex(A.size());
       //cout<<getRandomIndex(A.size())<<" ";
       //cout << A[i] << " ";
        result[i] += A[index[i]];
   }
   /*for (int i = 0; i < resultVector.size(); i++)
   {
    cout<<resultVector[i]<<" ";
   } */
   
   return result;
}


int main() {
    size_t N = 100000000;
    size_t K = 50000000;
    int size = 500000000;
    std::vector<double> A(N);
    std::vector<int> index(K);
    std::mt19937 rng(std::random_device{}()); 
    std::uniform_real_distribution<> dist(5000000000, 10000000000); 

    
    for (size_t i = 0; i < N; ++i) {
        do {
            A[i] = dist(rng); 
        } while (A[i] <= size); 
    }
    /*
    cout<< "Исходные элементы:"<<" ";
    for (auto num : A) {
        std::cout<< num << " ";
    }*/
    
    auto start = high_resolution_clock::now();
    auto summOfElements = Summ_Elements(A, K, index);
    auto stop = high_resolution_clock::now();
    duration<double, milli> time_span = duration_cast<duration<double, milli>>(stop - start);
    cout << "Время выполнения программы: " << time_span.count() << " миллисекунд" << endl;
    
    double total_sum = std::accumulate(summOfElements.begin(), summOfElements.end(), 0.0);
    cout << total_sum << '\n';
    //return 0;
}