
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <ctime>

using namespace std;

// PRODUCT INFORMATION
const int MAX_PRODUCTS = 100;
const int LOW_STOCK_LIMIT = 5;

string productName[MAX_PRODUCTS];
double productPrice[MAX_PRODUCTS];
int productStock[MAX_PRODUCTS];

int productCount = 0;
int transactionNumber = 0;

// FUNCTION DECLARATIONS
int readInt(string message);
double readDouble(string message);
string getCurrentDate();
string toLowerCase(string text);

void loadProducts();
void saveProducts();

void inventoryManagement();
void addProduct();
void updateProduct();
void restockProduct();
void deleteProduct();
void viewLowStock();

void billingManagement();
void viewProducts();
void searchProduct();

void transactionHistory();
void salesReport();

// MAIN PROGRAM
int main() {

    int choice;
    char confirm;

    loadProducts();

    do {

        cout << "\n========================================" << endl;
        cout << "   INTEGRATED INVENTORY AND BILLING" << endl;
        cout << "          MANAGEMENT SYSTEM" << endl;
        cout << "========================================" << endl;

        cout << "1. Inventory Management" << endl;
        cout << "2. Billing Management" << endl;
        cout << "3. View Products" << endl;
        cout << "4. Search Product" << endl;
        cout << "5. Sales Report" << endl;
        cout << "6. Transaction History" << endl;
        cout << "7. Exit" << endl;

        choice = readInt("\nEnter your choice: ");

        switch (choice) {

            case 1:
                inventoryManagement();
                break;

            case 2:
                billingManagement();
                break;

            case 3:
                viewProducts();
                break;

            case 4:
                searchProduct();
                break;

            case 5:
                salesReport();
                break;

            case 6:
                transactionHistory();
                break;

            case 7:

                cout << "\nAre you sure you want to exit? (Y/N): ";
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (confirm == 'Y' || confirm == 'y') {
                    cout << "\nThank you for using the system!" << endl;
                }
                else {
                    choice = 0;
                    cout << "\nReturning to main menu..." << endl;
                }

                break;

            default:
                cout << "\nInvalid choice. Please try again." << endl;
        }

    } while (choice != 7);

    return 0;
}

// INPUT VALIDATION
int readInt(string message) {

    int value;

    while (true) {

        cout << message;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Enter a whole number." << endl;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double readDouble(string message) {

    double value;

    while (true) {

        cout << message;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid input. Enter a valid number." << endl;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// GET CURRENT DATE
string getCurrentDate() {

    time_t now = time(0);
    tm *localTime = localtime(&now);

    char date[20];

    strftime(date, sizeof(date), "%Y-%m-%d", localTime);

    return string(date);
}

// CONVERT TEXT TO LOWERCASE
string toLowerCase(string text) {

    transform(text.begin(), text.end(), text.begin(),
        [](unsigned char c) {
            return static_cast<char>(tolower(c));
        });

    return text;
}

// FILE HANDLING: LOAD PRODUCTS
void loadProducts() {

    ifstream file("products.txt");

    if (!file) {
        return;
    }

    string line;

    while (getline(file, line) && productCount < MAX_PRODUCTS) {

        string name;
        string priceText;
        string stockText;

        stringstream ss(line);

        getline(ss, name, '|');
        getline(ss, priceText, '|');
        getline(ss, stockText);

        stringstream priceInput(priceText);
        stringstream stockInput(stockText);

        double price;
        int stock;

        if (priceInput >> price && stockInput >> stock &&
            !name.empty() && price >= 0 && stock >= 0) {

            productName[productCount] = name;
            productPrice[productCount] = price;
            productStock[productCount] = stock;

            productCount++;
        }
    }

    file.close();

    // LOAD TRANSACTION NUMBER
    ifstream transactionFile("transactions.txt");

    string record;

    while (getline(transactionFile, record)) {

        if (record.find("TRANSACTION #") == 0) {
            transactionNumber++;
        }
    }

    transactionFile.close();

    cout << "\nSaved product records loaded successfully!" << endl;
}

// FILE HANDLING: SAVE PRODUCTS
void saveProducts() {

    ofstream file("products.txt");

    if (!file) {
        cout << "Unable to save product records." << endl;
        return;
    }

    for (int i = 0; i < productCount; i++) {

        file << productName[i] << "|"
             << fixed << setprecision(2) << productPrice[i] << "|"
             << productStock[i] << endl;
    }

    file.close();
}

// INVENTORY MANAGEMENT
void inventoryManagement() {

    int choice;

    do {

        cout << "\n================================" << endl;
        cout << "      INVENTORY MANAGEMENT" << endl;
        cout << "================================" << endl;

        cout << "1. Add Product" << endl;
        cout << "2. Update Product" << endl;
        cout << "3. Restock Product" << endl;
        cout << "4. Delete Product" << endl;
        cout << "5. View Low Stock Products" << endl;
        cout << "6. Return to Main Menu" << endl;

        choice = readInt("\nEnter your choice: ");

        switch (choice) {

            case 1:
                addProduct();
                break;

            case 2:
                updateProduct();
                break;

            case 3:
                restockProduct();
                break;

            case 4:
                deleteProduct();
                break;

            case 5:
                viewLowStock();
                break;

            case 6:
                cout << "\nReturning to main menu..." << endl;
                break;

            default:
                cout << "\nInvalid choice." << endl;
        }

    } while (choice != 6);
}

// ADD PRODUCT
void addProduct() {

    if (productCount >= MAX_PRODUCTS) {
        cout << "\nProduct storage is full!" << endl;
        return;
    }

    cout << "\n========== ADD PRODUCT ==========" << endl;

    cout << "Enter Product Name: ";
    getline(cin, productName[productCount]);

    if (productName[productCount].empty() ||
        productName[productCount].find('|') != string::npos) {

        cout << "Invalid product name." << endl;
        return;
    }

    productPrice[productCount] = readDouble("Enter Product Price: ");

    if (productPrice[productCount] < 0) {
        cout << "Invalid product price." << endl;
        return;
    }

    productStock[productCount] = readInt("Enter Initial Stock: ");

    if (productStock[productCount] < 0) {
        cout << "Invalid stock quantity." << endl;
        return;
    }

    productCount++;

    saveProducts();

    cout << "\nProduct successfully added!" << endl;
}

// UPDATE PRODUCT
void updateProduct() {

    if (productCount == 0) {
        cout << "\nNo products available." << endl;
        return;
    }

    viewProducts();

    int number = readInt("\nEnter Product Number to Update: ");

    if (number < 1 || number > productCount) {
        cout << "Invalid product number." << endl;
        return;
    }

    int index = number - 1;

    cout << "\nCurrent Product: " << productName[index] << endl;

    string newName;

    cout << "Enter New Product Name: ";
    getline(cin, newName);

    if (newName.empty() || newName.find('|') != string::npos) {
        cout << "Invalid product name." << endl;
        return;
    }

    double newPrice = readDouble("Enter New Price: ");

    if (newPrice < 0) {
        cout << "Invalid price." << endl;
        return;
    }

    productName[index] = newName;
    productPrice[index] = newPrice;

    saveProducts();

    cout << "\nProduct successfully updated!" << endl;
}

// RESTOCK PRODUCT
void restockProduct() {

    if (productCount == 0) {
        cout << "\nNo products available." << endl;
        return;
    }

    viewProducts();

    int number = readInt("\nEnter Product Number to Restock: ");

    if (number < 1 || number > productCount) {
        cout << "Invalid product number." << endl;
        return;
    }

    int index = number - 1;

    int quantity = readInt("Enter Additional Stock: ");

    if (quantity <= 0 ||
        quantity > numeric_limits<int>::max() - productStock[index]) {

        cout << "Invalid restock quantity." << endl;
        return;
    }

    productStock[index] += quantity;

    saveProducts();

    cout << "\nStock successfully added!" << endl;
    cout << "New Available Stock: " << productStock[index] << endl;
}

// DELETE PRODUCT
void deleteProduct() {

    if (productCount == 0) {
        cout << "\nNo products available." << endl;
        return;
    }

    viewProducts();

    int number = readInt("\nEnter Product Number to Delete: ");

    if (number < 1 || number > productCount) {
        cout << "Invalid product number." << endl;
        return;
    }

    int index = number - 1;

    char confirm;

    cout << "Delete " << productName[index] << "? (Y/N): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (confirm == 'Y' || confirm == 'y') {

        for (int i = index; i < productCount - 1; i++) {

            productName[i] = productName[i + 1];
            productPrice[i] = productPrice[i + 1];
            productStock[i] = productStock[i + 1];
        }

        productCount--;

        saveProducts();

        cout << "\nProduct successfully deleted!" << endl;
    }
    else {
        cout << "\nDeletion cancelled." << endl;
    }
}

// LOW STOCK ALERT
void viewLowStock() {

    bool found = false;

    cout << "\n========== LOW STOCK ALERT ==========" << endl;
    cout << "Products with 5 or fewer items:" << endl;

    for (int i = 0; i < productCount; i++) {

        if (productStock[i] <= LOW_STOCK_LIMIT) {

            cout << "\nProduct: " << productName[i] << endl;
            cout << "Remaining Stock: " << productStock[i] << endl;

            found = true;
        }
    }

    if (!found) {
        cout << "\nNo low stock products." << endl;
    }

    cout << "=====================================" << endl;
}

// VIEW PRODUCTS
void viewProducts() {

    cout << "\n================ PRODUCT LIST ================" << endl;

    if (productCount == 0) {
        cout << "No products registered." << endl;
        return;
    }

    cout << left << setw(8) << "Number"
         << setw(22) << "Product Name"
         << setw(13) << "Price"
         << "Stock" << endl;

    cout << "----------------------------------------------" << endl;

    for (int i = 0; i < productCount; i++) {

        cout << left << setw(8) << i + 1
             << setw(22) << productName[i]
             << "P" << fixed << setprecision(2)
             << setw(12) << productPrice[i]
             << productStock[i] << endl;
    }

    cout << "==============================================" << endl;
}

// SEARCH PRODUCT
void searchProduct() {

    if (productCount == 0) {
        cout << "\nNo products available." << endl;
        return;
    }

    string searchName;

    cout << "\nEnter Product Name to Search: ";
    getline(cin, searchName);

    bool found = false;

    for (int i = 0; i < productCount; i++) {

        if (toLowerCase(productName[i]).find(
            toLowerCase(searchName)) != string::npos) {

            cout << "\nProduct Number: " << i + 1 << endl;
            cout << "Product Name: " << productName[i] << endl;
            cout << "Price: P" << fixed << setprecision(2)
                 << productPrice[i] << endl;
            cout << "Available Stock: " << productStock[i] << endl;

            found = true;
        }
    }

    if (!found) {
        cout << "\nProduct not found." << endl;
    }
}

// BILLING MANAGEMENT
void billingManagement() {

    if (productCount == 0) {
        cout << "\nNo products available for billing." << endl;
        return;
    }

    int cartIndex[MAX_PRODUCTS];
    int cartQuantity[MAX_PRODUCTS];

    int cartCount = 0;
    int productNumber;
    int quantity;

    double total = 0;
    double payment;
    double change;

    cout << "\n========== NEW TRANSACTION ==========" << endl;

    viewProducts();

    cout << "\nSelect products to purchase." << endl;
    cout << "Enter 0 when finished selecting products." << endl;

    do {

        productNumber = readInt("\nEnter Product Number (0 to finish): ");

        if (productNumber == 0) {
            break;
        }

        if (productNumber < 1 || productNumber > productCount) {
            cout << "Invalid product number." << endl;
            continue;
        }

        int index = productNumber - 1;

        quantity = readInt("Enter Quantity: ");

        if (quantity <= 0) {
            cout << "Invalid quantity." << endl;
            continue;
        }

        int existingPosition = -1;

        for (int i = 0; i < cartCount; i++) {

            if (cartIndex[i] == index) {
                existingPosition = i;
                break;
            }
        }

        int alreadySelected = 0;

        if (existingPosition != -1) {
            alreadySelected = cartQuantity[existingPosition];
        }

        if (quantity > productStock[index] - alreadySelected) {
            cout << "Insufficient stock!" << endl;
            continue;
        }

        if (existingPosition != -1) {

            cartQuantity[existingPosition] += quantity;
        }
        else {

            cartIndex[cartCount] = index;
            cartQuantity[cartCount] = quantity;

            cartCount++;
        }

        cout << "Product added to transaction!" << endl;

    } while (true);

    if (cartCount == 0) {
        cout << "\nNo products selected. Transaction cancelled." << endl;
        return;
    }

    // CALCULATE TOTAL
    for (int i = 0; i < cartCount; i++) {

        int index = cartIndex[i];

        total += productPrice[index] * cartQuantity[i];
    }

    // DISPLAY BILL
    cout << "\n=============== BILL RECEIPT ===============" << endl;

    cout << left << setw(20) << "Product"
         << setw(8) << "Qty"
         << setw(12) << "Price"
         << "Subtotal" << endl;

    cout << "--------------------------------------------" << endl;

    for (int i = 0; i < cartCount; i++) {

        int index = cartIndex[i];

        double subtotal = productPrice[index] * cartQuantity[i];

        cout << left << setw(20) << productName[index]
             << setw(8) << cartQuantity[i]
             << "P" << setw(11) << fixed << setprecision(2)
             << productPrice[index]
             << "P" << subtotal << endl;
    }

    cout << "--------------------------------------------" << endl;

    cout << "TOTAL AMOUNT: P" << fixed << setprecision(2)
         << total << endl;

    // PAYMENT
    do {

        payment = readDouble("\nEnter Customer Payment: P");

        if (payment < total) {
            cout << "Insufficient payment. Please enter enough money." << endl;
        }

    } while (payment < total);

    change = payment - total;

    // UPDATE STOCK
    for (int i = 0; i < cartCount; i++) {

        int index = cartIndex[i];

        productStock[index] -= cartQuantity[i];
    }

    saveProducts();

    // SAVE TRANSACTION
    transactionNumber++;

    string date = getCurrentDate();

    ofstream transactionFile("transactions.txt", ios::app);

    if (transactionFile) {

        transactionFile << fixed << setprecision(2);

        transactionFile << "\nTRANSACTION #" << transactionNumber
                        << " | DATE: " << date << endl;

        for (int i = 0; i < cartCount; i++) {

            int index = cartIndex[i];

            double subtotal = productPrice[index] * cartQuantity[i];

            transactionFile << productName[index]
                            << " | Quantity: " << cartQuantity[i]
                            << " | Price: P" << productPrice[index]
                            << " | Subtotal: P" << subtotal << endl;
        }

        transactionFile << "Total: P" << total << endl;
        transactionFile << "Payment: P" << payment << endl;
        transactionFile << "Change: P" << change << endl;
        transactionFile << "============================================" << endl;

        transactionFile.close();
    }
    else {
        cout << "\nWarning: Unable to save transaction history." << endl;
    }

    // SAVE SALES RECORD
    ofstream salesFile("sales.txt", ios::app);

    if (salesFile) {

        salesFile << date << "|"
                  << fixed << setprecision(2)
                  << total << endl;

        salesFile.close();
    }
    else {
        cout << "\nWarning: Unable to save sales record." << endl;
    }

    // FINAL RECEIPT
    cout << "\n============================================" << endl;
    cout << "              OFFICIAL RECEIPT" << endl;
    cout << "============================================" << endl;

    cout << "Transaction Number: " << transactionNumber << endl;
    cout << "Date: " << date << endl;

    for (int i = 0; i < cartCount; i++) {

        int index = cartIndex[i];

        cout << productName[index]
             << " x " << cartQuantity[i]
             << " = P" << fixed << setprecision(2)
             << productPrice[index] * cartQuantity[i] << endl;
    }

    cout << "--------------------------------------------" << endl;
    cout << "Total Amount: P" << total << endl;
    cout << "Customer Payment: P" << payment << endl;
    cout << "Change: P" << change << endl;

    cout << "============================================" << endl;
    cout << "       TRANSACTION COMPLETED!" << endl;
    cout << "============================================" << endl;

    cout << "\nRemaining Stock:" << endl;

    for (int i = 0; i < cartCount; i++) {

        int index = cartIndex[i];

        cout << productName[index] << ": "
             << productStock[index] << endl;
    }
}

// TRANSACTION HISTORY
void transactionHistory() {

    ifstream file("transactions.txt");

    cout << "\n========== TRANSACTION HISTORY ==========" << endl;

    if (!file) {
        cout << "No transaction records available." << endl;
        return;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {

        cout << line << endl;
        found = true;
    }

    if (!found) {
        cout << "No transaction records available." << endl;
    }

    file.close();

    cout << "=========================================" << endl;
}

// SALES REPORT
void salesReport() {

    ifstream file("sales.txt");

    string line;
    string dateText;
    string amountText;

    double todaySales = 0;
    double totalSales = 0;

    string today = getCurrentDate();

    cout << "\n========== SALES REPORT ==========" << endl;
    cout << "Report Date: " << today << endl;

    if (!file) {
        cout << "\nNo sales records available." << endl;
        return;
    }

    while (getline(file, line)) {

        stringstream ss(line);

        getline(ss, dateText, '|');
        getline(ss, amountText);

        double amount;

        stringstream amountInput(amountText);

        if (amountInput >> amount) {

            totalSales += amount;

            if (dateText == today) {
                todaySales += amount;
            }
        }
    }

    file.close();

    cout << fixed << setprecision(2);

    cout << "\nToday's Sales: P" << todaySales << endl;
    cout << "Total Recorded Sales: P" << totalSales << endl;

    cout << "==================================" << endl;
}