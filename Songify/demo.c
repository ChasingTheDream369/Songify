// Assignment 2 20T1 COMP1511: CS bEats
// beats.c
//
// This program was written by Uzaif Sheikh (z5252826)
// on 09/04/2020
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Fix default return value of add_musical_note_to_beat.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Add any extra #includes your code needs here.

#include "beats.h"

// Add your own #defines here.

////////////////////////////////////////////////////////////////////////

// You don't have to use the provided struct track, you are free to
// make your own struct instead.
// If you use the provided struct track, you will have to add fields
// to store other information.

struct track {
    // TODO: You will have to add extra fields here in Stage 2.
    struct beat *head;
    struct beat *current;
};

// You don't have to use the provided struct beat, you are free to
// make your own struct instead.
// If you use the provided struct beat, you may add fields
// to it to store other information.

struct beat {
    // You may choose to add or change fields in this struct.
    struct note *notes;
    struct beat *next;
};

// You don't have to use the provided struct note, you are free to
// make your own struct instead.
// If you use the provided struct note, you add fields
// to it to store other information.

struct note {
    // You may choose to add or change fields in this struct.
    int octave;
    int key;
    struct note *next;
};

// Add any other structs you define here.

////////////////////////////////////////////////////////////////////////

// Add prototypes for any extra functions you create here.



// Return a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);

    new_beat->next = NULL;
    new_beat->notes = NULL;

    // You do not need to change this function, unless you change
    // the implementation of struct beat.

    return new_beat;
}

// You need to implement the following functions.
// You can find descriptions of what each function should do in beats.h

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add a note to the end of a beat.
int add_note_to_beat(Beat beat, int octave, int key) {


    //if octave is negative or greater than 10
    if(octave < 0 || octave > 10){
        return INVALID_OCTAVE;
    }
    //if key is negative or greater than 12
    if(key < 0 || key > 12){
        return INVALID_KEY;
    }
    struct note* n = malloc(sizeof(struct note));
    //the note list is NULL
    if(beat->notes == NULL){
        beat->notes = n;
        n->octave = octave;
        n->key = key;
        n->next = NULL;
    }
    //when the note list have notes in it.
    else{
        struct note* counter = beat->notes;
        while(counter->next != NULL){
            counter = counter->next;
        }
        //if note is smaller than the previous one
        if((octave < counter->octave) || 
           (octave == counter->octave && key < counter->key) ||
           (octave == counter->octave && key == counter->key)){
            return NOT_HIGHEST_NOTE;
        }
        counter->next = n;
        n->octave = octave;
        n->key = key;
        n->next = NULL;
    }

    return VALID_NOTE;
}

// Print the contents of a beat.
void print_beat(Beat beat) {
    struct note* counter = beat->notes;
    while(counter != NULL){
        if(counter->key < 10){
            printf("%d 0%d",counter->octave,counter->key);
        }
        else{
            printf("%d %d",counter->octave,counter->key);
        }
        counter = counter->next;
        if(counter == NULL){
            printf("\n");
            return;
        }
        printf(" | ");
    }
    printf("\n");
    return;
}

// Count the number of notes in a beat that are in a given octave.
int count_notes_in_octave(Beat beat, int octave) {
    int count = 0;
    struct note* counter = beat->notes;
    while(counter != NULL){
        if(octave == counter->octave){
            count++;
        }
        counter = counter->next;
    }
    return count;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {
    // Note: there is no fprintf in this function, as the
    // Stage 1 autotests call create_track but expect it to return NULL
    // (so this function should do nothing in Stage 1).
    Track track = malloc(sizeof(struct track));
    track->head = NULL;
    track->current = NULL;

    return track;
}

// Add a beat after the current beat in a track.
void add_beat_to_track(Track track, Beat beat) {
    
    struct beat* counter = track->current;
    //if the track is null
    if(counter != NULL){
        beat->next = counter->next;
        counter->next = beat;    
    }                                   
    //if the track head is NOT null
    else{

        beat->next = track->head;
        track->head = beat;
    }

    
    return;
}

// Set a track's current beat to the next beat.
int select_next_beat(Track track) {

    struct beat* counter = track->current;
    if(track->current == NULL){
        track->current = track->head;
    }
    else{
        track->current = counter->next;
    }
    
    if(track->current == NULL){
        return TRACK_STOPPED;
    }
    return TRACK_PLAYING;
}

// Print the contents of a track.
void print_track(Track track) {
    int count_beats = 0;
    struct beat* counter = track->head;
    while(counter != NULL){

        count_beats++;
        if(track->current == counter){
            printf(">[%d] ",count_beats);
        }
        else{
            printf(" [%d] ",count_beats);
        }

        struct note* counter_1 = counter->notes;
        while(counter_1 != NULL){
            if(counter_1->key < 10){
                printf("%d 0%d",counter_1->octave,counter_1->key);
            }
            else{
                printf("%d %d",counter_1->octave,counter_1->key);
            }
            if(counter_1->next != NULL){
                printf(" | ");
            }
            counter_1 = counter_1->next;
        }
        printf("\n");
        counter = counter->next;
        if(counter == NULL){
            return;
        }
    }
    
    //printf("print_track not implemented yet.\n");
    return;
}

// Count beats after the current beat in a track.
int count_beats_left_in_track(Track track) {
    int count = 0;
    if(track->current == NULL){
        struct beat* counter = track->head;
        while(counter != NULL){
            count++;
            counter = counter->next;
        }
    }
    else{
        struct beat* counter_1 = track->current;
        counter_1 = counter_1->next;
        while(counter_1 != NULL){
            count++;
            counter_1 = counter_1->next;
        }
    }

    return count;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_beat but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than exit).

    struct note* previous = NULL;

    while(beat->notes != NULL){
        
        previous = (beat->notes)->next;
        free(beat->notes);
        beat->notes = previous;
        
    }   
    free(previous);
    free(beat->notes);
    
    if(beat->notes == NULL){
        free(beat);
    } 
    
    return;
}

// Free the memory of a track, and any memory it points to.
void free_track(Track track) {
    // Note: there is no printf in this function, as the
    // Stage 1 & 2 autotests call free_track but don't check whether
    // the memory has been freed (so this function should do nothing in
    // Stage 1 & 2, rather than print an error).

    struct beat* previous = NULL;
    while(track->head != NULL){

        previous = (track->head)->next;
        free(track->head);
        track->head = previous;
    }
    free(previous);
    free(track->head);
    free(track);
   /* if(track->head == NULL){
        //free(track);
    } */
    return;
}

// Remove the currently selected beat from a track.
int remove_selected_beat(Track track) {
    
    struct beat* counter = track->head;
    if(track->current == NULL){
        return TRACK_STOPPED;
    }
    if(track->current == track->head){
        track->head = counter->next;
        track->current = track->head;
        free(counter);
        if(track->current != NULL){
            return TRACK_PLAYING;
        }
    }

    else{
        while(counter->next != track->current){
            
            
            
            counter = counter->next;
        }
        struct beat* remove = counter->next;
        counter->next = remove->next;
        track->current = remove->next;
        free(remove);
        if(track->current != NULL){
            return TRACK_PLAYING;
        }
    }
    

    return TRACK_STOPPED;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 4 Functions                          //
////////////////////////////////////////////////////////////////////////

// Add note to beat, given its 'musical notation'.
int add_musical_note_to_beat(Beat beat, char *musical_notation) {
    printf("add_musical_note_to_beat not implemented yet.");
    return VALID_NOTE;
}


////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

// Cut a range of beats to the end of a track.
void cut_range_to_end(Track track, int range_length) {
    printf("cut_range_to_end not implemented yet.");
    return;
}

// Reverse a list of beats within a range of a track.
int reverse_range(Track track, int range_length) {
    printf("reverse_range not implemented yet.");
    return 0;
}

