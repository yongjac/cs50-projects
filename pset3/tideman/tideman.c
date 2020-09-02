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
void merge_sort(int l, int r);
void merge(int l, int m, int r);
bool no_cycle(int tail, int head);

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
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
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
    // TODO
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
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
    merge_sort(0, pair_count - 1);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        if (i < 3)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = no_cycle(pairs[i].winner, pairs[i].loser);
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count - 1; i ++)
    {
        bool win = true;
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                j = candidate_count;
                win = false;
            }
        }
        if (win)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

// l is for left index and r is for right index
void merge_sort(int l, int r)
{
    if (l < r)
    {
        int m = l + (r - 1) / 2;

        merge_sort(l, m);
        merge_sort(m + 1, r);

        merge(l, m, r);
    }
}

// merge two subarray
void merge(int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // create temp arrays
    pair L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
    {
        L[i] = pairs[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = pairs[m + 1 + j];
    }

    // Merge the temp arrays back into pairs
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (preferences[L[i].winner][L[i].loser] >= preferences[R[j].winner][R[j].loser])
        {
            pairs[k] = L[i];
            i++;
        }
        else
        {
            pairs[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1)
    {
        pairs[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2)
    {
        pairs[k] = R[j];
        j++;
        k++;
    }
}

// return true if no cycle
bool no_cycle(int tail, int head)
{
    if (tail == head)
    {
        return false;
    }
    bool nocycle = true;
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[head][i])
        {
            nocycle = no_cycle(tail, i);
            if (!nocycle)
            {
                return false;
            }
        }
    }
    return true;
}