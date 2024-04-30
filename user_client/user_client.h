//
// Created by nikitabakunovich on 22.03.24.
//

#ifndef COURSE_WORK_USER_CLIENT_H
#define COURSE_WORK_USER_CLIENT_H

int user_client();

void receive_message(int socket, char* buffer);
void send_message(int socket, const char* message);
bool parse_shot(const char* coordinates, int* x, int* y);

#endif //COURSE_WORK_USER_CLIENT_H
