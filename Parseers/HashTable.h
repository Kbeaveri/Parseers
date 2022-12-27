#include <string>
#include <iostream>

using namespace std;

int HashFunctionHorner(const std::string& s, int table_size, const int key);

struct HashFunction1
{
    int operator()(const std::string& s, int table_size) const
    {
        return HashFunctionHorner(s, table_size, table_size - 1); // êëþ÷è äîëæíû áûòü âçàèìîïðîñòû, èñïîëüçóåì ÷èñëà <ðàçìåð òàáëèöû> ïëþñ è ìèíóñ îäèí.
    }
};
struct HashFunction2
{
    int operator()(const std::string& s, int table_size) const
    {
        return HashFunctionHorner(s, table_size, table_size + 1);
    }
};

template <class T, class THash1 = HashFunction1, class THash2 = HashFunction2>
class HashTable
{
    static const int default_size = 8; // íà÷àëüíûé ðàçìåð íàøåé òàáëèöû
    constexpr static const double rehash_size = 0.75; // êîýôôèöèåíò, ïðè êîòîðîì ïðîèçîéäåò óâåëè÷åíèå òàáëèöû
    struct Node
    {
        T value;
        T value2;
        bool state; // åñëè çíà÷åíèå ôëàãà state = false, çíà÷èò ýëåìåíò ìàññèâà áûë óäàëåí (deleted)
        Node(const T& value_, const T& value2_) : value(value_), value2(value2_), state(true) {}
    };
    Node** arr; // ñîîòâåòñòâåííî â ìàññèâå áóäóò õðàíèòñÿ ñòðóêòóðû Node*
    int size; // ñêîëüêî ýëåìåíòîâ ó íàñ ñåé÷àñ â ìàññèâå (áåç ó÷åòà deleted)
    int buffer_size; // ðàçìåð ñàìîãî ìàññèâà, ñêîëüêî ïàìÿòè âûäåëåíî ïîä õðàíåíèå íàøåé òàáëèöû
    int size_all_non_nullptr; // ñêîëüêî ýëåìåíòîâ ó íàñ ñåé÷àñ â ìàññèâå (ñ ó÷åòîì deleted)
public:
    HashTable()
    {
        buffer_size = default_size;
        size = 0;
        size_all_non_nullptr = 0;
        arr = new Node * [buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr[i] = nullptr; // çàïîëíÿåì nullptr - òî åñòü åñëè çíà÷åíèå îòñóòñòâóåò, è íèêòî ðàíüøå ïî ýòîìó àäðåñó íå îáðàùàëñÿ
    }
    ~HashTable()
    {
        for (int i = 0; i < buffer_size; ++i)
            if (arr[i])
                delete arr[i];
        delete[] arr;
    }
    void Resize()
    {
        int past_buffer_size = buffer_size;
        buffer_size *= 2;
        size_all_non_nullptr = 0;
        size = 0;
        Node** arr2 = new Node * [buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr2[i] = nullptr;
        std::swap(arr, arr2);
        for (int i = 0; i < past_buffer_size; ++i)
        {
            if (arr2[i] && arr2[i]->state)
                Add(arr2[i]->value, arr2[i]->value2); // äîáàâëÿåì ýëåìåíòû â íîâûé ìàññèâ
        }
        // óäàëåíèå ïðåäûäóùåãî ìàññèâà
        for (int i = 0; i < past_buffer_size; ++i)
            if (arr2[i])
                delete arr2[i];
        delete[] arr2;
    }

    void Rehash()
    {
        size_all_non_nullptr = 0;
        size = 0;
        Node** arr2 = new Node * [buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr2[i] = nullptr;
        std::swap(arr, arr2);
        for (int i = 0; i < buffer_size; ++i)
        {
            if (arr2[i] && arr2[i]->state)
                Add(arr2[i]->value, arr2[i]->value2);
        }
        // óäàëåíèå ïðåäûäóùåãî ìàññèâà
        for (int i = 0; i < buffer_size; ++i)
            if (arr2[i])
                delete arr2[i];
        delete[] arr2;
    }
    bool Find(const T& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2())
    {
        int h1 = hash1(value, buffer_size); // çíà÷åíèå, îòâå÷àþùåå çà íà÷àëüíóþ ïîçèöèþ
        int h2 = hash2(value, buffer_size); // çíà÷åíèå, îòâåòñòâåííîå çà "øàã" ïî òàáëèöå
        int i = 0;
        while (arr[h1] != nullptr && i < buffer_size)
        {
            if (arr[h1]->value == value && arr[h1]->state)
                cout << arr[h1]->value << " " << h1;
            return true; // òàêîé ýëåìåíò åñòü
            h1 = (h1 + h2) % buffer_size;
            ++i; // åñëè ó íàñ i >=  buffer_size, çíà÷èò ìû óæå îáîøëè àáñîëþòíî âñå ÿ÷åéêè, èìåííî äëÿ ýòîãî ìû ñ÷èòàåì i, èíà÷å ìû ìîãëè áû çàöèêëèòüñÿ.
        }
        return false;
    }
    bool Remove(const T& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2())
    {
        int h1 = hash1(value, buffer_size);
        int h2 = hash2(value, buffer_size);
        int i = 0;
        while (arr[h1] != nullptr && i < buffer_size)
        {
            if (arr[h1]->value == value && arr[h1]->state)
            {
                arr[h1]->state = false;
                --size;
                return true;
            }
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }
        return false;
    }
    bool Add(const T& value, const T& value2, const THash1& hash1 = THash1(), const THash2& hash2 = THash2())
    {
        if (size + 1 > int(rehash_size * buffer_size))
            Resize();
        else if (size_all_non_nullptr > 2 * size)
            Rehash(); // ïðîèñõîäèò ðåõåø, òàê êàê ñëèøêîì ìíîãî deleted-ýëåìåíòîâ
        int h1 = hash1(value, buffer_size);
        int h2 = hash2(value, buffer_size);
        int i = 0;
        int first_deleted = -1; // çàïîìèíàåì ïåðâûé ïîäõîäÿùèé (óäàëåííûé) ýëåìåíò
        while (arr[h1] != nullptr && i < buffer_size)
        {
            if (arr[h1]->value == value && arr[h1]->state)
                return false; // òàêîé ýëåìåíò óæå åñòü, à çíà÷èò åãî íåëüçÿ âñòàâëÿòü ïîâòîðíî
            if (!arr[h1]->state && first_deleted == -1) // íàõîäèì ìåñòî äëÿ íîâîãî ýëåìåíòà
                first_deleted = h1;
            h1 = (h1 + h2) % buffer_size;
            ++i;
        }
        if (first_deleted == -1) // åñëè íå íàøëîñü ïîäõîäÿùåãî ìåñòà, ñîçäàåì íîâûé Node
        {
            arr[h1] = new Node(value, value2);
            ++size_all_non_nullptr; // òàê êàê ìû çàïîëíèëè îäèí ïðîáåë, íå çàáûâàåì çàïèñàòü, ÷òî ýòî ìåñòî òåïåðü çàíÿòî
        }
        else
        {
            arr[first_deleted]->value = value;
            arr[first_deleted]->value2 = value2;
            arr[first_deleted]->state = true;
        }
        ++size; // è â ëþáîì ñëó÷àå ìû óâåëè÷èëè êîëè÷åñòâî ýëåìåíòîâ
        return true;
    }
    void Print_all() {
        int i = 0;
        while (i < buffer_size) {
            if (arr[i] != nullptr) {
                cout << arr[i]->value << "\t" << arr[i]->value2 << "\t" << i << endl;
            }
            i++;
        }
    }
    void fout() {
        ofstream fout("OUTPUT.txt");
        int i = 0;
        while (i < buffer_size) {
            if (arr[i] != nullptr) {
                fout << "[" << i << "]" << "\t" << arr[i]->value2 << "\t" << arr[i]->value << endl;
            }
            i++;
        }
    }
};