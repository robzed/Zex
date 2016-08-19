//aim.h
int aim_from_zex(int capture_angle,int within_distance);	//returns the object that has been targetted
int aim_from_zex_exc_obj(int object_to_exclude, int laser_cat,int within_distance);

int aim_from_zex_rear(int laser_cat,int within_distance);
int aim_from_object(int aim_from, int aim_at_category, int within_distance);
int aim_from_zex_360(int within_distance);
int aim_from_object_no_limit(int aim_from, int aim_at_category, int within_distance);
int aim_from_object_no_limit_wep(int aim_from, int aim_at_category, int within_distance);





