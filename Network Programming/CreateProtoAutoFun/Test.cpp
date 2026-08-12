#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_CreateCharacter	0
bool npfCreateCharacter(SerializationBuffer* packet, unsigned int id, unsigned char dir, short x, short y, char hp)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_CreateCharacter;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << id;
	*packet << dir;
	*packet << x;
	*packet << y;
	*packet << hp;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_CreateOtherCharacter	1
bool npfCreateOtherCharacter(SerializationBuffer* packet, unsigned int id, unsigned char dir, short x, short y, char hp)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_CreateOtherCharacter;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << id;
	*packet << dir;
	*packet << x;
	*packet << y;
	*packet << hp;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_DeleteCharacter	2
bool npfDeleteCharacter(SerializationBuffer* packet, unsigned int id)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_DeleteCharacter;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << id;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_MoveStart	11
bool npfMoveStart(SerializationBuffer* packet, unsigned int id, unsigned char dir, short x, short y)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_MoveStart;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << id;
	*packet << dir;
	*packet << x;
	*packet << y;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_MoveStop	13
bool npfMoveStop(SerializationBuffer* packet, unsigned int id, unsigned char dir, short x, short y)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_MoveStop;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << id;
	*packet << dir;
	*packet << x;
	*packet << y;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_Attack1	21
bool npfAttack1(SerializationBuffer* packet, unsigned int id, unsigned char dir, short x, short y)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_Attack1;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << id;
	*packet << dir;
	*packet << x;
	*packet << y;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_Attack2	23
bool npfAttack2(SerializationBuffer* packet, unsigned int id, unsigned char dir, short x, short y)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_Attack2;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << id;
	*packet << dir;
	*packet << x;
	*packet << y;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_Attack3	25
bool npfAttack3(SerializationBuffer* packet, unsigned int id, unsigned char dir, short x, short y)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_Attack3;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << id;
	*packet << dir;
	*packet << x;
	*packet << y;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

#define dfNETWORK_PACKET_CODE 0x89
#define dfPACKET_SC_Damage	30
bool npfDamage(SerializationBuffer* packet, unsigned int attackID, unsigned int damageID, char damageHP)
{
	st_HEADER header;
	header._byCode = dfNETWORK_PACKET_CODE;
	header._byType = dfPACKET_SC_Damage;

	packet->setHeaderSize(sizeof(st_HEADER));
	*packet << attackID;
	*packet << damageID;
	*packet << damageHP;

	header._bySize = (unsigned char)(packet->getDataSize() - sizeof(st_HEADER));
	packet->headerWritePos();
	packet->putData((char*)&header, sizeof(header));
	packet->posReset();
	return true;
}

