#include "StdAfx.h"
#include "speed_tree_material.h"
#include "../resource/nresource_manager.h"
#include "IdvFilename.h"


namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nspt_material, nmaterial_base)
	nDEFINE_NAMED_CLASS(nspt_branch_material, nspt_material)
	nDEFINE_NAMED_CLASS(nspt_frond_material, nspt_material)
	nDEFINE_NAMED_CLASS(nspt_leaf_material, nspt_material)

	void nspt_branch_material::create(boost::shared_ptr<CSpeedTreeRT> spt, const resource_location& spt_loc)
	{
		// query & set materials
		set_const(spt->GetBranchMaterial());
		
		// scale ambient values
		CSpeedTreeRT::SLightShaderParams	m_sLightScalars;            // values controlled in SpeedTreeCAD for adjusting overall brightness
		spt->GetLightShaderParams(m_sLightScalars);
		scale_ambient(m_sLightScalars.m_fAmbientScalar);

		//---- textures (failure doesn't affect bSuccess)
		// query the textures from RT
		CSpeedTreeRT::SMapBank sMapBank;
		spt->GetMapBank(sMapBank);

		std::string strTexturePath = IdvPath(conv_string(spt_loc.file_name));
		// try to load any references textures
		for (int nLayer = CSpeedTreeRT::TL_DIFFUSE; nLayer < CSpeedTreeRT::TL_SHADOW; ++nLayer)
		{
			// branches
			std::string strTextureFilename = IdvNoPath(sMapBank.m_pBranchMaps[nLayer]);
			if (!strTextureFilename.empty( ))
			{
				std::string strFullPath = strTexturePath + strTextureFilename;

				resource_location tex_loc(spt_loc.pkg_name,
					conv_string(strFullPath));

				try
				{
					if (nLayer == CSpeedTreeRT::TL_DIFFUSE || nLayer == CSpeedTreeRT::TL_NORMAL
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
						|| nLayer == CSpeedTreeRT::TL_DETAIL
#endif
						)
					{
						m_texBranchMaps[nLayer] =
							nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block);
					}
				}
				catch(nexception& ne)
				{
					//m_texBranchMaps[nLayer] = load_fallback_texture(nLayer);
				}
			}
		}// end of for

		//-- material template script
		m_template = nresource_manager::instance()->load_material_template_script(
			resource_location(_T("engine_data:material/SPT_Branch.hlsl"))
			);
	}

	void nspt_branch_material::draw_effect_param(nshading_effect* effect_ptr) const
	{
		effect_ptr->set_texture("SPT_BranchDiffuseMap", 
			m_texBranchMaps[CSpeedTreeRT::TL_DIFFUSE]->get_render_texture() );

		effect_ptr->set_texture("SPT_BranchNormalMap", 
			m_texBranchMaps[CSpeedTreeRT::TL_NORMAL]->get_render_texture() );		
#ifdef SPEEDTREE_BRANCH_DETAIL_LAYER
		if( m_texBranchMaps[CSpeedTreeRT::TL_DETAIL] )
		{
			effect_ptr->set_texture("SPT_BranchDetailMap", 
				m_texBranchMaps[CSpeedTreeRT::TL_DETAIL]->get_render_texture() );
		}
#endif
	}

	nresource_texture::ptr nspt_branch_material::load_fallback_texture(int nLayer)
	{
		// todo
		return nresource_texture::ptr(NULL);
	}

	void nspt_frond_material::create(boost::shared_ptr<CSpeedTreeRT> spt, const resource_location& spt_loc)
	{
		// query & set materials
		set_const(spt->GetFrondMaterial());

		// scale ambient values
		CSpeedTreeRT::SLightShaderParams	m_sLightScalars;            // values controlled in SpeedTreeCAD for adjusting overall brightness
		spt->GetLightShaderParams(m_sLightScalars);
		scale_ambient(m_sLightScalars.m_fAmbientScalar);

		//---- textures (failure doesn't affect bSuccess)
		// query the textures from RT
		CSpeedTreeRT::SMapBank sMapBank;
		spt->GetMapBank(sMapBank);

		std::string strTexturePath = IdvPath(conv_string(spt_loc.file_name));

		// find composite leaf/frond diffuse map filename
		std::string strTextureFilename = IdvNoPath(sMapBank.m_pCompositeMaps[CSpeedTreeRT::TL_DIFFUSE]);
		std::string strFullPath = strTexturePath + strTextureFilename;

		// load leaf composite diffuse map
		{
			resource_location tex_loc(spt_loc.pkg_name,
				conv_string(strFullPath));
			
			m_texLeavesAndFronds[CSpeedTreeRT::TL_DIFFUSE] = 
				nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block);
		}

#if defined(SPEEDTREE_FROND_NORMAL_MAPPING) || defined(SPEEDTREE_LEAF_NORMAL_MAPPING)
		// load leaf composite normal map
		{
			// find composite leaf/frond normal map filename
			strTextureFilename = IdvNoPath(sMapBank.m_pCompositeMaps[CSpeedTreeRT::TL_NORMAL]);
			strFullPath = strTexturePath + strTextureFilename;

			resource_location tex_loc(spt_loc.pkg_name,
				conv_string(strFullPath));

			m_texLeavesAndFronds[CSpeedTreeRT::TL_NORMAL] = 
				nresource_manager::instance()->load_texture_2d(tex_loc, EIO_Block);
		}
#endif

		//-- material template script
		m_template = nresource_manager::instance()->load_material_template_script(
			resource_location(_T("engine_data:material/SPT_Frond.hlsl"))
			);
	}

	void nspt_frond_material::draw_effect_param(nshading_effect* effect_ptr) const
	{
		effect_ptr->set_texture("SPT_CompositeDiffuseLeafMap", 
			m_texLeavesAndFronds[CSpeedTreeRT::TL_DIFFUSE]->get_render_texture() );

#if defined(SPEEDTREE_FROND_NORMAL_MAPPING)
		effect_ptr->set_texture("SPT_CompositeNormalLeafMap", 
			m_texLeavesAndFronds[CSpeedTreeRT::TL_NORMAL]->get_render_texture() );		
#endif
	}

	void nspt_leaf_material::create(nresource_texture::ptr composite_diffuse, nresource_texture::ptr composite_normal)
	{
		m_composite_diffuse = composite_diffuse;
		m_composite_normal = composite_normal;

		//-- material template script
		m_template = nresource_manager::instance()->load_material_template_script(
			resource_location(_T("engine_data:material/SPT_Leaf.hlsl"))
			);
	}

	void nspt_leaf_material::draw_effect_param(nshading_effect* effect_ptr) const
	{
		effect_ptr->set_texture("SPT_CompositeDiffuseLeafMap", 
			m_composite_diffuse->get_render_texture() );

#if defined(SPEEDTREE_LEAF_NORMAL_MAPPING)
		effect_ptr->set_texture("SPT_CompositeNormalLeafMap", 
			m_composite_normal->get_render_texture() );		
#endif
	}
}//namespace nexus