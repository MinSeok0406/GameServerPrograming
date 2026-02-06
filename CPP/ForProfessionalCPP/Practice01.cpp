import <iostream>;
import <format>;
import Employee01;

using namespace std;
using namespace HR;

int main()
{
    Employee anEmployee
    {
        .firstInitial = 'J',
        .lastInitial = 'D',
        .employeeNumber = 42,
        .salary = 80000,
        .title = RANK::C_ENGINEER,
    };

    switch (anEmployee.title)
    {
    case RANK::MANAGER:
        cout << "MANAGER" << "\n";
        break;
    case RANK::H_ENGINEER:
        cout << "H_ENGINEER" << "\n";
        break;
    case RANK::C_ENGINEER:
        cout << "C_ENGINEER" << "\n";
        break;
    }

    return 0;
}