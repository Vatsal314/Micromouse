
#include <stdio.h>
#include <string.h>
#include <config.h>
#define MAX 1000
#include <floodfill.h>
#include "data_structures.h"
const float steplength=26.2;
short y_length = length;
char path_taken[length * length];
int path_index = 0;
char path[4000];
Queue queue;

int detectDist=180;



const double KP_WALL = 0.15, KD_WALL = 0.2,KP_YAW =0.1;//final run constants

bool last_was_back = false;
char short_path[4000];
int short_path_index = 0;

int isOpposite(char a, char b) {
  return (a == 'N' && b == 'S') ||
         (a == 'S' && b == 'N') ||
         (a == 'E' && b == 'W') ||
         (a == 'W' && b == 'E');
}

void reduceDirections(const char* input) {
  char stack[MAX];
  int top = -1;

  for (int i = 0; input[i] != '\0'; i++) {
    char dir = input[i];
    if (top >= 0 && isOpposite(stack[top], dir)) {
      top--;  // Cancel out opposite direction
    } else {
      stack[++top] = dir;  // Push direction to stack
    }
  }

  // Print reduced result
  Serial.println("Reduced directions:");
  for (int i = 0; i <= top; i++) {
    Serial.print(stack[i]);
    short_path[i]=stack[i];
  }
  Serial.println();
}
bool dup_arr[length][length][4] ={
    {{1,1,1,0},{1,1,0,0},{0,1,0,1},{0,1,1,0},{1,1,1,0},{1,1,1,0}},
    {{1,0,0,1},{0,0,1,0},{1,1,0,1},{0,0,0,1},{0,0,1,0},{1,0,1,0}},
    {{1,1,1,0},{1,0,1,0},{1,1,0,0},{0,1,0,0},{0,0,1,0},{1,0,1,0}},
    {{1,0,0,0},{0,0,1,1},{1,0,0,1},{0,0,1,1},{1,0,0,0},{0,0,1,1}},
    {{1,0,0,0},{0,1,1,0},{1,1,0,0},{0,1,0,1},{0,0,0,1},{0,1,1,1}},
    {{1,0,1,1},{1,0,0,1},{0,0,0,1},{0,1,0,1},{0,1,0,1},{0,1,1,1}}
    };


bool wall_data[length][length][4];

void initWalls() {
    for (int r = 0; r < length; r++) {
        for (int c = 0; c < length; c++) {
            wall_data[r][c][0] = (c == 0);         // West wall
            wall_data[r][c][1] = (r == 0);  // NORTH wall
            wall_data[r][c][2] = (c == length - 1);  // East wall
            wall_data[r][c][3] = (r= length-1);         // South wall
        }
    }
}


// bool wall_data[length][length][4] ={
//     {{1,1,0,0},{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,1,0}},
//     {{1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,1,0}},
//     {{1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,1,0}},
//     {{1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,1,0}},
//     {{1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,1,0}},
//     {{1,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,0,1},{0,0,1,1}}
//     };

void print_path_taken() {
    Serial.print("\nPath taken by bot: ");
    for (int i = 0; i < path_index; i++) {
        Serial.print(path_taken[i]);
        Serial.print(" ");
    }
    Serial.println();
    reduceDirections(path_taken);
}

void print_short_path() {
    Serial.print("\nshort path: ");
    for (int i = 0; i < short_path_index; i++) {
        Serial.print(short_path[i]);
        Serial.print(" ");
    }
    Serial.println();
}

int* minimum_cost(short arena_map[length][length], short bot_pos[2], int *sortedArray) {
    /*
        returns array with [0,1,2,3] as [l,s,r,b] in ascending order of their weights
        Function verified
    */

    // Getting values of neighbors
    int top, left, bottom, right;

    // If condition is to check for array out of bound condition
    if (bot_pos[0] == 0) { // if bot is at top row
        top = 1000;
    } else {
        top = arena_map[bot_pos[0] - 1][bot_pos[1]];
    }

    if (bot_pos[0] == (length-1)) { // if bot is at bottom row
        bottom = 1000;
    } else {
        bottom = arena_map[bot_pos[0] + 1][bot_pos[1]];
    }

    if (bot_pos[1] == 0) { // if bot is at leftmost column
        left = 1000;
    } else {
        left = arena_map[bot_pos[0]][bot_pos[1] - 1];
    }

    if (bot_pos[1] == (length-1)) { // if bot is at rightmost column
        right = 1000;
    } else {
        right = arena_map[bot_pos[0]][bot_pos[1] + 1];
    }

    int *return_value = (int *)calloc(4, sizeof(int));
    int temp_arr[4]; 

   switch (digitalRead(SWITCH)) {
        case 0:
            return_value[0] = 2; // r
            return_value[1] = 1; // s
            return_value[2] = 0; // l
            return_value[3] = 3; // b

            temp_arr[0] = right;
            temp_arr[1] = top;
            temp_arr[2] = left;
            temp_arr[3] = bottom;
            break;

        case 1:
            return_value[0] = 0; // l
            return_value[1] = 1; // s
            return_value[2] = 2; // r
            return_value[3] = 3; // b

            temp_arr[0] = left;
            temp_arr[1] = top;
            temp_arr[2] = right;
            temp_arr[3] = bottom;
            break;

        default:
            // handle unexpected output
            temp_arr[0] = left;
            temp_arr[1] = top;
            temp_arr[2] = right;
            temp_arr[3] = bottom;
            break;
    }

    int smallest = 0;



    // Sorting array (selection sort)
    for (int i = 0; i < 4; i++) {
        smallest = i;
        for (int j = i + 1; j < 4; j++) {
            if (temp_arr[smallest] > temp_arr[j]) {
                smallest = j;
            }
        }

        // Swap values
        int temp = temp_arr[i];
        temp_arr[i] = temp_arr[smallest];
        temp_arr[smallest] = temp;

        temp = return_value[i];
        return_value[i] = return_value[smallest];
        return_value[smallest] = temp;
    }

    // Copying sorted array to sortedArray
    for (int i = 0; i < 4; i++) {
        sortedArray[i] = temp_arr[i];
    }

    return return_value;
}

int minimum_value_accessible_neighbors(short arena_map[length][length], short pos[2], int *smallest_accessible_regardless, bool wall_data[length][length][4]) {
    /*returns 0 for left, 1 for forward, 2 for right, 3 for back, -1 if no minimum accessible neighbors
    Function verified
    */

    int sortedArray[4];
    int *min_cost = minimum_cost(arena_map, pos, sortedArray);

    for (int i = 0; i < 4; i++) {

        if (arena_map[pos[0]][pos[1]] > sortedArray[i]) { //Checking if current node is greater than minimum accessible neighbors.
            if (wall_data[pos[0]][pos[1]][min_cost[i]] == 0) { //Checking if node is accessible
                return min_cost[i];
            } else {
                continue;
            }
        } else {
            if (wall_data[pos[0]][pos[1]][min_cost[i]] == 0) { //Checking if node is accessible
                switch (min_cost[i]) { //assigning smallest_accessible_regardless to the smallest non-accessible neighbor
                    case 0:
                        *smallest_accessible_regardless = arena_map[pos[0]][pos[1] - 1];
                        break;
                    case 1:
                        *smallest_accessible_regardless = arena_map[pos[0] - 1][pos[1]];
                        break;
                    case 2:
                        *smallest_accessible_regardless = arena_map[pos[0]][pos[1] + 1];
                        break;
                    case 3:
                        *smallest_accessible_regardless = arena_map[pos[0] + 1][pos[1]];
                        break;
                    default:
                        break;
                }
                return -1;
            }
        }
    }
}

void rearrange_map(short arena_map[length][length], short base_pos[2], bool wall_data[length][length][4]) {
    // Changes value of map node cost in case the current node has a strictly lower cost than all of its accessible neighbors.
    // Function verified
    queue.push(base_pos[0], base_pos[1]);  // using Queue method
    short* poped;
    int min_access;
    int small;

    while (!queue.empty()) {  // using Queue method
        poped = queue.pop();  // using Queue method
        min_access = minimum_value_accessible_neighbors(arena_map, poped, &small, wall_data);  // returns index of minimum value accessible neighbor

        if (poped[0] < 0 || poped[0] > (length-1) || poped[1] < 0 || poped[1] > (length-1)) {
            continue;
        }

        if (min_access == -1) {  // if all accessible neighbors have higher cost than current node
            arena_map[poped[0]][poped[1]] = small + 1;

            for (int i = 0; i < 4; i++) {  // pushing accessible neighbors to queue
                if (wall_data[poped[0]][poped[1]][i] == 0) {
                    switch (i) {
                        case 0: queue.push(poped[0], poped[1] - 1); break;  // west
                        case 1: queue.push(poped[0] - 1, poped[1]); break;  // north
                        case 2: queue.push(poped[0], poped[1] + 1); break;  // east
                        case 3: queue.push(poped[0] + 1, poped[1]); break;  // south
                        default: break;
                    }
                }
            }
        }
    }
}

int direction_wrt_compass(short arena_map[length][length], short bot_pos[2], bool wall_data[length][length][4]) {
    // Checks which direction to move in with respect to a compass. i.e 0 => East, 1 => North, 2 => West, 3 => South. Function unverified

    int smallest_value;
    int small;
    int min_access;

    do {
        min_access = minimum_value_accessible_neighbors(arena_map, bot_pos, &small, wall_data);

        switch (min_access) {  // LSRB if nodes are equal
            case 0:
                path_taken[path_index++] = 'W';
                return 0;
            case 1:
                path_taken[path_index++] = 'N';
                return 1;
            case 2:
                path_taken[path_index++] = 'E';
                return 2;
            case 3:
                path_taken[path_index++] = 'S';
                return 3;
            case -1:
                rearrange_map(arena_map, bot_pos, wall_data);
                break;
        }

    } while (min_access == -1);

    return -1;  // to avoid fall-through if invalid min_access
}


int direction_wrt_bot(short arena_map[length][length], short bot_pos[2], int facing, bool wall_data[length][length][4]) {
    int direction1 = direction_wrt_compass(arena_map, bot_pos, wall_data);

    if (facing == direction1) {
        Serial.println("Forward");
        moveForward(steplength,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);
        if(getDistance(tofCenter)==-1){
            moveForward(1.5,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);
        }
        if (last_was_back) {
            last_was_back = false;
            short_path[short_path_index++] = 'F';
        } else {
            short_path[short_path_index++] = 'F';
        }
        return 1;
    } else if ((facing + 1) % 4 == direction1) {
        Serial.println("Right");
        TurnRight();
        moveForward(steplength,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);
        if(getDistance(tofCenter)==-1){
            moveForward(1.5,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);
        }
        if (last_was_back) {
            last_was_back = false;
            short_path[short_path_index++] = 'L'; // Opposite of 'R'
        } else {
            short_path[short_path_index++] = 'R';
        }
        return 2;
    } else if (facing == (direction1 + 1) % 4) {
        Serial.println("Left");
        TurnLeft();
        moveForward(steplength,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);
        if(getDistance(tofCenter)==-1){
            moveForward(1.5,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);
        }
        if (last_was_back) {
            last_was_back = false;
            short_path[short_path_index++] = 'R'; // Opposite of 'L'
        } else {
            short_path[short_path_index++] = 'L';
        }
        return 0;
    } else {
        Serial.println("Backward");
        Turn180();
        moveForward(steplength,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);
        if(getDistance(tofCenter)==-1){
            moveForward(1.5,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);
        }
        // Remove the last move if it exists
        if (short_path_index > 0) {
            short_path_index--;
        }
        last_was_back = true;
        return 3;
    }
}


#include "data_structures.h"  // Make sure to include your custom structures


// short arena_map[length][length] = {
//         { 4, 3, 2, 2, 3, 4 },
//         { 3, 2, 1, 1, 2, 3 },
//         { 2, 1, 0, 0, 1, 2 },
//         { 2, 1, 0, 0, 1, 2 },
//         { 3, 2, 1, 1, 2, 3 },
//         { 4, 3, 2, 2, 3, 4 },
//     };
short arena_map[length][length];

void initFloodfill(short arena_map[length][length], int length, int goal_row, int goal_col) {
        for (int r = 0; r < length; r++) {
            for (int c = 0; c < length; c++) {
                arena_map[r][c] = abs(r - goal_row) + abs(c - goal_col);
            }
        }
    }

int floodfill() {
    initWalls();

    initFloodfill(arena_map, length, 0, length -1);

    for (int r = 0; r < length; r++) {
        for (int c = 0; c < length; c++) {
            Serial.print(arena_map[r][c]);
        }
        Serial.print("\n");
    }
    delay(3000);
    short position[2] = {(length-1), 15};

    int facing = 1;
    int left_wall;
    int right_wall;
    int front_wall;
    while (true) {
        Serial.println("Wall data for current node: \n");
        int temp;
        left_wall = getDistance(tofLeft);
        right_wall = getDistance(tofRight);
        front_wall = getDistance(tofCenter);
        Serial.print("Left: ");
        Serial.print(left_wall);
        Serial.print(" Right: ");
        Serial.print(right_wall);
        Serial.print(" Front: ");
        Serial.println(front_wall);
        if(facing==0){
            if (getDistance(tofLeft) < detectDist && getDistance(tofLeft)>0) {
                wall_data[position[0]][position[1]][3] = 1; // S wall
            } //else {
                //wall_data[position[0]][position[1]][3] = 0;
            //}
            if (getDistance(tofCenter) < detectDist && getDistance(tofCenter)>0) {
                wall_data[position[0]][position[1]][0] = 1; // W wall
            } //else {
                //wall_data[position[0]][position[1]][0] = 0;
            //}
            if (getDistance(tofRight) < detectDist && getDistance(tofRight)>0) {
                wall_data[position[0]][position[1]][1] = 1; // N wall
            } //else {
                //wall_data[position[0]][position[1]][1] = 0;
            //}
        }
        if(facing==1){
            if (getDistance(tofLeft) < detectDist && getDistance(tofLeft)>0) {
                wall_data[position[0]][position[1]][0] = 1; // left wall
            } //else {
                //wall_data[position[0]][position[1]][0] = 0;
            //}
            if (getDistance(tofCenter) < detectDist && getDistance(tofCenter)>0) {
                wall_data[position[0]][position[1]][1] = 1; // front wall
            } //else {
                //wall_data[position[0]][position[1]][1] = 0;
            //}
            if (getDistance(tofRight) < detectDist && getDistance(tofRight)>0) {
                wall_data[position[0]][position[1]][2] = 1; // right wall
            } //else {
                //wall_data[position[0]][position[1]][2] = 0;
            //}
        }
        if(facing==2){
            if (getDistance(tofLeft) < detectDist && getDistance(tofLeft)>0) {
                wall_data[position[0]][position[1]][1] = 1; // N wall
            } //else {
                //wall_data[position[0]][position[1]][1] = 0;
            //}
            if (getDistance(tofCenter) < detectDist && getDistance(tofCenter)>0) {
                wall_data[position[0]][position[1]][2] = 1; // E wall
            } //else {
                //wall_data[position[0]][position[1]][2] = 0;
            //}
            if (getDistance(tofRight) < detectDist && getDistance(tofRight)>0) {
                wall_data[position[0]][position[1]][3] = 1; // S wall
            } //else {
                //wall_data[position[0]][position[1]][3] = 0;
            //}
        }
        if(facing==3){
            if (getDistance(tofLeft) < detectDist && getDistance(tofLeft)>0) {
                wall_data[position[0]][position[1]][2] = 1; // E wall
            } //else {
                //wall_data[position[0]][position[1]][2] = 0;
            //}
            if (getDistance(tofCenter) < detectDist && getDistance(tofCenter)>0) {
                wall_data[position[0]][position[1]][3] = 1; // S wall
            } //else {
                //wall_data[position[0]][position[1]][3] = 0;
            //}
            if (getDistance(tofRight) < detectDist && getDistance(tofRight)>0) {
                wall_data[position[0]][position[1]][0] = 1; // West wall
            } //else {
                //wall_data[position[0]][position[1]][0] = 0;
            //}
        }

        for(int j=0;j<4;j++){
            Serial.print("Wall data: ");
            Serial.print(j);
            Serial.print(" = ");
            Serial.print(wall_data[position[0]][position[1]][j]);
        }
        
        Serial.println("\n");
      
        if (arena_map[position[0]][position[1]] == 0) {
            Serial.println("Reached center!\n");
            print_path_taken();
            print_short_path();
            return 0;
        }

        int turn_direction = direction_wrt_bot(arena_map, position, facing, wall_data);

        switch (turn_direction) {
            case 0:
                facing = facing - 1;
                if (facing == -1) facing = 3;
                break;
            case 1:
                break;
            case 2:
                facing = (facing + 1) % 4;
                break;
            case 3:
                facing = (facing + 2) % 4;
                break;
        }

        // Save previous position before updating
        short prev_x = position[0];
        short prev_y = position[1];

        switch (facing) {
            case 0: position[1] -= 1; break;
            case 1: position[0] -= 1; break;
            case 2: position[1] += 1; break;
            case 3: position[0] += 1; break;
        }
        


        // Push previous position to queue for path tracking
        queue.push(prev_x, prev_y);
        Serial.print("Current position:" );
        Serial.print(position[0]);
        Serial.println(position[1]);
        Serial.print("Current facing:" );
        Serial.println(facing);
    }
    delay(15000);
    reduceDirections(short_path);
    
    return 0;
}

void leftWallFollowerLoop() {
  Serial.println("Left Wall follower started");
    while (true) {
        int left_wall  = getDistance(tofLeft);
        int right_wall = getDistance(tofRight);
        int front_wall = getDistance(tofCenter);

        Serial.print("Left: ");
        Serial.print(left_wall);
        Serial.print(" Right: "); 
        Serial.print(right_wall);
        Serial.print(" Front: ");
        Serial.println(front_wall);

        // 1. If left is open
        if (left_wall > detectDist or left_wall == -1) {
            TurnLeft();
            moveForward(steplength, KP_DIST_LEFT2, KD_DIST_LEFT2, KP_DIST_RIGHT2, KD_DIST_RIGHT2);
        }
        // 2. Else if front is open
        else if (front_wall > detectDist or front_wall == -1) {
            moveForward(steplength, KP_DIST_LEFT2, KD_DIST_LEFT2, KP_DIST_RIGHT2, KD_DIST_RIGHT2);
        }
        // 3. Else if right is open
        else if (right_wall > detectDist or right_wall == -1) {
            TurnRight();
            moveForward(steplength, KP_DIST_LEFT2, KD_DIST_LEFT2, KP_DIST_RIGHT2, KD_DIST_RIGHT2);
        }
        // 4. Else → dead end
        else {
            Turn180();
            moveForward(steplength, KP_DIST_LEFT2, KD_DIST_LEFT2, KP_DIST_RIGHT2, KD_DIST_RIGHT2);
        }

    }
}


//int directionIndex(char dir) {
   // switch (dir) {
     //   case 'N': return 0;
       // case 'E': return 1;
       // case 'S': return 2;
       // case 'W': return 3;
       // default: return -1;
    //}
//}

// Determine turn from 'from' to 'to'
//void makeTurn(char from, char to) {
   // int f = directionIndex(from);
    //int t = directionIndex(to);
    //int diff = (t - f + 4) % 4;

    //if (diff == 1) {
      //  TurnRight();
    //} else if (diff == 3) {
      //  TurnLeft();
    //} else if (diff == 2) {
      //  Turn180();
    //}
//}

// Final run function
//void final_run(const char* reduced) {
//     if (reduced[0] == '\0') return;

//     char current = reduced[0];
//     int steps = 1;

//     for (int i = 1; reduced[i] != '\0'; i++) {
//         char next = reduced[i];
//         if (next == current) {
//             steps++;
//         } else {
//             moveForward(25 * steps);
//             makeTurn(current, next);
//             current = next;
//             steps = 1;
//         }
//     }
//     moveForward(25 * steps);  // Final move
// //}



int directionIndex(char dir) {
    switch (dir) {
        case 'N': return 0;
        case 'E': return 1;
        case 'S': return 2;
        case 'W': return 3;
        default: return -1;
    }
}

void final_run(const char short_path[]) {
    if (short_path[0] == '\0') return; // empty path check

    int facingDir = directionIndex('N');       // always start facing North
    int currentDir = directionIndex(short_path[0]);
    int steps = 1;

    // If the first move is not in the same direction as facing, turn first
    if (currentDir != facingDir) {
        int diff = (currentDir - facingDir + 4) % 4;
        if (diff == 1) TurnRight();
        else if (diff == 3) TurnLeft();
        else if (diff == 2) Turn180();
        facingDir = currentDir;
    }

    for (int i = 1; short_path[i] != '\0'; i++) {
        int nextDir = directionIndex(short_path[i]);

        if (nextDir == currentDir) {
            steps++; // keep going straight
        } 
        else {
            // Move forward accumulated distance
            if(steps!=1){
                moveForward(steps*25*0.9 ,KP_DIST_LEFT2, KD_DIST_LEFT2 ,KP_DIST_RIGHT2 ,KD_DIST_RIGHT2,KP_WALL,KD_WALL,KP_YAW);
            }
            else{
                moveForward(steps*25 ,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);

            }

            // Turn towards the new direction
            int diff = (nextDir - facingDir + 4) % 4;
            if (diff == 1) TurnLeft();
            else if (diff == 3) TurnRight();
            else if (diff == 2) Turn180();

            facingDir = nextDir;
            currentDir = nextDir;
            steps = 1;
        }
    }

    // Final move after loop
            if(steps!=1){
                moveForward(steps*steplength*0.9 ,KP_DIST_LEFT2, KD_DIST_LEFT2 ,KP_DIST_RIGHT2 ,KD_DIST_RIGHT2,KP_WALL,KD_WALL,KP_YAW);
            }
            else{
                moveForward(steps*steplength ,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);

            }}

//moveForward(25,KP_DIST_LEFT, KD_DIST_LEFT ,KP_DIST_RIGHT ,KD_DIST_RIGHT);