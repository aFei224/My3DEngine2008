#include "StdAfx.h"
#include <sstream>
#include "directional_light_policy.h"
#include "util.h"
#include "shading_effect.h"

namespace nexus
{
	directional_light_policy::directional_light_policy(void)
	{
	}

	directional_light_policy::~directional_light_policy(void)
	{
	}

	void directional_light_policy::modify_drawing_policy_type(light_drawing_policy_type* dp_type)
	{
		wostringstream ss;
		ss	<< _T("shader_d3d9/lighting/")
			<< get_name()
			<< _T(".hlsl");

		dp_type->m_light_policy = ss.str();
	}
}//namespace nexus