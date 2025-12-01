#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int starttime;
    int endtime;
    int index;
}Schedule;
Schedule* s = NULL;
int num;

void sortByEndingTime() {
    for (int i = 0; i < num-1; i++) {
        for (int j = i + 1; j < num; j++) {
            if (s[i].endtime > s[j].endtime) {
                Schedule temp = s[i];
                s[i] = s[j];
                s[j] = temp;
            }
        }
    }
}

void selectActivity() {
    sortByEndingTime();

    printf("\nSelected Activities:\n");

    int lastEndTime = s[0].endtime;
    printf("Activity %d: (%d -> %d)\n", s[0].index, s[0].starttime, s[0].endtime);

    for (int i = 1; i < num; i++) {
        if (s[i].starttime >= lastEndTime) {
            printf("Activity %d: (%d -> %d)\n", s[i].index, s[i].starttime, s[i].endtime);
            lastEndTime = s[i].endtime;
        }
    }
}

int main() {
    printf("Enter the number of entries: ");
    scanf("%d", &num);
    s = malloc(num*sizeof(Schedule));
    for (int i=0; i<num; i++) {
        printf("\nEnter starting time: ");
        scanf("%d", &s[i].starttime);
        printf("\nEnter ending time: ");
        scanf("%d", &s[i].endtime);
        s[i].index = i+1;

        if (s[i].starttime >= s[i].endtime) {
            printf("Invalid: start time must be < end time!\n");
            i--;
        }
    }

    selectActivity();
    free(s);
    return 0;
}