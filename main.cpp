#include <iostream>
#include <string>
#include <limits>
#include <exception>
#include <cmath>
#include <iomanip>

#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"
#include "Deque.h"
#include "Vector.h"
#include "SquareMatrix.h"

// --- Helper functions for Deque functional methods ---
int SquareFunc(int x){return x * x;}
bool IsEven(int x){return x % 2 == 0;}
int SumReducer(int a, int b){return a + b;}
bool DescendingSort(int a, int b){return a > b;}

// --- Input Utility ---
template <typename T>
T SafeInput(const std::string& prompt){
    T value;
    while(true){
        std::cout << prompt;
        if(std::cin >> value){
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Input error. Try again.\n";
    }
}

// --- Printing Utilities ---
void PrintDeque(const Deque<int>& dq, const std::string& name = "Deque"){
    std::cout << name << ": [ ";
    for(int i = 0; ; i++){
        try{
            std::cout << dq.Get(i) << " ";
        }catch(...){break;}
    }
    std::cout << "]\n";
}

void PrintVector(const Vector<int>& v, const std::string& name = "Vector"){
    std::cout << name << " (dim " << v.GetDimension() << "): ( ";
    for(int i = 0; i < v.GetDimension(); i++){
        std::cout << v.Get(i) << (i + 1 < v.GetDimension() ? ", " : " ");
    }
    std::cout << ")\n";
}

void PrintMatrix(const SquareMatrix<int>& m, const std::string& name = "Matrix"){
    std::cout << name << " (" << m.GetSize() << "x" << m.GetSize() << "):\n";
    for(int i = 0; i < m.GetSize(); i++){
        for(int j = 0; j < m.GetSize(); j++){
            std::cout << std::setw(4) << m.Get(i, j) << " ";
        }
        std::cout << "\n";
    }
}

// --- Module Runners ---

void RunDeque(){
    Deque<int> dq;
    while(true){
        std::cout << "\n--- DEQUE MENU ---\n"
                  << "1) PushFront  2) PushBack   3) PopFront    4) PopBack\n"
                  << "5) Map (^2)   6) Where (even) 7) Reduce (sum) 8) Concat\n"
                  << "9) SubDeque   10) Contain?   11) Sort (desc) 12) Merge\n"
                  << "13) Print     0) Back\n";
        int choice = SafeInput<int>("Choice: ");
        if(choice == 0) return;

        try{
            switch(choice){
                case 1: dq.PushFront(SafeInput<int>("Value: ")); break;
                case 2: dq.PushBack(SafeInput<int>("Value: ")); break;
                case 3: std::cout << "Removed: " << dq.PopFront() << "\n"; break;
                case 4: std::cout << "Removed: " << dq.PopBack() << "\n"; break;
                case 5: {
                    Deque<int> mapped = dq.Map(SquareFunc);
                    PrintDeque(mapped, "Mapped (x^2)");
                    break;
                }
                case 6: {
                    Deque<int> filtered = dq.Where(IsEven);
                    PrintDeque(filtered, "Where (even)");
                    break;
                }
                case 7: {
                    int start = SafeInput<int>("Start value: ");
                    std::cout << "Reduce Result: " << dq.Reduce(SumReducer, start) << "\n";
                    break;
                }
                case 8: {
                    Deque<int> other; other.PushBack(88); other.PushBack(99);
                    PrintDeque(dq.Concat(other), "Concat with [88, 99]");
                    break;
                }
                case 9: {
                    int s = SafeInput<int>("Start index: "), e = SafeInput<int>("End index: ");
                    PrintDeque(dq.GetSubDeque(s, e), "SubDeque");
                    break;
                }
                case 10: {
                    Deque<int> sub; sub.PushBack(SafeInput<int>("Value to find: "));
                    std::cout << (dq.ContainSubDeque(sub) ? "Contains\n" : "Does not contain\n");
                    break;
                }
                case 11: dq.Sort(DescendingSort); std::cout << "Sorted descending.\n"; break;
                case 12: {
                    Deque<int> other; other.PushBack(50);
                    PrintDeque(dq.Merge(other), "Merge with [50]");
                    break;
                }
                case 13: PrintDeque(dq); break;
            }
        }catch(const std::exception& e){std::cout << "Error: " << e.what() << "\n";}
    }
}

void RunVector(){
    int dim = SafeInput<int>("Vector dimension: ");
    Vector<int> v1(dim), v2(dim);
    for(int i = 0; i < dim; i++) v1(i) = SafeInput<int>("V1[" + std::to_string(i) + "]: ");
    for(int i = 0; i < dim; i++) v2(i) = SafeInput<int>("V2[" + std::to_string(i) + "]: ");

    while(true){
        std::cout << "\n--- VECTOR MENU ---\n"
                  << "1) Add  2) MultiplyByScalar  3) ScalarProduct  4) Norm  5) Print  0) Back\n";
        int choice = SafeInput<int>("Choice: ");
        if(choice == 0) return;

        try{
            if(choice == 1) PrintVector(v1.Add(v2), "V1 + V2");
            else if(choice == 2) PrintVector(v1.MultyplyByScalar(SafeInput<int>("Scalar: ")), "Result");
            else if(choice == 3) std::cout << "Scalar Product: " << v1.ScalarProductOfVectors(v2) << "\n";
            else if(choice == 4) std::cout << "V1 Norm: " << v1.Norm() << "\n";
            else if(choice == 5){PrintVector(v1, "V1"); PrintVector(v2, "V2");}
        }catch(const std::exception& e){std::cout << "Error: " << e.what() << "\n";}
    }
}

void RunMatrix(){
    int size = SafeInput<int>("Square matrix size: ");
    SquareMatrix<int> m(size);
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            m(i, j) = SafeInput<int>("M["+std::to_string(i)+"]["+std::to_string(j)+"]: ");

    while(true){
        std::cout << "\n--- MATRIX MENU ---\n"
                  << "1) Add  2) MultByScalar  3) MultByMatrix  4) Norm\n"
                  << "5) Swap Rows  6) Swap Cols  7) MultRowByScalar  8) Print  0) Back\n";
        int choice = SafeInput<int>("Choice: ");
        if(choice == 0) return;

        try{
            switch(choice){
                case 1: PrintMatrix(m.Add(m), "M + M"); break;
                case 2: PrintMatrix(m.MultiplyByScalar(SafeInput<int>("Scalar: ")), "Result"); break;
                case 3: PrintMatrix(m * m, "M * M"); break;
                case 4: std::cout << "Frobenius Norm: " << m.Norm() << "\n"; break;
                case 5: {
                    int r1 = SafeInput<int>("Row 1: "), r2 = SafeInput<int>("Row 2: ");
                    PrintMatrix(m.SwapRows(r1, r2), "Swapped Rows");
                    break;
                }
                case 6: {
                    int c1 = SafeInput<int>("Col 1: "), c2 = SafeInput<int>("Col 2: ");
                    PrintMatrix(m.SwapColumns(c1, c2), "Swapped Cols");
                    break;
                }
                case 7: {
                    int r = SafeInput<int>("Row index: "), s = SafeInput<int>("Scalar: ");
                    PrintMatrix(m.MultiplyRowByScalar(r, s), "Result");
                    break;
                }
                case 8: PrintMatrix(m); break;
            }
        }catch(const std::exception& e){std::cout << "Error: " << e.what() << "\n";}
    }
}

int main(){
    while(true){
        std::cout << "\n=== MAIN MENU ===\n"
                  << "1) Deque Operations\n"
                  << "2) Vector Operations\n"
                  << "3) Matrix Operations\n"
                  << "0) Exit\n";
        int choice = SafeInput<int>("Select module: ");
        if(choice == 0) break;

        if(choice == 1) RunDeque();
        else if(choice == 2) RunVector();
        else if(choice == 3) RunMatrix();
        else std::cout << "Unknown choice.\n";
    }
    return 0;
}
