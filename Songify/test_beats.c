// Assignment 2 20T1 COMP1511: CS bEats
// test_beats.c
//
// This program was written by SHUBHAM-JOHAR (z5284381)
// on 25th APRIL 2020
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

    // Test 2: Testing when the octave is greater than 9 
    // or key is greater than 11. 
    if (add_note_to_beat(test_beat, 100, 2) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }

    if (add_note_to_beat(test_beat, 3, 23) != INVALID_KEY) {
        return DOES_NOT_MEET_SPEC;
    }

    if (add_note_to_beat(test_beat, 20, 20) != INVALID_OCTAVE) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 3: if the note is already added, then checking whether it rejects
    //the same value or not. 
    
    if (add_note_to_beat(test_beat, 2, 2) != NOT_HIGHEST_NOTE) {
        if (add_note_to_beat(test_beat, 2, 2) != NOT_HIGHEST_NOTE) {
            return DOES_NOT_MEET_SPEC;     
        }        
    }

    //TEST 4: testing whether it rejects a lower octave after adding
    //a higher octave
    if (add_note_to_beat(test_beat, 4, 4) != NOT_HIGHEST_NOTE) {
        if (add_note_to_beat(test_beat, 2, 4) != NOT_HIGHEST_NOTE) {
            return DOES_NOT_MEET_SPEC;     
        }        
    }

    return MEETS_SPEC;
}

// Testing for `count_notes_in_octave`
int test_count_notes_in_octave(void){
    
    // Test 1: Counting invalid octaves.
    Beat test_beat = create_beat();
    add_note_to_beat(test_beat, 3, 3);
    if (count_notes_in_octave(test_beat, -1) != 0) {
        return DOES_NOT_MEET_SPEC;
    }
    if (count_notes_in_octave(test_beat, 11) != 0) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 2: counting notes in octave 1.
    if (count_notes_in_octave(test_beat, 3) != 1) {
        return DOES_NOT_MEET_SPEC;
    }

    // Test 3: counting notes in octave 7.
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
    
    //checks if the head changes to the beat added.
    if (track_create->head != beat_create) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 2: Checking if the current beat changes or not..!!
    select_next_beat(track_create);
    if (track_create->current != beat_create) {
        return DOES_NOT_MEET_SPEC;
    }
    
    // Test 3: adding a beat after current and checking 
    //whether it gets added after current or not. 
    Beat new_beat1 = create_beat();
    add_beat_to_track (track_create ,new_beat1);

    if (track_create->current->next != new_beat1) {
        return DOES_NOT_MEET_SPEC;
    }

    //Test 4: checking whether the last beat is there.
    if (new_beat1->next != NULL) {
        return DOES_NOT_MEET_SPEC;
    } 

    return MEETS_SPEC;
}

// Test function for `remove_selected_beat`
int test_remove_selected_beat(void){
    
    //Test 1: removing a last beat and checking whether 
    //the track gets stopped. 
    Track track_for_remove = create_track();
    Beat beat_for_remove = create_beat();
    add_note_to_beat(beat_for_remove, 6, 0);
    
    add_beat_to_track (track_for_remove, beat_for_remove);
    select_next_beat (track_for_remove);

    if (remove_selected_beat(track_for_remove) != TRACK_STOPPED) {
        return DOES_NOT_MEET_SPEC;
    }

    //Test 2: removing the first beat and checking whether the 
    //track is still playing when two beats are added and one 
    //is removed from the beginning.
    
    Beat test_beat1 = create_beat();
    add_note_to_beat (test_beat1, 6, 6);
    add_beat_to_track (track_for_remove ,test_beat1);

    Beat test_beat2 = create_beat();
    add_note_to_beat(test_beat2, 7, 0);
    add_beat_to_track(track_for_remove ,test_beat2);
    select_next_beat(track_for_remove);

    if (remove_selected_beat(track_for_remove) != TRACK_PLAYING) {
        return DOES_NOT_MEET_SPEC;
    }

    //Test 3: chekcs by adding a new beat after current and then removing 
    //the selected beat, that whether current shifts to newly added beat
    //or it stops.
    Beat test_beat3 = create_beat();
    add_note_to_beat(test_beat3, 7, 4);
    add_beat_to_track(track_for_remove ,test_beat3);
    if (remove_selected_beat(track_for_remove) == TRACK_STOPPED) {
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}

// Test function for `add_musical_note_to_beat`
int test_add_musical_note_to_beat(void) {
    
    Beat new_beat = create_beat();
    //Test 1: checking whther it takes a valid musical notation.
    if (add_musical_note_to_beat(new_beat, "3G") != VALID_NOTE){
        return DOES_NOT_MEET_SPEC;
    }
    
    if (add_musical_note_to_beat(new_beat, "6G##") != VALID_NOTE){
        return DOES_NOT_MEET_SPEC;
    }
    
    //TEST 2: CHECKS whether it takes an invalid octave or not.
    if (add_musical_note_to_beat(new_beat,"11E") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    } 

    // Test 3: checks whether it rejects Two same muscial notation or not.
    if (add_musical_note_to_beat(new_beat,"5E") == VALID_NOTE) {
        if(add_musical_note_to_beat(new_beat,"5E") != INVALID_MUSICAL_NOTE){
            return DOES_NOT_MEET_SPEC;
        }
    }

    if (add_musical_note_to_beat(new_beat,"6A####") == VALID_NOTE){
        if (add_musical_note_to_beat(new_beat,"6C#") != INVALID_MUSICAL_NOTE){
            return DOES_NOT_MEET_SPEC;
        }   
    }

    //test 4: checks whether it rejects an invalid key alphabet or not.
    if (add_musical_note_to_beat(new_beat,"4U") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }

    //test 5 : checking whether after the first two characters there 
    //is any other character apart from '#'.
    if (add_musical_note_to_beat(new_beat,"4G####GG") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }
    
    //test 6: checks whether it takes "" as an invalid note or not.
    if (add_musical_note_to_beat(new_beat,"") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }
    
    //test 7: checks whether it takes the in '#' as the first 
    //character as an invalid note or not.
    if (add_musical_note_to_beat(new_beat,"#4G") != INVALID_MUSICAL_NOTE){
        return DOES_NOT_MEET_SPEC;
    }

    return MEETS_SPEC;
}


























