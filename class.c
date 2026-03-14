#include "class.h"

char kname[2][10] = {"A+~F", "P/F"};

int loadData(struct st_class* c[]) {
    int count = 0;
    FILE* file = fopen("classes.txt", "r");
    if (file == NULL) return 0;

    while (!feof(file)) {
        c[count] = (struct st_class*)malloc(sizeof(struct st_class));
        int r = fscanf(file, "%d %s %d %d", &(c[count]->code), c[count]->name, &(c[count]->unit), &(c[count]->grading));
        if (r < 4) {
            free(c[count]);
            break;
        }
        count++;
    }
    fclose(file);
    return count;
}

void printAllClasses(struct st_class* c[], int csize) {
    for (int i = 0; i < csize; i++) {
        printf("[%d] %s [credit %d - %s]\n", c[i]->code, c[i]->name, c[i]->unit, kname[c[i]->grading - 1]);
    }
}

void saveAllClasses(struct st_class* c[], int csize) {
    FILE* file = fopen("classes.txt", "w");
    for (int i = 0; i < csize; i++) {
        fprintf(file, "%d %s %d %d\n", c[i]->code, c[i]->name, c[i]->unit, c[i]->grading);
    }
    fclose(file);
}

void findClasses(char* name, struct st_class* c[], int csize) {
    int count = 0;
    printf("Searching (keyword : %s)\n", name);
    for (int i = 0; i < csize; i++) {
        if (strstr(c[i]->name, name)) {
            printf("[%d] %s [credit %d - %s]\n", c[i]->code, c[i]->name, c[i]->unit, kname[c[i]->grading - 1]);
            count++;
        }
    }
    printf("%d classes found.\n", count);
}

int addNewClass(struct st_class* c[], int csize) {
    struct st_class* p = (struct st_class*)malloc(sizeof(struct st_class));
    printf(">> code number > ");
    scanf("%d", &(p->code));

    for (int i = 0; i < csize; i++) {
        if (c[i]->code == p->code) {
            printf("이미 추가되어 있는 과목입니다!\n");
            free(p);
            return csize;
        }
    }
    printf(">> class name > ");
    scanf("%s", p->name);
    printf(">> credits > ");
    scanf("%d", &(p->unit));
    printf(">> grading (1: A+~F, 2: P/F) > ");
    scanf("%d", &(p->grading));

    c[csize] = p;
    return csize + 1;
}

void editClass(struct st_class* c[], int csize) {
    int code;
    struct st_class* p = NULL;
    printf(">> Enter a code of class > ");
    scanf("%d", &code);

    for (int i = 0; i < csize; i++) {
        if (c[i]->code == code) {
            p = c[i];
            break;
        }
    }

    if (p == NULL) {
        printf("코드가 존재하지 않는 코드 입니다.\n");
        return;
    }

    printf("> Current: [%d] %s [credits %d - %s]\n", p->code, p->name, p->unit, kname[p->grading - 1]);
    printf("> Enter new class name > ");
    scanf("%s", p->name);
    printf("> Enter new credits > ");
    scanf("%d", &(p->unit));
    printf("> Enter new grading(1:Grade, 2: P/F) > ");
    scanf("%d", &(p->grading));
    printf("> Modified.\n");
}

int applyMyClasses(int my[], int msize, struct st_class* c[], int csize) {
    int code;
    printf(">> Enter class code to apply > "); 
    scanf("%d", &code);
    
    // 1. 전체 목록에 존재하는 과목인지 확인 
    int found = 0; 
    for(int i = 0; i < csize; i++) {
        if(c[i]->code == code) {
            found = 1;
            printf("[%s] 과목을 찾았습니다.\n", c[i]->name); 
            break;
        }
    }
    
    if(!found) {
        printf("해당 코드를 가진 과목이 존재하지 않습니다.\n");
        return msize; 
    }

    // 2. 이미 신청한 과목인지 중복 체크 
    for(int i = 0; i < msize; i++) {
        if(my[i] == code) {
            printf("이미 수강 신청된 과목입니다.\n");
            return msize; // 개수 변화 없이 반환
        }
    }

    // 3. 신청 목록에 추가
    my[msize] = code;
    printf("수강 신청이 완료되었습니다.\n");
    return msize + 1; 
}

void printMyClasses(int my[], int msize, struct st_class* c[], int csize) {
    printf("> List of My Classes (%d classes)\n", msize);
    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < csize; j++) {
            if (my[i] == c[j]->code) {
                printf("%d. [%d] %s [credit %d - %s]\n", i + 1, c[j]->code, c[j]->name, c[j]->unit, kname[c[j]->grading - 1]);
                break;
            }
        }
    }
}

void saveMyClass(int my[], int msize, struct st_class* c[], int csize) {
    FILE* file = fopen("my_classes.txt", "w");
    if (file == NULL) return;

    int total_units = 0;      // 총 수강학점
    int grade_units = 0;      // Grade 방식(A+~F) 총 학점
    int pf_units = 0;         // P/F 방식 총 학점
    int grade_count = 0;      // Grade 방식 과목 수
    int pf_count = 0;         // P/F 방식 과목 수

    fprintf(file, "My Applied Classes List:\n");
    fprintf(file, "------------------------------------------\n");

    for (int i = 0; i < msize; i++) {
        for (int j = 0; j < csize; j++) {
            if (my[i] == c[j]->code) {
                // 1. 과목 정보 저장 (코드, 과목명, 학점, 평가방식)
                fprintf(file, "- [%d] %-15s | Credits: %d | Type: %s\n", 
                        c[j]->code, c[j]->name, c[j]->unit, kname[c[j]->grading - 1]);
                
                // 2. 전체 통계 누적
                total_units += c[j]->unit;

                // 3. 평가방식별 상세 통계 분기 로직 (if-else 사용)
                if (c[j]->grading == 1) { // Grade 방식 (A+~F)
                    grade_units += c[j]->unit;
                    grade_count++;
                } else { // P/F 방식
                    pf_units += c[j]->unit;
                    pf_count++;
                }
                break; 
            }
        }
    }

    // 4. 최종 통계 리포트 출력
    fprintf(file, "------------------------------------------\n");
    fprintf(file, "STATISTICS REPORT\n");
    fprintf(file, "1. Total Applied Classes: %d\n", msize);
    fprintf(file, "2. Total Applied Credits: %d\n", total_units);
    fprintf(file, "3. By Grading Type:\n");
    fprintf(file, "   - Grade Type (A+~F): %d classes, %d credits\n", grade_count, grade_units);
    fprintf(file, "   - P/F Type: %d classes, %d credits\n", pf_count, pf_units);
    fprintf(file, "------------------------------------------\n");

    fclose(file);
}