//
// Created by F1 on 8/22/2016.
//

#include "Floor.hpp"

Floor::Floor()
{
	global_pos = Vec3(0,0,0);
	altitude = 0;
}

void Floor::clear_floor_tiles()
{
	for(int i = 0; i < BUILDING_MAX_WIDTH; i++)
	{
		for(int j = 0; j < BUILDING_MAX_LENGTH; j++)
		{
			tile_type[i][j] = TILE_TYPE_EMPT;
			tile_subtype[i][j] = 0;
			tile_variant[i][j] = 0;

			tile_branch_type[i][j] = BRANCH_TYPE_NONE;
		}
	}
}


//Populates the floor matrices reading generated type, subtypes, and variants
void Floor::populate_floor()
{
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < length; j++)
		{
			int ttype = tile_type[i][j];
			int t_subtype = tile_subtype[i][j];

			//Setting references to models and collision maps
			Grid_Tile* obj = NULL;

			//tile_object[i][j] = Global_Tiles::instance->test_tiles[ttype];
			//TODO: do this with arrays
			switch(ttype)
			{
				default:
				case TILE_TYPE_EMPT:
					obj = Global_Tiles::instance->tile_styles[0]->empty_tile;
					break;
				case TILE_TYPE_WALL:
					//Default to empty wall
					obj = Global_Tiles::instance->tile_styles[0]->empty_tile;
					if(t_subtype)
						obj = Global_Tiles::instance->tile_styles[0]->wall_subtypes[t_subtype];
					break;
				case TILE_TYPE_RAIL:
					//Default to empty wall
					obj = Global_Tiles::instance->tile_styles[0]->empty_tile;
					if(t_subtype && RAIL_TYPE_IS_VALID[t_subtype])
						obj = Global_Tiles::instance->tile_styles[0]->rail_subtypes[t_subtype];
					break;
				case TILE_TYPE_OBST:
					obj = Global_Tiles::instance->tile_styles[0]->obst_tiles[t_subtype];
					break;
			}
			tile_object[i][j] = obj;
			tile_coll_map[i][j] = obj->coll_map;
			tile_model[i][j] = obj->model;
		}
	}
}

//Recursively divides the room **stack_ptr, returns the divided rooms on the array *stack_ptr
void Floor::recursive_bsp(Room** stack_ptr,const bool horizontal_divide)
{
	//Copy the room (we write to stack_ptr, so we need a backup)
	Room rm = (**stack_ptr);

#ifdef DEBUG_RBSP
	LOGE("recursive_bsp: x(%d,%d), y(%d,%d), horizontal_divide: %d",rm.min_x,rm.max_x,rm.min_y,rm.max_y,horizontal_divide);
#endif
	//Getting the size of the room
	char size_x = rm.max_x - rm.min_x;
	char size_y = rm.max_y - rm.min_y;

	//Stop dividing if the room is small enough
	//TODO: random chance to stop dividing before we reach min size

	//We do not want rooms that are 2 tiles wide, so stop early
	if(size_x < 4 && !horizontal_divide)
		return;

	//We do not want rooms that are 2 tiles long, so stop early
	if(size_y < 4 && horizontal_divide)
		return;

	//Impossible to divide further, so stop
	if(size_x <= 2 || size_y <= 2)
		return;

	//Get the division fraction [0,1)
	float frac = Random::rand();

	Room divide_point; //maxs are maxs of room 1, mins are mins of room 2

	if(horizontal_divide)
	{
		//Division is horizontal
		//This allows divisions at least 1 tile long
		//char delta = (char)(floorf(frac * (size_y - 1)) + 1);
		//This allows divisions at least 2 tiles long
		char delta = (char)(floorf(frac * (size_y - 3)) + 2);
		divide_point = Room(rm.min_x,rm.min_y + delta, rm.max_x, rm.min_y + delta);
	}
	else
	{
		//Division is vertical
		char delta = (char)(floorf(frac * (size_x - 3)) + 2);
		divide_point = Room( rm.min_x + delta, rm.min_y, rm.min_x + delta,rm.max_y);
	}
#ifdef DEBUG_RBSP
	LOGE("Dividing room: ( x(%d,%d), y(%d,%d) ) , ( x(%d,%d), y(%d,%d) )",rm.min_x,divide_point.max_x,rm.min_y,divide_point.max_y,  divide_point.min_x,rm.max_x,divide_point.min_y,rm.max_y);
#endif

	//create two additional stacks for the two rooms we divide into, and their subdivided rooms
	Room room_stack1[(length-1) * (width-1)];
	Room* room_stack_ptr1 = room_stack1;

	//Set the first room from this new stack as the first subdivided room
	*room_stack_ptr1 = Room(rm.min_x, rm.min_y, divide_point.max_x, divide_point.max_y);

#ifdef DEBUG_RBSP
	LOGE("\t---1assigned ptr1:(%p) as x(%d,%d) y(%d,%d)",room_stack_ptr1,room_stack_ptr1->min_x,room_stack_ptr1->max_x, room_stack_ptr1->min_y,room_stack_ptr1->max_y);
#endif

	//room_stack_ptr1 is now a pointer to the first room
	recursive_bsp(&room_stack_ptr1, !horizontal_divide);
	//room_stack_ptr1 is now a pointer to one past the last room

	//Moving back stack_ptr one element, so that the code below can move it up once every iteration
	//(this only needs to be done once)
	(*stack_ptr)--;

	//Copying all of the recursively subdivided rooms to our main stack
	for(Room* ptr = room_stack1; ptr <= room_stack_ptr1; ptr++)
	{
		(*stack_ptr)++;
		//Copying the room ptr to the stack
		(**stack_ptr) = *ptr;
#ifdef DEBUG_RBSP
		LOGE("\t1assigned ptr:(%p) as x(%d,%d) y(%d,%d) (from %p)", (*stack_ptr),(*stack_ptr)->min_x,(*stack_ptr)->max_x, (*stack_ptr)->min_y,(*stack_ptr)->max_y,ptr);
		LOGE("\tincremented ptr to: (%p)",(*stack_ptr));
#endif
	}

	Room room_stack2[(length-1) * (width-1)];
	Room* room_stack_ptr2 = room_stack2;

	//Set the first room from this second new stack as the second subdivided room
	*room_stack_ptr2 = Room(divide_point.min_x, divide_point.min_y, rm.max_x, rm.max_y);

#ifdef DEBUG_RBSP
	LOGE("\t---2assigned ptr2:(%p) as x(%d,%d) y(%d,%d)", room_stack_ptr2,room_stack_ptr2->min_x,room_stack_ptr2->max_x, room_stack_ptr2->min_y,room_stack_ptr2->max_y);
#endif
	recursive_bsp(&room_stack_ptr2, !horizontal_divide);

	//Copying all of the recursively subdivided rooms to our main stack
	for(Room* ptr = room_stack2; ptr <= room_stack_ptr2; ptr++)
	{
		(*stack_ptr)++;
		//Copying the room ptr to the stack
		(**stack_ptr) = *ptr;
#ifdef DEBUG_RBSP
		LOGE("\t2assigned ptr:(%p) as x(%d,%d) y(%d,%d) (from %p)", (*stack_ptr),(*stack_ptr)->min_x,(*stack_ptr)->max_x, (*stack_ptr)->min_y,(*stack_ptr)->max_y,ptr);
		LOGE("\tincremented ptr to: (%p)",(*stack_ptr));
#endif
	}
}

//Returns true if min <= x <= max
inline bool Floor::in_bounds(const char x,const char min,const char max)
{
	return (x >= min) && (x <= max);
}

//Returns true if a == b == c == d
inline bool Floor::all_equal(const char a,const char b,const char c,const char d)
{
	return (a == b) && (b == c) && (a == d);
}
//Checks if w1 is completely inside of w2
//0 means walls are not inside each other
//1 means w1 is inside of w2
//2 means w2 is inisde of w1
char Floor::is_wall_in_wall(const Wall* w1,const Wall* w2)
{
	//Check if walls are both horizontal and on the same y-coordinate
	if(all_equal(w1->y1,w1->y2,w2->y1,w2->y2))
	{
		//Check if w1 is inside of w2
		//If w1's left vertex is inside of w2's bounds
		if(in_bounds(w1->x1,w2->x1,w2->x2))
		{
			//If w1's right vertex is inside of w2's bounds
			if(in_bounds(w1->x2,w2->x1,w2->x2))
			{
				//w1 is inside of w2
				return 1;
			}
		}
		//Check if w2 is inside of w1
		//If w2's left vertex is inside of w1's bounds
		if(in_bounds(w2->x1,w1->x1,w1->x2))
		{
			//If w2's right vertex is inside of w1's bounds
			if(in_bounds(w2->x2,w1->x1,w1->x2))
			{
				//w2 is inside of w1
				return 2;
			}
		}
	}
	//Check if walls are both vertical and on the same x-coordinate
	else if(all_equal(w1->x1,w1->x2,w2->x1,w2->x2))
	{
		//Check if w1 is inside of w2
		//If w1's left vertex is inside of w2's bounds
		if(in_bounds(w1->y1,w2->y1,w2->y2))
		{
			//If w1's right vertex is inside of w2's bounds
			if(in_bounds(w1->y2,w2->y1,w2->y2))
			{
				//w1 is inside of w2
				return 1;
			}
		}
		//Check if w2 is inside of w1
		//If w2's left vertex is inside of w1's bounds
		if(in_bounds(w2->y1,w1->y1,w1->y2))
		{
			//If w2's right vertex is inside of w1's bounds
			if(in_bounds(w2->y2,w1->y1,w1->y2))
			{
				//w2 is inside of w1
				return 2;
			}
		}
	}
	//Walls are not inside of each other
	return 0;
}

//Adds wall to list if w is not in any wall on the list
//Replaces a wall in the list if w is completely inside of the wall and smaller
//Does not add w to the list if there is a wall in the list completely inside w and smaller than w
//Does not add w to the list if the wall makes up the building exterior
//Returns 0 if wall was not added, returns 1 otherwise
int Floor::add_wall_to_list(const Wall* w, Wall* list_start, Wall** list_end)
{
	//Checking if wall lies on the building exterior boundary
	if(w->x1 == w->x2 && (w->x1 == 0 || w->x1 == width-1))
	{
		return 0;
	}
	if(w->y1 == w->y2 && (w->y1 == 0 || w->y1 == length-1))
	{
		return 0;
	}


	char res;
	for(Wall* ptr = list_start; ptr < *list_end; ptr++)
	{
		res = is_wall_in_wall(w,ptr);
		if(!res)
			continue;
		if(res == 2)//ptr is in w
		{
			return 1;
		}
		if(res == 1)//w is in ptr
		{
			//Assign ptr as wall w
			*ptr = *w;
			return 1;
		}
	}
	//w is not in the list.
	**list_end = *w;
	(*list_end)++;
	return 1;
}

//Returns unique walls given a list of rooms
//Where rooms is a pointer to Rooms, last_room is a pointer to the last room
//Walls is a pointer to an array of enough walls
void Floor::get_unique_walls(Wall** wall_ptr, Room* rooms,const Room* last_room)
{
	Wall* first_wall = *wall_ptr;
	Wall* last_wall = *wall_ptr;

	Wall w;
	for(Room* ptr = rooms; ptr <= last_room; ptr++)
	{
		w = ptr->get_north_wall();
		add_wall_to_list(&w, first_wall, &last_wall);
		w = ptr->get_east_wall();
		add_wall_to_list(&w, first_wall, &last_wall);
		w = ptr->get_south_wall();
		add_wall_to_list(&w, first_wall, &last_wall);
		w = ptr->get_west_wall();
		add_wall_to_list(&w, first_wall, &last_wall);
	}
	*wall_ptr = last_wall;
}

void Floor::set_vert_wall_tiles(Wall* w)
{
	//Bottom-most tile
	tile_type[w->x1][w->y1] = TILE_TYPE_WALL;
	tile_subtype[w->x1][w->y1] |= WALL_TYPE_oooY;
	if(w->y1 == 0)
		tile_subtype[w->x1][w->y1] |= WALL_TYPE_ooyo;

	//Top-most tile
	tile_type[w->x1][w->y2] = TILE_TYPE_WALL;
	tile_subtype[w->x1][w->y2] |= WALL_TYPE_ooyo;
	if(w->y2 == length - 1)
		tile_subtype[w->x1][w->y2] |= WALL_TYPE_oooY;

	//Every tile in between first and last
	for(int i = w->y1+1; i < w->y2; i++)
	{
		tile_type[w->x1][i] = TILE_TYPE_WALL;
		tile_subtype[w->x1][i] |= WALL_TYPE_ooyY;
	}
}

void Floor::set_hor_wall_tiles(Wall* w)
{
	//Left-most tile
	tile_type[w->x1][w->y1] = TILE_TYPE_WALL;
	tile_subtype[w->x1][w->y1] |= WALL_TYPE_oXoo;
	if(w->x1 == 0)
		tile_subtype[w->x1][w->y1] |= WALL_TYPE_xooo;

	//Right-most tile
	tile_type[w->x2][w->y1] = TILE_TYPE_WALL;
	tile_subtype[w->x2][w->y1] |= WALL_TYPE_xooo;
	if(w->x2 == width - 1)
		tile_subtype[w->x2][w->y1] |= WALL_TYPE_oXoo;

	//Every tile in between first and last
	for(int i = w->x1+1; i < w->x2; i++)
	{
		tile_type[i][w->y1] = TILE_TYPE_WALL;
		tile_subtype[i][w->y1] |= WALL_TYPE_xXoo;
	}
}

//Branches left from tile[tile_x][tile_y]
void Floor::recursive_branch_left(int tile_x, int tile_y)
{
	tile_branch_type[tile_x][tile_y] |= BRANCH_TYPE_LEFT;
	tile_branch_type[tile_x-1][tile_y] |= (BRANCH_TYPE_FROM_RIGHT | BRANCH_TYPE_FORWARD);
	tile_branch_type[tile_x-1][tile_y+1] |= BRANCH_TYPE_FROM_FORWARD;

	//If we branched through a vertical wall, remove the wall segments
	if(tile_type[tile_x-1][tile_y+1] == TILE_TYPE_WALL)
	{
		tile_subtype[tile_x-1][tile_y+1] &= ~WALL_TYPE_ooyo;
		if(!tile_subtype[tile_x-1][tile_y+1])
			tile_type[tile_x-1][tile_y+1] = TILE_TYPE_EMPT;
	}
	if(tile_type[tile_x-1][tile_y] == TILE_TYPE_WALL)
	{
		tile_subtype[tile_x-1][tile_y] &= ~WALL_TYPE_oooY;
		if(!tile_subtype[tile_x-1][tile_y])
			tile_type[tile_x-1][tile_y] = TILE_TYPE_EMPT;
	}

	recursive_branch_player_path(tile_x-1,tile_y+1,BRANCH_TYPE_LEFT);
}

//Branches right from tile[tile_x][tile_y]
void Floor::recursive_branch_right(int tile_x, int tile_y)
{
	tile_branch_type[tile_x][tile_y] |= BRANCH_TYPE_RIGHT;
	tile_branch_type[tile_x+1][tile_y] |= (BRANCH_TYPE_FROM_LEFT | BRANCH_TYPE_FORWARD);
	tile_branch_type[tile_x+1][tile_y+1] |= BRANCH_TYPE_FROM_FORWARD;

	//If we branched through a vertical wall, remove the wall segments
	if(tile_type[tile_x+1][tile_y+1] == TILE_TYPE_WALL)
	{
		tile_subtype[tile_x+1][tile_y+1] &= ~WALL_TYPE_ooyo;
		if(!tile_subtype[tile_x+1][tile_y+1])
			tile_type[tile_x+1][tile_y+1] = TILE_TYPE_EMPT;
	}
	if(tile_type[tile_x+1][tile_y] == TILE_TYPE_WALL)
	{
		tile_subtype[tile_x+1][tile_y] &= ~WALL_TYPE_oooY;
		if(!tile_subtype[tile_x+1][tile_y])
			tile_type[tile_x+1][tile_y] = TILE_TYPE_EMPT;
	}

	recursive_branch_player_path(tile_x+1,tile_y+1,BRANCH_TYPE_RIGHT);
}

//Branches forward from tile[tile_x][tile_y]
void Floor::recursive_branch_forward(int tile_x, int tile_y)
{
	tile_branch_type[tile_x][tile_y] |= BRANCH_TYPE_FORWARD;
	tile_branch_type[tile_x][tile_y + 1] |= BRANCH_TYPE_FROM_FORWARD;
	recursive_branch_player_path(tile_x,tile_y + 1,BRANCH_TYPE_FORWARD);
}

//Returns probability that we need to branch towards goal_column
//Returns 0.0f - 1.0f, or any value > 1.0f if it's impossible to get to the goal tile
float Floor::prob_of_branch_to_goal(int tile_x, int tile_y)
{
	int goal_column = clamp(tile_x, goal_min_column, goal_max_column);
	//If the tile is already in the goal range:
	if(tile_x == goal_column)
		return 0.0f;

	//If the tile is past the last tile we can branch left/right from (also protects from division by zero)
	if((length - tile_y - 2) <= 0)
		return 2.0f;

	//If the tile is to the left of goal_column
	if(tile_x < goal_column)
	{
		return ((float)(goal_column - tile_x)) / ((length - tile_y) - 2);
	}
	//the tile is to the left of goal_column
	else
	{
		return ((float)(tile_x - goal_column)) / ((length - tile_y) - 2);
	}
}

//#define DEBUG_BRANCH_LOGIC

// General layout of the logic of this recursive method
	//============
	//1: Check if we're at the end of the building, or if this tile has already branched (terminating case)
	//============
	//2: Perform some tests that prohibit us from branching in certain directions
		//These are tests that cannot be passed with modifications to the wall layout
	//============
	//3: Perform additional tests that halt us from branching
		//These are tests that can be passed if we modify the wall layout
	//============
	//4: Calculate probabilities that we must branch left or right in order to get between gmin_x and gmax_x
	//============
	//5: Finally, analyze what directions we can still branch in, and perform the recursive branches
	//============
	//6: if we cannot branch, modify the wall layout in order to pass one of the failed tests from section 3
	//============
void Floor::recursive_branch_player_path(int tile_x, int tile_y, int prev_branch)
{
#ifdef DEBUG_BRANCH_LOGIC
	LOGE("Gen Branch on tile: [%d][%d]. Wall: %d, walltype: (%d,%d,%d,%d)",
		tile_x,tile_y,tile_type[tile_x][tile_y],
		(tile_subtype[tile_x][tile_y] & WALL_TYPE_xooo)/WALL_TYPE_xooo,
		(tile_subtype[tile_x][tile_y] & WALL_TYPE_oXoo)/WALL_TYPE_oXoo,
		(tile_subtype[tile_x][tile_y] & WALL_TYPE_ooyo)/WALL_TYPE_ooyo,
		(tile_subtype[tile_x][tile_y] & WALL_TYPE_oooY)/WALL_TYPE_oooY);
#endif

	//We have somehow reached this tile, regardless of whatever is on this tile: remove the vertical wall segments
	if(tile_type[tile_x][tile_y] == TILE_TYPE_WALL)
	{
		tile_subtype[tile_x][tile_y] &= ~WALL_TYPE_ooyY;

		//If we have xooo or oXoo but not both, remove it
		if(tile_subtype[tile_x][tile_y] != WALL_TYPE_xXoo)
		{
			tile_subtype[tile_x][tile_y] = 0;
		}

		if(!tile_subtype[tile_x][tile_y])
			tile_type[tile_x][tile_y] = TILE_TYPE_EMPT;

	}
	//Have we reached the end of the building?
	if(tile_y >= length - 1)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("End of building reached at tile[%d][%d]",tile_x,tile_y);
#endif
		tile_branch_type[tile_x][tile_y] |= (BRANCH_TYPE_FROM_FORWARD | BRANCH_TYPE_FORWARD);
		return;
	}

	//Storing as a pointer, because called functions may modify the value
	int *branched = &tile_branch_type[tile_x][tile_y];

	//If we have already branched anywhere from this tile, stop
		//WARNING: this may lead to undesired results, maybe just check left/right/forward individually
	if(*branched & (BRANCH_TYPE_LEFT | BRANCH_TYPE_RIGHT | BRANCH_TYPE_FORWARD))
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("We have already branched at tile[%d][%d]",tile_x,tile_y);
#endif
		return;
	}



	//Knowing what direction we cannot branch to will inform of us of what directions we must branch to.
	//We begin by assuming we can branch in all directions.
	unsigned int can_branch = (BRANCH_TYPE_FORWARD | BRANCH_TYPE_LEFT | BRANCH_TYPE_RIGHT);

	//========= Tests that cannot be broken ===========
	//Checking if branching left or branching right will put us out of bounds
	if(tile_x <= 0)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Cannot branch left at tile[%d][%d]",tile_x,tile_y);
#endif
		can_branch &= ~BRANCH_TYPE_LEFT;
	}
	if(tile_x >= width - 1)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Cannot branch right at tile[%d][%d]",tile_x,tile_y);
#endif
		can_branch &= ~BRANCH_TYPE_RIGHT;
	}

	//Checking if the end tile of a left branch can still get us to our goal range
	if(prob_of_branch_to_goal(tile_x-1,tile_y+1) > 1.0f)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Cannot branch left at tile[%d][%d] (could not get to goal_column from branch)",tile_x,tile_y);
#endif
		can_branch &= ~BRANCH_TYPE_LEFT;
	}
	//Checking if the end tile of a right branch can still get us to our goal range
	if(prob_of_branch_to_goal(tile_x+1,tile_y+1) > 1.0f)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Cannot branch right at tile[%d][%d] (could not get to goal_column from branch)",tile_x,tile_y);
#endif
		can_branch &= ~BRANCH_TYPE_RIGHT;
	}

	//========== Tests that can be broken (we can change the wall layout to pass the test) ========

	bool failed_breakable_tests[7] = {false,false,false,false,false,false,false};
	//Do not branch left if the next tile is a wall (no room to handle obstacle)
	if(tile_x > 0 && tile_y < length - 1)
	{
		if((tile_type[tile_x-1][tile_y+1] == TILE_TYPE_WALL) && (tile_subtype[tile_x-1][tile_y+1] & WALL_TYPE_xXoo))
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("Cannot branch left (there is an hwall immediately after branch) at tile[%d][%d]",tile_x,tile_y);
#endif
			can_branch &= ~BRANCH_TYPE_LEFT;
			failed_breakable_tests[0] = true;
		}
	}
	//Do not branch right if the next tile is a wall (no room to handle obstacle)
	if(tile_x < width - 1 && tile_y < length - 1)
	{
		if((tile_type[tile_x+1][tile_y+1] == TILE_TYPE_WALL) && (tile_subtype[tile_x+1][tile_y+1] & WALL_TYPE_xXoo))
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("Cannot branch right (there is an hwall immediately after branch) at tile[%d][%d]",tile_x,tile_y);
#endif
			can_branch &= ~BRANCH_TYPE_RIGHT;
			failed_breakable_tests[1] = true;
		}
	}

	//Checking if we will branch into a ooyo wall tile which will stop us from being able to branch forward
	if(tile_y < length - 2)
	{
		//Is the tile a ooyo or oooY wall?
		if((tile_type[tile_x][tile_y+2] == TILE_TYPE_WALL) && (tile_subtype[tile_x][tile_y+2] & WALL_TYPE_ooyo))
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("Cannot branch forward (there is a vwall 2 tiles ahead) at tile[%d][%d]",tile_x,tile_y);
#endif
			can_branch &= ~BRANCH_TYPE_FORWARD;
			failed_breakable_tests[2] = true;
		}
	}
	//Checking if the next tile is a ooyY wall tile which does not allow us to branch forward
	if(tile_y < length - 1)
	{
		//Is the tile a ooyo or oooY wall?
		if((tile_type[tile_x][tile_y+1] == TILE_TYPE_WALL) && (tile_subtype[tile_x][tile_y+1] & WALL_TYPE_ooyY))
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("Cannot branch forward (next tile is a vwall ahead) at tile[%d][%d]",tile_x,tile_y);
#endif
			can_branch &= ~BRANCH_TYPE_FORWARD;
			failed_breakable_tests[3] = true;
		}
	}

	//We cannot branch left or right on a horizontal wall tile
	if((tile_type[tile_x][tile_y] == TILE_TYPE_WALL) && (tile_subtype[tile_x][tile_y] & WALL_TYPE_xXoo))
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Cannot branch left or right (we are on a hwall) at tile[%d][%d]",tile_x,tile_y);
#endif
		can_branch &= ~(BRANCH_TYPE_LEFT | BRANCH_TYPE_RIGHT);
		failed_breakable_tests[4] = true;
	}

	//We cannot branch left if we branch across a horizontal wall tile
	if((tile_x > 0) && (tile_type[tile_x-1][tile_y] == TILE_TYPE_WALL) && (tile_subtype[tile_x-1][tile_y] & WALL_TYPE_oXoo))
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Cannot branch left (branch across an hwall) at tile[%d][%d]",tile_x,tile_y);
#endif
		can_branch &= ~(BRANCH_TYPE_LEFT);
		failed_breakable_tests[5] = true;
	}
	//We cannot branch right if we branch across a horizontal wall tile
	if((tile_x < width -1) && (tile_type[tile_x+1][tile_y] == TILE_TYPE_WALL) && (tile_subtype[tile_x+1][tile_y] & WALL_TYPE_xooo))
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Cannot branch right (branch across an hwall) at tile[%d][%d]",tile_x,tile_y);
#endif
		can_branch &= ~(BRANCH_TYPE_RIGHT);
		failed_breakable_tests[6] = true;
	}

#ifdef DEBUG_BRANCH_LOGIC
	LOGE("Branch tests finished at tile[%d][%d]: (%d) L:%d, R:%d, F:%d",tile_x,tile_y,can_branch,
		(can_branch & BRANCH_TYPE_LEFT)/BRANCH_TYPE_LEFT,
		(can_branch & BRANCH_TYPE_RIGHT)/BRANCH_TYPE_RIGHT,
		(can_branch & BRANCH_TYPE_FORWARD)/BRANCH_TYPE_FORWARD);
#endif

	//================================================================================================================
	//This next section modulates probabilities of branching based on certain requirements
	//================================================================================================================

#ifdef DEBUG_BRANCH_LOGIC
	LOGE("Modulating branch probabilities");
#endif

	float mod_prob_l = prob_of_branch_left;
	float mod_prob_r = prob_of_branch_right;
	float mod_prob_f = prob_of_branch_forward;

	//If we previously branched left, decrease the prob of branching right
	if(prev_branch == BRANCH_TYPE_LEFT)
	{
		mod_prob_r *= prob_of_branch_right_given_branch_left;
	}
	//If we previously branched right, decrease the prob of branching left
	if(prev_branch == BRANCH_TYPE_RIGHT)
	{
		mod_prob_l *= prob_of_branch_left_given_branch_right;
	}

#ifdef DEBUG_BRANCH_LOGIC
	LOGE("Modulating branch probabilities 1");
#endif

	//Calculations for ensuring that we branch the player towards somewhere between gmin_x and gmax_x
	//These are the tiles that line up with the next building
	//Therefore, the player needs to end up on these tiles to be able to jump to the next building
	float prob_of_force_branch;
	//Getting the nearest column to the one we're on that is in the goal range [gmin_x,gmax_x]
	int goal_column = clamp(tile_x,goal_min_column,goal_max_column);

#ifdef DEBUG_BRANCH_LOGIC
	LOGE("Modulating branch probabilities 2");
#endif

	//We are to the left of the goal column (branch is right)
	if(tile_x < goal_column)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("tile_x < goal_column");
#endif

		prob_of_force_branch = prob_of_branch_to_goal(tile_x,tile_y);
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Prob of branching to column %d is %.2f",goal_column,prob_of_force_branch);
#endif

		//Try to branch
		if(Random::rand() < prob_of_force_branch)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("Trying to branch right...");
#endif
			if(can_branch & BRANCH_TYPE_RIGHT)
			{
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Branched right from force prob at tile[%d][%d]!",tile_x,tile_y);
#endif
				//do the branch & return
				recursive_branch_right(tile_x,tile_y);
				return;
			}
			//If we could not branch, try to force the branch
			if(Random::rand() < prob_of_force_branch)
			{
				//force the branch & return
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Attempting to force a branch to the right.");
#endif
				can_branch &= ~(BRANCH_TYPE_FORWARD | BRANCH_TYPE_LEFT);
				//The following flow of control will be executed:
					//We won't be able to branch anywhere
					//That will fix one of the failed tests that disallowed us to branch
					//Then this method will be called again
					//If our Random::rand() is once again less than prob_of_force_branch
					//this continues until we fix whatever caused us not to be able to branch
			}
		}
	}
	//We are to the right of the goal column (branch is left)
	if(tile_x > goal_column)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("tile_x > goal_column");
#endif
		prob_of_force_branch = prob_of_branch_to_goal(tile_x,tile_y);
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Prob of branching to column %d is %.2f",goal_column,prob_of_force_branch);
#endif

		//Try to branch
		if(Random::rand() < prob_of_force_branch)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("Trying to branch left...");
#endif
			if(can_branch & BRANCH_TYPE_LEFT)
			{
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Branched left from force prob at tile[%d][%d]!",tile_x,tile_y);
#endif
				//do the branch & return
				recursive_branch_left(tile_x,tile_y);
				return;
			}
			//If we could not branch, try to force the branch
			if(Random::rand() < prob_of_force_branch)
			{
				//force the branch & return
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Attempting to force a branch to the left.");
#endif
				can_branch &= ~(BRANCH_TYPE_FORWARD | BRANCH_TYPE_RIGHT);
				//The following flow of control will be executed:
					//We won't be able to branch anywhere
					//That will fix one of the failed tests that disallowed us to branch
					//Then this method will be called again
					//If our Random::rand() is once again less than prob_of_force_branch
					//this continues until we fix whatever caused us not to be able to branch
			}
		}
	}

	//Depending on what directions we can branch to, we are put in 8 distinct categories:
	//We can branch in any direction: regular branching logic ============================================
	//====================================================================================================
	if(can_branch == (BRANCH_TYPE_FORWARD | BRANCH_TYPE_LEFT | BRANCH_TYPE_RIGHT))
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Can branch in all 3 directions at tile[%d][%d]",tile_x,tile_y);
#endif
		bool branch_l = false;
		bool branch_r = false;
		bool branch_f = false;

		//Probability of branching forward given we choose left, right, or forward
		float prob_branch_f = mod_prob_f / (mod_prob_f + mod_prob_l + mod_prob_r);
		float prob_branch_l = mod_prob_l / (mod_prob_f + mod_prob_l + mod_prob_r);
		float prob_branch_r = mod_prob_r / (mod_prob_f + mod_prob_l + mod_prob_r);

		//Probability of branching left/right given we choose left or right
		float prob_branch_l_lr = mod_prob_l / (mod_prob_l + mod_prob_r);
		float prob_branch_r_lr = mod_prob_r / (mod_prob_l + mod_prob_r);

		if(Random::rand() < prob_branch_f)
		{
			branch_f = true;
			if(Random::rand() < prob_branch_l)
				branch_l = true;
			if(Random::rand() < prob_branch_r)
				branch_r = true;
		}
		else if(Random::rand() < prob_branch_l_lr)
			{
				branch_l = true;
				if(Random::rand() < prob_branch_r_lr)
					branch_r = true;
			}
			else
			{
				branch_r = true;
			}

		if(branch_l)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branched left at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_left(tile_x,tile_y);
		}
		if(branch_r)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branched right at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_right(tile_x,tile_y);
		}
		if(branch_f)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branched forward at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_forward(tile_x,tile_y);
		}

	}
	//====================================================================================================================
	//We can only branch forward -> must branch forward
	//====================================================================================================================
	if(can_branch == BRANCH_TYPE_FORWARD)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Must branch forward at tile[%d][%d]",tile_x,tile_y);
#endif
		recursive_branch_forward(tile_x,tile_y);
	}
	//====================================================================================================================
	//We can only branch left -> must branch left
	//====================================================================================================================
	if(can_branch == BRANCH_TYPE_LEFT)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Must branch left at tile[%d][%d]",tile_x,tile_y);
#endif
		recursive_branch_left(tile_x,tile_y);
	}
	//====================================================================================================================
	//We can only branch right -> must branch right
	//====================================================================================================================
	if(can_branch == BRANCH_TYPE_RIGHT)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Must branch right at tile[%d][%d]",tile_x,tile_y);
#endif
		recursive_branch_right(tile_x,tile_y);
	}
	//====================================================================================================================
	//We can only branch left or right -> choose one or both randomly
	//====================================================================================================================
	if(can_branch == (BRANCH_TYPE_LEFT | BRANCH_TYPE_RIGHT))
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Must branch left or right at tile[%d][%d]",tile_x,tile_y);
#endif
		bool branch_left = false;
		bool branch_right = false;

		//Calculating accurate probabilities of branching left / right / both
		float prob_l = mod_prob_l / (mod_prob_l + mod_prob_r);
		float prob_r = mod_prob_r / (mod_prob_l + mod_prob_r);

		//50% chance we branch left or branch right
		if(Random::rand() < prob_l)
		{
			branch_left = true;
			//Branch right as well?
			if(Random::rand() < prob_r)
			{
				branch_right = true;
			}
		}
		else
		{
			branch_right = true;
		}

		if(branch_left)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branching left at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_left(tile_x,tile_y);
		}
		if(branch_right)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branching right at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_right(tile_x,tile_y);
		}
	}
	//====================================================================================================================
	//We can only branch forward or left -> choose one or both randomly
	//====================================================================================================================
	if(can_branch == (BRANCH_TYPE_LEFT | BRANCH_TYPE_FORWARD))
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Must branch left or forward at tile[%d][%d]",tile_x,tile_y);
#endif
		bool branch_left = false;
		bool branch_forward = false;
		//Calculating accurate probabilities of branching forward / left / both
		float prob_l = mod_prob_l / (mod_prob_l + mod_prob_f);
		float prob_f = mod_prob_f / (mod_prob_l + mod_prob_f);

		if(Random::rand() < prob_l)
		{
			branch_left = true;
			//branch forward as well?
			if(Random::rand() < prob_f)
			{
				branch_forward = true;
			}
		}
		else
		{
			branch_forward = true;
		}

		if(branch_left)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branching left at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_left(tile_x,tile_y);
		}
		if(branch_forward)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branching forward at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_forward(tile_x,tile_y);
		}
	}
	//====================================================================================================================
	//We can only branch forward or right -> choose one or both randomly
	//====================================================================================================================
	if(can_branch == (BRANCH_TYPE_RIGHT | BRANCH_TYPE_FORWARD))
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Must branch right or forward at tile[%d][%d]",tile_x,tile_y);
#endif
		bool branch_right = false;
		bool branch_forward = false;
		//Calculating accurate probabilities of branching forward / left / both
		float prob_r = mod_prob_r / (mod_prob_r + mod_prob_f);
		float prob_f = mod_prob_f / (mod_prob_r + mod_prob_f);

		if(Random::rand() < prob_r)
		{
			branch_right = true;
			//branch forward as well?
			if(Random::rand() < prob_f)
			{
				branch_forward = true;
			}
		}
		else
		{
			branch_forward = true;
		}

		if(branch_right)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branching right at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_right(tile_x,tile_y);
		}
		if(branch_forward)
		{
#ifdef DEBUG_BRANCH_LOGIC
			LOGE("branching forward at tile[%d][%d]",tile_x,tile_y);
#endif
			recursive_branch_forward(tile_x,tile_y);
		}
	}
	//====================================================================================================================
	//We cannot branch anywhere.
	//This is a contradiction that we have backed ourselves into a corner with
	//If this occurs, we should probably return a failed state up the recursion ladder and back up, trying a different path
	//====================================================================================================================
	if(can_branch == BRANCH_TYPE_NONE)
	{
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Warning: we cannot branch anywhere from tile[%d][%d]",tile_x,tile_y);
		LOGE("Failed Breakable Tests: [%d,%d,%d,%d,%d,%d,%d]",
			failed_breakable_tests[0],
			failed_breakable_tests[1],
			failed_breakable_tests[2],
			failed_breakable_tests[3],
			failed_breakable_tests[4],
			failed_breakable_tests[5],
			failed_breakable_tests[6]);
#endif
		//Our current solution: keep track of every test we failed which made us not be able to branch anywhere
		//Choose a random rule to break, and modify the floor tile layout to reflect the update, and call recursive bsp at this same tile

		//Count the number of failed breakable tests
		int failed_tests = 0;
		for(int i = 0; i < 7; i++)
		{
			if(failed_breakable_tests[i])
				failed_tests++;
		}
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("We failed %d tests",failed_tests);
#endif
		if(failed_tests == 0)
		{
			LOGE("Warning: we ran into a dead end with 0 tests failed at tile:[%d][%d]",tile_x,tile_y);
			return;
		}
		//Choosing a random failed test to break
		int test_to_break = Random::rand_int_in_range(0,failed_tests);
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("We choose to break test number %d",test_to_break);
#endif
		int test_to_break_index = test_to_break;
		//Getting the index of the test to break
		for(int i = 0; i < 7; i++)
		{
			if(failed_breakable_tests[i])
			{
				if(test_to_break_index == 0)
				{
					test_to_break_index = i;
					break;
				}
				test_to_break_index--;
			}
		}
#ifdef DEBUG_BRANCH_LOGIC
		LOGE("Test number %d has the index %d",test_to_break,test_to_break_index);
#endif

		switch(test_to_break_index)
		{
			//Do not branch left if the next tile is a wall (no room to handle obstacle)
			case 0:
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Removing horizontal wall from left branch");
#endif
				//We will remove the horizontal wall from where we end up if we branch left
				tile_type[tile_x-1][tile_y+1] = TILE_TYPE_EMPT;
				tile_subtype[tile_x-1][tile_y+1] = 0;
				//Removing connecting component from the wall to the left of it
				if((tile_x-1 > 0) && (tile_type[tile_x-2][tile_y+1] == TILE_TYPE_WALL))
				{
					tile_subtype[tile_x-2][tile_y+1] &= ~WALL_TYPE_oXoo;
					if(!tile_subtype[tile_x-2][tile_y+1])
						tile_type[tile_x-2][tile_y+1] = TILE_TYPE_EMPT;
				}
				//Removing connecting component from the wall to the right of it
				if(tile_type[tile_x][tile_y+1] == TILE_TYPE_WALL)
				{
					tile_subtype[tile_x][tile_y+1] &= ~WALL_TYPE_xooo;
					if(!tile_subtype[tile_x][tile_y+1])
						tile_type[tile_x][tile_y+1] = TILE_TYPE_EMPT;
				}
				break;
			//Do not branch right if the next tile is a wall (no room to handle obstacle)
			case 1:
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Removing horizontal wall from right branch");
#endif
				//We will remove the horizontal wall from where we end up if we branch right
				tile_type[tile_x+1][tile_y+1] = TILE_TYPE_EMPT;
				tile_subtype[tile_x+1][tile_y+1] = 0;
				//Removing connecting component from the wall to the left of it
				if(tile_type[tile_x][tile_y+1] == TILE_TYPE_WALL)
				{
					tile_subtype[tile_x][tile_y+1] &= ~WALL_TYPE_oXoo;
					if(!tile_subtype[tile_x][tile_y+1])
						tile_type[tile_x][tile_y+1] = TILE_TYPE_EMPT;
				}
				//Removing connecting component from the wall to the right of it
				if((tile_x+1 < width-1)&&(tile_type[tile_x+2][tile_y+1] == TILE_TYPE_WALL))
				{
					tile_subtype[tile_x+2][tile_y+1] &= ~WALL_TYPE_xooo;
					if(!tile_subtype[tile_x+2][tile_y+1])
						tile_type[tile_x+2][tile_y+1] = TILE_TYPE_EMPT;
				}
				break;
			//Checking if we will branch into a ooyo wall tile which will stop us from being able to branch forward
			case 2:
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Removing ooyo component from tile 2 tiles ahead");
#endif
				//We will remove the offending ooyo component from the tile
				tile_subtype[tile_x][tile_y+2] &= ~WALL_TYPE_ooyo;
				if(!tile_subtype[tile_x][tile_y+2])
					tile_type[tile_x][tile_y+2] = TILE_TYPE_EMPT;
				//Removing connecting segment from the tile before it
				if(tile_type[tile_x][tile_y+1] == TILE_TYPE_WALL)
				{
					tile_subtype[tile_x][tile_y+1] &= ~WALL_TYPE_oooY;
					if(!tile_subtype[tile_x][tile_y+1])
						tile_type[tile_x][tile_y+1] = TILE_TYPE_EMPT;
				}
				break;
			//Checking if the next tile is a ooyY wall tile which does not allow us to branch forward
			case 3:
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Removing ooyY component from the next tile");
#endif
				//We will remove the offending ooyo component from the tile
				tile_subtype[tile_x][tile_y+1] &= ~WALL_TYPE_ooyY;
				if(!tile_subtype[tile_x][tile_y+1])
					tile_type[tile_x][tile_y+1] = TILE_TYPE_EMPT;
				//Removing connecting segment from the tile after it
				if((tile_y < length-1)&&(tile_type[tile_x][tile_y+1] == TILE_TYPE_WALL))
				{
					tile_subtype[tile_x][tile_y+2] &= ~WALL_TYPE_ooyo;
					if(!tile_subtype[tile_x][tile_y+2])
						tile_type[tile_x][tile_y+2] = TILE_TYPE_EMPT;
				}
				break;
			//We cannot branch left or right on a horizontal wall tile
			case 4:
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Removing xXoo component from the next tile");
#endif
				//We will remove the horizontal wall tile
				tile_type[tile_x][tile_y] = TILE_TYPE_EMPT;
				tile_subtype[tile_x][tile_y] = 0;
				//Removing connecting component from the wall to the left of it
				if((tile_x > 0) && (tile_type[tile_x-1][tile_y] == TILE_TYPE_WALL))
				{
					tile_subtype[tile_x-1][tile_y] &= ~WALL_TYPE_oXoo;
					if(!tile_subtype[tile_x-1][tile_y])
						tile_type[tile_x-1][tile_y] = TILE_TYPE_EMPT;
				}
				//Removing connecting component from the wall to the right of it
				if(tile_type[tile_x+1][tile_y] == TILE_TYPE_WALL)
				{
					tile_subtype[tile_x+1][tile_y] &= ~WALL_TYPE_xooo;
					if(!tile_subtype[tile_x+1][tile_y])
						tile_type[tile_x+1][tile_y] = TILE_TYPE_EMPT;
				}
				break;
			//We cannot branch left if we branch across a horizontal wall tile
			case 5:
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Removing oXoo component from tile we branch left across");
#endif
				tile_subtype[tile_x-1][tile_y] &= ~WALL_TYPE_oXoo;
				if(!tile_subtype[tile_x-1][tile_y])
					tile_type[tile_x-1][tile_y] = TILE_TYPE_EMPT;
				break;
			//We cannot branch right if we branch across a horizontal wall tile
			case 6:
#ifdef DEBUG_BRANCH_LOGIC
				LOGE("Removing xooo component from tile we branch right across");
#endif
				tile_subtype[tile_x+1][tile_y] &= ~WALL_TYPE_xooo;
				if(!tile_subtype[tile_x+1][tile_y])
					tile_type[tile_x+1][tile_y] = TILE_TYPE_EMPT;
				break;

			default:
				break;
		}
		//Now that we've broken the test that failed, retry branch player_path
		recursive_branch_player_path(tile_x,tile_y,prev_branch);
	}
	//====================================================================================================================
}
//Sets the tile at [x][y] as a rail tile
//If the tile is not a rail, sets it's subtype as rail_type
//If the tile was already a rail, adds rail_type to it's subtype
void Floor::set_rail_type(int x, int y, int rail_type)
{
	//This exit condition not replace any walls with rails
	//if(tile_type[x][y] == TILE_TYPE_WALL && tile_subtype[x][y])
	//	return;

	//If the tile is a wall
	if(tile_type[x][y] == TILE_TYPE_WALL)
	{
		//If we are placing a L or R rail
		if(rail_type & (RAIL_TYPE_L | RAIL_TYPE_R))
		{
			//If the wall has both ooyo and oooY components
			if((tile_subtype[x][y] & WALL_TYPE_ooyo) && (tile_subtype[x][y] & WALL_TYPE_oooY))
			{
				//Do not place a rail (not necessary)
				return;
			}
		}
		if(rail_type == RAIL_TYPE_L)
		{
			//If the wall has a oXoo component
			if(tile_subtype[x][y] & WALL_TYPE_oXoo)
				return;
		}
		if(rail_type == RAIL_TYPE_R)
		{
			//If the wall has a xooo component
			if(tile_subtype[x][y] & WALL_TYPE_xooo)
				return;
		}
	}

	int prev_type = RAIL_TYPE_NONE;
	if(tile_type[x][y] == TILE_TYPE_RAIL)
	{
		prev_type = tile_subtype[x][y];
	}
	prev_type |= rail_type;

	//If the new type is not a valid type, don't change anything
	if(!RAIL_TYPE_IS_VALID[prev_type])
		return;

	tile_type[x][y] = TILE_TYPE_RAIL;
	tile_subtype[x][y] = prev_type;
}

void Floor::place_rail_tiles()
{
	int btype;
	int btype2;

	//Reading tile_branch_type matrix, and setting the appropriate tiles as rails
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < length; j++)
		{
			btype = tile_branch_type[i][j];

			//if (I do not branch left nor came from left) and came from forward and branch forward
			//and the tile to the left did not (come from forward and branch forward)
			//add a forward rail to my left
			if(!(btype & BRANCH_TYPE_LEFT) && !(btype & BRANCH_TYPE_FROM_LEFT))
			{
				if((btype & BRANCH_TYPE_FROM_FORWARD) && (btype & BRANCH_TYPE_FORWARD))
				{
					if(i > 0)
					{
						btype2 = tile_branch_type[i-1][j];
						if(!((btype2 & BRANCH_TYPE_FROM_FORWARD) && (btype2 & BRANCH_TYPE_FORWARD)))
							set_rail_type(i-1,j,RAIL_TYPE_L);
					}
				}
			}

			//if (I do not branch right nor came from right) and came from forward and branch forward
			//and the tile to the right did not (come from forward and branch forward)
			//add a forward rail to my right
			if(!(btype & BRANCH_TYPE_RIGHT) && !(btype & BRANCH_TYPE_FROM_RIGHT))
			{
				if((btype & BRANCH_TYPE_FROM_FORWARD) && (btype & BRANCH_TYPE_FORWARD))
				{
					if(i < width - 1)
					{
						btype2 = tile_branch_type[i+1][j];
						if(!((btype2 & BRANCH_TYPE_FROM_FORWARD) && (btype2 & BRANCH_TYPE_FORWARD)))
							set_rail_type(i+1,j,RAIL_TYPE_R);
					}
				}
			}

			//If (I do not branch forward) and came from forward and branch right
			//add a TR rail at my tile
			if(!(btype & BRANCH_TYPE_FORWARD) && (btype & BRANCH_TYPE_FROM_FORWARD) && (btype & BRANCH_TYPE_RIGHT))
			{
				set_rail_type(i,j,RAIL_TYPE_TR);
			}

			//If (I do not branch forward) and came from forward and branch left
			//add a TL rail at my tile
			if(!(btype & BRANCH_TYPE_FORWARD) && (btype & BRANCH_TYPE_FROM_FORWARD) && (btype & BRANCH_TYPE_LEFT))
			{
				set_rail_type(i,j,RAIL_TYPE_TL);
			}

			//If I come from left and branch forward and (do not come from forward)
			//add a TR2 rail at my tile
			if(!(btype & BRANCH_TYPE_FROM_FORWARD) && (btype & BRANCH_TYPE_FROM_LEFT) && (btype & BRANCH_TYPE_FORWARD))
			{
				set_rail_type(i,j,RAIL_TYPE_TR2);
			}

			//If I come from right and branch forward and (do not come from forward)
			//add a TL2 rail at my tile
			if(!(btype & BRANCH_TYPE_FROM_FORWARD) && (btype & BRANCH_TYPE_FROM_RIGHT) && (btype & BRANCH_TYPE_FORWARD))
			{
				set_rail_type(i,j,RAIL_TYPE_TL2);
			}

			//If I come from right and branch forward, and come from forward and branch left
			//add a TL2 | TL rail at my tile
			if((btype & BRANCH_TYPE_FROM_RIGHT) && (btype & BRANCH_TYPE_FORWARD) && (btype & BRANCH_TYPE_FROM_FORWARD) && (btype & BRANCH_TYPE_LEFT))
			{
				set_rail_type(i,j,RAIL_TYPE_TL2 | RAIL_TYPE_TL);
			}

			//If I come from left and branch forward, and come from forward and branch right
			//add a TR2 | TR rail at my tile
			if((btype & BRANCH_TYPE_FROM_LEFT) && (btype & BRANCH_TYPE_FORWARD) && (btype & BRANCH_TYPE_FROM_FORWARD) && (btype & BRANCH_TYPE_RIGHT))
			{
				set_rail_type(i,j,RAIL_TYPE_TR2 | RAIL_TYPE_TR);
			}
		}
	}

}

void Floor::generate(Vec3 p, int floor_num, Vec3 mins, Vec3 maxs,Vec3 dims, Vec3 player_pos, int _goal_min_column, int _goal_max_column)
{
	if(generated)
		return;
	global_pos = p + Vec3(0,0,floor_num*WINDOW_TILE_SIZE);
	altitude = global_pos.z;
	global_mins = mins;
	global_maxs = maxs;
	global_mins.z = global_maxs.z = altitude;
	width = (int)(dims.x);
	length = (int)(dims.y);

	//=========== BSP Floor Generation ============
	//Allocating the max possible number of rooms that can be created
	Room room_stack[(length-1) * (width-1)];
	Room* last_room_ptr = room_stack;

	*last_room_ptr = Room(0,2,(char)(width-1),(char)(length-3));//width/length will never exceed 255... we're okay here

	recursive_bsp(&last_room_ptr,true);
	//last_room_ptr now points to the last room on the stack


	Wall unique_walls[length * width * 4];
	Wall* next_free_wall = unique_walls;

	get_unique_walls(&next_free_wall, room_stack, last_room_ptr);
	//next_free_wall now points to one pointer past the last wall assigned

	//Iterating through walls, adding the appropriate collision tile
	for(Wall* ptr = unique_walls; ptr < next_free_wall; ptr++)
	{
		//Skip walls randomly
		if(Random::rand() < 0.3f)
		{
			//LOGE("skipped a wall!");
			continue;
		}

		//If this wall is vertical
		if(ptr->x1 == ptr->x2)
		{
			set_vert_wall_tiles(ptr);
		}
		else //this wall is horizontal
		{
			set_hor_wall_tiles(ptr);
		}
	}
	// ============ end BSP Floor Generation ============

	// ============ Player Route Generation =============
	int player_start_column = (int)floorf((player_pos.x - global_mins.x)/TILE_SIZE);
	goal_min_column = _goal_min_column;
	goal_max_column = _goal_max_column;

	// ==================================================
	branch_debug_point_count = 0;
	//Allowing 1 tile of room before any branching is allowed
	tile_branch_type[player_start_column][0] = BRANCH_TYPE_FROM_FORWARD | BRANCH_TYPE_FORWARD;
	tile_branch_type[player_start_column][1] = BRANCH_TYPE_FROM_FORWARD;


	recursive_branch_player_path(player_start_column,1,BRANCH_TYPE_FORWARD);
	place_rail_tiles();

	//Place obstacles in xXoo walls that we cross
	//Go over the tiles in the floor once more, checking if there are any half-walls in our path, and remove them.
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < length; j++)
		{
			//If we came from forward and branch forward
			if(tile_branch_type[i][j] & (BRANCH_TYPE_FORWARD | BRANCH_TYPE_FROM_FORWARD))
			{
				if(tile_type[i][j] == TILE_TYPE_WALL)
				{
					if(tile_subtype[i][j] == WALL_TYPE_xXoo)
					{
						tile_type[i][j] = TILE_TYPE_OBST;
						tile_subtype[i][j] = Random::rand_int_in_range(0,10);
						continue;
					}
					//If we cross a half-wall, remove the half wall
					if(tile_subtype[i][j] == WALL_TYPE_xooo || tile_subtype[i][j] == WALL_TYPE_oXoo)
					{
						tile_type[i][j] = TILE_TYPE_EMPT;
						tile_subtype[i][j] = 0;
					}
				}
			}
		}
	}

	// =========== end Player Route Generation ===========

	//TEMP: assigning tiles as obstacles
	/*int obst_types[8] = {0,1,4,5,6,7,8,9};

	for(int i = 0; i < width && i < 8; i++)
	{
		tile_type[i][3] = TILE_TYPE_OBST;
		tile_subtype[i][3] = obst_types[i];
	}*/


	populate_floor();

	//Populating the dynamic floor model:
	int model_count = length * width;
	Static_Model* models[model_count];
	Mat4 transforms[model_count];
	int index = 0;
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < length; j++)
		{
			models[index] = tile_model[i][j];
			transforms[index] = Mat4::TRANSLATE(Vec3(TILE_SIZE*i,TILE_SIZE*j,0));
			index++;
		}
	}

	//dynamic_floor_model->populate_model(models,transforms,model_count);
	dynamic_floor_model->populate_model(models,transforms,model_count);

	//Temp iterating through all tiles adding debug branch points to array
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < length; j++)
		{
			if(branch_debug_point_count >= 3000 - 12)
			{
				LOGE("Ran out of room for debugging tiles.");
				break;
			}
			Vec3 tile_ofs = Vec3(i*TILE_SIZE,j*TILE_SIZE,1.0f);
			int branched = tile_branch_type[i][j];
			if(branched & BRANCH_TYPE_FROM_FORWARD)
			{
				if(branched & BRANCH_TYPE_LEFT)
				{
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(1,0,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(0,1,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(2.5,0,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(0,2.5,0);
				}
				if(branched & BRANCH_TYPE_RIGHT)
				{
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(1,0,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(3.5,2.5,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(2.5,0,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(3.5,1,0);
				}
				if(branched & BRANCH_TYPE_FORWARD)
				{
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(1,0,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(1,3.5,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(2.5,0,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(2.5,3.5,0);
				}
			}
			if(branched & BRANCH_TYPE_FROM_LEFT)
			{
				if(branched & BRANCH_TYPE_FORWARD)
				{
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(0,2.5,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(1,3.5,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(0,1,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(2.5,3.5,0);
				}
			}
			if(branched & BRANCH_TYPE_FROM_RIGHT)
			{
				if(branched & BRANCH_TYPE_FORWARD)
				{
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(3.5,2.5,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(2.5,3.5,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(3.5,1,0);
					branch_debug_points[branch_debug_point_count++] = tile_ofs + Vec3(1,3.5,0);
				}
			}
		}
	}
	generated = true;
}


int Floor::render(Mat4 vp)
{

	if(!generated)
		return 1;

	Global_Tiles::instance->tile_styles[0]->variants[0]->bind_variant();
	Material* mat = Global_Tiles::instance->tile_styles[0]->variants[0]->mat;
	Mat4 m;
	Mat4 world_trans = Mat4::TRANSLATE(global_mins);

	dynamic_floor_model->bind_mesh_data(mat);
	m = world_trans;
	mat->bind_value(Shader::PARAM_M_MATRIX, (void*) m.m);
	Mat4 mvp = vp * m;
	mat->bind_value(Shader::PARAM_MVP_MATRIX, (void*) mvp.m);
	Mat3 m_it = m.inverted_then_transposed().get_mat3();
	mat->bind_value(Shader::PARAM_M_IT_MATRIX, (void*) m_it.m);
	dynamic_floor_model->render_without_bind();

	//===== Rendering Debug branch lines =======
	/*if(!debug_branch_mat)
		return 1;

	float edges[branch_debug_point_count * 3];

	for(int i = 0; i < branch_debug_point_count; i++)
	{
		edges[3*i] = branch_debug_points[i].x;
		edges[3*i + 1] = branch_debug_points[i].y;
		edges[3*i + 2] = branch_debug_points[i].z;
	}

	debug_branch_mat->bind_material();
	Mat4 mvp = vp * world_trans;
	debug_branch_mat->bind_value(Shader::PARAM_MVP_MATRIX,(void*) mvp.m);

	float color[] = {1.0f,0.0f,0.0f,1.0f};
	debug_branch_mat->bind_value(Shader::PARAM_COLOR_ADD,(void*) color);
	debug_branch_mat->bind_value(Shader::PARAM_VERTICES,(void*) edges);
	int vert_count = branch_debug_point_count - (branch_debug_point_count % 2);
	glDrawArrays(GL_LINES, 0, vert_count);*/
	//==========================================
	return 1;
}

void Floor::clear()
{
	altitude = 0;
	width = length = 0;
	global_mins = Vec3::ZERO();
	global_maxs = Vec3::ZERO();
	clear_floor_tiles();
	if(dynamic_floor_model)
		dynamic_floor_model->clear_model();
	generated = false;
}

//Assumes point p is defined relative to global_mins
//Returns the voxel at point p
//if the point p is out of bounds, returns voxel clip type CLIP_WINDOW, and clip shape as either COL_DIR_FORWARD or COL_DIR_RIGHT
//depending on whether or not the out of bounds occured on the x-axis or y-axis
Voxel Floor::get_voxel_at(Vec3 p)
{
	//position given is in floor space, 0,0 being near left corner
	//get tile indices for the position
	if(is_local_y_out_of_bounds(p))
	{
		return Voxel(CLIP_SOLID,COL_DIR_FORWARD);
	}
	if(is_local_x_out_of_bounds(p))
	{
		return Voxel(CLIP_SOLID,COL_DIR_RIGHT);
	}

	int tile_x = (int) floorf(p.x/TILE_SIZE);
	int tile_y = (int) floorf(p.y/TILE_SIZE);

	if(tile_x < 0 || tile_y < 0 || tile_x >= width || tile_y >= length)
	{
		LOGW("Warning: tried reaching out of bounds tile: (floor dims: (%d x %d), index: (%d x %d))",width,length,tile_x,tile_y);
		return Voxel(CLIP_SOLID);
	}

	//Our position within the tile
	Vec3 tile_p = Vec3(efmodf(p.x,TILE_SIZE),efmodf(p.y,TILE_SIZE),0);
	int vox_x = (int) (floorf(tile_p.x/GRID_SIZE));
	int vox_y = (int) (floorf(tile_p.y/GRID_SIZE));

	if(vox_x < 0 || vox_y < 0 || vox_x >= TILE_VOXEL_DIMS || vox_y >= TILE_VOXEL_DIMS)
	{
		LOGW("Warning: tried reaching out of bounds voxel: (index: (%d x %d))",vox_x,vox_y);
		return Voxel(CLIP_SOLID);
	}

	Voxel v = tile_coll_map[tile_x][tile_y]->get_vox_at(vox_x,vox_y);
	//FIXME: remove this
	//if(v.clip_type != CLIP_EMPTY)
	//	LOGE("Tile[%d][%d], Voxel[%d][%d] = %d",tile_x,tile_y,vox_x,vox_y,v.clip_type);
	return v;
}

//Returns if the floor has a voxel that is solid at the point p
//Assumes p is relative to global_mins
char Floor::is_solid_at(Vec3 p)
{
	Voxel vox = get_voxel_at(p);

	//Our position within the voxel
	Vec3 vox_p = Vec3(efmodf(p.x,GRID_SIZE),efmodf(p.y,GRID_SIZE),0.0f);

	return vox.is_solid_at(vox_p.x, vox_p.y);
}

//Is the global y coord out of the floor bounds?
bool Floor::is_y_out_of_bounds(Vec3 p)
{
	if(p.y < global_mins.y || p.y > global_maxs.y)
		return true;
	return false;
}
//Is the global x coord out of the floor bounds?
bool Floor::is_x_out_of_bounds(Vec3 p)
{
	if(p.x < global_mins.x || p.x > global_maxs.x)
		return true;
	return false;
}
//Is the y coord (relative to our min) out of the floor bounds?
bool Floor::is_local_y_out_of_bounds(Vec3 p)
{
	if(p.y < 0 || p.y > global_maxs.y - global_mins.y)
		return true;
	return false;
}
//Is the x coord (relative to our min) out of the floor bounds?
bool Floor::is_local_x_out_of_bounds(Vec3 p)
{
	if(p.x < 0 || p.x > global_maxs.x - global_mins.x)
		return true;
	return false;
}


//	Returns a maneuver if there exists a maneuver in the this floor's tileset such that:
//	(the input required to start the maneuver is input_type) AND (the player is within the bounding box required to start the maneuver)
//	returns NULL if no such tile exists

//==== Quick note on what tiles we have to check ====
//Previously, the 1st keyframe of a maneuver had to lie within that tile
//But that is no longer the case

//As a rule of thumb, the start of the maneuver must lie within the tile itself, or the tile before it
//so for a given player position, we must check the tile the player is on and the tile after it (+1 in y direction)
//===================================================

//p is the global position
Maneuver* Floor::input_to_maneuver(Vec3 p, int input_type, Vec3& tile_ofs)
{
	//get tile indices for the position
	if(is_x_out_of_bounds(p) || is_y_out_of_bounds(p))
	{
		//LOGW("Warning: X or Y coord to check is out of bounds: coords:(%f,%f), mins:(%f,%f), maxs:(%f,%f)",p.x,p.y,global_mins.x,global_mins.y,global_maxs.x,global_maxs.y);
		return NULL;
	}

	//finding player pos relative to left near corner of floor
	Vec3 floor_pos = p - global_mins;

	int tile_x = (int) floorf(floor_pos.x/TILE_SIZE);
	int tile_y = (int) floorf(floor_pos.y/TILE_SIZE);

	if(tile_x < 0 || tile_y < 0 || tile_x >= width || tile_y >= length)
	{
		LOGW("Warning: tried reaching out of bounds tile: (floor dims: (%d x %d), index: (%d x %d))",width,length,tile_x,tile_y);
		return NULL;
	}
	//Checking the tile the player is on
	Vec3 mins;
	Vec3 maxs;
	Maneuver* man = NULL;

	//Getting player pos relative to the tile
	p = floor_pos - Vec3(tile_x * TILE_SIZE, tile_y * TILE_SIZE, 0);
	Grid_Tile* tile = tile_object[tile_x][tile_y];

	for(int i = 0; i < tile->maneuver_count; i++)
	{
		man = tile->maneuvers[i];
		//If we have the correct input
		if(input_type & man->input_required)
		{
			//If we are in the correct area
			mins = man->keyframes[0]->mins;
			maxs = man->keyframes[0]->maxs;
			if(p.x >= mins.x && p.x <= maxs.x && p.y >= mins.y && p.y <= maxs.y)
			{
				//Assigning tile_ofs
				tile_ofs.x = global_mins.x + tile_x * TILE_SIZE;
				tile_ofs.y = global_mins.y + tile_y * TILE_SIZE;
				tile_ofs.z = global_mins.z;
				return man;
			}
		}
	}

	//Checking the next tile
	tile_y++;

	if(tile_y >= length)
	{
		//There is no tile after
		return NULL;
	}

	//Getting player pos relative to the tile
	p = floor_pos - Vec3(tile_x * TILE_SIZE, tile_y * TILE_SIZE, 0);
	tile = tile_object[tile_x][tile_y];


	for(int i = 0; i < tile->maneuver_count; i++)
	{
		man = tile->maneuvers[i];
		//If we have the correct input
		if(input_type & man->input_required)
		{
			//If we are in the correct area
			mins = man->keyframes[0]->mins;
			maxs = man->keyframes[0]->maxs;
			if(p.x >= mins.x && p.x <= maxs.x && p.y >= mins.y && p.y <= maxs.y)
			{
				//Assigning tile_ofs
				tile_ofs.x = global_mins.x + tile_x * TILE_SIZE;
				tile_ofs.y = global_mins.y + tile_y * TILE_SIZE;
				tile_ofs.z = global_mins.z;
				return man;
			}
		}
	}
	return NULL;
}


//	Returns a traversal if there exists a traversal if pos is within the last tile, and if:
//	(the input required to start the traversal is input_type) AND (the player is within the bounding box required to start the traversal)
//	returns NULL otherwise
//Assigns tile_ofs to the world position of whatever tile the maneuver is on
Traversal* Floor::input_to_traversal(Vec3 p, int input_type, Vec3& tile_ofs)
{
	if(is_x_out_of_bounds(p) || is_y_out_of_bounds(p))
	{
		LOGW("Warning: X or Y coord to check is out of bounds: coords:(%f,%f), mins:(%f,%f), maxs:(%f,%f)",p.x,p.y,global_mins.x,global_mins.y,global_maxs.x,global_maxs.y);
		return NULL;
	}

	//finding player pos relative to left near corner of floor
	Vec3 floor_pos = p - global_mins;

	//get tile indices for the position
	int tile_x = (int) floorf(floor_pos.x/TILE_SIZE);
	int tile_y = (int) floorf(floor_pos.y/TILE_SIZE);

	//Checking if we aren't on the last row of tiles:
	if(tile_y != length - 1)
		return NULL;
	//Checking if we are within goal columns:
	if(tile_x < goal_min_column || tile_x > goal_max_column)
		return NULL;

	Vec3 mins;
	Vec3 maxs;
	Traversal* trav = NULL;

	//Getting player pos relative to the tile
	p = floor_pos - Vec3(tile_x * TILE_SIZE, tile_y * TILE_SIZE, 0);


	for(int i = 0; i < BUILDING_TRAV_COUNT; i++)
	{
		trav = Global_Tiles::instance->bldg_travs[i];

		if(input_type & trav->input_required)
		{
			mins = trav->keyframes[0]->mins;
			maxs = trav->keyframes[0]->maxs;

			if(p.x >= mins.x && p.x <= maxs.x && p.y >= mins.y && p.y <= maxs.y)
			{
				//Assigning tile_ofs
				tile_ofs.x = global_mins.x + tile_x * TILE_SIZE;
				tile_ofs.y = global_mins.y + tile_y * TILE_SIZE;
				tile_ofs.z = global_mins.z;
				return trav;
			}
		}
	}


	return NULL;
}
//Returns the global tile position that p lies on
Vec3 Floor::get_tile_ofs_at_pos(Vec3 p)
{
	p = p - global_mins;
	p = Vec3(floorf(p.x/TILE_SIZE) * TILE_SIZE,floorf(p.y/TILE_SIZE) * TILE_SIZE,0);
	p = p + global_mins;
	return p;
}
