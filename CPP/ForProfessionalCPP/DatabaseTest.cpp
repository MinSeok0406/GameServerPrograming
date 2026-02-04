//import <iostream>;
//import Database;
//import Employee;
//
//using namespace std;
//using namespace Records;
//
//
//int main()
//{
//    Database myDB;
//    Employee& emp1 { myDB.addEmployee("Greg", "Wallis") };
//    emp1.fire();
//
//    Employee& emp2 { myDB.addEmployee("Marc", "White") };
//    emp2.setSalary(100'000);
//
//    Employee& emp3 { myDB.addEmployee("John", "Doe") };
//    emp3.setSalary(10'000);
//    emp3.promote();
//
//    cout << "all employees: " << "\n\n";
//    myDB.displayAll();
//
//    cout << "\n" << "current employees: " << "\n\n";
//    myDB.displayCurrent();
//
//    cout << "\n" << "former employees: " << "\n\n";
//    myDB.displayFormer();
//
//    return 0;
//}