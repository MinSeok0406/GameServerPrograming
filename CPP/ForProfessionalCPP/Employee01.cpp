module Employee01;

double HR::AirlineTicket::calculatePriceInDollors() const
{
    if (hasEliteSuperRewardsStatus() == false)
    {
        return 0;
    }

    return 5.0 * getNumberOrMiles();
}

std::string HR::AirlineTicket::getPassengerName() const
{
    return m_passengerName;
}

void HR::AirlineTicket::setPassengerName(const std::string& name)
{
    m_passengerName = name;
}

int HR::AirlineTicket::getNumberOrMiles() const
{
    return m_numberOfMiles;
}

void HR::AirlineTicket::setNumberOrMiles(int miles)
{
    m_numberOfMiles = miles;
}

bool HR::AirlineTicket::hasEliteSuperRewardsStatus() const
{
    return m_hasEliteSuperRewardsStatus;
}

void HR::AirlineTicket::setHasEliteSuperRewardsStatus(bool status)
{
    m_hasEliteSuperRewardsStatus = status;
}

int HR::AirlineTicket::getPrimierPassenger() const
{
    return m_primierPassenger;
}

void HR::AirlineTicket::setPrimierPassenger(int primier)
{
    m_primierPassenger = primier;
}
