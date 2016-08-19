/*
 *  register_game.h
 *  Zex
 *
 *  Created by Rob Probin on 17/05/2006.
 *  Copyright 2006 Lightsoft. All rights reserved.
 *
 */

#ifndef REGISTER_GAME_H
#define REGISTER_GAME_H

void handle_register_game();
void check_game_parameters(void);			// fake name for what is a blacklist check
void check_game_param_slowly(void);			// background blacklist check

#endif // REGISTER_GAME_H

/* CVS LOG
*
* $Log: register_game.h,v $
* Revision 1.3  2006/06/25 11:16:10  rob
* Zex2005: Game has now got a blacklist list which can be used to remove rogue serial numbers flying around the internet when we update the application.
*
* Revision 1.2  2006/05/20 10:36:21  stu
* Zex2005: Code: Registration in progress
*
* Revision 1.1  2006/05/18 07:08:26  rob
* Zex2005: Menu for registration code entry.
*
*
*/
