// Assignment 2 20T1 COMP1511: CS bEats
// test_beats.c
//
// This program was written by YOUR-NAME-HERE (z5555555)
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

    // Test 2: if octave and key are not valid, testing when octave is greater than 9 
    // or key is greater than 11 
    if (add_note_to_beat(test_beat, 100, 2) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }

    if (add_note_to_beat(test_beat, 3, 23) != INVALID_KEY) {
        return DOES_NOT_MEET_SPEC;
    }

    if (add_note_to_beat(test_beat, 20, 20) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 3: if the note is already added 
    
    if (add_note_to_beat(test_beat, 6, 7) != NOT_HIGHEST_NOTE) {
        if (add_note_to_beat(test_beat, 6, 7) != NOT_HIGHEST_NOTE) {
            return DOES_NOT_MEET_SPEC;     
        }        
    }
    if (add_note_to_beat(test_beat, 4, 2) != NOT_HIGHEST_NOTE) {
        if (add_note_to_beat(test_beat, 2, 2) != NOT_HIGHEST_NOTE) {
            return DOES_NOT_MEET_SPEC;     
        }        
    }
    // TODO: Write Test 3
    return MEETS_SPEC;
}

// Testing for `count_notes_in_octave`
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

    // Test 2: counting beats in octave 1
    if (count_notes_in_octave(test_beat, 1) != 1) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: counting beats in octave 7
    if (count_notes_in_octave(test_beat, 7) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    return MEETS_SPEC;
}

// Test function for `add_beat_to_track`
int test_add_beat_to_track(void){
    
    // Test 1: Testing if the beat is added to the track
    //Creates new track and new_beat
    Track track_create = create_track();
    Beat beat_create = create_beat();
    
    //adds new beat to the track
    add_beat_to_track (track_create, beat_create);
    
    //checks if the created track head changes to the beat 
    //created
    if (track_create->head != beat_create) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 2: Checking if the current is working!!
    select_next_beat(track_create);
    if (track_create->current != beat_create) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 3: adding a beat to current 
    Beat new_beat1 = create_beat();
    add_beat_to_track (track_create ,new_beat1);

    if (track_create->current->next != new_beat1) {
        return DOES_NOT_MEET_SPEC;
    }

    //Test 4: checking whether the last beat is there
    if (new_beat1->next != NULL) {
        return DOES_NOT_MEET_SPEC;
    } 

    return MEETS_SPEC;
}

// Test function for `remove_selected_beat`
int test_remove_selected_beat(void){
    
    //Test 1: removing a last beat and checking whether the track gets stopped 
    Track track_for_remove = create_track();
    Beat beat_for_remove = create_beat();
    add_note_to_beat(beat_for_remove, 4, 0);
    
    add_beat_to_track (track_for_remove, beat_for_remove);
    select_next_beat (track_for_remove);

    if (remove_selected_beat(track_for_remove) != TRACK_STOPPED) {
        return DOES_NOT_MEET_SPEC;
    }

    //Test 2: removing first beat and checking whether the track is still playing when 
    //two beats are added and one is removed from the beginning.
    
    Beat test_beat1 = create_beat();
    add_note_to_beat (test_beat1, 2, 3);
    add_beat_to_track (track_for_remove ,test_beat1);

    Beat test_beat2 = create_beat();
    add_note_to_beat(test_beat2, 5, 0);
    add_beat_to_track(track_for_remove ,test_beat2);
    select_next_beat(track_for_remove);

    if(remove_selected_beat(track_for_remove) != TRACK_PLAYING){
        return DOES_NOT_MEET_SPEC;
    }

    //Test 3:  
    return DOES_NOT_MEET_SPEC;
}

// Test function for `add_musical_note_to_beat`
int test_add_musical_note_to_beat(void) {
    
    Beat new_beat = create_beat();
    // Test 1: invaild musical notation
    if (add_musical_note_to_beat(new_beat, "-20") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }

    if (add_musical_note_to_beat(new_beat,"11E") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    } 

    // Test 2: Two same muscial notation
    if (add_musical_note_to_beat(new_beat,"4E") == INVALID_MUSICAL_NOTE) {
        if(add_musical_note_to_beat(new_beat,"4E") != INVALID_MUSICAL_NOTE){
            return DOES_NOT_MEET_SPEC;
        }
    }

    // Test 3: two equal notes !!
    if (add_musical_note_to_beat(new_beat,"4A##") == INVALID_MUSICAL_NOTE){
        if (add_musical_note_to_beat(new_beat,"4B") != INVALID_MUSICAL_NOTE){
            return DOES_NOT_MEET_SPEC;
        }   
    }
    if (add_musical_note_to_beat(new_beat,"4U") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }

    if (add_musical_note_to_beat(new_beat,"4G####GG") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }
    
    if (add_musical_note_to_beat(new_beat,"") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }

    if (add_musical_note_to_beat(new_beat,"#4G") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}