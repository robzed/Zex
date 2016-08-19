// buy.h
void buy(int slot_docked_at);
void setup_stock(int slot_number);	//of mothership
void load_up_stock_strings();
void do_yer_actual_buy(int stock_item_id, int screen_class);
void draw_string_with_cr(int x, int y, Str255 the_string);
int do_trading_computer(int actual_price, int stock_index);
void update_ship(int item_id);
int check_needed(int item_id);





