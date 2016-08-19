// Name:   		Damage.h
// Date Started:	10/12/98
// Author(s):		ID
//
// Copyright(c) Lightsoft Software   		

void InitDamageBar();			
void SplatBar(int damage, int x_pos, int y_pos);
void Hull_NewLife(void);
void damage_me(int damage);
void update_hull_damage();
void update_shield_damage(int shield_value);
void SplatShield(int damage, int x_pos, int y_pos);
void SplatBox(int x_pos, int y_pos, int width);
void damage_something();
void damage_me_no_sound(int damage);
void Show_Hull_NewLife(void);


#define HULL_TOTAL 50 
