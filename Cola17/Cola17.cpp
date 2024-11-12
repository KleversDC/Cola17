#include <iostream>
using namespace std;

class ViolationRecord {
public:
    char offense[100];
    ViolationRecord* next;

    ViolationRecord(const char* offenseDescription) : next(nullptr) {
        int i = 0;
        while (offenseDescription[i] != '\0' && i < 99) {
            offense[i] = offenseDescription[i];
            i++;
        }
        offense[i] = '\0';
    }
};

class TreeNode {
public:
    char carNumber[10];
    ViolationRecord* violations;
    TreeNode* left;
    TreeNode* right;

    TreeNode(const char* carNum) : violations(nullptr), left(nullptr), right(nullptr) {
        int i = 0;
        while (carNum[i] != '\0' && i < 9) {
            carNumber[i] = carNum[i];
            i++;
        }
        carNumber[i] = '\0';
    }

    void addViolation(const char* offenseDescription) {
        ViolationRecord* newViolation = new ViolationRecord(offenseDescription);
        newViolation->next = violations;
        violations = newViolation;
    }

    void printViolations() {
        ViolationRecord* current = violations;
        while (current) {
            cout << "  Offense: " << current->offense << endl;
            current = current->next;
        }
    }
};

class TrafficDatabase {
private:
    TreeNode* root;

    TreeNode* addRecord(TreeNode* node, const char* carNum, const char* offenseDescription) {
        if (!node) {
            TreeNode* newNode = new TreeNode(carNum);
            newNode->addViolation(offenseDescription);
            return newNode;
        }
        if (compareCarNumbers(carNum, node->carNumber) < 0) {
            node->left = addRecord(node->left, carNum, offenseDescription);
        }
        else if (compareCarNumbers(carNum, node->carNumber) > 0) {
            node->right = addRecord(node->right, carNum, offenseDescription);
        }
        else {
            node->addViolation(offenseDescription);
        }
        return node;
    }

    void printAll(TreeNode* node) {
        if (!node) return;
        printAll(node->left);
        cout << "Car Number: " << node->carNumber << endl;
        node->printViolations();
        printAll(node->right);
    }

    void printByCarNumber(TreeNode* node, const char* carNum) {
        if (!node) return;
        int cmp = compareCarNumbers(carNum, node->carNumber);
        if (cmp < 0) {
            printByCarNumber(node->left, carNum);
        }
        else if (cmp > 0) {
            printByCarNumber(node->right, carNum);
        }
        else {
            cout << "Car Number: " << node->carNumber << endl;
            node->printViolations();
        }
    }

    void printInRange(TreeNode* node, const char* startNum, const char* endNum) {
        if (!node) return;
        if (compareCarNumbers(startNum, node->carNumber) < 0) {
            printInRange(node->left, startNum, endNum);
        }
        if (compareCarNumbers(startNum, node->carNumber) <= 0 && compareCarNumbers(endNum, node->carNumber) >= 0) {
            cout << "Car Number: " << node->carNumber << endl;
            node->printViolations();
        }
        if (compareCarNumbers(endNum, node->carNumber) > 0) {
            printInRange(node->right, startNum, endNum);
        }
    }

    int compareCarNumbers(const char* num1, const char* num2) {
        int i = 0;
        while (num1[i] != '\0' && num2[i] != '\0') {
            if (num1[i] != num2[i]) return num1[i] - num2[i];
            i++;
        }
        return num1[i] - num2[i];
    }

    void deleteTree(TreeNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);

        ViolationRecord* current = node->violations;
        while (current) {
            ViolationRecord* temp = current;
            current = current->next;
            delete temp;
        }
        delete node;
    }

public:
    TrafficDatabase() : root(nullptr) {}

    void addRecord(const char* carNum, const char* offenseDescription) {
        root = addRecord(root, carNum, offenseDescription);
    }

    void printAll() {
        printAll(root);
    }

    void printByCarNumber(const char* carNum) {
        printByCarNumber(root, carNum);
    }

    void printInRange(const char* startNum, const char* endNum) {
        printInRange(root, startNum, endNum);
    }

    ~TrafficDatabase() {
        deleteTree(root);
    }
};

int main() {
    TrafficDatabase db;

    db.addRecord("ABC123", "Speeding");
    db.addRecord("DEF456", "Parking violation");
    db.addRecord("ABC123", "No seatbelt");
    db.addRecord("GHI789", "Speeding");

    cout << "Full database:" << endl;
    db.printAll();

    cout << "\nRecords for car number ABC123:\n";
    db.printByCarNumber("ABC123");

    cout << "\nRecords for car numbers between ABC123 and GHI789:\n";
    db.printInRange("ABC123", "GHI789");

}
