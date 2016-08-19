//item types enum list for SHIP systems, not for buy

enum {
//these can be damaged and repaired
KRADAR,	//NORMAL RADAR
KLONG_RANGE_RADAR, 
KCANNON,
KLASER,	//FRONT LASER
KMISSILES,	//MISSILE TARGETTING COMPUTER
KREPAIR_SYSTEMS,	//CAN BE 1 OR 2
KFLIGHT_COMPUTER,	//FLIGHT COMPUTER - ORientation Control System - ORCS
KENGINE,
//these cant be damaged
KMISSILE_PYLON1,KMISSILE_PYLON2,KMISSILE_PYLON3,	//3 MISSILE PYLONS
KSHIELDS,	//SHIELDS
KNORMAL_FUEL_TANK,	//NORMAL FUEL

//things from here on can be LOST
KREAR_LASER,
K360,	//360 DEGREES TARGETTING
KSHOCKWAVE,KECM,KMISSILE_FACTORY,

KFAST_DOCK, KFLATPACK, KFUEL_MAXIMISER,KRETRO_ROCKETS,
KHYPERSPACE,
KNAV_COMPUTER,	//RADAR VECTORS
KEJECTION,	//EJECTION POD

KTRADING_COMPUTER, KAUTO_HULL_REPAIR, KBOID_ADDITION,
KASTEROID_ADDITION,	//weapons do more damage to asteroids
KLAST,	//not used but used as a count of items for array dimensioning
};

#define ITEMS_COUNT KLAST

//kit_description
//status=
//-1=lost
//0=not fitted
//1 fitted and serviceable
//2, 3 Unserviceable
//type is used to indicate what the item is as defined in the item types enum list in buy.int
//value is a controller specific thing - for example fuel quantity or missile type
//damage is a number between 0 and 100 - where 100 is not damaged and 0 is completely knackered!
//and wil lresult in status becoming zero.
//the class is the basic class - weapons ,equipment etc.
//modifier for example sheilds strength 3 - the subclass contains 3
typedef struct
{
int status, value1;
unsigned char damage,obj_class,modifier,pad1; } kit_desc;

//items in the hold
#define MAX_HOLD_ITEMS 4
#define ORES 10
#define FOOD 11
#define TECH 12
#define LUXURY 13
#define FIREARMS 14
#define DRINKS 15
#define LIVESTOCK 16

typedef struct
{
char string [0x24];
short dofq;	  //1=tonnes, 4=grammes	
short obj_class;	  //10-ores, 11-food, 12-tech, 13-luxury/textiles, 14-firearms
short base_price_human;
short base_price_alien;
short transport_risk,pirate_risk;
short MFR; //manufacturer: 1=human, 2=alien
short markup,stock,buy_price,sell_price;
short id;	//serial number (index - 0 based)
} hold_desc;

#define SERVICEABLE 1
#define UNSERVICEABLE 2
#define LOST 0


