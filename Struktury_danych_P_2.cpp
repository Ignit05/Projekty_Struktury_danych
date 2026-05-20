#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;
//Przechowuje dane i ich priorytet
struct Element {
    int value;
    int priority;
};

// 1. Kolejka Kopiec binarny (Max-Heap)
class PriorityQueueHeap {
    vector<Element> heap;

    void heapifyUp(int idx) {
        while (idx > 0 && heap[(idx - 1) / 2].priority < heap[idx].priority) {
            swap(heap[idx], heap[(idx - 1) / 2]);
            idx = (idx - 1) / 2;
        }
    }

    void heapifyDown(int idx) {
        int maxIdx = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        if (left < heap.size() && heap[left].priority > heap[maxIdx].priority) maxIdx = left;
        if (right < heap.size() && heap[right].priority > heap[maxIdx].priority) maxIdx = right;
        if (idx != maxIdx) {
            swap(heap[idx], heap[maxIdx]);
            heapifyDown(maxIdx);
        }
    }

public:
    void push(int val, int prio) {
        heap.push_back({val, prio});
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    Element peek() {
        if (heap.empty()) throw runtime_error("Kolejka jest pusta");
        return heap[0];
    }

    int size() { return heap.size(); }

    void modifyPriority(int val, int newPrio) {
        for (int i = 0; i < heap.size(); i++) {
            if (heap[i].value == val) {
                int oldPrio = heap[i].priority;
                heap[i].priority = newPrio;
                if (newPrio > oldPrio) heapifyUp(i);
                else heapifyDown(i);
                return;
            }
        }
    }
};

// 2. Kolejka lista posortowana
class PriorityQueueSorted {
    vector<Element> data;
public:
    void push(int val, int prio) {
        auto it = lower_bound(data.begin(), data.end(), prio, [](const Element& e, int p) {
            return e.priority > p; // sortuje malejąco
        });
        data.insert(it, {val, prio});
    }

    void pop() { if (!data.empty()) data.erase(data.begin()); }
    Element peek() { return data[0]; }
    int size() { return data.size(); }
    
    void modifyPriority(int val, int newPrio) {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (it->value == val) {
                data.erase(it);
                push(val, newPrio);
                return;
            }
        }
    }
};

// 3. Kolejka Lista niesortowana
class PriorityQueueUnsorted {
    vector<Element> data;
public:
    void push(int val, int prio) { data.push_back({val, prio}); }

    int getHighestPrioIdx() {
        int idx = 0;
        for (int i = 1; i < data.size(); i++) {
            if (data[i].priority > data[idx].priority) idx = i;
        }
        return idx;
    }

    void pop() {
        if (data.empty()) return;
        data.erase(data.begin() + getHighestPrioIdx());
    }

    Element peek() { return data[getHighestPrioIdx()]; }
    int size() { return data.size(); }

    void modifyPriority(int val, int newPrio) {
        for (auto& e : data) {
            if (e.value == val) {
                e.priority = newPrio;
                return;
            }
        }
    }
};

int main() {
    cout << "Test: Kolejka lista nieposortowana" << endl;

    PriorityQueueUnsorted pqUnsorted;
    pqUnsorted.push(10, 1);
    pqUnsorted.push(50, 5);
    pqUnsorted.push(103, 3);

    cout << "Lista niesortowana - najwyzszy priorytet (expected 5): " << pqUnsorted.peek().priority << endl;
    
    pqUnsorted.modifyPriority(10, 10);
    cout << "Po modyfikacji (expected 10): " << pqUnsorted.peek().priority << endl;

    pqUnsorted.pop();
    cout << "Po usunieciu - najwyzszy priorytet (expected 5): " << pqUnsorted.peek().priority << endl;
    cout << "Rozmiar kolejki(expected 2): " << pqUnsorted.size() << endl;

    cout << "\nTesty zakonczone pomyslnie!" << endl;
    return 0;
}