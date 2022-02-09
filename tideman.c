#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j = an edge points form candidate i to candidate j (i preferred over j)
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++) //for each voter
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1); //express your (j+i)th preference

            if (!vote(j, name, ranks)) //if the preference doesn't matches any candidate
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //sees whether the name corrisponds to one character and if so >>in ranks[rank] specificies that it's rankth preference is the ith candidate
    int i;

    for (i = 0; i < candidate_count; i ++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //takes ranks[0] >>the voter preferred the candidate number the one contained in ranks[0] over the others
    int i, j;
    int cur; //to save the numebr of the preferred candidate
    int following; //to save time to time the number of the following ones
    //ranks has candidate_count options
    for (i = 0; i < candidate_count - 1; i++) //for each element of rank, see the next ones
    {
        cur = ranks[i];
        for (j = i + 1; j < candidate_count; j++)
        {
            following = ranks[j];
            preferences[cur][following] ++; //update the preferences
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int count;
    int i, j;
    //from 0 to pair_count - 1
    pair_count = candidate_count * (candidate_count - 1)/2;
    // you have to confront preferences[i][j] with preferences[i][j] >>if they are equal do nothing else if [i][j] > [j][j] i winner, j loser, and viceversa
    count = 0;
    for (i = 0; i < candidate_count - 1 && count < pair_count; i++)
    {
        for (j = 0; j < candidate_count && count < pair_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[count].winner = i;
                pairs[count].loser = j;
                count ++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[count].winner = j;
                pairs[count].loser = i;
                count ++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int start; //initial strenght of victory
    int i, j;
    pair tmp;

    for (i = 0; i < pair_count - 1; i++)
    {
        for (j = i + 1; j < pair_count; j++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] > preferences[pairs[j].winner][pairs[j].loser])
            {
                tmp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = tmp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //locked bool candidates count
    int i, j;
    int winner, loser;
    //inizialization
    for (i = 0; i < pair_count; i++)
    {
        for (j = 0; j < pair_count; j ++)
        {
            locked[i][j] = false;
        }
    }
    //locking without creating circles
    for (i = 0; i < pair_count; i++)
    {
        winner = pairs[i].winner;
        loser = pairs[i].loser;
        if (locked[winner][loser] != true)
        {
            locked[winner][loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int i, j;
    int found;
    int printed; //to end the circle
    printed = 0;
    for (j = 0; j < candidate_count && printed == 0; j++)
    {
        found = 0;
        for (i = 0; i < candidate_count && found == 0; i++)
        {
            if (locked[i][j] == true)
            {
                found = 1;
            }
        }
        if (found == 0)
        {
            printf("%s\n", candidates[j]);
            printed = 1;
        }
    }
    return;
}