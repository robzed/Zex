// Zex - 2D Space Trading Game with 3D graphics
// Copyright (C) 1998-2005  Stuart Ball and Robert Probin
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

// buy.h
void buy(int slot_docked_at);
void setup_stock(int slot_number);	//of mothership
void load_up_stock_strings();
void do_yer_actual_buy(int stock_item_id, int screen_class);
void draw_string_with_cr(int x, int y, Str255 the_string);
int do_trading_computer(int actual_price, int stock_index);
void update_ship(int item_id);
int check_needed(int item_id);





