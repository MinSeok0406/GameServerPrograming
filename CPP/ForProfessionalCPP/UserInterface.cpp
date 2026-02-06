//#include <exception>
//import <iostream>;
//import <stdexcept>;
//import <format>;
//import <string>;
//import Employee;
//import Database;
//
//using namespace std;
//using namespace Records;
//
//int displayMenu();
//void doHire(Database& db);
//void doFire(Database& db);
//void doPromote(Database& db);
//
//int main()
//{
//    Database employeeDB;
//    bool done { false };
//    while (!done)
//    {
//        int selection { displayMenu() };
//        switch (selection)
//        {
//        case 0:
//            done = true;
//            break;
//        case 1:
//            doHire(employeeDB);
//            break;
//        case 2:
//            doFire(employeeDB);
//            break;
//        case 3:
//            doPromote(employeeDB);
//            break;
//        case 4:
//            employeeDB.displayAll();
//            break;
//        case 5:
//            employeeDB.displayCurrent();
//            break;
//        case 6:
//            employeeDB.displayFormer();
//            break;
//        default:
//            cerr << "Unknown command." << "\n";
//            break;
//        }
//    }
//}
//
//int displayMenu()
//{
//    int selection;
//    cout << "\n";
//    cout << "Employee Database" << "\n";
//    cout << "-------------------" << "\n";
//    cout << "1) Hire a new employee" << "\n";
//    cout << "2) Fire an employee" << "\n";
//    cout << "3) Promote an employee" << "\n";
//    cout << "4) List all employees" << "\n";
//    cout << "5) List all current employees" << "\n";
//    cout << "6) List all former employees" << "\n";
//    cout << "0) Quit" << "\n";
//    cout << "\n";
//    cout << "---> ";
//    cin >> selection;
//    return selection;
//}
//
//void doHire(Database& db)
//{
//    string firstName;
//    string lastName;
//
//    cout << "First name? ";
//    cin >> firstName;
//
//    cout << "Last name? ";
//    cin >> lastName;
//
//    auto& employee { db.addEmployee(firstName, lastName) };
//    cout << format("Hired employee {} {} with employee number {}.", firstName, lastName, employee.getEmployeeNumber()) << "\n";
//}
//
//void doFire(Database& db)
//{
//    int employeeNumber;
//    cout << "Employee number? ";
//    cin >> employeeNumber;
//
//    try
//    {
//        auto& emp { db.getEmployee(employeeNumber) };
//        emp.fire();
//        cout << format("Employee {} terminated.", employeeNumber) << "\n";
//    }
//    catch (const logic_error& exception)
//    {
//        cerr << format("Unable to terminated employee: ", exception.what()) << "\n";
//    }
//}
//
//void doPromote(Database& db)
//{
//    int employeeNumber;
//    cout << "Employee number? ";
//    cin >> employeeNumber;
//
//    int raiseAmount;
//    cout << "How much of a raise? ";
//    cin >> raiseAmount;
//
//    try
//    {
//        auto& emp { db.getEmployee(employeeNumber) };
//        emp.promote(raiseAmount);
//    }
//    catch (const logic_error& exception)
//    {
//        cerr << format("Unable to promote employee: {}", exception.what()) << "\n";
//    }
//}
