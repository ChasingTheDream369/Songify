// Assignment 2 20T1 COMP1511: CS bEats
// beats.c
//
// This program was written by SHUBHAM JOHAR (z5284381)
// on 18 APRIL 2020
//
// Version 1.0.0: Assignment released.
// Version 1.0.1: Fix default return value of add_musical_note_to_beat.

//All the #includes present in the file are written here
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include<ctype.h>
#include "beats.h"

// All the #defines included in the file are here.
#define CURRENT_NOT_NULL 15
#define CURRENT_NULL 23
#define NUM_NOTES 22
#define CURRENT_EQUAL 20
#define BEFORE_CURRENT 21
////////////////////////////////////////////////////////////////////////

//structure for track
struct track {
    struct beat *head;
    struct beat* current;
};

//structure for beat
struct beat {
    struct note *notes;
    struct beat *next;
};

//structure for note
struct note {
    int octave;
    int key;
    struct note *next;
};

////////////////////////////////////////////////////////////////////////

//Prototypes for all the extra functions defined.
int add_musical_note_ascending (Beat beat, int key, int octave, struct note* musical_note);
int reverse_beats_current (Track track, Beat beat_before_current, int range_length, int before_current_or_equal);

// Returns a malloced Beat with fields initialized.
Beat create_beat(void) {
    Beat new_beat = malloc(sizeof (struct beat));
    assert(new_beat != NULL);

    new_beat->next = NULL;
    new_beat->notes = NULL;
    
    return new_beat;
}

////////////////////////////////////////////////////////////////////////
//                         Stage 1 Functions                          //
////////////////////////////////////////////////////////////////////////

// Adds a note to the end of a beat.
int add_note_to_beat (Beat beat, int octave, int key) {
    
    //checks if the octave is negative or greater than 
    //or equal to 10 and returns invalid octave if it is true. 
    if (octave < 0 || octave >= 10) {
        return INVALID_OCTAVE;
    }
    
    //checks if the key is negative or greater than 
    //or equal to 12 and returns invalid key if it is true.
    if (key < 0 || key >= 12) {
        return INVALID_KEY;
    }
    
    //creates a new_note by mallocing memory to the new_note.
    struct note* new_note = malloc(sizeof *new_note);
    
    /*chekcs if the beat is empty and then adds the note,
    with assigning the octave and key values to the 
    new_note->octave and key*/

    if (beat->notes == NULL) {
        
        beat->notes = new_note;
        new_note->octave = octave;
        new_note->key = key;
        new_note->next = NULL;
    
    }
    
    // /if the beat is not empty then it adds the note in the end
    //of the list of notes
    else {
        
        struct note* add_note_next = beat->notes;
        
        //loops through all the notes already addeed.
        while (add_note_next -> next != NULL) {
            add_note_next = add_note_next->next;
        }
        
        /*checks whether the octave is less than the previous note octave 
        ,the key is less than previous note or the note is same as previous
        note and if it is, then it frees the note and returns NOT_HIGHEST_NOTE*/
        if (octave < add_note_next->octave ||
        (octave == add_note_next->octave && key < add_note_next->key) ||
        (octave == add_note_next->octave && key == add_note_next->key)) {
            free(new_note);
            return NOT_HIGHEST_NOTE;
        } 
        
        //adds the new_note after the note which 
        //was the last note previously.
        add_note_next->next = new_note;
        new_note->octave = octave;
        new_note->key = key;
        new_note->next = NULL;

    }

    return VALID_NOTE;

}

// Print the contents of a beat.
void print_beat (Beat beat) {
    
    struct note* beat_printer = beat->notes;
    
    //checks if the beat is empty
    if (beat_printer == NULL) {
        printf ("\n");
    }

    else {
        
        while (beat_printer != NULL) {
            
            //if a key in a note is less than 10 then it prints 
            //a 0 before the key.
            if (beat_printer->key <10) {
                printf ("%d 0%d", beat_printer->octave, beat_printer->key);
            }
            
            //if key is more than 10 then it prints just the key number
            if (beat_printer->key >= 10) {
                printf ("%d %d", beat_printer->octave, beat_printer->key);
            }
            
            //breaks when it reaches the last note to prevent the 
            //printing of " | " after the last note.
            if (beat_printer->next == NULL) {
                break;
            }
            
            beat_printer = beat_printer->next;
            //prints "|" after every note.
            printf (" | ");
        }
        
        printf ("\n");
    
    }

}

// Counts the number of notes in a beat that are in a given octave.
int count_notes_in_octave (Beat beat, int octave) {
    
    int num_notes_octave = 0;
    struct note*  octave_counter = beat->notes;
    
    while (octave_counter != NULL) {
        
        //when the octave of a note equals to the 'octave',
        //num_notes_octave is incremented, to know num of notes
        //in a octave.
        if (octave_counter->octave == octave) {
            num_notes_octave++;
        }
        octave_counter = octave_counter->next;

    }
    return num_notes_octave;

}

////////////////////////////////////////////////////////////////////////
//                         Stage 2 Functions                          //
////////////////////////////////////////////////////////////////////////

// Return a malloced track with fields initialized.
Track create_track(void) {
    Track new_track = malloc (sizeof *new_track);
    new_track->current = NULL;
    new_track->head = NULL;
    return new_track;

}

// Add a beat after the current beat in a track.
void add_beat_to_track(Track track, Beat beat) {
    
    //when the current is null, the beats are added at the head of track.
    if (track->current == NULL) {
        beat->next = track->head;
        track->head = beat;
    }
    
    //when the current is not null all new beats are added after current.
    else {
        beat->next = track->current->next;
        track->current->next = beat;
    }

}

// Set a track's current beat to the next beat.
int select_next_beat(Track track) {
    
    //when the current is null current beat becomes the head.
    if (track->current == NULL) {
        track->current = track->head;
    }
    
    //otherwise the current always moves to the next beat.
    else {
        track->current = track->current->next;
    }
    
    //whenever current reaches the end of beats the track is stopped
    if (track->current == NULL) {
        return TRACK_STOPPED;
    }
    
    return TRACK_PLAYING;
}

// Print the contents of a track.
void print_track(Track track) {

    Beat track_printer = track->head;
    int beat_counter = 0;
    
    //looping through all the beats in the track
    while (track_printer != NULL) {
        
        //counts the beat number to denote it in 
        //front of beats while printing.
        beat_counter++;
        
        if (track->current == track_printer) {
            /*when loop reaches the current beat it prints >
            to denote the current beat*/
            printf(">[%d] ", beat_counter);
        }

        //other than current all beats are denoted by just beat number
        //in front of beat. 
        else {
            printf(" [%d] ", beat_counter);
        }
        
        //prints the notes in the beat
        print_beat(track_printer);
        track_printer = track_printer->next;

    }

    return;
}

// Count beats after the current beat in a track.
int count_beats_left_in_track(Track track) {
    
    Beat beats_mover = track->current;
    int beats_counter_left = 0;
    int current_checker = 0;
    
    /*when the current is null it sets the beat_mover to head
    and current_checker to current_null to count all
    beats in the track*/
    
    if (track->current == NULL) {
        beats_mover = track->head;
        current_checker = CURRENT_NULL;
    }
    
    //counts the beats in track including current when current
    //is not null.
    while (beats_mover != NULL) {
        beats_counter_left++;
        beats_mover = beats_mover->next;
    }
    
    //if current is null then it returns all the beats in the track.
    if (current_checker == CURRENT_NULL) {
        return beats_counter_left;
    }
    
    //apart from current being null it always returns 
    //the number of all the beats except current.
    return (beats_counter_left-1);

}

////////////////////////////////////////////////////////////////////////
//                         Stage 3 Functions                          //
////////////////////////////////////////////////////////////////////////

// Free the memory of a beat, and any memory it points to.
void free_beat(Beat beat) {
    
    //when there is no note in the beat it directly frees the beat
    if (beat -> notes == NULL) {
        free(beat);
    }
    
    //when there are notes in the beat.
    else {
        
        /*removes all the notes by looping through the beat and resets 
        the beat->notes at every iteration before freeing the previous 
        note, for not losing the first value of linked list every time 
        a note is removed*/
        struct note* beat_remover = beat->notes;
        while (beat_remover != NULL) {
            
            //sets beat->notes to its next note 
            beat->notes = beat->notes->next;
            
            //removes the previous note
            free(beat_remover);
            beat_remover = beat->notes;
        }
        
        //frees the beat in the end after removing all the notes
        free(beat);
    }

}

// Free the memory of a track, and any memory it points to.
void free_track(Track track) {

    //when there is no beat it directly frees the track
    if (track->head == NULL) {
        free(track);
    }
    
    //when there are beats in the track.
    else {
        
        /*removes all the beats by looping through the beats and resets 
        the track->head at every iteration before freeing the previous 
        beat for not losing the first value of linked list every 
        time a beat is removed.*/
        Beat track_free_beat = track->head;
        
        while (track_free_beat != NULL) {
            
            //sets track->head to its next beat
            track->head = track->head->next;
            
            //removes te previous beat
            free_beat(track_free_beat);
            track_free_beat = track->head;
        }
        
        //frees the beat in the end after removing all the beats.
        free(track);
    }
    
}

// Remove the currently selected beat from a track.
int remove_selected_beat(Track track) {
    
    //when the current is null then the track is stopped and no
    //beat is removed.
    if (track->current == NULL) {
        return TRACK_STOPPED;
    }
    
    //when current is head,current is moved to next beat and
    //head is removed. Therafter, head is reset to new current.
    if (track->current == track->head) {
        
        //current moves to next beat
        track->current = track->current->next;
        
        //head is removed
        free_beat(track->head);
        
        //head is changed to new current
        track->head = track->current;
        if (track->current != NULL) {
            return TRACK_PLAYING;
        }
    
    }

    else {
        
        //when the current is not head or null then we 
        //loop through to the beat before current
        Beat selected_beat_free = track->head;
        while (selected_beat_free->next != track->current) {
            selected_beat_free = selected_beat_free->next;
        }
        
        //beat to be removed is set to the next beat of 
        //beat before current.
        Beat remove_beat = selected_beat_free->next;
        
        //new_current is set to the next position of beat to be removed.
        Beat new_current = remove_beat->next;
        
        //link between the beat before current and remove_beat is 
        //broken and it is set with new current.
        selected_beat_free->next = new_current;

        //remove_beat is then freed.
        free_beat(remove_beat);
        
        //current is then reset to new_current.
        track->current = new_current;
        
        //track keeps on playing until the current becomes null
        if (track->current != NULL) {
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

    int octave =0;
    int key = 0;
    int i = 0;
    
    //loops through all the contents of musical_notation added
    while (musical_notation[i] != '\0') {
        
        //checks whether the first character is a digit or not.
        if (i == 0) {
            
            //if the first charcter is not a digit then 
            //it is an invalid_musical_note.
            if (musical_notation[i] < '0' || musical_notation[i] > '9') {
                return INVALID_MUSICAL_NOTE;
            }
            
            //otherwise the ASCII code of digits is converted 
            //to its integer equivalent and stored in the octave.
            octave = atoi(&musical_notation[i]);

        }
        
        //conditions for second character
        if (i == 1) {
            
            //if the second character is not a alphabet from 'A' to 'G' 
            //then it is an invalid_musical_note
            if (musical_notation[i] < 'A' || musical_notation[i] > 'G') {
                return INVALID_MUSICAL_NOTE;
            }
            
            //initialising an array of characters containing 
            //all characters from 'A' to 'G'.
            char music_notes[NUM_NOTES] = "ABCDEFG";

            //intialising an integer array for the 
            //key equivalent of characters.
            int key_conversion[NUM_NOTES-1] = {0,2,3,5,7,8,10};
            int j = 0;

            while (music_notes[j] != '\0') {
               
                /*when second character of the musical_notation equals to 
                the character in music_notes then the key for note 
                is set to the integer equivalent in key_conversion.*/
                if (musical_notation[i] == music_notes[j]) {
                    key = key_conversion[j];
                }

                j++;
            }
        
        }
        
        //conditions for characters after the second character.
        if (musical_notation[i] == '#' && i > 1) {
            
            //key is incremented for every # in musical_notation
            key++;
            if (key > 11) {
                //octave is increased when key = 12
                if (key == 12) {
                    octave++;
                }
                //key is reduced by 12 after it surpasses 11.
                key = key - 12;
            }

        }
        
        //if any character appears other than '#' after 
        //the second character then it is invalid. 
        if (i > 1 && musical_notation[i] != '#') {
            return INVALID_MUSICAL_NOTE;
        }
        
        i++;

    }
    
    //mallocs the memory for the new musical_note to be added.
    struct note* musical_note = malloc(sizeof *musical_note);
    
    //when the beat is empty then the note is directly added 
    //at the first position.
    if (beat->notes == NULL) {
        
        beat->notes = musical_note;
        musical_note->octave = octave;
        musical_note->key = key;
        musical_note->next = NULL;
    
    }
    
    //when the beat is not empty then the note is added 
    //in ascending order.
    else {
        
        /*function add_musical_ascending adds the note in 
        ascending order and returns whether note is 
        valid or invalid.*/
        int valid_or_invalid = add_musical_note_ascending(beat, key, octave, musical_note);
        return valid_or_invalid;
    
    }
    
    return VALID_NOTE;     
    
}

int add_musical_note_ascending (Beat beat, int key, int octave, struct note* musical_note) {
    
    struct note* musical_note_add = beat->notes;
    struct note* previous_note = NULL;
    //adding the musical_note in the ascending order
    while(musical_note_add != NULL){
            
        //when the octave and key are repeated then it is an 
        //invalid_note and the memory malloced is freed for it directly.
        if (musical_note_add->octave == octave && musical_note_add->key == key) {
            free(musical_note);
            return INVALID_MUSICAL_NOTE;
        }
            
        //condition for adding the note after the first note in ascending order
        else if (previous_note == NULL && ((musical_note_add->octave > octave) || 
            (musical_note_add->octave == octave && musical_note_add->key > key))) {
                
            //makes the musical_note to be added as the first beat 
            //when the first note octave and key are more than octave and key.
            beat->notes = musical_note;
            musical_note->octave = octave;
            musical_note->key = key;
            //adds the previous first note to be the 
            //next of musical_note added.
            musical_note->next = musical_note_add;
            return VALID_NOTE;
            
        }
            
        /*when the looped note's octave is greater than octave and 
        previous octave is less than or equal to octave, then we will 
        break the loop and add the musical_note in between*/
        else if (previous_note != NULL && (musical_note_add->octave > octave) 
        && (previous_note->octave <= octave)) {
            break;
        }
            
        /*when the looped note's octave is equal to the octave and previous key 
        is less than key and the current note key is greater than key then 
        we will break the loop and add the musical_note is added in between*/
        else if (previous_note != NULL && (previous_note->octave == octave)
        && (previous_note->key < key) && (musical_note_add->key > key)) {
            break;
            
        }
        
        //moves the previous note and musical_note_add ahead in every iteration.
        previous_note = musical_note_add;
        musical_note_add = musical_note_add->next;
        
    }
    
    /*when the loop breaks, then the musical_note is added in between
    of the previous_note and musical_note_add, and its octave and key 
    are set to octave and key.*/
    previous_note->next = musical_note;
    musical_note->octave = octave;
    musical_note->key = key;
    musical_note->next = musical_note_add;

    return VALID_NOTE;
}


////////////////////////////////////////////////////////////////////////
//                         Stage 5 Functions                          //
////////////////////////////////////////////////////////////////////////

// Cut a range of beats to the end of a track.
void cut_range_to_end(Track track, int range_length) {
    
    //when the range is negative and current beat is not set then
    //nothing should happen.
    if (range_length <= 0 || track->current == NULL) {
        return;
    } 

    Beat current_to_last = track->current;
    int beats_current_to_last = 0;
    
    //counts the number of beats from current to last beat 
    //including current beat.
    while (current_to_last != NULL) {
        beats_current_to_last++;
        current_to_last = current_to_last->next;
    }
    
    //when range given is more than beats from current to last
    //then it does nothing.
    if (range_length >= beats_current_to_last) {
        return;
    }
    
    Beat last_beat = track->head;
    
    //looping to the last beat in the track.
    while (last_beat->next != NULL) {
        last_beat = last_beat->next;
    }
    
    //Finding the last beat in the range_length given.
    Beat range_last_counter = track->current;
    int number_of_cuts = 1;
    while (number_of_cuts < range_length) {
        range_last_counter = range_last_counter->next;
        number_of_cuts++;
    }
    
    //Finding the beat_before_current
    Beat beat_before_current = track->head;
    
    //when beat before current is current and head.
    if (beat_before_current == track->current) {
        
        //head becomes the next beat after the 
        //last beat in the range
        track->head = range_last_counter->next;
        
        //current is cut from the original position 
        //and it is attached after last_beat
        last_beat->next = track->current;
        
        //and the last beat in the range becomes 
        //the new last_beat in the track
        range_last_counter->next = NULL;
    
    }
    
    //conditions for checking when beat_before_current 
    //is not current or head.
    else {
        
        //looping to the beat_before_current 
        while (beat_before_current->next != track->current) {
            beat_before_current = beat_before_current->next;
        }
        
        /*beat_before_current breaks the link with current and
        forms a link with the beat after last beat in the range.*/
        beat_before_current->next = range_last_counter->next;
        
        //current is cut from the original position 
        //and is attached after the last_beat.
        last_beat->next = track->current;
        
        //the last_beat in the range becomes the new 
        //last_beat in the track.
        range_last_counter->next = NULL;

    }

}

// Reverse a list of beats within a range of a track.
int reverse_range(Track track, int range_length) {
    
    //when the range is negative and track is stopped 
    //then it returns 0 beats are reversed.
    if (range_length < 0 || track->current == NULL) {
        return 0;
    }
    
    Beat current_to_last = track->current;
    int beats_current_to_last = 0;
    
    //counts the number of beats from current to 
    //last beat including current beat.
    while (current_to_last != NULL) {
        beats_current_to_last++;
        current_to_last = current_to_last->next;
    }
    
    /*when range of reversal given is more than beats 
    ahead of current then range of reversal is equal 
    to the beats ahead of current.*/
    if (range_length > beats_current_to_last) {
        range_length = beats_current_to_last;
    }
    
    //finds the beat before current.
    Beat beat_before_current = track->head;
    int reversed_beats = 0;

    //when the beat_before_current is equal to the head and current.
    if (beat_before_current == track->current) {
        
        /*reversed_beats_current function is called when 
        before_current is equal to current, which reverses 
        the range of beats giving the condition of CURRENT_EQUAL 
        denoting a special reversal.*/
        int before_current_or_equal = CURRENT_EQUAL;
        reversed_beats = reverse_beats_current (track, beat_before_current, range_length, before_current_or_equal);
    
    }
    
    //when beat_before_current not equals to the current
    else {
        
        //looping to the beat before current.
        while (beat_before_current->next != track->current) {
            beat_before_current = beat_before_current->next;
        }
        
        /*reversed_beats_current function is called when 
        beat_before_current is not current, which reverses 
        the range of beats gicing the condition BEFORE_CURRENT,
        which reverses the beats whenever current not equals head.*/
        int before_current_or_equal = BEFORE_CURRENT;
        reversed_beats = reverse_beats_current (track,beat_before_current, range_length, before_current_or_equal);
        
    }
    
    return reversed_beats;
}

int reverse_beats_current (Track track, Beat beat_before_current, int range_length, int before_current_or_equal) {

    int num_beats_reverse = 0;
    Beat current_beat = track->current;
    Beat beat_after_current = current_beat->next;
    Beat beat_after_current_next = NULL;

    /*looping till the end of the range and reversing 
    all the beats in the range with also counting 
    the number of beats reversed.*/   
    while (num_beats_reverse < (range_length-1)) {

        //beat_after_current_next is set to the 
        //beat after the beat_after_current.        
        beat_after_current_next = beat_after_current->next;
        
        /*beat_after_curret breaks the link with its 
        next beat and forms new link with current_beat,
        to begin the reversing of list.*/
        beat_after_current->next = current_beat;
        
        //current_beat is set to its next beat.
        current_beat = beat_after_current;
        
        //beat_after_current is set to its next beat 
        //and the reversal continues,till all beats 
        //in range are reversed.
        beat_after_current = beat_after_current_next;
        num_beats_reverse++;
        
    }
    
    //when the beat_before_current is current
    if (before_current_or_equal == CURRENT_EQUAL) {
        
        //starting_beat is set to head of track,
        Beat starting_beat = track->head;
        
        /*starting_beat breaks its link with its 
        next beat and makes a new link, with beat existing 
        next to the range of beats to be reversed.*/
        starting_beat->next = beat_after_current;
        
        //The head of track is reset to the last beat 
        //in range of beats to be reversed,
        //which is current_beat after reversing.
        track->head = current_beat;

    }
    
    //when the beat_before_current is not 
    //current and it exists before current
    if (before_current_or_equal == BEFORE_CURRENT) {

        /*After reversing the beats, current breaks its existing 
        link and makes new link with the beat existing after the
        last_beat in the range of list to be reversed.*/
        track->current->next = beat_after_current;

        /*After reversing beat_before_current forms a new link 
        with the last beat in the range of lists to be reversed 
        which is the current_beat after reversing.*/
        beat_before_current->next = current_beat;

    }

    return num_beats_reverse;

}
