#include <stdio.h>
#include <time.h>
#include <unistd.h>  // For sleep function

int main() {
    int alarmHour, alarmMinute;
    time_t currentTime;
    struct tm *localTime;
    
    // Get the current time
    time(&currentTime);
    localTime = localtime(&currentTime);
    
    // Input alarm time
    printf("Enter alarm time (24-hour format)\n");
    printf("Hour: ");
    scanf("%d", &alarmHour);
    printf("Minute: ");
    scanf("%d", &alarmMinute);
    
    // Ensure the alarm time is valid
    if (alarmHour < 0 || alarmHour > 23 || alarmMinute < 0 || alarmMinute > 59) {
        printf("Invalid time entered.\n");
        return 1;
    }
    
    printf("Alarm set for %02d:%02d\n", alarmHour, alarmMinute);
    
    // Loop until the current time matches the alarm time
    while (1) {
        time(&currentTime);
        localTime = localtime(&currentTime);
        
        if (localTime->tm_hour == alarmHour && localTime->tm_min == alarmMinute) {
            printf("ALARM! The time is now %02d:%02d\n", localTime->tm_hour, localTime->tm_min);
            break;
        }
        
        sleep(30);  // Check every 30 seconds
    }
    
    return 0;
}
