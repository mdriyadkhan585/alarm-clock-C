#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // For sleep function

#define MAX_ALARMS 10

typedef struct {
    int hour;
    int minute;
    int second;
    int active;
} Alarm;

void print_current_time(struct tm *localTime) {
    printf("\nCurrent time: %02d:%02d:%02d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
}

void print_alarms(Alarm alarms[], int count) {
    printf("\nActive Alarms:\n");
    if (count == 0) {
        printf("No alarms set.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        if (alarms[i].active) {
            printf("Alarm %d: %02d:%02d:%02d\n", i + 1, alarms[i].hour, alarms[i].minute, alarms[i].second);
        }
    }
}

int convert_to_24_hour(int hour, char period) {
    if (period == 'P' && hour != 12) {
        return hour + 12;
    } else if (period == 'A' && hour == 12) {
        return 0;
    }
    return hour;
}

void set_alarm(Alarm alarms[], int *count) {
    if (*count >= MAX_ALARMS) {
        printf("Maximum number of alarms reached.\n");
        return;
    }

    Alarm newAlarm;
    int hour12;
    char period;

    printf("\nEnter alarm time (12-hour format):\n");
    printf("Hour (1-12): ");
    scanf("%d", &hour12);
    printf("Minute (0-59): ");
    scanf("%d", &newAlarm.minute);
    printf("Second (0-59): ");
    scanf("%d", &newAlarm.second);
    printf("AM/PM (A/P): ");
    scanf(" %c", &period);  // Note the space before %c to skip whitespace

    // Ensure the input values are valid
    if (hour12 < 1 || hour12 > 12 || newAlarm.minute < 0 || newAlarm.minute > 59 || newAlarm.second < 0 || newAlarm.second > 59 || (period != 'A' && period != 'P')) {
        printf("Invalid input entered. Please try again.\n");
        return;
    }

    newAlarm.hour = convert_to_24_hour(hour12, period);
    newAlarm.active = 1; // Set the alarm as active
    alarms[*count] = newAlarm;
    (*count)++;
    printf("Alarm set for %02d:%02d:%02d %cM\n", hour12, newAlarm.minute, newAlarm.second, period);
}

void cancel_alarm(Alarm alarms[], int *count) {
    int alarmNumber;
    print_alarms(alarms, *count);
    printf("\nEnter the number of the alarm to cancel: ");
    scanf("%d", &alarmNumber);
    
    if (alarmNumber < 1 || alarmNumber > *count) {
        printf("Invalid alarm number.\n");
        return;
    }

    alarms[alarmNumber - 1].active = 0;
    printf("Alarm %d cancelled.\n", alarmNumber);
}

void menu(Alarm alarms[], int *count) {
    char option[10];

    while (1) {
        time_t currentTime;
        struct tm *localTime;
        time(&currentTime);
        localTime = localtime(&currentTime);

        print_current_time(localTime);

        printf("\nOptions:\n");
        printf("1. Set Alarm\n");
        printf("2. Cancel Alarm\n");
        printf("3. Show Alarms\n");
        printf("4. Done\n");
        printf("Choose an option: ");
        
        fgets(option, sizeof(option), stdin);
        option[strcspn(option, "\n")] = 0;  // Remove newline character

        if (strcmp(option, "1") == 0) {
            set_alarm(alarms, count);
        } else if (strcmp(option, "2") == 0) {
            cancel_alarm(alarms, count);
        } else if (strcmp(option, "3") == 0) {
            print_alarms(alarms, *count);
        } else if (strcmp(option, "4") == 0) {
            printf("Alarm setup complete. Monitoring alarms...\n");
            break;
        } else {
            printf("Invalid option. Please choose again.\n");
        }
    }
}

int main() {
    Alarm alarms[MAX_ALARMS];
    int alarmCount = 0;

    menu(alarms, &alarmCount);

    // Alarm monitoring loop
    while (1) {
        time_t currentTime;
        struct tm *localTime;
        time(&currentTime);
        localTime = localtime(&currentTime);

        // Check if any alarm is due
        for (int i = 0; i < alarmCount; i++) {
            if (alarms[i].active &&
                localTime->tm_hour == alarms[i].hour &&
                localTime->tm_min == alarms[i].minute &&
                localTime->tm_sec == alarms[i].second) {
                printf("\nALARM! The time is now %02d:%02d:%02d\n", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
                alarms[i].active = 0; // Deactivate alarm after it goes off
            }
        }

        sleep(1); // Sleep for a second before checking again
    }

    return 0;
}
