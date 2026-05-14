#include <iostream>
#include <string>
#include <limits>
#include <exception>

#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"
#include "Deque.h"
#include "Vector.h"
#include "SquareMatrix.h"

using SequencePtr = Sequence<int>*;

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
        std::cout << "Invalid input. Try again.\n";
    }
}

void AddToSequenceStorage(DynamicArray<SequencePtr>& storage, SequencePtr item){
    int currentSize = storage.GetSize();
    storage.Resize(currentSize + 1);
    storage.Set(currentSize, item);
}

void PrintSequence(const SequencePtr sequence, int id){
    if(!sequence){
        std::cout << "[" << id << "] Empty\n";
        return;
    }

    std::cout << "[" << id << "] { ";
    for(int i = 0; i < sequence->GetLength(); i++){
        std::cout << sequence->Get(i);
        if(i + 1 < sequence->GetLength()){
            std::cout << ", ";
        }
    }
    std::cout << " }\n";
}

void CreateNewSequence(DynamicArray<SequencePtr>& storage, int& currentIndex){
    int type = SafeInput<int>("Sequence type (1 - MutableArray, 2 - ImmutableArray, 3 - List): ");
    int count = SafeInput<int>("Elements count: ");

    int* data = new int[count];
    for(int i = 0; i < count; i++){
        data[i] = SafeInput<int>("Elem #" + std::to_string(i) + ": ");
    }

    SequencePtr newSequence = nullptr;
    if(type == 1){
        newSequence = new MutableArraySequence<int>(data, count);
    }
    else if(type == 2){
        newSequence = new ImmutableArraySequence<int>(data, count);
    }
    else{
        newSequence = new ListSequence<int>(data, count);
    }

    delete[] data;
    AddToSequenceStorage(storage, newSequence);
    currentIndex = storage.GetSize() - 1;
}

void HandleSequenceOperation(DynamicArray<SequencePtr>& storage, int& currentIndex, int choice){
    if(currentIndex < 0 || currentIndex >= storage.GetSize()){
        std::cout << "No active sequence.\n";
        return;
    }

    SequencePtr sequence = storage.Get(currentIndex);
    SequencePtr result = nullptr;

    switch(choice){
        case 4:{
            int value = SafeInput<int>("Value to append: ");
            result = sequence->Append(value);
            break;
        }
        case 5:{
            int value = SafeInput<int>("Value to prepend: ");
            result = sequence->Prepend(value);
            break;
        }
        case 6:{
            int index = SafeInput<int>("Index: ");
            int value = SafeInput<int>("Value: ");
            result = sequence->InsertAt(value, index);
            break;
        }
        case 7:{
            int index = SafeInput<int>("Concat with sequence index: ");
            if(index < 0 || index >= storage.GetSize()) throw IndexOutOfRange();
            result = sequence->Concat(storage.Get(index));
            break;
        }
        case 8:{
            int start = SafeInput<int>("Start: ");
            int end = SafeInput<int>("End: ");
            result = sequence->GetSubsequence(start, end);
            break;
        }
        default:
            return;
    }

    if(result != sequence){
        AddToSequenceStorage(storage, result);
        currentIndex = storage.GetSize() - 1;
    }
}

void DemoDequeVectorMatrix(){
    std::cout << "\n--- Demo: Deque ---\n";
    Deque<int> deque;
    deque.PushBack(10);
    deque.PushFront(5);
    deque.PushBack(15);
    std::cout << "Deque: [" << deque.Get(0) << ", " << deque.Get(1) << ", " << deque.Get(2) << "]\n";

    std::cout << "\n--- Demo: Vector ---\n";
    int leftData[] = {1, 2, 3};
    int rightData[] = {4, 5, 6};
    Vector<int> left(leftData, 3);
    Vector<int> right(rightData, 3);
    Vector<int> sum = left.Add(right);
    std::cout << "v1 + v2 = (" << sum.Get(0) << ", " << sum.Get(1) << ", " << sum.Get(2) << ")\n";

    std::cout << "\n--- Demo: SquareMatrix ---\n";
    int matrixData[] = {1, 2, 3, 4};
    SquareMatrix<int> matrix(matrixData, 2);
    SquareMatrix<int> scaled = matrix * 2;
    std::cout << "Matrix * 2 -> ["
              << scaled.Get(0, 0) << " " << scaled.Get(0, 1) << "; "
              << scaled.Get(1, 0) << " " << scaled.Get(1, 1) << "]\n\n";
}

int main(){
    DynamicArray<SequencePtr> storage(0);
    int currentIndex = -1;

    while(true){
        std::cout << "--- Menu ---\n"
                  << "1) Create sequence\n"
                  << "2) Show sequences\n"
                  << "3) Select active sequence\n"
                  << "4) Append\n"
                  << "5) Prepend\n"
                  << "6) Insert\n"
                  << "7) Concat\n"
                  << "8) Subsequence\n"
                  << "9) Delete active\n"
                  << "10) Demo Deque/Vector/Matrix\n"
                  << "0) Exit\n";

        int choice = SafeInput<int>("Pick: ");

        try{
            if(choice == 0){
                for(int i = 0; i < storage.GetSize(); i++){
                    delete storage.Get(i);
                }
                return 0;
            }

            switch(choice){
                case 1:
                    CreateNewSequence(storage, currentIndex);
                    break;
                case 2:
                    for(int i = 0; i < storage.GetSize(); i++) PrintSequence(storage.Get(i), i);
                    break;
                case 3:{
                    int index = SafeInput<int>("Index: ");
                    if(index < 0 || index >= storage.GetSize()) throw IndexOutOfRange();
                    currentIndex = index;
                    break;
                }
                case 4: case 5: case 6: case 7: case 8:
                    HandleSequenceOperation(storage, currentIndex, choice);
                    break;
                case 9:{
                    if(currentIndex < 0 || currentIndex >= storage.GetSize()) break;
                    delete storage.Get(currentIndex);
                    for(int i = currentIndex; i < storage.GetSize() - 1; i++){
                        storage.Set(i, storage.Get(i + 1));
                    }
                    storage.Resize(storage.GetSize() - 1);
                    currentIndex = storage.GetSize() > 0 ? 0 : -1;
                    break;
                }
                case 10:
                    DemoDequeVectorMatrix();
                    break;
                default:
                    std::cout << "Unknown choice.\n";
            }
        }
        catch(const std::exception& exception){
            std::cout << "Error: " << exception.what() << "\n";
        }
    }
}
