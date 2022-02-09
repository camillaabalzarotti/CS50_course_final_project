#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    int found;
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    int i;
    int isthere;

    isthere = 0;
    for (i = 0; i < MAX && isthere == 0; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            isthere = 1;
            candidates[i].votes ++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int max; //maximum numebr of votes
    int i; //index to scroll the array
    // scroll the array of candidates one time to determine the maximum number of votes
    max = 0;
    for (i = 0; i < MAX; i++)
    {
        if (candidates[i].votes > max)
        {
            max = candidates[i].votes;
        }
    }

    for (i = 0; i < MAX; i++)
    {
        if (candidates[i].votes == max)
        {
            printf("%s", candidates[i].name);
            printf("\n");
        }
    }
    return;
}
