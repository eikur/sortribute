#ifndef __ENTITY_H__
#define __ENTITY_H__

class Entity 
{
public:
	enum Types
	{
		player,
		unknown
	};

private:
	Types m_type;

public:
	Entity(Types type) : m_type(type)
	{}

	virtual ~Entity()
	{}

};

#endif // __ENTITY_H__