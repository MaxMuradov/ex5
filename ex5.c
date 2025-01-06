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

void freeSong(Song* ptr) {
    if (ptr != NULL)
    {
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
        free(ptr->name);
        ptr->name = NULL;
        for (int i = ptr->songsNum - 1; i >= 0; i--)
            freeSong(ptr->songs[i]);
        free(ptr);
    }
}

void deleteSong(struct Playlist chosenPlaylist) {
    int index = 0;
    printf("choose a song to delete, or 0 to quit:\n");
    scanf_s(" %d", &index);

    Song* ptr = chosenPlaylist.songs[index - 1];
    freeSong(ptr);

    chosenPlaylist.songsNum--;
    for (int i = index - 1; i < chosenPlaylist.songsNum; i++)
        chosenPlaylist.songs[i] = chosenPlaylist.songs[i + 1];

    chosenPlaylist.songs = (Song**)realloc(chosenPlaylist.songs, chosenPlaylist.songsNum * (sizeof(Song*)));

    printf("Song deleted successfully.\n");
}

Playlist* deletePlaylist(Playlist* ptr, int* size)
{
    int index = 0;
    printf("Choose a playlist:\n");
    for (int i = 0; i < *size; i++)
    {
        printf("\t%d. %s\n", i + 1, ptr[i].name);
    }
    printf("\t%d. Back to main menu\n", *size + 1);
    scanf_s(" %d", &index);

    Playlist* tmp = &ptr[index -1];
    freePlaylist(tmp);
    *size = *size - 1;

    for (int i = index - 1; i < *size; i++)
        ptr[i] = ptr[i + 1];

    if (*size > 0)
        ptr = (Playlist*)realloc(ptr, *size * sizeof(Playlist));
    else
        ptr = NULL;

    return ptr;

    printf("Playlist deleted\n");
}

void playSong(Song* chosenSong) {
    printf("Now playing %s:\n", chosenSong->title);
    printf("$ %s $\n", chosenSong->lyrics);
    chosenSong->streams++;
}

void PlayPlaylist(Playlist chosenPlaylist)
{
    for (int i = 0; i < chosenPlaylist.songsNum; i++, printf("\n"))
    {
        playSong(chosenPlaylist.songs[i]);
    }

}

char* scanstr(int b00l)
{
    if (b00l == 1)
    {
        scanf_s("%*c");
    }
    int memsize = 16;
    char *tmp = (char*)calloc(memsize, sizeof(char));
    int i = 0;
    do {
        scanf_s("%c", &tmp[i]);
        if (i == memsize - 1 && tmp[i] != '\n')
        {
            memsize *= 2;
            tmp = (char*)realloc(tmp, memsize);
        }
    } while (tmp[i++] != '\n');
    tmp[i - 1] = '\0';

    return tmp;
}

struct Playlist* createPlayList()
{
    Playlist *ptr = (Playlist*)malloc(sizeof(Playlist));
    printf("Enter playlist's name:\n");
    ptr->name = scanstr(1);
    ptr->songs = NULL;
    ptr->songsNum = 0;

    return ptr;
}

struct Song* AddSong()
{
    Song* ptr = (Song*)malloc(sizeof(Song));
    printf("Enter song's details\n");

    printf("Title:\n");
    ptr->title = scanstr(1);

    printf("Artist:\n\r");
    ptr->artist = scanstr(0);

    printf("Year of release:\n");
    scanf_s(" %d", &ptr->year);

    printf("Lyrics:\n");
    ptr->lyrics = scanstr(1);

    ptr->streams = 0;
    
    return ptr;
}



void ShowPlaylist(struct Playlist ChosenPlaylist)
{
    int menu = -1;

    while (menu != 0)
    {
        if (ChosenPlaylist.songs == NULL)
        {

        }
        else
        {
            for (int i = 0; i < ChosenPlaylist.songsNum; i++, printf("\n"))
            {
                printf("%d.  ", i + 1);
                printf("Title: %s\n", ChosenPlaylist.songs[i]->title);
                printf("Artist: %s\n", ChosenPlaylist.songs[i]->artist);
                printf("Released: %d\n", ChosenPlaylist.songs[i]->year);
                printf("Streams: %d\n", ChosenPlaylist.songs[i]->streams);
            }
        }

        printf("choose a song to play, or 0 to quit:\n");
        scanf_s(" %d", &menu);
        if (menu == 0)
            return;
        if (menu - 1 < ChosenPlaylist.songsNum)
        {
            playSong(ChosenPlaylist.songs[menu - 1]);
        }
        else
            return;
    }
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

void swapsong(Song* firstsong, Song* secondsong)
{
    Song* ptr = firstsong;
    firstsong = secondsong;
    secondsong = ptr;
}

void sortPlaylist(struct Playlist chPlaylist) {
    int menu = -1;

    printf("\nchoose:\n");
    printf("1.  sort by year\n2.  sort by streams - ascending order\n");
    printf("3.  sort by streams - descending order\n4.  sort alphabeticaly\n");

    scanf_s(" %d", &menu);
    for (int i = 0; i < chPlaylist.songsNum - 1; i++)
    {
        if (menu == 1)
        {
            for (int j = i + 1; j < chPlaylist.songsNum; j++)
                if (chPlaylist.songs[i]->year > chPlaylist.songs[j]->year)
                    swapsong(chPlaylist.songs[i], chPlaylist.songs[j]);
        }
        if (menu == 2)
        {
            for (int j = i + 1; j < chPlaylist.songsNum; j++)
                if (chPlaylist.songs[i]->streams > chPlaylist.songs[j]->streams)
                    swapsong(chPlaylist.songs[i], chPlaylist.songs[j]);
        }
        if (menu == 3)
        {
            for (int j = i + 1; j < chPlaylist.songsNum; j++)
                if (chPlaylist.songs[i]->streams < chPlaylist.songs[j]->streams)
                    swapsong(chPlaylist.songs[i], chPlaylist.songs[j]);
        }
        else if (menu < 1 || menu > 3)
        {
            for (int j = i + 1; j < chPlaylist.songsNum; j++)
                if (chPlaylist.songs[i]->title[0] > chPlaylist.songs[j]->title[0])
                    swapsong(chPlaylist.songs[i], chPlaylist.songs[j]);
        }

    }


    printf("sorted\n");
}

void InPlaylist(struct Playlist ChosenPlaylist)
{
    int menu = -1;
    printf("playlist %s:\n", ChosenPlaylist.name);
    while (menu != 6)
    {
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. Back\n");
        scanf_s(" %d", &menu);
        switch (menu)
        {
        case 1:
            ShowPlaylist(ChosenPlaylist);
        break;

        case 2:
            
            ChosenPlaylist.songs = (Song**)realloc(ChosenPlaylist.songs, ++ChosenPlaylist.songsNum * (sizeof(Song*)));
            ChosenPlaylist.songs[ChosenPlaylist.songsNum - 1] = AddSong();
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

        }
    }

}

void WatchPlayList(struct Playlist *ArrPlaylist, int szArr)
{
    int menu = 0;


    if (ArrPlaylist == NULL)
    {
        while (menu != 1)
        {
        printf("Choose a playlist:\n");
        printf("\t1. Back to main menu\n");
        
            scanf_s("%d", &menu);
            if (menu != 1)
                printf("Invalid Option\n");
        }
        return;
    }
    else
    {
        printf("Choose a playlist:\n");
        for (int i = 0; i < szArr; i++)
        {
            printf("\t%d. %s\n", i + 1, ArrPlaylist[i].name);
        }
        printf("\t%d. Back to main menu\n", szArr + 1);

        scanf_s(" %d", &menu);

        if (menu == szArr + 1)
            return;
        InPlaylist(ArrPlaylist[menu - 1]);

    }


}

int main() {
    int menu = 0;
    struct Playlist *Array_of_playlist = NULL;
    int sizeofArr = 0;

    while (menu != 4)
    {
        printPlaylistsMenu();
        scanf_s(" %d", &menu);
        switch (menu) {
        case 1:
            WatchPlayList(Array_of_playlist, sizeofArr);
        break;

        case 2:
            sizeofArr++;
            if (Array_of_playlist != NULL)
                Array_of_playlist = (Playlist*)realloc(Array_of_playlist, sizeofArr * (sizeof(Playlist)));
            else
                Array_of_playlist = (Playlist*)malloc(sizeofArr * (sizeof(Playlist)));
            Array_of_playlist[sizeofArr - 1] = *createPlayList();
        break;

        case 3:
            Array_of_playlist = deletePlaylist(Array_of_playlist, &sizeofArr);
        break;
        }
    }
    printf("Goodbye!\n");
}
