#pragma once


#include "template_pool.h"

template<typename T>
class template_guard
{
public:
	template_guard( template_pool<T> *ppool )
	{
		m_ppool = ppool;
		m_pobj = NULL;
	};

	~template_guard(void)
	{
		if (m_ppool == NULL)
		{
			return;
		}
		if (m_pobj == NULL)
		{
			return;
		}
		m_ppool->restore_obj(m_pobj);

	};

	int fetch_obj_from_pool(void)
	{
		if (m_pobj != NULL)
		{
			return 0;
		}
		if (m_ppool == NULL)
		{
			return -1;
		}
		//T **ppobj;
		T *pobj;
		int result = m_ppool->fetch_obj(&pobj);
		if (result == -1)
		{
			m_pobj = NULL;
			return -1;
		}
		m_pobj = pobj;
		return 0;
	}
	T *get(void)
	{
		return m_pobj;
	}


private:
	template_pool<T>  *m_ppool;
	T *m_pobj;
};
