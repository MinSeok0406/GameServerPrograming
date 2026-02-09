#include <array>
#include <vector>
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

    Employee anEmployee2
    {
        .firstInitial = 'A',
        .lastInitial = 'B',
        .employeeNumber = 60,
        .salary = 60000,
        .title = RANK::H_ENGINEER,
    };

    Employee anEmployee3
    {
        .firstInitial = 'C',
        .lastInitial = 'F',
        .employeeNumber = 80,
        .salary = 100000,
        .title = RANK::MANAGER,
    };

    vector<Employee> arr;
    arr.push_back(anEmployee);
    arr.push_back(anEmployee2);
    arr.push_back(anEmployee3);

    for (const auto& i : arr)
    {
        cout << "First Name: " << i.firstInitial << "\n";
        cout << "Last Name: " << i.lastInitial << "\n";
        cout << "Employ Num: " << i.employeeNumber << "\n";
        cout << "Salary : " << i.salary << "\n";
        switch (i.title)
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

        cout << "\n\n";
    }

    AirlineTicket alt;
    alt.setHasEliteSuperRewardsStatus(true);
    alt.setNumberOrMiles(100);
    alt.setPassengerName("Àå¹Î¼®");
    alt.setPrimierPassenger(50);

    cout << format("Name: {}", alt.getPassengerName()) << "\n";
    cout << format("Number: {}", alt.getNumberOrMiles()) << "\n";
    cout << format("Primier: {}", alt.getPrimierPassenger()) << "\n";

    return 0;
}