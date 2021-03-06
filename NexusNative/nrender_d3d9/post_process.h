#ifndef _NEXUS_D3D9_POST_PROCESS_H_
#define _NEXUS_D3D9_POST_PROCESS_H_
#include "d3d_ptr.h"

namespace nexus
{
	// render target item
	struct rt_item
	{
		d3d_texture_ptr	tex;
		d3d_surface_ptr	surf;

		int release()
		{
			int r1 = surf.reset();
			int r2 = tex.reset();			
			return r1>r2?r1:r2;
		}

		vector2 size() const
		{
			D3DSURFACE_DESC Desc;
			tex.get()->GetLevelDesc(0,&Desc);
			return vector2((float)Desc.Width,(float)Desc.Height);
		}
	};

	class post_process
	{
	public:
		typedef shared_ptr<post_process> ptr;

		post_process(void)			{		}
		virtual ~post_process(void)	{		}
		
		virtual void render() const  = 0;

	protected:
	//	post_process::ptr	m_next;	// for chain of post process
	};
}//namespace nexus

#endif //_NEXUS_D3D9_POST_PROCESS_H_