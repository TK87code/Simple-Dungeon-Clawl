#ifndef MESSAGE_H
#define MESSAGE_H

#define TEXT_SIZE 20
#define MAX_MESSAGE_LENGTH 100
#define MAX_MESSAGES 6

enum {
	MESSAGE_NARATIVE = 1, // yellow character
	MESSAGE_SYSTEM, // white character
	MESSAGE_COMBAT, // RED character
};

extern int message_insert(char *message, int type);
extern void message_show(int text_size, int starts_from_x, int starts_from_y);


#endif // !MESSAGE_H