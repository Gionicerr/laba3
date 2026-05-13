#include <iostream>
#include <string>
#include <limits>
#include <exception>
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"

using SequencePtr = Sequence<int>*;

template <typename T>
T SafeInput(const std::string& prompt){
    T value;
    while(true){
        std::cout<<prompt;
        if(std::cin>>value){
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"Invalueid input. Try again.\n";
    }
}

void AddToSequenceStorage(DynamicArray<SequencePtr>& storage, SequencePtr item){
    int current_size = storage.GetSize();
    storage.Resize(current_size+1);
    storage.Set(current_size, item);
}

void CreateNewSequence(DynamicArray<SequencePtr>& storage, int& current_index){
    int type = SafeInput<int>("Sequence type:    (1 - Array; 2 - List)");
    int count = SafeInput<int>("Elements count: ");
    int* data = new int[count];
    for(int i = 0; i < count; i++){
        data[i] = SafeInput<int>("Elem #" + std::to_string(i) + ": ");
    }
    SequencePtr new_sequence = nullptr;
    if(type == 1){
        int mode = SafeInput<int>("Mutable - 1; Immutable - 0: \n");
        if(mode){
            new_sequence = new MutableArraySequence<int>(data, count);
        }
        else{
            new_sequence = new ImmutableArraySequence<int>(data, count);
        }
    }
    else{
        new_sequence = new ListSequence<int>(data, count);
    }
    delete[] data;
    AddToSequenceStorage(storage, new_sequence);
    current_index = storage.GetSize();
    std::cout<<"Sequence selected: "<<current_index<<"\n";
}

void PrintSequence(const SequencePtr sequence, int id){
    if(!sequence){
        std::cout<<"[ "<<id<<" ] Empty\n";
        return;
    }
    std::cout<<"[ "<<id<<" ] : { ";
    for(int i = 0; i < sequence->GetLength(); i++){
        std::cout<<sequence->Get(i);
        if(i < sequence->GetLength()-1){
            std::cout<<", ";
        }
    }
    std::cout<<" }\n";
}

void HandleOperations(DynamicArray<SequencePtr>& storage, int& current_index, int choice){
    if(current_index < 0 || current_index >= storage.GetSize()){
        std::cout<<"No sequence selected.\n";
        return;
    }
    SequencePtr sequence = storage.Get(current_index);
    SequencePtr result = nullptr;
    switch(choice){
        case 1:{
            int value = SafeInput<int>("value to append: ");
            result = sequence->Append(value);
            break;
        }
        case 2:{
            int value = SafeInput<int>("value to prepend: ");
            result = sequence->Prepend(value);
            break;
        }
        case 3:{
            int index = SafeInput<int>("Index: ");
            int value = SafeInput<int>("value: ");
            result = sequence->InsertAt(value, index);
            break;
        }
        case 4:{
            int target_index = SafeInput<int>("Index of sequence to concat with: ");
            if(target_index >= 0 && target_index < storage.GetSize()){
                result = sequence->Concat(storage.Get(target_index));
            }
            else{
                std::cout << "Invalueid index.\n";
                return;
            }
            break;
        }
        case 5:{
            int start = SafeInput<int>("Start index: ");
            int end = SafeInput<int>("End index: ");
            result = sequence->GetSubsequence(start, end);
            break;
        }
        case 6:
            break;
        }
    if(result != sequence){
        AddToSequenceStorage(storage, result);
        current_index = storage.GetSize() - 1;
        std::cout<<"Immutable operation done.\n";
    }
    else{
        std::cout<<"Mutable operation done.\n";
    }
}
int main(){
    DynamicArray<SequencePtr> storage(0);
    int current_index = -1;
    while(true){
        std::cout<<"---Menu---\n";
        if(current_index >= 0 && current_index < storage.GetSize()){
            std::cout<<"Active: ";
            PrintSequence(storage.Get(current_index), current_index);
        }
        else{
            std::cout<<"No sequence selected.\n";
        }
        std::cout<<"1) Create\n2) Show all\n3) Select\n4) Append\n5) Prepend\n6) Insert\n7) Concat\n8) Subseq\n9) Delete current\n0) Exit\n";
        int choice = SafeInput<int>("Pick: ");
        try{
            if(choice == 0){
                for(int i = 0; i < storage.GetSize(); i++){
                    delete storage.Get(i);
                }
                break;
            }
            switch(choice){
                case 1:{
                    CreateNewSequence(storage, current_index);
                    break;
                }
                case 2:{
                    for(int i = 0; i < storage.GetSize(); i++){
                        PrintSequence(storage.Get(i), i);
                    }
                    break;
                }
                case 3:{
                    int index = SafeInput<int>("Index: ");
                    if(index > storage.GetSize()) throw IndexOutOfRange();
                    else{
                        current_index = index;
                    }
                    break;
                }
                case 4: case 5: case 6: case 7: case 8:
                    HandleOperations(storage, current_index, choice);
                    break;
                case 9:{
                    if(current_index < 0) break;
                    delete storage.Get(current_index);
                    for(int i = 0; i < storage.GetSize()-1; i++){
                        storage.Set(i, storage.Get(i+1));
                    }
                    storage.Resize(storage.GetSize()-1);
                    current_index = (storage.GetSize() > 0) ? 1 : -1;
                    break;
                }
                default:
                    std::cout<<"Unkown choice\n";
            }
        }
        catch(const std::exception& e){
            std::cout<<"Error: "<<e.what()<<"\n";
        }
    }
    return 0;
}