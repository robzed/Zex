//3d structures defs
//oldmac//#include <DrawSprocket.h>

#define __3d_structures__
#define MAX_NUMBER_OF_DYN_OBJECTS 2000	//on test run logged max of 486
#define MAX_NUMBER_OF_STAT_OBJECTS 180
#define MAX_NUMBER_OF_POLYS 6000		
#define MAX_NUMBER_POLYS_PER_OBJ 130
#define NUMBER_OF_CMAP_OBJS 4000	//this can be wanged up - it's just a list of ints
#define MAX_TOUCHING_POLYS 6	//number of polys touching a point for gourad
//shading types for poly structure
#define FLAT 0
#define GOURAD 1
#define TEXTURE 2

#define IS_NOT_TRANSLUCENT 0
#define IS_TRANSLUCENT 1

#define zex_object 0
#define baddie_object 1
#define scenary_object 2
#define ALIEN 1
#define HUMAN 0
#define SCENARY 2

#define NPC_CLASS_FIGHTER 0
#define NPC_CLASS_TRAVELLER 1
#define NPC_CLASS_GUARD 2

typedef struct
{
   float x, y, z;
} _3D;



typedef struct
{
int x,y;
} _2D;

typedef struct
{
float x,y;
} _float2D;



typedef short  _Connection;

typedef struct
{
short the_node;
short u;
short v;
short type;	//0=normal, 1=gourad, 2=texture
} textured_connections;


typedef struct
{
RGBColor colour;
int points_start;
} Zex_Colour;




typedef struct
{
textured_connections *poly_connections_ptr;	//ptr to the connection list for this poly
_3D face_normal;	//the face DIRECTION vector
_3D face_normal_translated;	//after rotation

RGBColor Prgb;	  //the colour
float average_z;	//average z of poly
int Plight_value;	//the lighting value
int PPoints_offset;	//offset to points for 2dpoly draw
Ptr Ptexture;	//the ptr to the texture
unsigned long Pid;	//the name for OGL
short Ptouching1[MAX_TOUCHING_POLYS];	  //for true gourad - a list of up to 6 polys touching this one point
short Ptouching2[MAX_TOUCHING_POLYS];	  //for true gourad - a list of up to 6 polys touching this one point
short Ptouching3[MAX_TOUCHING_POLYS];	  //for true gourad - a list of up to 6 polys touching this one point

//unsigned char  Ptexture;	  //the texture
//unsigned char  PTranslucent;	  //the translucency
char p_visible;	//poly is visible?
char draw_code;	//0=normal, 1=translucent, 2 ff=transparent - use different filler

//int special_1, special_2;	//???
//int eng_culled;	//if 0, do not backface cull this poly
} _ZPoly;


typedef struct
{
_ZPoly *_2DPoly_ptr;
int x1,y1,x2,y2,x3,y3;
} _2DPoly;

typedef struct //_ZPoly_objs_struct_tag
{
int in_use;
_ZPoly the_obj_polys[MAX_NUMBER_POLYS_PER_OBJ];
//} _ZPoly_objs[MAX_NUMBER_OF_DYN_OBJECTS];
//} _ZPoly_objs_base;
//typedef _ZPoly_objs_base _ZPoly_objs[MAX_NUMBER_OF_DYN_OBJECTS];
} _ZPoly_objs;



typedef struct {
_3D *points_ptr;		//ptr to the points for this object
_ZPoly *polys_ptr;;	//ptr to the polys for this object
int polys_slot;		//the polys slot
} anim_obj_bits;

//dynamic textures
typedef struct {
char tex_data [128*128+40];	//+40 just to make me feel [a lot] better.
int in_use;
int name;	//ogl name
} dynamic_texture;


enum {
TYPE_ZEX,
TYPE_ESHUTTLE,
TYPE_FALCON,
TYPE_ECLIPSE,
TYPE_SMALL,
TYPE_BLUE,
TYPE_GREEN,
TYPE_ASHUTTLE,
TYPE_PHOENIX,
TYPE_PYTHON,
TYPE_EXCALIBUR,
TYPE_DONT_CARE,
};

typedef struct
{
unsigned char in_use;
unsigned char laser_type;
unsigned char cannon_type;
unsigned char shields_type;
short craft_type;	//see enum above
short  time_docked;	 //when this item was docked
} docked_item_type;

//mothership memory block 
typedef struct
{
                        //0-100
float production_rating;	//how quickly it can turn a new ship out - based on supply
float max_power_rating;	//max possible (0-100)
float power_rating;	 //current raw power available - drops charge shields and as production goes up (0-100)
                         //of raw materials 0-100
float supply_rating;	 //affects above 3 - filled in from closeness to planet
                       //0 - 100
unsigned char in_use;	 //1 if so, else 0=free
unsigned char like_zex;	//used in alien mships - 1 initially, incremented if zex fires on mship
                       
unsigned char allegiance;
unsigned char visited;	//1 if we have actually docked here  

int supply_planet_index;	//which planet this MS is supplied from
int shield_value;	//copied into and out of OCB's
int callsign;

_float2D real_coords;

int max_ships_in_dock;
int ships_in_dock;	//how many in dock
docked_item_type docked_queue[8];	//max number of ships docked
int surrender_timer;	//if >0 can't surrender (surrender sets to 1000)
short galactic_id;

} mothership_block;



typedef struct
{
UInt32 seed;
char inhabited;
char type;	//gas, solid, liquid etc
short production_quota;	//how quickly it can generate materials if inahbited
_float2D real_coords;
float planet_z;
unsigned char name[20];	//pstring actually
short flash_timer;
short galactic_id;

} planet_block;

typedef struct
{
_float2D real_coords;
short galactic_id;
unsigned short seed;

} scenary_block;

typedef struct
{
UInt32 toll;	//how much it costs
_float2D real_coords;
short galactic_id;

} warphole_block;

typedef struct
{
UInt32 code;	//needs to be 32 bit because of the encoding we use Stu, so don't optimise it!
_float2D real_coords;
short galactic_id;
//depending on the code, these may be used as feedback from kill
UInt8 object1_alive;
UInt8 object2_alive;

} specials_block;


#define NUMBER_OF_SECTIONS_IN_A_QUAD_X 10	//how many sections in X
#define NUMBER_OF_SECTIONS_IN_A_QUAD_Y 10	//same for Y
#define SECTION_SIZE 2000000	//size of a section
			//hence a quad is 20e6 units along a side
#define NUMBER_OF_MOTHERSHIPS_IN_A_SECTION 3
#define NUMBER_OF_MOTHERSHIPS_PER_QUAD NUMBER_OF_MOTHERSHIPS_IN_A_SECTION*NUMBER_OF_SECTIONS_IN_A_QUAD_Y*NUMBER_OF_SECTIONS_IN_A_QUAD_X
#define NUMBER_OF_PLANETS_PER_QUAD 340
#define NUMBER_OF_SCENARY_PER_QUAD 17500
#define NUMBER_OF_SPECIALS_PER_QUAD 60

#define GALAXY_START_Y NUMBER_OF_SECTIONS_IN_A_QUAD_Y * 2 * SECTION_SIZE
#define GALAXY_START_X 0


typedef struct
{
int serial_number;	//0-3
warphole_block the_warphole;
mothership_block quadrant_motherships[NUMBER_OF_MOTHERSHIPS_PER_QUAD];
planet_block quadrant_planets[NUMBER_OF_PLANETS_PER_QUAD];
scenary_block quadrant_scenary[NUMBER_OF_SCENARY_PER_QUAD];
specials_block quadrant_specials[NUMBER_OF_SPECIALS_PER_QUAD];
} quadrant_map;

//finally the galactic map is 4 quadrants
typedef struct
{
int version;
int current_quadrant;
_float2D zex_current_coords;

quadrant_map the_quadrant[4];
} galactic_map, *galactic_map_ptr; // , **galactic_map_handle;

//define array of ZPolyObjs (MAX_NUMBER_POLYS_PER_OBJ polys per object max)
//typedef struct
//{
//_ZPolyObjs dynamic_polys[MAX_NUMBER_OF_DYN_OBJECTS];
//} dynamic_polys_array,*dynamic_polys_arrayPtr,**dynamic_polys_arrayHandle;


//A box can be defined with two 3d points which identifies the top left front and
//rear bottom right. 
//This structure defines the bounding box for an object

//From this data we can recreated a rotated and transformed bounding box in little space.
//we can do collision detection from this data in the tradsitional way.



//object def
typedef struct
{

int	in_use;	//1 if so, else 0
short is_visible;	//1 if so, else 0 from obj fov filter prior to main 3d calcs
short eng_invisible;	//if 1, this objects polys go right the way through until final draw and then stop
                    //used to make something invisible
OSType Oid;	//E.G. B001 - four character generaic identifier
int unique_id;	//an id from the level file; generally zero but set for doors that need to be found by switches 
int Orotx, Oroty, Orotz;	//object rotation from static def
int Orotdx, Orotdy, Orotdz;	//object delta rotation
float Omovedx, Omovedy, Omovedz;	//object delta movement per frame
//int number_owned;	//number of children owned if top level object
float Oworldx, Oworldy, Oworldz; //where the object is to go in the world
float previous_x,previous_y;	//colldetect stuff filled in by note_previous at end of loop
float Ostartx, Ostarty; //where the object started in the world - used by ssome monster controllers
int control_use_1, control_use_2, control_use_3, control_use_4;	//for controller use
int current_velocity,wanted_velocity;
int control_use_7, control_use_8, control_use_9, control_use_10;	//for controller use
int time_out,go_home_time;	//time out+1 every frame, when gets to go_home_time 
                          //NPC finds a mothership and docks
int going_to;	//home is where this object was generated. going_to is where 
                        //it want's to go to when not doing anything else

int who_spawned_this_object;	//who fired it etc
int callsign;
int last_heading;	//last known zang
int wanted_roll;	//o to 30 or 0 to 330

short cu11,cu12,cu13;
short controller;	//the main controller for this object
short galactic_id;	//unique id from map so we can tell if an object is already present when we try to load it

short swerve_direction;	//when avoiding which way? Filled by load_dyn_object randomly
short attack_mode;
short attack_what;	//something that is attacking is attacking this object
short invincible_count;	//if not zero, object is invincible
short max_vel,min_vel;	//maximum and minimum velocities
short craft_type;	//see enum above mship_block - not used in training, only game from gen_NPC
short rank;		//the rank of the pilot
unsigned short command;		 //for wingmen only; the wingman continually sets bit 15 (0x8000) so it knows when a new command comes in
unsigned short command_memory;	 //copied to here when we've ack'd it
unsigned short command_timer;	 //when this reaches zero, wingman acts on command
unsigned short current_mode;	 //wingmans modus operandi - 1=normal with zex, 2=stay here

//int Oreference;	//this objects reference ID
//int immediate_child;	  //this objects immediate child
int number_of_polys;	//how many polys make up this object
int number_of_points;	//how many points
_3D *points_ptr;		//ptr to the points for this object
_3D *world_points_ptr;	//ptr to the transformed world points for this object
_ZPoly *polys_ptr;	//ptr to the polygon definitions
int active_polys_slot;	//the index in the dynamic active polys array so we can kill them quickly
dynamic_texture * dynamic_tex_ptr;	//if object is using a dynamic texture this is the ptr else 0


_3D object_normal;	//the object normal
_3D object_normal_translated; //translated object normal (which way it's facing)
//various control items
float radius;	//bounds sphere of object
_3D box_min;	//collision box
_3D box_max;
_3D box_min_translated;	//collision box
_3D box_max_translated;
float extern_movedx,extern_movedy;	//exterrnal force inputs from object collisions
int extern_rotdz;	//new 210699
long hit_by_slot;	//dynamic slot of object that hit this object else -1, so we can find info about what hit this object
long docked_object_slot;	//motherships slot number, -1 if not docked
mothership_block *  mship_ptr;	//ptr to mothership data or zero.
short shield_value;	//when zero if collision then die (unless ZEX which has hull protection as well)
short original_shield_value;	//used when undocking to restore shileds
short mass;	//mass of this object
UInt16 dq;	//how much damage this thing does when hit - 080499
//these are purely for motherships
SInt16 docking_what;	//what object is the mothership docking
//see 3d engine design - controllers for these
UInt8 aggressive;	//if set, controller looks for summat attack
UInt8 obj_class; //fighter, traveller, guard
UInt8 sub_controller;	//mon3, mon4 etc.

UInt8 on_death_code;	//so we can update galactic map when this thing dies

UInt8 docked;	//1 if so
UInt8 crashable; //if 1 then object is collidable
UInt8 colltype;	//collision detection type. 3=default (sphere and box 1=sphere, 2=box
UInt8 just_hit;	//object controllers use this to take evasive action. Inc'd in coll_det, dec'd by controller
UInt8 no_trivial_rejection;	//should be set by monsters that must run when not in view
UInt8 object_category;	//2 if a scenary object, 1 if a monster object, 0 if a zex object - used in coll det
UInt8 has_roll;	//1 if object rolls as a result of heading_delta - from object loader
UInt8 showing_shield;	//1 if object is showing its shield
UInt8 request_dock;	//>0 if so - causes a mothership to consider docking object
//following 4 added 160199
UInt8 laser_category;
UInt8 cannon_category;
UInt8 missile_category;
UInt8 number_of_missiles;
UInt8 shields_class;	//0 not rechargeable, >0 recharge rate per frame
UInt8 is_armed;	//set to 1 by default; a level controlelr can set this to zero to stop a monster firing
//animation control - d10 - see 3d engine design under ANIMATION for details.
UInt8 anim_control;	//0=stop at end of cycle, 1=continuous
UInt8 anim_state;	//0=stopped, 1=running
UInt8 anim_frame_rate;	  //how often objects are switched.
UInt8 anim_counter;	//used in conjunction with anim-frame_rate
UInt8 anim_frame;
int anim_max_object;	//slot number of next object in sequence
//anim_object_ptrs anim_points_polys;	//ptrs to points and polys for eaach object
anim_obj_bits the_anim_ptrs[6];
} ZObject;




//Dynamic objects allocation
typedef struct
{
ZObject object_list[MAX_NUMBER_OF_DYN_OBJECTS];
} DynObjectsFixedSize,*DynObjectsFixedSizePtr; //,**DynObjectsFixedSizeHandle;

//Static Objects allocation
typedef struct
{
ZObject object_list[MAX_NUMBER_OF_STAT_OBJECTS];
} StatObjectsFixedSize,*StatObjectsFixedSizePtr; //,**StatObjectsFixedSizeHandle;

typedef struct
{
_ZPoly *poly_ptr;	//the poly
_3D point1,point2,point3; //the points
} visible_poly;

typedef struct
{
int previous_slot;	//z before
int next_slot;	//next z (double linked list)
float z_value;

_ZPoly *poly_data;
} _ZPolySort;

typedef struct
{
float world_x,world_y,world_z;	  //the position of the object
float Omovedx, Omovedy, Omovedz;	//object delta movement per frame
int delta_rot_x,delta_rot_y,delta_rot_z;
int rot_x,rot_y,rot_z;
int controller_ref,control_use_1,control_use_2;
int control_use_3,control_use_4,control_use_7;
int current_velocity,wanted_velocity,attack_mode,who_spawned_this_object;
int go_home_time;
short shield_value,mass;
short max_vel,min_vel;	//maximum and minimum velocities

UInt16 dq;	//how much damage this thing does when hit - 080499
UInt8 no_trivial_rejection,object_category;
UInt8 has_roll;	//1 if object rolls as a result of heading_delta - from object loader

int going_to;
UInt8 aggressive;
UInt8 obj_class; //fighter, traveller, guard
UInt8 sub_controller;	//mon3, mon4 etc.
UInt8 shields_class;	//0 not rechargeable, >0 recharge rate per frame

///following 4 added 160199
UInt8 laser_cat;
UInt8 cannon_cat;
UInt8 missile_cat;
UInt8 number_of_missiles;
_3D normal;
int anim_frame_rate;	//how often objects are switched.
int anim_max_object;	//slot number of next object in sequence
int unique_id;	//an id from the level file; generally zero but set for doors that need to be found by switches 
//anim_object_ptrs anim_points_polys;	//ptrs to points and polys for eaach object
anim_obj_bits the_anim_ptrs[3];
} load_dyn_object_pb;


typedef struct
{
unsigned int object_id[20];
} anim_pb;


typedef struct
{
int cmap_objs[NUMBER_OF_CMAP_OBJS];
} cmap_objects;

//some macros to get sin and cos of an angle between 0 and 359
//#define SIN(x) SinTable[((UInt32)x&255)]
//#define COS(x) CosTable[((UInt32)x&255)]	//safe (slower) accessors

#define SIN(x) SinTable[((UInt32)x)]
#define COS(x) CosTable[((UInt32)x)]	//unsafe accesors


