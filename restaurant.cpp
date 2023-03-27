#include "main.h"

struct Node {
    table* data;
    Node* next;
    Node* prev;

    Node(table* data) {
        this->data = data;
        next = NULL;
        prev = NULL;
    }
};

class MyVector {
   private:
    Node *head, *tail;
    int length;

   public:
    MyVector() {
        head = NULL;
        tail = NULL;
        length = 0;
    }

    void push_back(table* data) {
        Node* newNode = new Node(data);
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        length++;
    }
    Node* begin() {
        return head;
    }

    Node* end() {
        return tail;
    }

    ~MyVector() {
        Node* temp = head;
        while (temp != NULL) {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
    }
};

struct waitList {
    table* first;
    table* last;
    int node_count;

    waitList() {
        first = NULL;
        last = NULL;
        node_count = 0;
    }

    void delNode(int ID) {
        table* t = first;
        while (1) {
            if (t->ID == ID) {
                t->age = 0;
                t->name = "";
                break;
            }
            if (t == last) break;
            t = t->next;
        }
    }
    void enQueue(int ID, string name, int age) {
        table* t = new table(ID, name, age, NULL);
        if (first == NULL) {
            first = t;
            last = t;
            node_count += 1;
        } else if (node_count < MAXSIZE) {
            last->next = t;
            node_count += 1;
            last = last->next;
        }
    }

    table* dequeue() {
        if (first == NULL) {
            return NULL;
        }
        table* rtn = first;
        first = first->next;
        node_count -= 1;

        return rtn;
    }

    bool empty() {
        return (node_count == 0);
    }

    void selectionSort(int num) {
        if (first == NULL) return;
        // cout << last->ID << " " << last->name << " " << last->age << "\n";
        table* current = first;
        for (int i = 1; i <= num; i++) {
            table* anotherPointer = current;
            int mx = current->age;
            string name = current->name;
            for (int j = 1; j <= node_count - i; j++) {
                // cout << i << " " << j << endl;
                if (anotherPointer == last) continue;
                anotherPointer = anotherPointer->next;
                if (anotherPointer->age > mx) {
                    mx = anotherPointer->age;
                    name = anotherPointer->name;
                }
            }
            if (mx > current->age) {
                anotherPointer = current->next;
                while (1) {
                    if (anotherPointer->name == name && anotherPointer->age == mx) {
                        swap(anotherPointer->age, current->age);
                        swap(anotherPointer->name, current->name);
                        break;
                    }
                    anotherPointer = anotherPointer->next;
                }
            }
            current = current->next;
        }
    }
};
// List contain all customer (include waitting and have a seat)
waitList* queue = new waitList();
string nameLast = "";
void reg(ifstream& read, restaurant* r, waitList* waiting);
table* regm(ifstream& read, restaurant* r);
void cle(ifstream& read, restaurant* r, table* CombineTable);
void ps(ifstream& read, restaurant* r, int num);
void pq(ifstream& read, restaurant* r, int num, waitList* waitting);
void pt(ifstream& read, restaurant* r);
void sq(ifstream& read, restaurant* r, waitList* waiting);

// Hàm này để kiểm tra xem chuỗi có phải là số không để xử lý trường hợp REG có ID và ko có ID
bool isNumeric(const string& str) {
    for (const char& c : str) {
        if (!isdigit(c))
            return false;
    }
    return true;
}
//
void simulate(string filename, restaurant* r) {
    // Tạo input stream giống như cin, nhưng khác ở chỗ nó đọc từ file chứ ko đọc từ bàn phím
    ifstream read(filename);

    // Chuỗi này để đọc operation ở đầu mỗi dòng trong file
    string operation = "";

    // Waitlist
    waitList* waiting = new waitList();

    // Luu ban dau tien bi gop
    table* CombineTable = nullptr;
    // Đọc đến khi hết file
    while (!read.eof()) {
        // cout << operation << "\n";
        if (operation == "") read >> operation;
        // cout << operation << "\n";
        if (operation == "REG") {
            // Truyền cái input stream vào hàm reg để đọc tiếp thông tin của khách hàng
            // Lưu ý mình chỉ duy trì 1 input stream, nên nó sẽ đọc tiếp từ vị trí trước đó
            reg(read, r, waiting);
            operation = "";
        } else if (operation == "REGM") {
            if (CombineTable == nullptr) {
                CombineTable = regm(read, r);
                operation = "";
                // cout << CombineTable->ID << "\n";
            }
        } else if (operation == "CLE") {
            cle(read, r, CombineTable);
            operation = "";

        } else if (operation == "PS") {
            string temp = "";
            read >> temp;
            if (temp != "" && isNumeric(temp)) {
                ps(read, r, stoi(temp));
                operation = "";
            } else {
                ps(read, r, MAXSIZE);
                operation = temp;
            }
        } else if (operation == "PQ") {
            string temp = "";
            read >> temp;
            if (temp != "" && isNumeric(temp)) {
                pq(read, r, stoi(temp), waiting);
                operation = "";
            } else {
                pq(read, r, MAXSIZE, waiting);
                operation = temp;
            }
        } else if (operation == "PT") {
            pt(read, r);
            operation = "";
        } else if (operation == "SQ") {
            sq(read, r, waiting);
            operation = "";
        }
    }
    // cout << waiting->node_count << "\n";
    table* tmp;
    while (!waiting->empty()) {
        tmp = waiting->dequeue();
        delete tmp;
    }

    table* current = r->recentTable;
    for (int i = 1; i <= MAXSIZE; i++) {
        table* nextTable = current->next;
        if (current->ID == 15) {
            if (nextTable->ID != 1) {
                current->next = CombineTable;
                break;
            }
        } else if (nextTable->ID - current->ID != 1) {
            current->next = CombineTable;
            break;
        }
        current = nextTable;
    }

    current = r->recentTable;
    int currentID = current->ID;
    current = current->next;
    while (current->ID != currentID) {
        table* delTable = current;
        current = current->next;
        delete delTable;
    }
}

bool emptyTable(table* t) {
    return (t->name == "" && t->age == 0);
}
void reg(ifstream& read, restaurant* r, waitList* waiting) {
    string temp, name;
    int ID = 0, age;
    // Đọc token ngay sau REG
    read >> temp;
    // Nếu ko phải số thì đây là lệnh REG name age
    if (!isNumeric(temp)) {
        name = temp;
        read >> age;
    }
    // Nếu là số thì đây là lệnh REG ID name age
    else {
        ID = stoi(temp);
        read >> name;
        read >> age;
    }
    // cout << ID << " " << name << endl;
    if (ID != 0) {
        bool flag = 1;
        int count = 0;
        table* current = r->recentTable;
        while (flag) {
            if (current->ID == ID) {
                while (1) {
                    if (count == MAXSIZE + 1) {
                        waiting->enQueue(ID, name, age);
                        flag = 0;
                        break;
                    }
                    count++;
                    if (emptyTable(current)) {
                        queue->enQueue(ID, name, age);
                        nameLast = name;
                        current->age = age;
                        current->name = name;
                        flag = 0;
                        break;
                    }
                    current = current->next;
                }
                break;
            }
            count++;
            if (count == MAXSIZE + 1) {
                waiting->enQueue(ID, name, age);
                flag = 0;
                break;
            }
            current = current->next;
        }
    } else {
        int freeFirst = MAXSIZE + 1;
        int count = 0;
        table* current = r->recentTable;
        while (1) {
            if (count == MAXSIZE) break;
            if (current->age == 0) freeFirst = min(freeFirst, current->ID);
            current = current->next;
            count++;
        }
        if (freeFirst == MAXSIZE + 1) {
            queue->enQueue(ID, name, age);
            waiting->enQueue(ID, name, age);
        } else {
            while (1) {
                if (current->ID == freeFirst) {
                    queue->enQueue(ID, name, age);
                    nameLast = name;
                    current->age = age;
                    current->name = name;
                    break;
                }
                current = current->next;
            }
        }
    }
}
table* regm(ifstream& read, restaurant* r) {
    table* current = r->recentTable;
    int age, num, IDMax = 0, count = 0;
    string temp;
    string name;
    read >> name;
    read >> temp;
    age = stoi(temp);
    read >> temp;
    num = stoi(temp);

    while (1) {
        if (count > MAXSIZE * num) break;
        count++;
        if (emptyTable(current)) {
            bool check = 1;
            table* current1 = current->next;
            for (int j = 1; j < num; j++) {
                count++;
                if (!emptyTable(current1)) {
                    check = 0;
                    current = current1;
                    break;
                }
                current1 = current1->next;
            }
            if (check == 1) IDMax = max(IDMax, current->ID);
        }
        current = current->next;
    }
    while (1) {
        if (current->ID == IDMax) {
            table* theLastTable = current;
            table* rtn = current->next;
            for (int i = 1; i < num; i++) theLastTable = theLastTable->next;
            current->age = age;
            current->name = name;
            current->next = theLastTable->next;
            nameLast = name;
            queue->enQueue(IDMax, name, age);
            // cout << "hungdz " << current->ID << "\n";
            return rtn;
            break;
        }
        current = current->next;
    }
}

void cle(ifstream& read, restaurant* r, table* CombineTable) {
    int id;
    string temp;
    read >> temp;
    id = stoi(temp);

    if (id < 1 || id > MAXSIZE) return;
    table* current = r->recentTable;
    while (1) {
        if (current->ID == id) {
            nameLast = current->name;
            queue->delNode(id);
            // xoa ban current trong queue
            current->age = 0;
            current->name = "";
            table* nextTable = current->next;
            if ((nextTable->ID - current->ID) != 1 && (current->ID != 15 && nextTable->ID != 1)) current->next = CombineTable;
            break;
        }
        current = current->next;
    }
}
// PS la cai mang cu lin
void ps(ifstream& read, restaurant* r, int num) {
    MyVector nums;
    int dem = 0;
    table* current = queue->first;
    while (1) {
        if (current->name != "") nums.push_back(current), dem++;
        if (current == queue->last) break;
        current = current->next;
    }
    int count = 0;
    for (Node* i = nums.end(); i != nums.begin(); i = i->prev) {
        cout << i->data->name << "\n";
        count++;
        if (count == num) break;
    }
    if (count < num) cout << nums.begin()->data->name << "\n";
    return;
}

void pq(ifstream& read, restaurant* r, int num, waitList* waitting) {
    if (waitting->node_count == 0) {
        cout << "Empty\n";
        return;
    }
    table* current = waitting->first;
    while (num--) {
        cout << current->name << "\n";
        if (current == waitting->last) break;
        current = current->next;
    }
    return;
}
void pt(ifstream& read, restaurant* r) {
    table* current = r->recentTable->next;
    while (1) {
        if (current->name == nameLast) {
            break;
        }
        current = current->next;
    }
    string currentName = current->name;
    do {
        cout << current->ID << "-" << current->name << "\n";
        current = current->next;
    } while (currentName != current->name);
}

void sq(ifstream& read, restaurant* r, waitList* waiting) {
    string temp;
    read >> temp;
    int num = stoi(temp);
    // cout << waiting->node_count << "\n";
    waiting->selectionSort(num);
    // cout << waiting->node_count << "\n";
    if (waiting->empty()) {
        cout << "Empty\n";
    } else {
        table* nodeHienTai = waiting->first;
        while (num--) {
            cout << nodeHienTai->name << "\n";
            nodeHienTai = nodeHienTai->next;
        }
    }
}