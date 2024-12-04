#include <string.h>
#include "Raylib.h"
#include "message.h"


static char messages[MAX_MESSAGES][MAX_MESSAGE_LENGTH] = {'\0'}; // This stores actual text showing the screen.
static int messages_type[MAX_MESSAGES] = { 0 }; // This stores message type, for coloring text

int message_insert(char *message, int type)
{
	// Error checking: message length
	if (strlen(message) > MAX_MESSAGE_LENGTH - 1){	
		printf("ERROR: Too long message.");
		exit(1);
	}
    
	// Find empty space in the array, if find, insert new message and return
	for (int i = 0; i < MAX_MESSAGES; i++){
		if (*messages[i] == '\0') {
			strcpy_s(messages[i], MAX_MESSAGE_LENGTH, message);
			messages_type[i] = type;
			return 0; // return on success to insert
		}
	}
	
	// if there were no empty space in the array, insert at the end of it after copying the rest
	for (int i = 0; i < MAX_MESSAGES - 1; i++){
		strcpy_s(messages[i], MAX_MESSAGE_LENGTH, messages[i + 1]);
		messages_type[i] = messages_type[i + 1];
	}
	
	strcpy_s(messages[MAX_MESSAGES - 1], MAX_MESSAGE_LENGTH, message);
	messages_type[MAX_MESSAGES - 1] = type;
    
	return 0;
}

void message_show(int text_size, int starts_from_x, int starts_from_y)
{
	// Show text in the message array with corresponded colors
	for (int i = 0; i < MAX_MESSAGES; i++){
		switch(messages_type[i]) {
            case MESSAGE_NARATIVE: 
			DrawText(messages[i], starts_from_x, starts_from_y + (i * text_size), text_size, YELLOW);
			break;
            case MESSAGE_SYSTEM:
			DrawText(messages[i], starts_from_x, starts_from_y + (i * text_size), text_size, WHITE);
			break;
            case MESSAGE_COMBAT:
			DrawText(messages[i], starts_from_x, starts_from_y + (i * text_size), text_size, RED);
			break;
		}
	}
}