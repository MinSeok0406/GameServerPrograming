export module Employee01;

import <iostream>;
import <format>;
import <string>;

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

    export class AirlineTicket
    {
    public:
        double calculatePriceInDollors() const;
        std::string getPassengerName() const;
        void setPassengerName(const std::string& name);

        int getNumberOrMiles() const;
        void setNumberOrMiles(int miles);

        bool hasEliteSuperRewardsStatus() const;
        void setHasEliteSuperRewardsStatus(bool status);

        int getPrimierPassenger() const;
        void setPrimierPassenger(int primier);

    private:
        std::string m_passengerName { "Unknown Passenger" };
        int m_numberOfMiles { 0 };
        bool m_hasEliteSuperRewardsStatus { false };
        int m_primierPassenger { 0 };
    };
}