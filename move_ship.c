#include "asteroids.h"
// This function determines the action that the ship should take in order to avoid asteroids.
struct ship_action move_ship(int field[][FIELD_WIDTH], void *ship_state) {
    // Get the current position of the ship
    int *ship_pos_ptr = (int *)ship_state;
    int ship_pos = (ship_pos_ptr != NULL) ? *ship_pos_ptr : FIELD_HEIGHT / 2;
    // Initialize an array of distances, where each element is the distance to the nearest asteroid
    int distance[FIELD_HEIGHT] = {FIELD_WIDTH, FIELD_WIDTH, FIELD_WIDTH};
    // Initialize the farthest and second farthest lanes and the ship action
    int farthest_lane = -1, second_farthest_lane = -1, action = 0;
    // Loop through each lane and find the distance to the nearest asteroid
    for (int i = 0; i < FIELD_HEIGHT; i++) {
        for (int j = 1; j < FIELD_WIDTH; j++) {
            if (field[i][j] == 1) {
                distance[i] = j - 1;
                break;
            }
        }
        // Determine the farthest and second farthest lanes with asteroids, aside the ship's current lane
        if (distance[i] > distance[farthest_lane] && i != ship_pos) {
            second_farthest_lane = farthest_lane;
            farthest_lane = i;
        } else if (distance[i] > distance[second_farthest_lane] && i != ship_pos) {
            second_farthest_lane = i;
        }
    }
    // Determine which direction the ship should move based on the farthest and second farthest lanes with asteroids
    if (farthest_lane != -1 && farthest_lane < ship_pos) {
        action = -1; // Move up
    } else if (farthest_lane != -1 && farthest_lane > ship_pos) {
        action = 1; // Move down
    } else if (second_farthest_lane != -1 && second_farthest_lane < ship_pos) {
        action = -1; // Move up
    } else if (second_farthest_lane != -1 && second_farthest_lane > ship_pos) {
        action = 1; // Move down
    }
    // Check if the ship would collide with an asteroid in the next move
    int new_ship_pos = ship_pos + action;
    if (field[new_ship_pos][1] == 1) {
        // If the ship would collide, try to move to the second farthest lane with asteroids,
        if (second_farthest_lane != -1 && second_farthest_lane < ship_pos) {
            action = -1; // Move up
        } else if (second_farthest_lane != -1 && second_farthest_lane > ship_pos) {
            action = 1; // Move down
            // If there is no second farthest lane
        } else {
            action = (ship_pos > 1) ? -1 : 1; // Move to closest safe lane
        }
        new_ship_pos = ship_pos + action;
    }
    // Create a new ship state with the updated position
    int *new_ship_state = (int *)malloc(sizeof(int));
    *new_ship_state = new_ship_pos;
    return (struct ship_action){action, new_ship_state};
}