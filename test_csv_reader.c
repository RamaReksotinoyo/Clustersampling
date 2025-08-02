/* ============================================================================
 * 🤡 THIS TESTS WAS WRITTEN BY AI MODEL 🤡
 * ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>  // for unlink()

static int tests_run = 0;
static int tests_passed = 0;

#define TEST_ASSERT(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("✓ PASS: %s\n", message); \
    } else { \
        printf("✗ FAIL: %s\n", message); \
    } \
} while(0)

#define RUN_TEST(test) do { \
    printf("\n=== Running %s ===\n", #test); \
    test(); \
} while(0)


typedef struct {
    int district;
    int people;
    double total_spend;
} SurveyData;

int read_csv_file(const char* filename, SurveyData* data) {
    FILE* file;
    char line[100];
    int line_number = 0;
    int count = 0;
    char* token;

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return -1;
    }

    if (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        printf("%s", line);  // Keep newline for header
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        /* Remove newline character */
        line[strcspn(line, "\n")] = 0;
        printf("%s\n", line);

        /* Parse district */
        token = strtok(line, ",");
        if (token == NULL) {
            fprintf(stderr, "Error: Invalid format at line %d\n", line_number);
            fclose(file);
            return -1;
        }
        data[count].district = atoi(token);

        // parse people count
        token = strtok(NULL, ",");
        if (token == NULL) {
            fprintf(stderr, "Error: Invalid format at line %d\n", line_number);
            fclose(file);
            return -1;
        }
        data[count].people = atoi(token);

        // parse total spend
        token = strtok(NULL, ",");
        if (token == NULL) {
            fprintf(stderr, "Error: Invalid format at line %d\n", line_number);
            fclose(file);
            return -1;
        }
        data[count].total_spend = atof(token);

        count++;
    }

    fclose(file);
    
    if (count == 0) {
        fprintf(stderr, "Error: No data records found\n");
        return -1;
    }
    
    return count;
}


void create_test_csv(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file != NULL) {
        fprintf(file, "%s", content);
        fclose(file);
    }
}

void delete_test_file(const char* filename) {
    unlink(filename);
}


void test_normal_csv_file() {
    const char* test_file = "test_normal.csv";
    const char* csv_content = 
        "District,People,Total Spend\n"
        "1,100,1000.50\n"
        "2,200,2000.75\n"
        "3,150,1500.25\n";
    
    create_test_csv(test_file, csv_content);
    
    SurveyData data[10];
    int result = read_csv_file(test_file, data);
    
    TEST_ASSERT(result == 3, "Should read 3 records from normal CSV");
    TEST_ASSERT(data[0].district == 1, "First district should be 1");
    TEST_ASSERT(data[0].people == 100, "First people count should be 100");
    TEST_ASSERT(data[0].total_spend == 1000.50, "First total spend should be 1000.50");
    
    TEST_ASSERT(data[1].district == 2, "Second district should be 2");
    TEST_ASSERT(data[1].people == 200, "Second people count should be 200");
    TEST_ASSERT(data[1].total_spend == 2000.75, "Second total spend should be 2000.75");
    
    TEST_ASSERT(data[2].district == 3, "Third district should be 3");
    TEST_ASSERT(data[2].people == 150, "Third people count should be 150");
    TEST_ASSERT(data[2].total_spend == 1500.25, "Third total spend should be 1500.25");
    
    delete_test_file(test_file);
}

void test_single_record_csv() {
    const char* test_file = "test_single.csv";
    const char* csv_content = 
        "District,People,Total Spend\n"
        "100,500,5000.99\n";
    
    create_test_csv(test_file, csv_content);
    
    SurveyData data[10];
    int result = read_csv_file(test_file, data);
    
    TEST_ASSERT(result == 1, "Should read 1 record from single record CSV");
    TEST_ASSERT(data[0].district == 100, "District should be 100");
    TEST_ASSERT(data[0].people == 500, "People count should be 500");
    TEST_ASSERT(data[0].total_spend == 5000.99, "Total spend should be 5000.99");
    
    delete_test_file(test_file);
}

void test_header_only_csv() {
    const char* test_file = "test_header_only.csv";
    const char* csv_content = "District,People,Total Spend\n";
    
    create_test_csv(test_file, csv_content);
    
    SurveyData data[10];
    int result = read_csv_file(test_file, data);
    
    TEST_ASSERT(result == -1, "Should return -1 for header-only CSV");
    
    delete_test_file(test_file);
}

void test_nonexistent_file() {
    SurveyData data[10];
    int result = read_csv_file("nonexistent_file.csv", data);
    
    TEST_ASSERT(result == -1, "Should return -1 for nonexistent file");
}

void test_empty_file() {
    const char* test_file = "test_empty.csv";
    const char* csv_content = "";
    
    create_test_csv(test_file, csv_content);
    
    SurveyData data[10];
    int result = read_csv_file(test_file, data);
    
    TEST_ASSERT(result == -1, "Should return -1 for empty file");
    
    delete_test_file(test_file);
}

void test_large_numbers() {
    const char* test_file = "test_large.csv";
    const char* csv_content = 
        "District,People,Total Spend\n"
        "999,10000,999999.99\n"
        "1,1,0.01\n";
    
    create_test_csv(test_file, csv_content);
    
    SurveyData data[10];
    int result = read_csv_file(test_file, data);
    
    TEST_ASSERT(result == 2, "Should read 2 records with large numbers");
    TEST_ASSERT(data[0].district == 999, "First district should be 999");
    TEST_ASSERT(data[0].people == 10000, "First people count should be 10000");
    TEST_ASSERT(data[0].total_spend == 999999.99, "First total spend should be 999999.99");
    
    TEST_ASSERT(data[1].district == 1, "Second district should be 1");
    TEST_ASSERT(data[1].people == 1, "Second people count should be 1");
    TEST_ASSERT(data[1].total_spend == 0.01, "Second total spend should be 0.01");
    
    delete_test_file(test_file);
}

void test_with_your_actual_data() {
    const char* test_file = "test_actual_data.csv";
    const char* csv_content = 
        "District,People,Total Spend\n"
        "412,171,64050000\n"
        "324,185,64500000\n"
        "182,363,123900000\n";
    
    create_test_csv(test_file, csv_content);
    
    SurveyData data[10];
    int result = read_csv_file(test_file, data);
    
    TEST_ASSERT(result == 3, "Should read 3 records from actual data format");
    TEST_ASSERT(data[0].district == 412, "First district should be 412");
    TEST_ASSERT(data[0].people == 171, "First people count should be 171");
    TEST_ASSERT(data[0].total_spend == 64050000.0, "First total spend should be 64050000");
    
    delete_test_file(test_file);
}


double calculate_average_spend(SurveyData* data, int count) {
    if (count <= 0) return 0.0;
    
    int total_people = 0;
    double total_spend = 0.0;
    
    for (int i = 0; i < count; i++) {
        total_people += data[i].people;
        total_spend += data[i].total_spend;
    }
    
    if (total_people == 0) return 0.0;
    return total_spend / (double)total_people;
}

void test_integration_csv_and_calculation() {
    const char* test_file = "test_integration.csv";
    const char* csv_content = 
        "District,People,Total Spend\n"
        "1,100,1000\n"
        "2,200,2000\n";  // Total: 300 people, 3000 spend -> avg = 10.0
    
    create_test_csv(test_file, csv_content);
    
    SurveyData data[10];
    int result = read_csv_file(test_file, data);
    
    TEST_ASSERT(result == 2, "Should read 2 records for integration test");
    
    double avg = calculate_average_spend(data, result);
    TEST_ASSERT(avg == 10.0, "Average spend should be 10.0");
    
    delete_test_file(test_file);
}

/* ============================================================================
 * PERFORMANCE TEST
 * ============================================================================ */

void test_performance_many_records() {
    const char* test_file = "test_performance.csv";
    FILE* file = fopen(test_file, "w");
    
    // Create CSV with many records
    fprintf(file, "District,People,Total Spend\n");
    for (int i = 1; i <= 100; i++) {
        fprintf(file, "%d,%d,%.2f\n", i, i * 10, i * 1000.0);
    }
    fclose(file);
    
    SurveyData data[150];  // Ensure enough space
    int result = read_csv_file(test_file, data);
    
    TEST_ASSERT(result == 100, "Should read 100 records for performance test");
    TEST_ASSERT(data[0].district == 1, "First record should be correct");
    TEST_ASSERT(data[99].district == 100, "Last record should be correct");
    
    delete_test_file(test_file);
}

/* ============================================================================
 * MAIN TEST RUNNER
 * ============================================================================ */

int main() {
    printf("=== CSV Reader Function Test Suite ===\n");
    printf("Testing read_csv_file() function...\n");
    
    // Run all tests
    RUN_TEST(test_normal_csv_file);
    RUN_TEST(test_single_record_csv);
    RUN_TEST(test_header_only_csv);
    RUN_TEST(test_nonexistent_file);
    RUN_TEST(test_empty_file);
    RUN_TEST(test_large_numbers);
    RUN_TEST(test_with_your_actual_data);
    RUN_TEST(test_integration_csv_and_calculation);
    RUN_TEST(test_performance_many_records);
    
    // Print summary
    printf("\n=== Test Results ===\n");
    printf("Total tests: %d\n", tests_run);
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_run - tests_passed);
    
    if (tests_run == tests_passed) {
        printf("ALL TESTS PASSED! \n");
        return 0;
    } else {
        printf("SOME TESTS FAILED! \n");
        return 1;
    }
}
