//camera_control.h

#define DEFAULT_CAMERA_ZOOM 13000
#define MIN_CAMERA_ZOOM 3500
#define MAX_CAMERA_ZOOM 19500

void run_camera_control(void);
void get_camera_normal(_3D *normal_here);
void reset_camera();
void camera_to_zex();
void point_camera_at(int the_slot);
void point_camera_at_coords(float x, float y);

