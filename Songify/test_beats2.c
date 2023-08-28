 // Assignment 2 20T1 COMP1511: CS bEats
// test_beats.c
//
// This program was written by Uzaif Sheikh (z5252826)
// on INSERT-DATE-HERE
//
// Version 1.0.0: Assignment released.
#include <stdio.h>
#include <stdlib.h>

#include "test_beats.h"
#include "beats.h"

struct track {
    struct beat *head;
    struct beat *current;
};

struct beat {
    struct note *notes;
    struct beat *next;
};

// Test function for `add_note_to_beat`
int test_add_note_to_beat(void) {
    // Test 1: Rejecting negative inputs.
    Beat test_beat = create_beat();
    if (add_note_to_beat(test_beat, -1, -1) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, -1, 0) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 1, -1) != INVALID_KEY) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: If octave is more than 10
    if(add_note_to_beat(test_beat, 10, 0) != INVALID_OCTAVE){
        return DOES_NOT_MEET_SPEC;
    }
    if(add_note_to_beat(test_beat, 12, 0) != INVALID_OCTAVE){
        return DOES_NOT_MEET_SPEC;
    }
    if (add_note_to_beat(test_beat, 2, 12) != INVALID_KEY) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: If notes is the same.
    if (add_note_to_beat(test_beat, 5, 6) != NOT_HIGHEST_NOTE) {
        if (add_note_to_beat(test_beat, 5, 6) != NOT_HIGHEST_NOTE) {
            return DOES_NOT_MEET_SPEC;     
        }        
    }
    if (add_note_to_beat(test_beat, 3, 2) != NOT_HIGHEST_NOTE) {
        if (add_note_to_beat(test_beat, 2, 2) != NOT_HIGHEST_NOTE) {
            return DOES_NOT_MEET_SPEC;     
        }        
    }
    
    return MEETS_SPEC;
}

// Test function for `count_notes_in_octave`
int test_count_notes_in_octave(void){
    // Test 1: Counting invalid octaves.
    Beat test_beat = create_beat();
    add_note_to_beat(test_beat, 1, 1);
    
    if (count_notes_in_octave(test_beat, -1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(test_beat, 11) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: counting for octave 4 and key 1
    
    if (count_notes_in_octave(test_beat, 4) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3:counting for oactave 7. 
    
    if (count_notes_in_octave(test_beat, 7) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}

// Test function for `add_beat_to_track`
int test_add_beat_to_track(void){

    // Test 1: Testing if it's adding beat to the track
    Track new_track = create_track();
    Beat new_beat = create_beat();

    add_beat_to_track(new_track,new_beat);

    if(new_track->head != new_beat){
        return DOES_NOT_MEET_SPEC;
    }
    

    // Test 2: Checking if the current is working!!
    select_next_beat(new_track);
    if(new_track->current != new_beat){
        return DOES_NOT_MEET_SPEC;
    }

    

    // Test 3: adding a beat to current 
    Beat new_beat1 = create_beat();
    add_beat_to_track(new_track,new_beat1);

    if(new_track->current->next != new_beat1){
        return DOES_NOT_MEET_SPEC;
    }
    

    return MEETS_SPEC;
}

// Test function for `remove_selected_beat`
int test_remove_selected_beat(void){
    // Test 1:
    Track test_track = create_track();
    Beat test_beat = create_beat();
    add_note_to_beat(test_beat, 4, 0);

    add_beat_to_track(test_track,test_beat);
    select_next_beat(test_track);

    if(remove_selected_beat(test_track) != TRACK_STOPPED){
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2:
    Beat test_beat1 = create_beat();
    add_note_to_beat(test_beat1, 2, 3);
    add_beat_to_track(test_track,test_beat1);

    Beat test_beat2 = create_beat();
    add_note_to_beat(test_beat2, 5, 0);
    add_beat_to_track(test_track,test_beat2);
    select_next_beat(test_track);

    if(remove_selected_beat(test_track) != TRACK_PLAYING){
        return DOES_NOT_MEET_SPEC;
    }


    return MEETS_SPEC;
}

// Test function for `add_musical_note_to_beat`
int test_add_musical_note_to_beat(void){
    Beat new_beat = create_beat();
    // Test 1: invaild musical notation
    if(add_musical_note_to_beat(new_beat,"-2") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }
    if(add_musical_note_to_beat(new_beat,"12D") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    } 

    // Test 2: Two same muscial notation
    if(add_musical_note_to_beat(new_beat,"4E") != INVALID_MUSICAL_NOTE){
        if(add_musical_note_to_beat(new_beat,"4E") != INVALID_MUSICAL_NOTE){
            return DOES_NOT_MEET_SPEC;
        }
    }

    // Test 3: two equal notes !!
    if(add_musical_note_to_beat(new_beat,"4A##") != INVALID_MUSICAL_NOTE){
        if(add_musical_note_to_beat(new_beat,"4B") == INVALID_MUSICAL_NOTE){
            return DOES_NOT_MEET_SPEC;
        }   
    }
    if(add_musical_note_to_beat(new_beat,"4U") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }
 


    return DOES_NOT_MEET_SPEC;
}



























