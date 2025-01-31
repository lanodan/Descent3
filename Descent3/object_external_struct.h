/*
 * $Logfile: /DescentIII/main/object_external_struct.h $
 * $Revision: 19 $
 * $Date: 4/28/99 11:28a $
 * $Author: Jason $
 *
 * Contains the structures needed for object definition (can be exported if needed to DLLs)
 *
 * $Log: /DescentIII/main/object_external_struct.h $
 * 
 * 19    4/28/99 11:28a Jason
 * made real viseffects not cast light
 * 
 * 18    4/22/99 4:13p Matt
 * Deleted sounds array from the object struct, since it was never used.
 * 
 * 17    4/21/99 12:41p Jason
 * make explosion system framerate independant
 * 
 * 16    4/18/99 8:13p Chris
 * Fixed the floating flare problems (where windows where broken out and
 * the flare remained)
 * 
 * 15    4/06/99 6:02p Matt
 * Added score system
 * 
 * 14    4/05/99 4:39p Jason
 * added groovy new smoke trails
 * 
 * 13    4/05/99 10:31a Matt
 * Changed soundsource volume from int to float. Duh.
 * 
 * 12    4/03/99 4:24p Jason
 * sped up attached vis effects by a large amount
 * 
 * 11    3/28/99 5:56p Matt
 * Added sparking effect for objects
 * 
 * 10    2/25/99 11:01a Matt
 * Added new explosion system.
 * 
 * 9     2/25/99 10:30a Jason
 * added nonvis generic/robot system
 * 
 * 8     2/22/99 2:04p Jason
 * added different damages for players and generics
 * 
 * 7     2/21/99 4:20p Matt
 * Added SoundSource objects (and reformatted parts of the object header
 * files).
 * 
 * 6     2/09/99 9:58a Chris
 * Massive BOA update  :)  Terrain happy now.  Vis happy now.  Sound happy
 * now.
 * 
 * 5     2/03/99 12:43a Chris
 * Added Obj_GetGroundPos
 * 
 * 4     2/01/99 4:17p Jason
 * more changes for multisafe
 * 
 * 3     1/22/99 8:53p Jeff
 * added custom-default script overrides
 * 
 * 2     1/21/99 11:15p Jeff
 * pulled out some structs and defines from header files and moved them
 * into seperate header files so that multiplayer dlls don't require major
 * game headers, just those new headers.  Side effect is a shorter build
 * time.  Also cleaned up some header file #includes that weren't needed.
 * This affected polymodel.h, object.h, player.h, vecmat.h, room.h,
 * manage.h and multi.h
*
* $NoKeywords: $
*/

#include "pstypes.h"
#include "vecmat_external.h"
#include "robotfirestruct.h"

#include "polymodel_external.h"

#include "aistruct.h" //Not to be included for multiplayer game sdk

#ifndef OBJECT_EXTERNAL_STRUCT_H
#define OBJECT_EXTERNAL_STRUCT_H

//IMPORTANT: make sure MAX_OBJECTS meshes with handle bit allocation
#define MAX_OBJECTS			1500		//total number of objects in world

// Splinter stuff
#define MAX_VERTS_PER_SPLINTER	4

/*
 *		MACROS
 */

//Flags and masks for cell numbers
#define ROOMNUM_CELLNUM_FLAG	0x80000000
#define ROOMNUM_CELLNUM_MASK	0x7fffffff

//Get a cell number from a room number
#define CELLNUM(roomnum)  ((roomnum) & ROOMNUM_CELLNUM_MASK)

//Make a room number from a cell number
#define MAKE_ROOMNUM(cellnum) ((cellnum) | ROOMNUM_CELLNUM_FLAG)

//Determine if a roomnum is really a cell number
#define ROOMNUM_OUTSIDE(roomnum) (((roomnum) & ROOMNUM_CELLNUM_FLAG) != 0)

//Determine if an object is outside
#define OBJECT_OUTSIDE(objp) ROOMNUM_OUTSIDE((objp)->roomnum)


/*
 *		STRUCTURES
 */

//lighting info
typedef struct 
{
	int flags; // see above
	float light_distance;
	float red_light1,green_light1,blue_light1;
	float red_light2,green_light2,blue_light2;
	float time_interval;
	float	flicker_distance;
	float directional_dot;
	int timebits;
	ubyte angle;
	ubyte lighting_render_type;
} light_info;

typedef struct
{
	int type_flags;		// see EF_FLAGS above
	
	float alpha;			// alpha value
	float deform_range;	// how many units to deform when drawing
	float cloak_time;		// how much time left cloaked
	float deform_time;	// how much time left deforming
	float color_time;		// how much time left colored
	float r,g,b;		

	// For fading in/out
	float fade_time;
	float fade_max_time;

	float	damage_time;
	float damage_per_second;	// how much damage this object takes per second
	float last_damage_time;		//	last time this object took damage
	int	damage_handle;			// the object handle of the owner of the damage

	float volume_change_time;
	vector volume_old_pos;
	int volume_old_room;

	// For powerups only
	float last_object_hit_time;
	int last_object_hit;		

	// For specular lighting
	vector spec_pos;
	float spec_mag;
	float spec_r,spec_g,spec_b;
	
	// For dynamic volume lighting
	ubyte dynamic_this_frame;
	float dynamic_red,dynamic_green,dynamic_blue;

	// For liquid object
	float liquid_time_left;
	ubyte liquid_mag;
	
	// For freeze objects
	float freeze_scalar;

	// For attach objects
	int attach_line_handle;			// handle to the object that this object is tethered to

	//Sound for special effects
	int sound_handle;

	//For spark effect
	float	spark_delay;		//delay between sparks
	float	spark_timer;		//how long until next spark
	float spark_time_left;	//how long until sparking stops

} effect_info_s;

// Describes the next animation state for a robot
typedef struct custom_anim
{
	float  server_time;
	ushort server_anim_frame;

	unsigned short anim_start_frame;
	unsigned short anim_end_frame;
	float  anim_time;
	float  max_speed;

	short	anim_sound_index;
	ubyte flags;
	char next_anim_type;
} custom_anim;

typedef struct multi_turret
{
	float time;
	float last_time;
	ubyte num_turrets;
	float *last_keyframes;
	float *keyframes;
	ubyte flags;
} multi_turret;

//Information specific to objects that render as a polygon model
typedef struct polyobj_info 
{
	short		model_num;				// Which polygon model this object is
	short		dying_model_num;		// The dying model for this object

	float		anim_start_frame;
	float		anim_frame;				// The model's current animation frame
	float		anim_end_frame;
	float		anim_time;
	uint		anim_flags;				// Looping/notify at finish/pending
	float		max_speed;

	union
	{
		custom_anim multi_anim_info;		// Multiplayer client info
		custom_anim custom_anim_info;		// Single-player/Server custom anim AIA_CUSTOM info
	};
	multi_turret multi_turret_info;

	uint		subobj_flags;			// Mask of which subobjects to draw
	int		tmap_override;			// If this is not -1, map all faces to this
} polyobj_info;

//A shard of, presumably, glass
typedef struct shard_info_s {
	vector	points[3];
	float		u[3],v[3];
	vector	normal;
	short		tmap;
} shard_info_s;

typedef struct line_info_s {
	vector end_pos;			// start pos is the object's .pos field
} line_info_s;

typedef struct blast_info_s 
{
	float max_size;
	int bm_handle;
} blast_info_s;

typedef struct dying_info_s {
	int	death_flags;		// Info about the death
	float delay_time;			// How long until object dies
	int	killer_playernum;	// The player who wille this object, or -1 if not a player
	float last_spark_time;
	float last_fireball_time;
	float last_smoke_time;
} dying_info_s;

typedef struct debris_info_s {
	int	death_flags;		// a copy of the parent's death flags
	float last_smoke_time;
} debris_info_s;

typedef struct laser_info_s 
{
	short		parent_type;	 	// The type of the parent of this object
	short		src_gun_num;		// The src gunpoint that this object fired from

	int		last_hit_handle;	//	For persistent weapons (survive object collision), object it most recently hit.
	int		track_handle;		//	Object this object is tracking.
	float		last_track_time;	// Last track time (see if an object is visible)

	int		hit_status;			// Zero not used
	vector	hit_pnt;
	vector	hit_wall_pnt;
	vector	hit_wall_normal;
	int		hit_room;
	int		hit_pnt_room;
	short		hit_face;

	float		multiplier;			//	Power if this is a fusion bolt (or other super weapon to be added).
	float		thrust_left;		// How many seconds of thrust are left before the weapon stops thrusting

	float		last_drop_time;	// Last time a particle was dropped from this weapon
	vector		last_smoke_pos;	// Last place smoke was dropped from this weapon
	bool		casts_light;	// Whether or not this weapon casts light
} laser_info_s;

typedef struct powerup_info_s 
{
	int		count;			//how many/much we pick up (vulcan cannon only?)
} powerup_info_s;

typedef struct splinter_info_s 
{
	ubyte subobj_num;
	short facenum;
	vector	verts[MAX_VERTS_PER_SPLINTER];
	vector center;
} splinter_info_s;

//Data for sourcesource objects
typedef struct {
	int sound_index;
	float volume;
} soundsource_info_s;

//information for physics sim for an object
// Some of this stuff is not object instance dependant -- so, it could be moved into
// a different struct to save space.  (But, then we cannot change it -- i.e we might want a powerup
// to change the mass of an object...)  Wait to move until we are optimizing -- see Chris if you move any fields
// out of the physics_info struct.  Thanx!
typedef struct physics_info 
{
	vector	velocity;			// Velocity vector of this object
	vector	thrust;				// Constant force applied to this object
	union
	{
		vector	rotvel;				// Rotational velecity (angles)
		float turn_rate;
	};
	union
	{	
		vector	rotthrust;			// Rotational acceleration
	};
	angle		turnroll;			// Rotation caused by turn banking
	float		last_still_time;	// The current delta position a wiggle has caused.
	int		num_bounces;		// Number of bounces before exploding (PHYSICS_UNLIMITED_BOUNCE is for unlimited bouncing)

	float		coeff_restitution;// What percent of velocity is kept after a bounce 
	float		mass;					// The mass of this object							     -- what about moving into type info
	float		drag;					// How fast this slows down						     -- what about moving into type info
	float		rotdrag;				// How much resistance to a change in spin rate   -- what about moving into type info
	union
	{
		float	full_thrust;		// Maximum thrust magnitude						     -- what about moving into type info
		float max_velocity;
	};
	union
	{
		float	full_rotthrust;	// Maximum rotation thrust magnitude			     -- what about moving into type info
		float	max_turn_rate;
	};
	float		max_turnroll_rate;// How fast is the maximum turnroll rate		     -- what about moving into type info
	float		turnroll_ratio;   // How much roll for a given turning rate		     -- what about moving into type info
	float		wiggle_amplitude; // The amplitude of an object's wiggle			     -- what about moving into type info
	float		wiggles_per_sec;	// How fast something wiggles						     -- what about moving into type info

	vector	dest_pos;			// destination position for interpolating velocity (for multiplayer only)

	union
	{
		float	hit_die_dot;
		int stuck_room;
	};

	union
	{
		float max_speed_time;
		int stuck_portal;
	};

	uint		flags;				// Misc physics flags
} physics_info;

typedef struct shockwave_info
{
	uint damaged_list[(MAX_OBJECTS/32) + 1];  
} shockwave_info;

typedef struct object_link_info
{
	int parent_handle;
	int sobj_index;
	vector fvec;
	vector uvec;
	vector pos;
} object_link_info;

typedef struct{
	ushort	DLLID;
	ushort	script_id;
	void	*script_instance;
}tOSIRISScriptNode;

typedef struct{
	tOSIRISScriptNode custom_script;
	tOSIRISScriptNode mission_script;
	tOSIRISScriptNode level_script;
	tOSIRISScriptNode default_script;
}tOSIRISScript;

//The data for an object
typedef struct object {
	ubyte		type;				// what type of object this is... robot, weapon, hostage, powerup, fireball
	ubyte		dummy_type;			// stored type of an OBJ_DUMMY
	ushort		id;				// which form of object...which powerup, robot, etc.
	ulong		flags;

	char		*name;			// the name of this object, or NULL

	int		handle;			//	unique handle for this object.  See defines above
	short		next,prev;		// id of next and previous connected object in Objects, -1 = no connection

	ubyte		control_type;  // how this object is controlled
	ubyte		movement_type; // how this object moves
	ubyte		render_type;	//	how this object renders
	ubyte		lighting_render_type;	// how this object is lit.  See flags above

	int		roomnum;			// room number or terrain cell containing object

	vector	pos;				// absolute x,y,z coordinate of center of object
	matrix	orient;			// orientation of object in world
	vector	last_pos;		// where object was last frame

	ushort	renderframe;	// framenum this object was last rendered

	vector	wall_sphere_offset;
	vector	anim_sphere_offset;

	float		size;				// 3d size of object - for collision detection
	float		shields; 		// Starts at maximum, when <0, object dies..

	sbyte		contains_type;	//	Type of object this object contains (eg, spider contains powerup)
	sbyte		contains_id;	//	ID of object this object contains (eg, id = blue type = key)
	sbyte		contains_count;// number of objects of type:id this object contains
	sbyte		pad3;				// keep alignment

	float		creation_time;	// absolute time when this object was created
	float		lifeleft;		// how long until goes away, if OF_USES_LIFELEFT flag is set
	float		lifetime;		// How long this object stays alive (in seconds)

	int		parent_handle;    // The handle of this object's parent

	int		attach_ultimate_handle;
	int		attach_parent_handle;
	int		*attach_children; // List of object handles for connected children

	ubyte		weapon_fire_flags;	// Used to indicate special weapon effects.  See flags above.

	char		attach_type;
	short lowest_attached_vis;
	union
	{
		float	attach_dist;
		short	attach_index;
	};

	//Movement info, determined by MOVEMENT_TYPE
	union 
	{
		physics_info	phys_info;		//a physics object
		shockwave_info	shock_info;
		object_link_info  obj_link_info;
	} mtype;

	//Collition detection stuff
	vector	min_xyz,max_xyz;		// the current min & max extents of this object's sphere

	//Current weapon battery info for this object
	dynamic_wb_info	*dynamic_wb;

	//Explosion information
	float impact_size;
	float impact_time;
	float impact_player_damage;
	float impact_generic_damage;
	float impact_force;

	// Object change information
	int change_flags;

	// object generic vis flags
	int generic_nonvis_flags;
	int generic_sent_nonvis;
	
	lightmap_object lm_object;	// The lightmap object for this object

	//Control info, determined by CONTROL_TYPE
	union {								
		laser_info_s 			laser_info;
		powerup_info_s			powerup_info;
		splinter_info_s		splinter_info;
		blast_info_s			blast_info;
		dying_info_s			dying_info;
		debris_info_s			debris_info;
		soundsource_info_s	soundsource_info;
	} ctype;

	ai_frame			*ai_info;  // AI information pointer

	//Render info, determined by RENDER_TYPE
	union {
		polyobj_info	pobj_info;		//polygon model
		shard_info_s	shard_info;		//shard
		#ifdef _DEBUG
		line_info_s		line_info;		//line info
		#endif
		ddgr_color		sphere_color;	//for RT_EDITOR_SPHERE
	} rtype;

	effect_info_s	*effect_info;
	light_info		*lighting_info;		//Pointer to lighting info, or NULL if inherits from type

	//Something to do with multiplayer, possibly, but it's hard to know for sure
	//because some people are incapable of commented their code.
	ushort position_counter;

	//OSIRIS Script Info (new OSIRIS)	
	tOSIRISScript *osiris_script;

	char *custom_default_script_name;
	char *custom_default_module_name;
} object;

#endif
