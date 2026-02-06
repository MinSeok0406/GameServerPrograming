export module Employee01;

namespace HR
{
    export enum class RANK
    {
        MANAGER,
        H_ENGINEER,
        C_ENGINEER,
    };

    export struct Employee
    {
        char firstInitial;
        char lastInitial;
        int employeeNumber;
        int salary;
        RANK title;
    };
}