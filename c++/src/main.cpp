#include "Team.h"
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>

#include "../libs/json.hpp"

using namespace std;
using json = nlohmann::json;

FILE* callTheBlueAlliance();
void parseTempFile(FILE* tempFile);
void scoreLineup();

int main() {

    //Step 1: Get the Data from Blue Alliance
    FILE* tempFile = callTheBlueAlliance();

    if (tempFile == NULL) {
        return -1;
    }

    //Step 2: Save that Data into a String OR Straight-up Parse with Modern-JSON
    parseTempFile(tempFile);

    //Step 3: Run and Score Lineup
    scoreLineup();

    return 0;
}

FILE* callTheBlueAlliance() {
    curl_global_init(CURL_GLOBAL_ALL); //Initialize CURL

    CURL* myHandle;  //Handle for Requests
    CURLcode result; //Result of Curl Retrieval

    FILE* tempFile = NULL; //Temporary File

    myHandle = curl_easy_init(); //Initialize the Handle

    if (myHandle) {                      //If the Handle is Initialized Succesfully
        struct curl_slist* chunk = NULL; //Headers List

        //Add the Headers for The Blue Alliance API
        chunk = curl_slist_append(chunk, "accept: application/json");
        chunk = curl_slist_append(chunk, "X-TBA-Auth-Key: biuWVtbepPQnfgdMwPJnAmomoCm8Uw0VaO1UBdHsWzKJJCTOdUUprjHEsSIss01i");

        //Get the Temporary File we use to hold the data
        tempFile = fopen("tempFile.txt", "w+");

        if (tempFile != NULL) {
            curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, tempFile);
        } else {
            cout << "ERROR WRITING TO TEMPFILE.txt" << endl;
            return NULL;
        }

        //Set the URL and Header for the Request
        curl_easy_setopt(myHandle, CURLOPT_URL, "https://www.thebluealliance.com/api/v3/event/2020gagai/district_points");
        curl_easy_setopt(myHandle, CURLOPT_HTTPHEADER, chunk); //Headers
        curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, NULL);

        result = curl_easy_perform(myHandle); //Try to get the result

        if (result != CURLE_OK) {
            free(tempFile);
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
        } else {
            cout << "No Errors in Reading Data" << endl;
        }

        curl_easy_cleanup(myHandle);
        curl_slist_free_all(chunk);
    }

    return tempFile;
}

void parseTempFile(FILE* tempFile) {
    string stringifiedFile = ""; //File to be converted to string
    char currentLine[1000];

    rewind(tempFile); //Restart Temp File

    while (!feof(tempFile)) { //Parse the entire file
        fgets(currentLine, 1000, tempFile);
        stringifiedFile += currentLine;
    }

    auto jsonparsed = json::parse(stringifiedFile); //This value will hold the parsed file.

    for (auto& x : jsonparsed["points"].items()) { //Part we care about.
        auto key = x.key();

        unsigned int teamNum = stoi(key.substr(3));
        int totalDistrictPoints = x.value()["total"];

        insertNewTeam(new Team(teamNum, totalDistrictPoints));
    }

    fclose(tempFile);
}

void scoreLineup() {
    char continueLooping;
    string name;
    do {
        int numTeams, currentTeamNum, totalPoints = 0;

        cin >> name;
        cin >> numTeams; //Read Number of Teams
        
        //TODO: Adjust to work in a linked list way
        int* teamNumbers = new int[4];
        double* teamScores = new double[4];

        for (int i = 0; i < numTeams; i++) {
            cin >> currentTeamNum; //Read Current Team Number

            teamNumbers[i] = currentTeamNum;
            teamScores[i] = getTeamPoints(currentTeamNum);

            totalPoints += getTeamPoints(currentTeamNum);
        }

        cout << name << "'s Linuep" << endl;
        cout << " Team # | District Points" << endl;
        for (int i = 0; i < numTeams; i++) {
            cout << " " << left << setw(6) << teamNumbers[i] << " | " << teamScores[i] << endl;
        }
        cout << "Total Points: " << totalPoints << endl << endl;;

        cin >> continueLooping;
    } while (continueLooping != 'q');
}