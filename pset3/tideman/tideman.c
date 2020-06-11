#include <cs50.h>
#include <stdio.h>
#include <string.h>


// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
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
    int i;
    int j;
    for (i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (i = 0; i < candidate_count; i++)
    {
        for (j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
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
    // TODO
    // Validate candidate's name
    int k;
    for (k = 0; k < candidate_count; k++)
    {
        if (strcmp(candidates[k], name) == 0)
        {
            // store in array ranks the index of the candidate in order of preference
            ranks[rank] = k;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    // take the vote saved in the array ranks and update the 2D array (matrix) preferences
    // This code goes to the x,y positions in preferences where 'x' is the winner and 'y' the loser.

    // iterate over ranks except for the last position, as candidate in last position doesn'y bit anyone hence doesn't update preferences.
    int i;
    int j;
    for (i = 0; i < candidate_count - 1; i++)
    {
        int k = ranks[i]; // gets the position of the winner in array preferences

        // iterate over the ranks below ranks[i], as those ranks '(ranks[i+1])'' lost to ranks[i]
        for (j = i + 1; j < candidate_count; j++)
        {
            // gets the position of the loser in array preferences
            int l = ranks[j];
            preferences[k][l]++;
            //printf("%i\n", preferences[k][l]);
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    // iterate over candidates
    int i;
    int j;
    for (i = 0; i < candidate_count - 1; i++)
    {
        // iterate over the rest of the candidates
        for (j = i + 1; j < candidate_count; j++)
        {
            // find the winner and loser
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                // update pair_count
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                // update pair_count
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // subtract winner's score minus loser's score
    int strength[pair_count];
    int i;
    int j;
    int new_count;
    for (i = 0; i < pair_count; i++)
    {
        strength[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        //printf("%i\n", strength[i]);
    }

    // bubble sort the smallest at the end
    int counter_loop;
    do
    {
        counter_loop = 1;
        // 'new_count' will allow to change the rabge of the 'for' loop
        // every time a new for loop starts we need to do one iteration less
        // as one element gets bubbled to the farther right in each loop
        for (j = 0, new_count = (candidate_count - counter_loop); j < new_count; j++)
        {
            if (strength[j] < strength[j + 1])
            {
                int temp_strength = strength[j];
                int temp_winner = pairs[j].winner;
                int temp_loser = pairs[j].loser;
                pairs[j].winner = pairs[j + 1].winner;
                pairs[j].loser = pairs[j + 1].loser;
                pairs[j + 1].winner = temp_winner;
                pairs[j + 1].loser = temp_loser;
                strength[j] = strength[j + 1];
                strength[j + 1] = temp_strength;
            }
            // Stops comparing if pair doesn't swap and the jth element is previous
            // to (j <'new_count') which means the rest is ordered so no need to keep iterating
            else if ( j == new_count - 1)
            {
                // add two to 'counter_loop' so the next loop will loop two elements less
                // as the last comparison didn't need to be swapped which means that in one loop
                // the jth and the jth-1 element are ordered.
                counter_loop += 2;
                break;
            }
            // add one to 'counter_loop' so the next loop will loop one element less
            counter_loop++;
        }
    }
    while(candidate_count - counter_loop > 1);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    // Go to 2D array 'locked' based on ith element of pairs
    int i;
    int k;
    for (i = 0; i < pair_count; i++)
    {
        int i_winner = pairs[i].winner;
        int i_loser = pairs[i].loser;
        int temp_count = 0;

        for (k = 0; k < candidate_count; k++)
        {
            // has any one beaten i_winner before? in othe words, has i_winner been an i_loser to another candidate?
            if (locked[k][i_winner] == true)
            {
                // the one who beat i_winner has been beaten by i_loser?
                if (locked[i_loser][k] == true)
                {
                    // if true then don't lock the current pair
                    // locked[i_winner][i_loser] = false;
                    break;
                }
                else
                {
                    temp_count++;
                }
            }
            else
            {
                if (i == (pair_count - 1) && locked[i_loser][k] == true)
                {
                    break;
                }
                else
                {
                    temp_count++;
                }
            }
        }
        // if no one has beaten i_winner before then lock (true) the pair
        if (temp_count == candidate_count)
        {
            locked[i_winner][i_loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // find the columns which values are all false by coaunting the number of 'false' values in each column
    int array_false[candidate_count];
    int i;
    int j;
    for (i = 0; i < candidate_count; i++)
    {
        int count_false = 0;
        for (j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                count_false++;
            }
        }
        array_false[i] = count_false;
    }

    // get the winner(s) column's index and loop trough the rows that belong to that index
    int array_true[candidate_count];
    int k;
    int l;
    for (k = 0; k < candidate_count; k++)
    {
        int count_true = 0;
        for (l = 0; l < candidate_count; l++)
        {
            if (locked[k][l] == true)
            {
                count_true++;
            }
        }
        array_true[k] = count_true;
    }

    // winner is the column with all false and has more trues in a row
    int winner_count = 0;
    int m;
    for (m = 0; m < candidate_count; m++)
    {
        if (array_false[m] == candidate_count)
        {
            winner_count++;
        }
    }

    int array_winner[winner_count];
    int n;
    int p;
    for (n = 0; n < winner_count; n++)
    {
        for (p = 0; p < candidate_count; p++)
        {
            if (array_false[p] == candidate_count)
            {
                array_winner[n] = p;
            }
        }
    }

    int winner_index = 0;
    if (winner_count == 1)
    {
        winner_index = array_winner[0];
    }
    else
    {
        int array_winner_row[winner_count];
        int q;
        int r;
        for (q = 0; q < winner_count; q++)
        {
            array_winner_row[q] = array_true[array_winner[q]];
        }

        for (r = 0; r < winner_count - 1; r++)
        {

            if (array_winner_row[winner_index] < array_winner_row[r + 1])
            {
                winner_index = r + 1;
            }
        }
    }
    printf("%s\n", candidates[winner_index]);
    return;
}

