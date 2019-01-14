/*
Copyright (C) 1996-2001 Id Software, Inc.
Copyright (C) 2002-2009 John Fitzgibbons and others
Copyright (C) 2007-2008 Kristian Duske
Copyright (C) 2010-2014 QuakeSpasm developers
Copyright (C) 2016 Axel Gneiting

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef __GLQUAKE_H
#define __GLQUAKE_H

void GL_WaitForDeviceIdle (void);
qboolean GL_BeginRendering (int *x, int *y, int *width, int *height);
qboolean GL_AcquireNextSwapChainImage (void);
void GL_EndRendering (qboolean swapchain_acquired);
qboolean GL_Set2D (void);

extern	int glx, gly, glwidth, glheight;

// r_local.h -- private refresh defs

#define ALIAS_BASE_SIZE_RATIO		(1.0 / 11.0)
					// normalizing factor so player model works out to about
					//  1 pixel per triangle
#define	MAX_LBM_HEIGHT		480

#define TILE_SIZE		128		// size of textures generated by R_GenTiledSurf

#define SKYSHIFT		7
#define	SKYSIZE			(1 << SKYSHIFT)
#define SKYMASK			(SKYSIZE - 1)

#define BACKFACE_EPSILON	0.01

#define	MAX_GLTEXTURES	2048
#define NUM_COLOR_BUFFERS 2
#define INITIAL_STAGING_BUFFER_SIZE_KB	16384

#define FAN_INDEX_BUFFER_SIZE 126

void R_TimeRefresh_f (void);
void R_ReadPointFile_f (void);
texture_t *R_TextureAnimation (texture_t *base, int frame);

typedef struct surfcache_s
{
	struct surfcache_s	*next;
	struct surfcache_s 	**owner;		// NULL is an empty chunk of memory
	int			lightadj[MAXLIGHTMAPS]; // checked for strobe flush
	int			dlight;
	int			size;		// including header
	unsigned		width;
	unsigned		height;		// DEBUG only needed for debug
	float			mipscale;
	struct texture_s	*texture;	// checked for animating textures
	byte			data[4];	// width*height elements
} surfcache_t;


typedef struct
{
	pixel_t		*surfdat;	// destination for generated surface
	int		rowbytes;	// destination logical width in bytes
	msurface_t	*surf;		// description for surface to generate
	fixed8_t	lightadj[MAXLIGHTMAPS];
							// adjust for lightmap levels for dynamic lighting
	texture_t	*texture;	// corrected for animating textures
	int		surfmip;	// mipmapped ratio of surface texels / world pixels
	int		surfwidth;	// in mipmapped texels
	int		surfheight;	// in mipmapped texels
} drawsurf_t;


typedef enum {
	pt_static, pt_grav, pt_slowgrav, pt_fire, pt_explode, pt_explode2, pt_blob, pt_blob2
} ptype_t;

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
typedef struct particle_s
{
// driver-usable fields
	vec3_t		org;
	float		color;
// drivers never touch the following fields
	struct particle_s	*next;
	vec3_t		vel;
	float		ramp;
	float		die;
	ptype_t		type;
} particle_t;

#define WORLD_PIPELINE_COUNT 8

typedef struct
{
	VkDevice							device;
	qboolean							device_idle;
	qboolean							validation;
	VkQueue								queue;
	VkCommandBuffer						command_buffer;
	VkPipeline							current_pipeline;
	VkClearValue						color_clear_value;
	VkFormat							swap_chain_format;
	VkPhysicalDeviceProperties			device_properties;
	VkPhysicalDeviceMemoryProperties	memory_properties;
	uint32_t							gfx_queue_family_index;
	VkFormat							color_format;
	VkFormat							depth_format;
	VkSampleCountFlagBits				sample_count;
	qboolean							supersampling;
	qboolean							non_solid_fill;

	// Extensions
	qboolean							dedicated_allocation;

	// Buffers
	VkImage								color_buffers[NUM_COLOR_BUFFERS];

	VkImage								colorlut_buffer;

	// Index buffers
	VkBuffer							fan_index_buffer;

	// Staging buffers
	int									staging_buffer_size;

	// Render passes
	VkRenderPass						main_render_pass;
	VkClearValue						main_clear_values[4];
	VkRenderPassBeginInfo				main_render_pass_begin_infos[2];
	VkRenderPass						ui_render_pass;
	VkRenderPassBeginInfo				ui_render_pass_begin_info;
	VkRenderPass						warp_render_pass;

	// Pipelines
	VkPipeline							basic_alphatest_pipeline[2];
	VkPipeline							basic_blend_pipeline[2];
	VkPipeline							basic_notex_blend_pipeline[2];
	VkPipeline							basic_poly_blend_pipeline;
	VkPipelineLayout					basic_pipeline_layout;
	VkPipeline							world_pipelines[WORLD_PIPELINE_COUNT];
	VkPipelineLayout					world_pipeline_layout;
	VkPipeline							water_pipeline;
	VkPipeline							water_blend_pipeline;
	VkPipeline							raster_tex_warp_pipeline;
	VkPipeline							particle_pipeline;
	VkPipeline							sprite_pipeline;
	VkPipeline							sky_color_pipeline;
	VkPipeline							sky_box_pipeline;
	VkPipeline							sky_layer_pipeline;
	VkPipelineLayout					sky_layer_pipeline_layout;
	VkPipeline							alias_pipeline;
	VkPipeline							alias_blend_pipeline;
	VkPipeline							alias_alphatest_pipeline;
	VkPipelineLayout					alias_pipeline_layout;
	VkPipeline							postprocess_pipeline;
	VkPipelineLayout					postprocess_pipeline_layout;
	VkPipeline							screen_warp_pipeline;
	VkPipelineLayout					screen_warp_pipeline_layout;
	VkPipeline							cs_tex_warp_pipeline;
	VkPipelineLayout					cs_tex_warp_pipeline_layout;
	VkPipeline							showtris_pipeline;
	VkPipeline							showtris_depth_test_pipeline;
	VkPipelineLayout					showtris_pipeline_layout;
	VkPipeline							grade_pipeline;
	VkPipelineLayout					grade_pipeline_layout;
	VkPipeline							blur_pipeline;
	VkPipelineLayout					blur_pipeline_layout;

	// Descriptors
	VkDescriptorPool					descriptor_pool;
	VkDescriptorSetLayout				ubo_set_layout;
	VkDescriptorSetLayout				single_texture_set_layout;
	VkDescriptorSetLayout				input_attachment_set_layout;
	VkDescriptorSet						screen_warp_desc_set;
	VkDescriptorSetLayout				screen_warp_set_layout;
	VkDescriptorSetLayout				single_texture_cs_write_set_layout;
	VkDescriptorSet						grade_desc_set;
	VkDescriptorSetLayout				grade_set_layout;
	VkDescriptorSet						blur_desc_sets[2];
	VkDescriptorSetLayout				blur_set_layout;

	// Samplers
	VkSampler							point_sampler;
	VkSampler							linear_sampler;
	VkSampler							clamped_linear_sampler;
	VkSampler							point_aniso_sampler;
	VkSampler							linear_aniso_sampler;

	// Matrices
	float								projection_matrix[16];
	float								view_matrix[16];
	float								view_projection_matrix[16];

	//Dispatch table
	PFN_vkCmdBindPipeline				vk_cmd_bind_pipeline;
	PFN_vkCmdPushConstants				vk_cmd_push_constants;
	PFN_vkCmdBindDescriptorSets			vk_cmd_bind_descriptor_sets;
	PFN_vkCmdBindIndexBuffer			vk_cmd_bind_index_buffer;
	PFN_vkCmdBindVertexBuffers			vk_cmd_bind_vertex_buffers;
	PFN_vkCmdDraw						vk_cmd_draw;
	PFN_vkCmdDrawIndexed				vk_cmd_draw_indexed;
	PFN_vkCmdPipelineBarrier			vk_cmd_pipeline_barrier;
	PFN_vkCmdCopyBufferToImage			vk_cmd_copy_buffer_to_image;
} vulkanglobals_t;

extern vulkanglobals_t vulkan_globals;

//====================================================

extern	qboolean	r_cache_thrash;		// compatability
extern	vec3_t		modelorg, r_entorigin;
extern	entity_t	*currententity;
extern	int		r_visframecount;	// ??? what difs?
extern	int		r_framecount;
extern	mplane_t	frustum[4];
extern	int render_pass_index;
extern	qboolean render_warp;

//
// view origin
//
extern	vec3_t	vup;
extern	vec3_t	vpn;
extern	vec3_t	vright;
extern	vec3_t	r_origin;

//
// screen size info
//
extern	refdef_t	r_refdef;
extern	mleaf_t		*r_viewleaf, *r_oldviewleaf;
extern	int		d_lightstylevalue[256];	// 8.8 fraction of base light value

extern	cvar_t	r_norefresh;
extern	cvar_t	r_drawentities;
extern	cvar_t	r_drawworld;
extern	cvar_t	r_drawviewmodel;
extern	cvar_t	r_speeds;
extern	cvar_t	r_pos;
extern	cvar_t	r_waterwarp;
extern	cvar_t	r_fullbright;
extern	cvar_t	r_lightmap;
extern	cvar_t	r_shadows;
extern	cvar_t	r_wateralpha;
extern	cvar_t	r_lavaalpha;
extern	cvar_t	r_telealpha;
extern	cvar_t	r_slimealpha;
extern	cvar_t	r_dynamic;
extern	cvar_t	r_novis;

extern	cvar_t	gl_clear;
extern	cvar_t	gl_cull;
extern	cvar_t	gl_smoothmodels;
extern	cvar_t	gl_affinemodels;
extern	cvar_t	gl_polyblend;
extern	cvar_t	gl_nocolors;

extern	cvar_t	gl_playermip;

extern	cvar_t	gl_subdivide_size;
extern	float	load_subdivide_size; //johnfitz -- remember what subdivide_size value was when this map was loaded

extern int		gl_stencilbits;

//johnfitz -- anisotropic filtering
extern	float		gl_max_anisotropy;
extern	qboolean	gl_anisotropy_able;

//johnfitz -- polygon offset
#define OFFSET_BMODEL 1
#define OFFSET_NONE 0
#define OFFSET_DECAL -1
#define OFFSET_FOG -2
#define OFFSET_SHOWTRIS -3

//johnfitz -- rendering statistics
extern int rs_brushpolys, rs_aliaspolys, rs_skypolys, rs_particles, rs_fogpolys;
extern int rs_dynamiclightmaps, rs_brushpasses, rs_aliaspasses, rs_skypasses;
extern float rs_megatexels;

//johnfitz -- track developer statistics that vary every frame
extern cvar_t devstats;
typedef struct {
	int		packetsize;
	int		edicts;
	int		visedicts;
	int		efrags;
	int		tempents;
	int		beams;
	int		dlights;
} devstats_t;
extern devstats_t dev_stats, dev_peakstats;

//ohnfitz -- reduce overflow warning spam
typedef struct {
	double	packetsize;
	double	efrags;
	double	beams;
	double	varstring;
} overflowtimes_t;
extern overflowtimes_t dev_overflows; //this stores the last time overflow messages were displayed, not the last time overflows occured
#define CONSOLE_RESPAM_TIME 3 // seconds between repeated warning messages

typedef struct
{
	float	position[3];
	float	texcoord[2];
	byte	color[4];
} basicvertex_t;

//johnfitz -- moved here from r_brush.c
extern int gl_lightmap_format, lightmap_bytes;
#define MAX_LIGHTMAPS 512 //johnfitz -- was 64
extern gltexture_t *lightmap_textures[MAX_LIGHTMAPS]; //johnfitz -- changed to an array

extern qboolean r_drawflat_cheatsafe, r_fullbright_cheatsafe, r_lightmap_cheatsafe, r_drawworld_cheatsafe; //johnfitz

extern float	map_wateralpha, map_lavaalpha, map_telealpha, map_slimealpha; //ericw

//johnfitz -- fog functions called from outside gl_fog.c
void Fog_ParseServerMessage (void);
float *Fog_GetColor (void);
float Fog_GetDensity (void);
void Fog_EnableGFog (void);
void Fog_DisableGFog (void);
void Fog_SetupFrame (void);
void Fog_NewMap (void);
void Fog_Init (void);

void R_NewGame (void);

void R_AnimateLight (void);
void R_MarkSurfaces (void);
void R_CullSurfaces (void);
qboolean R_CullBox (vec3_t emins, vec3_t emaxs);
void R_StoreEfrags (efrag_t **ppefrag);
qboolean R_CullModelForEntity (entity_t *e);
void R_RotateForEntity (float matrix[16], vec3_t origin, vec3_t angles);
void R_MarkLights (dlight_t *light, int num, mnode_t *node);

void R_InitParticles (void);
void R_DrawParticles (void);
void CL_RunParticles (void);
void R_ClearParticles (void);

void R_TranslatePlayerSkin (int playernum);
void R_TranslateNewPlayerSkin (int playernum); //johnfitz -- this handles cases when the actual texture changes
void R_UpdateWarpTextures (void);

void R_DrawWorld (void);
void R_DrawAliasModel (entity_t *e);
void R_DrawBrushModel (entity_t *e);
void R_DrawSpriteModel (entity_t *e);

void R_DrawTextureChains_Water (qmodel_t *model, entity_t *ent, texchain_t chain);

void GL_BuildLightmaps (void);
void GL_DeleteBModelVertexBuffer (void);
void GL_BuildBModelVertexBuffer (void);
void GLMesh_LoadVertexBuffers (void);
void GLMesh_DeleteVertexBuffers (void);

int R_LightPoint (vec3_t p);

void GL_SubdivideSurface (msurface_t *fa);
void R_BuildLightMap (msurface_t *surf, byte *dest, int stride);
void R_RenderDynamicLightmaps (msurface_t *fa);
void R_UploadLightmaps (void);

void R_DrawWorld_ShowTris(void);
void R_DrawBrushModel_ShowTris (entity_t *e);
void R_DrawAliasModel_ShowTris (entity_t *e);
void R_DrawParticles_ShowTris(void);

void DrawGLPoly (glpoly_t *p, float color[3], float alpha);
void GL_MakeAliasModelDisplayLists (qmodel_t *m, aliashdr_t *hdr);

void Sky_Init (void);
void Sky_DrawSky (void);
void Sky_NewMap (void);
void Sky_LoadTexture (texture_t *mt);
void Sky_LoadSkyBox (const char *name);

void R_ClearTextureChains (qmodel_t *mod, texchain_t chain);
void R_ChainSurface (msurface_t *surf, texchain_t chain);
void R_DrawTextureChains (qmodel_t *model, entity_t *ent, texchain_t chain);
void R_DrawWorld_Water (void);

float GL_WaterAlphaForSurface (msurface_t *fa);

int GL_MemoryTypeFromProperties(uint32_t type_bits, VkFlags requirements_mask, VkFlags preferred_mask);

void R_CreateDescriptorPool();
void R_CreateDescriptorSetLayouts();
void R_InitSamplers();
void R_CreatePipelineLayouts();
void R_CreatePipelines();
void R_DestroyPipelines();

static inline void R_BindPipeline(VkPipeline pipeline) {
	if(vulkan_globals.current_pipeline != pipeline) {
		vulkan_globals.vk_cmd_bind_pipeline(vulkan_globals.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vulkan_globals.current_pipeline = pipeline;
	}
}

void R_InitStagingBuffers();
void R_SubmitStagingBuffers();
byte * R_StagingAllocate(int size, int alignment, VkCommandBuffer * command_buffer, VkBuffer * buffer, int * buffer_offset);

void R_InitGPUBuffers();
void R_SwapDynamicBuffers();
void R_FlushDynamicBuffers();
byte * R_VertexAllocate(int size, VkBuffer * buffer, VkDeviceSize * buffer_offset);
byte * R_IndexAllocate(int size, VkBuffer * buffer, VkDeviceSize * buffer_offset);
byte * R_UniformAllocate(int size, VkBuffer * buffer, uint32_t * buffer_offset, VkDescriptorSet * descriptor_set);

void GL_SetObjectName(uint64_t object, VkDebugReportObjectTypeEXT objectType, const char * name);

#endif	/* __GLQUAKE_H */

