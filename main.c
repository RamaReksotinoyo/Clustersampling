#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CSV_FILE "../dataset.csv"
#define MAX_DISTRICTS 500
#define TOTAL_DISTRICTS 415
#define Z_95_CI 1.96

typedef struct 
{
    int district;
    int people;
    double total_spend;
} SurveyData;


int read_csv_file(const char* filename, SurveyData* data){
    FILE* file;
    char line[100];
    int line_number = 0;
    int count = 0;
    int fafifu;
    char* token;

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return -1;
    }

    if (fgets(line, sizeof(line), file) != NULL){
        line_number++;
        // line[strcspn(line, "\n")] = 0;
        printf("%s\n", line);
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
        if (token == NULL){
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

void display_basic_stats(int sampled_districts) {
    printf("\nNumber of all districts in city A: %d\n", TOTAL_DISTRICTS);
    printf("Number of sampled districts in city A: %d\n", sampled_districts);
    printf("Total records: %d\n\n", sampled_districts + 1); /* +1 for header */
}

double calculate_sc_squared(SurveyData* data, int count, double r) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        double term = data[i].total_spend - (r * data[i].people);
        sum += pow(term, 2);
    }
    return sum / (count - 1);
}

double calculate_variance_tau_hat(int N, int n, double sc_squared) {
    return N * (N - n) * (sc_squared / n);
}

// Modified calculate_and_display_metrics function
void calculate_and_display_metrics(SurveyData* data, int count) {
    int total_people = 0;
    double total_spend = 0.0;
    double average_spend;
    
    // Calculate totals
    for (int i = 0; i < count; i++) {
        total_people += data[i].people;
        total_spend += data[i].total_spend;
    }
    
    printf("Total people across all districts: %d\n", total_people);
    printf("Total spend across all districts: %.2f\n", total_spend);
    
    if (total_people > 0) {
        average_spend = total_spend / (double)total_people;
        printf("\nAverage monthly spend per person on goods delivery: %.2f\n", average_spend);
        
        // Calculate variance components
        double sc_squared = calculate_sc_squared(data, count, average_spend);
        double var_y_total_est = TOTAL_DISTRICTS * (TOTAL_DISTRICTS - count) * (sc_squared / count);
        
        printf("\nVariance Calculations:\n");
        printf("Sampled cluster variance (sc²): %.4f\n", sc_squared);
        printf("Variance of total pop. est.: (Rp %.0f)^2\n", sqrt(var_y_total_est));
        
        // Calculate mean people per district and total population estimate
        double M_bar = (double)total_people / count;
        double M_tot_est = TOTAL_DISTRICTS * M_bar;
        
        printf("\nPopulation Estimates:\n");
        printf("Mean people per district: %.0f\n", M_bar);
        printf("Estimated total population in city A: %.0f\n", M_tot_est);
        
        // Variance of mean per person estimator
        double var_y_mean_est = var_y_total_est / pow(M_tot_est, 2);
        printf("Variance of mean per person est.: (Rp %.0f)^2\n", sqrt(var_y_mean_est));
        
        // Confidence interval calculations
        printf("\nConfidence Interval:\n");
        printf("z-stat (95%% CI): %.2f\n", Z_95_CI);
        double d = Z_95_CI * sqrt(var_y_mean_est);
        printf("Margin of error (d): Rp %.0f\n", d);
        printf("95%% Confidence Interval: Rp %.2f ± Rp %.0f\n", 
               average_spend, d);
        printf("                      (Rp %.0f - Rp %.0f)\n",
               average_spend - d, average_spend + d);
    } else {
        printf("\nError: No people data available for calculation\n");
    }
}

int main(){
    SurveyData survey_data[MAX_DISTRICTS];
    int data;

    data = read_csv_file(CSV_FILE, survey_data);
    if (data <= 0) {
        fprintf(stderr, "Failed to process CSV file\n");
        return 1;
    }
    printf("data: %d \n", data);

    display_basic_stats(data);
    calculate_and_display_metrics(survey_data, data);

    printf("\n");
    return 0;
}
