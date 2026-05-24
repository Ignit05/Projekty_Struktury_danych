#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;

// Przechowuje dane i ich priorytet
struct Element {
    int value;
    int priority;
};

// Kolejka Kopiec binarny (Max-Heap)
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

// Kolejka lista posortowana
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

// Kolejka Lista niesortowana
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


// --- FUNKCJE OBSŁUGI PLIKÓW ---

// Generuje losowe dane i zapisuje je do pliku tekstowego
void zapiszDaneDoPliku(const string& nazwaPliku, int ilosc) {
    ofstream plik(nazwaPliku);
    if (!plik.is_open()) {
        throw runtime_error("Nie udalo sie utworzyc pliku do zapisu!");
    }

    random_device rd; 
    mt19937 gen(rd()); 
    uniform_int_distribution<> valDist(1, 1000000); 
    uniform_int_distribution<> prioDist(1, 1000);

    for (int i = 0; i < ilosc; ++i) {
        // Zapisujemy parę: wartość [spacja] priorytet [nowa linia]
        plik << valDist(gen) << " " << prioDist(gen) << "\n";
    }

    plik.close();
    cout << "[INFO] Wygenerowano nowe dane i zapisano do pliku: " << nazwaPliku << endl;
}

// Wczytuje dane z pliku tekstowego do wektora
vector<Element> wczytajDaneZPliku(const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    vector<Element> dane;

    if (!plik.is_open()) {
        throw runtime_error("Nie udalo sie otworzyc pliku do odczytu!");
    }

    int val, prio;
    // Wczytujemy dopóki udaje się czytać pary liczb
    while (plik >> val >> prio) {
        dane.push_back({val, prio});
    }

    plik.close();
    cout << "[INFO] Pomyslnie wczytano " << dane.size() << " elementow z pliku." << endl;
    return dane;
}

// --- FUNKCJE TESTUJĄCE (bez zmian) ---

template <typename QueueType>
void basicTest(const string& name) {
    cout << "--- Test logiki: " << name << " ---" << endl;
    QueueType pq;
    pq.push(10, 1);
    pq.push(50, 5);
    pq.push(103, 3);
    cout << "Najwyzszy priorytet (oczekiwane 5): " << pq.peek().priority << endl;
    pq.modifyPriority(10, 10);
    cout << "Po modyfikacji (oczekiwane 10): " << pq.peek().priority << endl;
    pq.pop();
    cout << "Po usunieciu - najwyzszy priorytet (oczekiwane 5): " << pq.peek().priority << endl;
    cout << "Rozmiar kolejki (oczekiwane 2): " << pq.size() << endl;
    cout << endl;
}

template <typename QueueType>
void performanceTest(const string& name, const vector<Element>& testData) {
    QueueType pq;
    
    // 1. Pomiar DODAWANIA
    auto startPush = chrono::high_resolution_clock::now();
    for (const auto& el : testData) {
        pq.push(el.value, el.priority);
    }
    auto endPush = chrono::high_resolution_clock::now();

    // 2. Pomiar MODYFIKACJI
    auto startModify = chrono::high_resolution_clock::now();
    for (const auto& el : testData) {
        pq.modifyPriority(el.value, el.priority + 5); 
    }
    auto endModify = chrono::high_resolution_clock::now();

    // 3. Pomiar USUWANIA
    auto startPop = chrono::high_resolution_clock::now();
    while(pq.size() > 0) {
        pq.pop();
    }
    auto endPop = chrono::high_resolution_clock::now();

    // Obliczanie czasów bezpośrednio w milisekundach (jako liczby zmiennoprzecinkowe double)
    chrono::duration<double, std::milli> durationPush = endPush - startPush;
    chrono::duration<double, std::milli> durationModify = endModify - startModify;
    chrono::duration<double, std::milli> durationPop = endPop - startPop;
    
    double total = durationPush.count() + durationModify.count() + durationPop.count();

    // Wyświetlenie wyników w milisekundach (ms)
    cout << "=== Wyniki dla: " << name << " ===" << endl;
    cout << "  Dodawanie (50000x push):     " << durationPush.count() << " ms" << endl;
    cout << "  Modyfikacja (50000x modify): " << durationModify.count() << " ms" << endl;
    cout << "  Usuwanie (50000x pop):       " << durationPop.count() << " ms" << endl;
    cout << "  Laczny czas operacji:       " << total << " ms" << endl;
    cout << "----------------------------------------" << endl;
}

// --- GŁÓWNY PROGRAM ---

int main() {
    // 1. Wykonanie szybkich testów poprawności algorytmów
    basicTest<PriorityQueueUnsorted>("Lista Nieposortowana");
    basicTest<PriorityQueueSorted>("Lista Posortowana");
    basicTest<PriorityQueueHeap>("Kopiec Binarny");

    cout << "========================================" << endl;
    cout << "PRZYGOTOWANIE DANYCH WEJSCIOWYCH..." << endl;

    string nazwaPliku = "dane_testowe.txt";
    int iloscElementow = 50000;

    // Próbujemy otworzyć plik, żeby sprawdzić czy istnieje
    ifstream plikSprawdzajacy(nazwaPliku);
    if (!plikSprawdzajacy.good()) {
        // Plik nie istnieje – generujemy go RAZ
        cout << "[WARN] Plik " << nazwaPliku << " nie istnieje. Generowanie danych..." << endl;
        plikSprawdzajacy.close(); // zamykamy uchwyt przed zapisem
        zapiszDaneDoPliku(nazwaPliku, iloscElementow);
    } else {
        cout << "[INFO] Znaleziono istniejacy plik: " << nazwaPliku << ". Dane zostana wczytane." << endl;
        plikSprawdzajacy.close();
    }

    // Wczytujemy stałe dane z pliku
    vector<Element> testData;
    try {
        testData = wczytajDaneZPliku(nazwaPliku);
    } catch (const exception& e) {
        cerr << "Blad: " << e.what() << endl;
        return 1;
    }

    cout << "Rozpoczynam sprawiedliwe testy wydajnosci na stalym zestawie danych...\n" << endl;

    // 3. Testy wydajności z rozbiciem na operacje
    performanceTest<PriorityQueueUnsorted>("Lista Nieposortowana", testData);
    performanceTest<PriorityQueueSorted>("Lista Posortowana", testData);
    performanceTest<PriorityQueueHeap>("Kopiec Binarny", testData);

    cout << "Wszystkie badania wydajnosci zostaly wykonane!" << endl;
    return 0;
}