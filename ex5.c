/******************
Name: Maxim Muradov
ID: 346975386
Assignment: ex4
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

//some recommendations for functions, you may implement however you want
void swapsong(Song** firstsong, Song** secondsong)
{
    //swaping songs
    Song* temp = *firstsong;
    *firstsong = *secondsong;
    *secondsong = temp;
}

void freeSong(Song* ptr) {
    if (ptr != NULL)
    {
        //if ptr not NULL free all data
        free(ptr->title);
        ptr->title = NULL;
        free(ptr->artist);
        ptr->artist = NULL;
        free(ptr->lyrics);
        ptr->lyrics = NULL;
        free(ptr);
    }
}

void freePlaylist(struct Playlist* ptr) {
    if (ptr != NULL)
    {
        //if ptr not NULL free all data
        free(ptr->name);
        ptr->name = NULL;
        for (int i = ptr->songsNum - 1; i >= 0; i--)
            freeSong(ptr->songs[i]);
        free(ptr->songs);
        free(ptr);
    }
}

void printPlaylist(Playlist* ChosenPlaylist)
{
    //just print
    if (ChosenPlaylist->songs != NULL)
    {
        for (int i = 0; i < ChosenPlaylist->songsNum; i++, printf("\n"))
        {
            printf("%d. ", i + 1);
            printf("Title: %s\n", ChosenPlaylist->songs[i]->title);
            printf("   Artist: %s\n", ChosenPlaylist->songs[i]->artist);
            printf("   Released: %d\n", ChosenPlaylist->songs[i]->year);
            printf("   Streams: %d\n", ChosenPlaylist->songs[i]->streams);
        }
    }
}

void deleteSong(Playlist* chosenPlaylist) {
    int index = 0;
    printPlaylist(chosenPlaylist);
    printf("choose a song to delete, or 0 to quit:\n");
    scanf(" %d", &index);

    if (index == 0 || index > chosenPlaylist->songsNum) {
        printf("Invalid option\n");
        return;
    }

    // Shift the songs
    for (int i = index - 1; i < chosenPlaylist->songsNum - 1; i++) {
        swapsong(&chosenPlaylist->songs[i], &chosenPlaylist->songs[i + 1]);
    }
    
    Song* ptr = chosenPlaylist->songs[chosenPlaylist->songsNum - 1];
    freeSong(ptr);
    //just to be sure im doing it thru ptr
    chosenPlaylist->songsNum--;
    

    printf("Song deleted successfully.\n");
}

Playlist** deletePlaylist(Playlist** playlists, int* size) {

    int index = 0;
        printf("Choose a playlist:\n");
        for (int i = 0; i < *size; i++) {
            printf("\t%d. %s\n", i + 1, playlists[i]->name);
        }
        printf("\t%d. Back to main menu\n", *size + 1);
        scanf(" %d", &index);

        if (index == *size + 1) {
            return playlists; // Return without modifying
        }

        if (index <= 0 || index > *size) {
            printf("Invalid option\n");
            return playlists;
        }

    // Free the selected playlist
    freePlaylist(playlists[index - 1]);

    // Shift the remaining playlists
    for (int i = index - 1; i < *size - 1; i++) {
        playlists[i] = playlists[i + 1];
    }

    
    (*size)--;
    //realloc isnt neccesary ik but it was good for check leaks
    if (*size > 0) {
        playlists = (Playlist**)realloc(playlists, (*size) * sizeof(Playlist*));
    }
    else {
        playlists = NULL;
    }

    printf("Playlist deleted.\n");
    return playlists;
}

void playSong(Song* chosenSong) {
    //u know play song...
    printf("Now playing %s:\n", chosenSong->title);
    printf("$ %s $\n", chosenSong->lyrics);
    chosenSong->streams++;
}

void PlayPlaylist(Playlist* chosenPlaylist)
{
    //for of playing songs
    for (int i = 0; i < chosenPlaylist->songsNum; i++, printf("\n"))
        playSong(chosenPlaylist->songs[i]);
}

char* scanstr(int b00l)
{
    if (b00l == 1)
    {
        //if i need to clear buffer
        scanf("%*[^\n]");
        scanf("%*c");
    }
    int memsize = 16;
    char* tmp = (char*)calloc(memsize, sizeof(char)), ch;
    int i = 0;
    do {
        scanf("%c", &ch);
        if(ch == '\r'){
            continue;
        }
        tmp[i] = ch;
        if (i == memsize - 1 && tmp[i] != '\n')
        {
            memsize *= 2;
            tmp = (char*)realloc(tmp, memsize);
        }
    } while (tmp[i++] != '\n');
    tmp[i - 1] = '\0';
    //realloc memory if str exceed memory that was
    
    return tmp;
}

struct Playlist* createPlayList()
{
    //only creating playlist
    Playlist* ptr = (Playlist*)malloc(sizeof(Playlist));
    printf("Enter playlist's name:\n");
    ptr->name = scanstr(1);
    ptr->songs = NULL;
    ptr->songsNum = 0;

    return ptr;
}

struct Song* AddSong()
{
    //creating song
    Song* ptr = (Song*)malloc(sizeof(Song));
    printf("Enter song's details\n");

    printf("Title:\n");
    ptr->title = scanstr(1);

    printf("Artist:\n\r");
    ptr->artist = scanstr(0);

    printf("Year of release:\n");
    scanf(" %d", &ptr->year);

    printf("Lyrics:\n");
    ptr->lyrics = scanstr(1);

    ptr->streams = 0;

    return ptr;
}

void ShowPlaylist(struct Playlist* ChosenPlaylist)
{
    //showplaylist menu
    int menu = -1;
    printPlaylist(ChosenPlaylist);

    while (menu != 0)
    {
        printf("choose a song to play, or 0 to quit:\n");
        scanf(" %d", &menu);
        if (menu == 0)
            return;
        if (menu - 1 < ChosenPlaylist->songsNum)
            playSong(ChosenPlaylist->songs[menu - 1]);
        else
            return;
    }
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

//Bublesorts to sort songs in chosen order 
void sort_by_year(struct Playlist* chPlaylist)
{
    for (int i = 0; i < chPlaylist->songsNum - 1; i++)
        for (int j = i + 1; j < chPlaylist->songsNum; j++)
            if (chPlaylist->songs[i]->year > chPlaylist->songs[j]->year)
                swapsong(&chPlaylist->songs[i], &chPlaylist->songs[j]);
}

void sort_by_alphabet(struct Playlist* chPlaylist)
{
    for (int i = 0; i < chPlaylist->songsNum - 1; i++)
        for (int j = i + 1; j < chPlaylist->songsNum; j++)
            if (strcmp(chPlaylist->songs[i]->title, chPlaylist->songs[j]->title) > 0)
                swapsong(&chPlaylist->songs[i], &chPlaylist->songs[j]);
}

void sort_by_streams(struct Playlist* chPlaylist, char ch)
{
    for (int i = 0; i < chPlaylist->songsNum - 1; i++)
        for (int j = i + 1; j < chPlaylist->songsNum; j++)
        {
            if (chPlaylist->songs[i]->streams > chPlaylist->songs[j]->streams && ch == '+')
                swapsong(&chPlaylist->songs[i], &chPlaylist->songs[j]);
            if (chPlaylist->songs[i]->streams < chPlaylist->songs[j]->streams && ch == '-')
                swapsong(&chPlaylist->songs[i], &chPlaylist->songs[j]);
        }
}

void sortPlaylist(struct Playlist *chPlaylist) {
    int menu = -1;
    //sort menu
    printf("\nchoose:\n");
    printf("1.  sort by year\n2.  sort by streams - ascending order\n");
    printf("3.  sort by streams - descending order\n4.  sort alphabetically\n");

    scanf(" %d", &menu);
    if (menu == 1)
        sort_by_year(chPlaylist);
    if (menu == 2)
        sort_by_streams(chPlaylist, '+');
    if (menu == 3)
        sort_by_streams(chPlaylist, '-');
    else if (menu < 1 || menu > 3)
        sort_by_alphabet(chPlaylist);

    printf("sorted\n");
}

void InPlaylist(struct Playlist *ChosenPlaylist)
{
    //playlist menu
    int menu = -1;
    printf("playlist %s:\n", ChosenPlaylist->name);
    while (menu != 6)
    {
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
        if(scanf(" %d", &menu)){
            switch (menu)
            {
            case 1:
                ShowPlaylist(ChosenPlaylist);
                break;

            case 2:
                //realloc memory +1 song if added
                ChosenPlaylist->songs = (Song**)realloc(ChosenPlaylist->songs, (++ChosenPlaylist->songsNum) * (sizeof(Song*)));
                ChosenPlaylist->songs[ChosenPlaylist->songsNum - 1] = AddSong();
                break;

            case 3:
                deleteSong(ChosenPlaylist);
                break;

            case 4:
                sortPlaylist(ChosenPlaylist);
                break;

            case 5:
                PlayPlaylist(ChosenPlaylist);
                break;
            case 6:
                break;
            default:
                printf("Invalid Option\n");

            }
        } else {
            //delete in case of failed scanf.
            scanf("%*[\n]%*c");
        }
    }

}

void WatchPlayList(struct Playlist** ArrPlaylist, int szArr)
{
    //Also menu
    int menu = 0;
    if (ArrPlaylist == NULL)
    {
        while (menu != 1)
        {
            printf("Choose a playlist:\n");
            printf("\t1. Back to main menu\n");

            scanf("%d", &menu);
            if (menu != 1)
                printf("Invalid Option\n");
        }
        return;
    }
    else
    {
        while (menu != szArr + 1)
        {
            printf("Choose a playlist:\n");
            for (int i = 0; i < szArr; i++)
            {
                printf("\t%d. %s\n", i + 1, ArrPlaylist[i]->name);
            }
            printf("\t%d. Back to main menu\n", szArr + 1);

            scanf(" %d", &menu);

            if (menu == szArr + 1)
                return;
            if (menu <= szArr && menu >= 1)
                InPlaylist(ArrPlaylist[menu - 1]);
            else
                printf("Invalid Option\n");
        }
    }


}

void Total_exit(Playlist** Array_of_playlist, int* sizeofArr)
{
    //free all playlists that in array
    if (Array_of_playlist != NULL)
    {
        for (int i = *sizeofArr - 1; i >= 0; i--)
        {
            freePlaylist(Array_of_playlist[i]);
        }
    }
    //Array[0]
    *sizeofArr = 0;
}

int main() {
    int menu = 0;
    struct Playlist** Array_of_playlist = NULL;
    int sizeofArr = 0;

    while (menu != 4)
    {
        printPlaylistsMenu();
        scanf(" %d", &menu);
        switch (menu) {
        case 1:
            WatchPlayList(Array_of_playlist, sizeofArr);
            break;

        case 2:
            //++size and realloc +1 ptr of Array and push to array of ptr
            sizeofArr++;
            if (Array_of_playlist != NULL)
                Array_of_playlist = (Playlist**)realloc(Array_of_playlist, sizeofArr * (sizeof(Playlist*)));
            else
                Array_of_playlist = (Playlist**)malloc(sizeofArr * (sizeof(Playlist*)));
            Array_of_playlist[sizeofArr - 1] = createPlayList();
            break;

        case 3:
            Array_of_playlist = deletePlaylist(Array_of_playlist, &sizeofArr);
            break;
        case 4:
            //free all + free ptr of Arrays_Playlist himself
            Total_exit(Array_of_playlist, &sizeofArr);
            free(Array_of_playlist);
            Array_of_playlist = NULL;
            break;
        default:
            printf("Invalid Option\n");
        }
    }
    printf("Goodbye!\n");
}
//see you next time)
