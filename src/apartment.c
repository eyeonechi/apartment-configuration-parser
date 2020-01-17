/* Program concept and description :                                       *
 * A simple linked-list implementation of the assignment.                  *
 * apart_t is a linked list with room_t linked lists in each node.         *
 *           ...         ...         ...         ...                       *
 *            ^           ^           ^           ^                        *
 *          room_t      room_t      room_t      room_t                     *
 *            ^           ^           ^           ^                        *
 *          room_t      room_t      room_t      room_t                     *
 *            ^           ^           ^           ^                        *
 *         apart_t --> apart_t --> apart_t --> apart_t --> ...             *
 *                                                                         *
 * Each time a room is read, a new room_t node is chained to the previous. *
 * New apart_t nodes are chained when a new apartment is read.             *
 * Reading, printing and freeing are done via recursion where the 'next'   *
 * pointer is passed on into subsequent recursive calls, which terminate   *
 * when MAX_APART or MAX_ROOMS is reduced to 0, or when an empty node is   *
 * reached. (List terminates with a zero-value-initialised node)           */

/***************************************************************************/
 
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/***************************************************************************/

/* Maximum allowed apartment data */
#define MAX_APART 100 /* Maximum number of apartments  */
#define MAX_ROOMS 100 /* Maximum number of rooms       */
#define END_ROOMS  -1 /* Sentinel value : End of rooms */

/* Differentiates apartment areas */
#define DRY 3
#define UTI 7

/* Stage 2 and 3 related printing constants */
#define APT_NUM "Apartment %d\n-------------\n"
#define ROOM    "    %-8s %1d %5.2f %5.2f "
#define NO_AREA "  ---\n"
#define AREA    "%6.2f\n"
#define TOTAL   "    Total area %18.2f metres^2\n\n"

/* Return values for checkNextRoom function */
#define SAME_ROOM 1
#define DIFF_ROOM 0

/* Stage 4 related printing constants */
#define DIV "+-------+----------------+----------------+----------------+\n"
#define HDR "| Apart |    Dry areas   |    Wet areas   |  Utility areas |\n"
#define PRT "|  %3d  | %6.2f %5.1f%%  | %6.2f %5.1f%%  | %6.2f %5.1f%%  |\n"

/***************************************************************************/

typedef struct apart_t* Apart; /* Pointer to Apart struct */
typedef struct room_t*  Room;  /* Pointer to Room struct  */

/* Apart structure */
struct apart_t {
	int    num;        /* Apartment number        */
	double dryArea;    /* Total dry area          */
	double dryPercent; /* Dry area percentage     */
	double wetArea;    /* Total wet area          */
	double wetPercent; /* Wet area percentage     */
	double utiArea;    /* Total utility area      */
	double utiPercent; /* Utility area percentage */
	double totArea;    /* Total area              */
	Room   room;       /* Room list head pointer  */
	Apart  next;       /* Next apartment pointer  */
};

/* Room structure */
struct room_t {
	int    type;       /* Room type               */
	int    num;        /* Room number             */
	double xsize;      /* x dimension of room     */
	double ysize;      /* y dimension of room     */
	double area;       /* Area of room            */
	Room   next;       /* Next room pointer       */
};

/***************************************************************************/

/* Function prototypes */
Apart createApartment();
Apart newApartment();
Room  newRoom();
void  readApartment(Apart apt);
void  recursiveReadApartment(Apart apt, int lim);
void  recursiveReadRoom(Apart apt, Room room, int lim);
void  calcRoomArea(Apart apt, Room room);
void  calcApartArea(Apart apt);
void  printApartment(Apart apt);
void  recursivePrintApartment(Apart apt, char **areas);
void  recursivePrintRoom(Room room, char **areas);
void  printSummary(Apart apt);
void  recursivePrintSummary(Apart apt);
void  freeApartment(Apart apt);
void  recursiveFreeApartment(Apart apt);
void  recursiveFreeRoom(Room room);
int   checkNextRoom(Room room);

/***************************************************************************/

/* Handles the process of reading and printing apartment data */
int main(int argc, char **argv) {
	Apart apt = createApartment();
	readApartment(apt);
	printApartment(apt);
	freeApartment(apt);
	return EXIT_SUCCESS;
}

/***************************************************************************/

/* Parent function : main()                                 *
 * Handles creation of the initial apart_t and room_t nodes */
Apart createApartment() {
	Apart apt = newApartment();
	apt->room = newRoom();
	return apt;
}

/***************************************************************************/

/* Returns a pointer to a new, empty Apart structure */
Apart newApartment() {
	Apart apartment = (Apart)calloc(sizeof(*apartment), sizeof(*apartment));
	assert(apartment);
	return apartment;
}

/***************************************************************************/

/* Returns a pointer to a new, empty Room structure */
Room newRoom() {
	Room room = (Room)calloc(sizeof(*room), sizeof(*room));
	assert(room);
	return room;
}

/***************************************************************************/

/* Parent function : main()       *
 * Handles reading apartment data */
void readApartment(Apart apt) {
	recursiveReadApartment(apt, MAX_APART);
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : readApartment()                          *
 * Reads apartment numbers. Terminates when lim becomes 0     */
void recursiveReadApartment(Apart apt, int lim) {
	if (lim && apt && scanf("%d\n", &apt->num) == 1) {
		recursiveReadRoom(apt, apt->room, MAX_ROOMS);
		calcApartArea(apt);

		/* Chains a new apart_t node at the end */
		apt->next = newApartment();
		apt->next->room = newRoom();
		recursiveReadApartment(apt->next, lim - 1);
	}
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : recursiveReadApartment()            *
 * Reads room details. Terminates when lim becomes 0     */
void recursiveReadRoom(Apart apt, Room room, int lim) {
	if (lim && room && scanf("%d", &room->type)
		&& room->type != END_ROOMS) {
		scanf("%d %lf %lf", &room->num, &room->xsize, &room->ysize);
		calcRoomArea(apt, room);

		/* Chains a new room_t node at the end */
		room->next = newRoom();
		recursiveReadRoom(apt, room->next, lim - 1);
	}
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : recursiveReadApartment()                  *
 * Calculates apartment subsection areas and their percentages */
void calcApartArea(Apart apt) {
	apt->totArea += apt->dryArea + apt->wetArea + apt->utiArea;
	if (apt->totArea) {
		apt->dryPercent = apt->dryArea * 100 / apt->totArea;
		apt->wetPercent = apt->wetArea * 100 / apt->totArea;
		apt->utiPercent = apt->utiArea * 100 / apt->totArea;
	}
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : recursiveReadRoom()                   *
 * Increases apartment subsection areas based on room type */
void calcRoomArea(Apart apt, Room room) {                                                                                     
	room->area = room->xsize * room->ysize;
	apt->dryArea += (room->type <= DRY) * room->area;
	apt->utiArea += (room->type >= UTI) * room->area;
	apt->wetArea += (room->type > DRY && room->type < UTI) * room->area;
}

/***************************************************************************/

/* Parent function : main()                         *
 * Handles output printing following specifications */
void printApartment(Apart apt) {
	char *areas[] = {"Hallway", "Bedroom", "Living", "Bathroom", "Kitchen",
	                 "Laundry", "Storage", "Garage", "Balcony"};
	recursivePrintApartment(apt, areas);
	printSummary(apt);
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : printApartment()          *
 * Prints stage 2 and 3 output (apartments)    *
 * Terminates upon reaching empty apart_t node */
void recursivePrintApartment(Apart apt, char **areas) {
	if (apt && apt->num) {
		printf(APT_NUM, apt->num);
		recursivePrintRoom(apt->room, areas);
		printf(TOTAL, apt->totArea);
		recursivePrintApartment(apt->next, areas);
	}
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : recursivePrintApartment() *
 * Prints stage 2 and 3 output (rooms)         *
 * Terminates upon reaching empty room_t node  */
void recursivePrintRoom(Room room, char **areas) {
	if (room && room->num) {
		printf(ROOM, areas[room->type - 1], room->num,
			room->xsize, room->ysize);
		room->next->area += room->area * checkNextRoom(room);
		recursivePrintRoom(room->next, areas);
	}
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : recursivePrintRoom()                    *
 * Returns 1 if room->next is identical to room, 0 otherwise */
int checkNextRoom(Room room) {
	if (room->type == room->next->type && room->num == room->next->num) {
		printf(NO_AREA);
		return SAME_ROOM;
	}
	printf(AREA, room->area);
	return DIFF_ROOM;
}

/***************************************************************************/

/* Parent function : printApartment() *
 * Prints stage 4 output              */
void printSummary(Apart apt) {
	printf(DIV);
	printf(HDR);
	printf(DIV);
	recursivePrintSummary(apt);
	printf(DIV);
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : printSummary()            *
 * Prints stage 4 ourput (areas)               *
 * Terminates upon reaching empty apart_t node */
void recursivePrintSummary(Apart apt) {
	if (apt && apt->num) {
		printf(PRT, apt->num, apt->dryArea, apt->dryPercent, apt->wetArea,
			apt->wetPercent, apt->utiArea, apt->utiPercent);
		recursivePrintSummary(apt->next);
	}
}

/***************************************************************************/

/* Parent function : main()                   *
 * Handles freeing apartment allocated memory */
void freeApartment(Apart apt) {
	recursiveFreeApartment(apt);
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : freeApartment()        *
 * Frees memory allocated to apart_t struct */
void recursiveFreeApartment(Apart apt) {
	recursiveFreeRoom(apt->room);
	if (apt && apt->num) {
		recursiveFreeApartment(apt->next);
	}
	free(apt);
	apt = NULL;
}

/**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^**/

/* Parent function : recursiveFreeApartment() *
 * Frees memory allocated to room_t struct    */
void recursiveFreeRoom(Room room) {
	if (room && room->num) {
		recursiveFreeRoom(room->next);
	}
	free(room);
	room = NULL;
}