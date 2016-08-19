typedef struct
{
short sx,sy;	//string pos
short lex,ley;	//line end
} spoint;

void do_inventory();
void do_inventory_single_frame(bool docked);
int do_sell_kit(int obj_class);	//draws picture and returns - speed is not an issue.
void draw_topbottom_lines();

const char* rank_to_string(int rank);
const char* rank_to_string_no_space(int rank);
