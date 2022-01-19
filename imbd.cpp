


/**
 * This project include making the apropiates lists
 * for managing the Data of the movies read from the
 * CSV file
 * 
 * -->> Some functions are also implemented at the end!
 * -->> UnComment the function you want to try  in the "main"!
*/







#include <iostream>
#include <istream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <list>
#include <iterator>
#include <algorithm>
#include <iomanip>  


using namespace std;

/**
 * Using forward class decleration 
 * for hadling class related error
*/

class ActorNode;
class MovieNode;
class DirectorNode;
class MovieDirectedNode;
class MoviesActedInNode;
class YearNode;
class RatingNode;
class GenreNode;

/**
 * Global Nodes for saving the location
 * of the curent processing nodes
 * and refreccing the MOvie node with it
*/

MovieNode *currentMovNode;
ActorNode *currActor1;
ActorNode *currActor2;
ActorNode *currActor3;
ActorNode *currentActor;
DirectorNode *currentDirectorNode;
YearNode *currentYearNode;
RatingNode *currRattingNode;
GenreNode *currGenreNode;


float strToFlt(string flot_str) {
    try{
        return stof(flot_str);
    }
    catch(const std::exception& e){
        // std::cerr << e.what() << " Error flt value:  "<< flot_str <<'\n';
        return 0;
    }
}

int strToInt(string int_str){
    try{
        return stoi(int_str);
    }
    catch(const std::exception& e){   // std::cerr << e.what() << '\n';
        return 0;
    }
}

bool isNumber(string s){
	for (int i = 0; i < s.length(); i++)
		if (isdigit(s[i]) == false)
			return false;
    if(s.empty())  return false;
	return true;
}

bool isAlpha(string s){
	for (int i = 0; i < s.length(); i++)
		if (isalpha(s[i]) == false && s[i] != '_')
			return false;
    if(s.empty())  return false;
	return true;
}



/**
 * Maintaining a genre list
 * -->> putting movies of specific
 * genres in a specific node
*/

class MoviesUnderGnere {
    public:
    MovieNode* movieNode;
    MoviesUnderGnere* next;
};



class Genre{

    public:
    string genre;
    MoviesUnderGnere *movieNodeStart;

};

class GenreNode {
    public:
    Genre data;
    GenreNode *next;
};

class GenreList {

    public:

    GenreNode *start = NULL;
    GenreNode *last = NULL;
    GenreNode *loc = NULL;
    GenreNode *preLoc = NULL;


    bool isEmpty(){
        return (start == NULL);
    }

    /**
     * Reading the genre data of the file
     * using the vectors
    */

    vector<string> genreVector(string genresString) {
        vector<string> genres;
        string genre;
        stringstream genresStream(genresString);

        genre.clear();

        while (getline(genresStream, genre, '|')) 
        { 
            genres.push_back(genre); 
        } 
        return genres;
    }

    void addingGenreNodes(string genres){
        vector<string> row = genreVector(genres);
        int i = 0;

        for(auto item : row ) {
            addNode(makeGenreNode(item));
            i++;
        }
    }

    void addNode(GenreNode* genreNode) {

        search(genreNode->data.genre);

        if(loc == NULL){
            GenreNode *newnode = genreNode;

            if (isEmpty()){
                start = newnode;
                last = newnode;
            }
            else {
            last->next = newnode;
            last = newnode;
            }

            addMovieRef(newnode);
        }
        else {
            addMovieRef(loc);
            }
    }

    GenreNode* makeGenreNode(string genre) {

        GenreNode* genreNode = new GenreNode();
        genreNode->data.genre = genre;

        return genreNode;
    }

    void addMovieRef(GenreNode* newnode) {
        
        GenreNode* genreNode = new GenreNode();
        genreNode = newnode;

        MoviesUnderGnere* moviesUnderGnere = new MoviesUnderGnere();
        moviesUnderGnere->movieNode = currentMovNode;

        if (genreNode->data.movieNodeStart != NULL) 
        {
            MoviesUnderGnere *temp = genreNode->data.movieNodeStart;

            while (temp->next != NULL) {
                temp = temp->next;
            }

            temp->next  = moviesUnderGnere;
        }
        else {
            genreNode->data.movieNodeStart = moviesUnderGnere;
        }
    }

    /**
     * Checking that if the node is not already in the list
     * --> Not adding it again to the list if so
     * --> loc will contain the position of the node in case data founf
     * --> loc will be null if actor notfound
    */

   void search(string genre){
        loc = start;

        while( loc != NULL && loc -> data.genre != genre){
            preLoc = loc;
            loc = loc -> next; 
        }

   }


    void print(){

        GenreNode *temp = start;

        while(temp != NULL){
            cout << "Genre : " << temp->data.genre;
            cout << endl;
            temp = temp->next;
        }

    }
};

/**
 * Managing a movie rating list class
 * movies with the same rating wil be in the
 * same rating node
 * -->> this will also be managed as sorted
*/


class MoviesInThatRating {
    public:
    MovieNode *movieNode;
    MoviesInThatRating *next;
};


class Rating{
    public:
    float rating;
    MoviesInThatRating *moviesRatingStart;
};


class RatingNode {
    public:
    Rating data;
    RatingNode *next;

};


class RatingList {

    public:

    RatingNode *start = NULL;
    RatingNode *last = NULL;
    RatingNode *loc = NULL;
    RatingNode *preLoc = NULL;


    bool isEmpty(){
        return (start == NULL);
    }

    /*
    * for inserting the value at the
    * top of the list 
    *    */
    void insertAtFirst(RatingNode* ratingNode){
        RatingNode *newnode = ratingNode;
        
        newnode -> next = start;
        start = newnode;

        currRattingNode = newnode;


    }

    /*
    * for inserting the value at the
    * end of the list 
    *    */
    void insertAtLast(RatingNode* ratingNode){
        RatingNode *newnode = ratingNode;
    
        newnode -> next = NULL;
        last -> next = newnode;
        last = newnode;

        currRattingNode = newnode;
    }
    /**
     * inserting the value in a sorted oder
     * and updating the nodes accordingly
    */
    void insertSorted(RatingNode* ratingNode){

        RatingNode *newnode = ratingNode;

        float ratToAdd = ratingNode->data.rating; 
        search(ratToAdd);
       
        if(isEmpty()) {
            start = newnode;
            last = newnode;
        }


        else if (ratToAdd < start->data.rating){
            insertAtFirst(newnode);
            return;
        }
        else if (ratToAdd > last -> data.rating){
             insertAtLast(newnode);
             return;
        }

        if(loc != NULL){
            
            if(loc->data.rating == ratToAdd){
                // cout << "Value already exists " << endl;
                currRattingNode = loc;
            }
          
            else{
                // cout <<  loc->data.rating << endl ;
                newnode -> next = preLoc->next;
                preLoc->next = newnode;
                currRattingNode = newnode;
            }
        }
        else {
            currRattingNode = newnode;
        }
    }    

    /**
     * Checking that if the node is not already in the list
     * --> Not adding it again to the list if so
     * --> loc will contain the position of the node in case data founf
     * --> loc will be null if rating notfound
    */

   void search(float rating){
        loc = start;
        preLoc = NULL;

        while( loc != NULL && loc -> data.rating < rating){
            preLoc = loc;
            loc = loc -> next; 
        }
   }


    void print(){

        RatingNode *temp = start;

        while(temp != NULL){
            cout << "Rating : " << temp->data.rating;
            cout << endl;
            temp = temp->next;
        }
    }

    RatingNode* ratingLoc() {
        return loc;
    }

};


/**
 * Managing a year class and nodes will be added in sorted order
*/

class MoviesInThatYear {
    public:
    MovieNode *movieNode;
    MoviesInThatYear *next;
};

class Year{

    public:
    int year;
    MoviesInThatYear *moviesInYearStart;
};


class YearNode {
    public:
    Year data;
    YearNode *next;

};


class YearList {

    public:

    YearNode *start = NULL;
    YearNode *last = NULL;
    YearNode *loc = NULL;
    YearNode *preLoc = NULL;


    bool isEmpty(){
        return (start == NULL);
    }

    void insertAtFirst(YearNode* yearNode){
        YearNode *newnode = yearNode;
        
        newnode -> next = start;
        start = newnode;

        currentYearNode = newnode;
    }

    void insertAtLast(YearNode* yearNode){
        YearNode *newnode = yearNode;
        // cout << "last was : " << last->data.year << endl; 
        newnode -> next = NULL;
        last -> next = newnode;
        last = newnode;

        currentYearNode = newnode;
    }
   
    /**
     * inserting the value in a sorted oder
     * and updating the nodes accordingly
    */
    void insertSorted(YearNode* yearNode){

        if(yearNode != NULL) {

            YearNode *newnode = yearNode;

            int yearToAdd = yearNode->data.year; 
            search(yearToAdd);
          
            if(isEmpty()) {
                start = newnode;
                last = newnode;
            }
            else if (yearToAdd < start->data.year){
                insertAtFirst(newnode);
                return;
            }
            else if (yearToAdd > last -> data.year){
                insertAtLast(newnode);
                return;
            }

            if(loc != NULL){
                
                if(loc->data.year == yearToAdd){
                    currentYearNode = loc;

                }
            
                else{
                    newnode -> next = preLoc->next;
                    preLoc->next = newnode;

                    currentYearNode = newnode;
                }
            }
            else {
                currentYearNode = newnode;
            }
        }
        else 
        {
            currentYearNode = NULL;
        }
    }    


   void search(int year){
        loc = start;
        preLoc = NULL;

        while( loc != NULL && loc -> data.year < year){
            // cout << loc->data.year << endl;
            preLoc = loc;
            loc = loc -> next; 
        }
      
   }

    void print(){
        YearNode *temp = start;

        while(temp != NULL){
            cout << "Year: " << temp->data.year;
            cout << endl;
            temp = temp->next;
        }
    }

    YearNode* actorLoc() {
        return loc;
    }

};

/**
 * Now maitaing a director list
 * -> Each director node have the move node he had directed
 * -> Each hace its other information i.e name, fb_likes etc.
*/

class MovieDirectedNode {
    public:
    MovieNode *movieNode;
    MovieDirectedNode *next;
};


class Director{
    public:
    string director_name;
    string director_facebook_likes;
    MovieDirectedNode *movieDirectedNodeSart;

};

class DirectorNode {
    public:
    Director data;
    DirectorNode *next;

};


class DirectorList {

    public:

    DirectorNode *start = NULL;
    DirectorNode *last = NULL;
    DirectorNode *loc = NULL;
    DirectorNode *preLoc = NULL;


    bool isEmpty(){
        return (start == NULL);
    }

    void addDirectorNode(DirectorNode *directorNode){
        search(directorNode->data.director_name);

        if(loc == NULL){
            DirectorNode *newnode = directorNode;

            if (isEmpty()){
                start = newnode;
                last = newnode;
            }
            else {
            last->next = newnode;
            last = newnode;
            }

            currentDirectorNode = newnode;
        }
        else {
            currentDirectorNode = loc;
        }


    }

   void search(string directorName){
        loc = start;

        while( loc != NULL && loc -> data.director_name != directorName){
            preLoc = loc;
            loc = loc -> next; 
        }
   }

    void print(){

        DirectorNode *temp = start;

        while(temp != NULL){
            cout << "Director Name: " << temp->data.director_name;
            cout << endl;
            temp = temp->next;
        }
    }

    DirectorNode* actorLoc() {
        return loc;
    }

};



class Actor{

    public:
    string actor_name;
    string actr_facebook_likes;
    MoviesActedInNode *movieNodeStart;

    
};

class ActorNode {
    public:
    Actor data;
    ActorNode *next;

};

class ActorList {

    public:

    ActorNode *start = NULL;
    ActorNode *last = NULL;
    ActorNode *loc = NULL;
    ActorNode *preLoc = NULL;


    bool isEmpty(){
        return (start == NULL);
    }

    /**
     * creating a new actors nodes and adding
     * them to list
    */
    void addActorNode(ActorNode *actorNode){
        search(actorNode->data.actor_name);

        if(loc == NULL){
            // cout << "Adding node to main list " << actorNode->data.actor_name << endl;
            ActorNode *newnode = actorNode;

            if (isEmpty()){
                start = newnode;
                last = newnode;
            }
            else {
            last->next = newnode;
            last = newnode;
            }

            currentActor = newnode;
        }
        else {
            // cout << loc->data.actor_name << "found!!!!"<< endl;
            // cout << actorNode->data.actor_name <<" Actor Already Exist!" << endl;
            currentActor = loc;
        }


    }

   void search(string actorName){
        loc = start;

        while( loc != NULL && loc -> data.actor_name != actorName){
            preLoc = loc;
            loc = loc -> next; 
        }

   }

    void print(){

        ActorNode *temp = start;
        while(temp != NULL){
            cout << "Actor Name: " << temp->data.actor_name;
            cout << " Actor FB likes: " << temp->data.actr_facebook_likes;
            cout << endl;
            temp = temp->next;
        }

    }

    ActorNode* actorLoc() {
        return loc;
    }
};



/**
 * This movie node helps in maintaining a small list 
 * of three actors acted in a movie
*/

class ActorInMovieNode {
    public:
    ActorNode *actorNode;
    ActorInMovieNode *next;

    ActorInMovieNode() {
        actorNode = NULL;
        next = NULL;
    }
};


class ActorsActedList {

    public:

    ActorInMovieNode *start = NULL;
    ActorInMovieNode *last = NULL;

    bool isEmpty() {
        return start == NULL;
    }

    void addNode(ActorNode *actorNodeData) {

        if(!actorNodeData->data.actor_name.empty()){
            ActorInMovieNode *newnode = new ActorInMovieNode();
            newnode->actorNode = actorNodeData;

            if (isEmpty()) {
                start = newnode;
                last = newnode;
            }
            else {
                last->next = newnode;
                last = newnode;
            }
        }
    }

    ActorInMovieNode* getStart(){
        return start;
    }

};


class Movie{
    public:

    string movie_Title;

    unsigned short int title_year;    
    float imdb_score;
     
    DirectorNode *director;

    unsigned short int num_critic_for_reviews;
    unsigned short int duration;

    // saving a pointer of the actor node
    ActorInMovieNode *actorActedNodeStart;

    string genre;
    int gross;

    unsigned short int num_voted_users;
    unsigned short int cast_total_facebook_likes;

    unsigned short int facenumber_in_poster;
    string plot_keywords;
    string movie_imdb_link;

    unsigned short int num_user_for_reviews;
    string language;
    string country;
    string content_rating;
    int budget;
    float aspect_ratio;
    unsigned short int movie_facebook_likes;
    bool color;

};

class MovieNode{
    public:

    Movie data;
    MovieNode *next;

    MovieNode(){};

};

class MoviesActedInNode{
    public:
    MovieNode *pdata;
    MoviesActedInNode *next;

    MoviesActedInNode() {
        pdata = NULL;
        next = NULL;
    }
};

class MovieList{

    public:

    MovieNode *start = NULL;
    MovieNode *last = NULL;



    bool isEmpty(){
        return (start == NULL);
    }


    void addMovieNode(MovieNode* newnode){
        // MovieNode *newnode = new MovieNode();
        currentMovNode = newnode;

        addReftoActorNode(newnode);
        addReftoDirectorNode(newnode);
        if(currentYearNode != NULL) addReftoYearNode(newnode);
        if(currRattingNode != NULL) addReftoRatingNode(newnode);

        // newnode->data = movieData;
       
       if (isEmpty()){
            start = newnode;
            last = newnode;
        }
        else {
        last->next = newnode;
        last = newnode;
        }
    }



    void addReftoRatingNode(MovieNode* newnode) {
        // currentYearNode->data.movieDirectedNodeSart
        MoviesInThatRating* movInRating = new MoviesInThatRating();
        movInRating->movieNode = newnode;

        if(currRattingNode->data.moviesRatingStart != NULL)
        {
            MoviesInThatRating *temp = currRattingNode->data.moviesRatingStart;

            while (temp->next != NULL) {
                temp = temp->next;
            }

            temp->next  = movInRating;
        }
        else
        {
            currRattingNode->data.moviesRatingStart = movInRating;
        }
    }

    void addReftoYearNode(MovieNode* newnode) {
        // currentYearNode->data.movieDirectedNodeSart
        MoviesInThatYear* movInYear = new MoviesInThatYear();
        movInYear->movieNode = newnode;

        if(currentYearNode->data.moviesInYearStart != NULL)
        {
            MoviesInThatYear *temp = currentYearNode->data.moviesInYearStart;

            while (temp->next != NULL) {
                temp = temp->next;
            }

            temp->next  = movInYear;
        }
        else
        {
            currentYearNode->data.moviesInYearStart = movInYear;
        }

    }

    void addReftoDirectorNode (MovieNode* newnode) {

        // cout << "Current Director is : " << currentDirectorNode->data.director_name << endl;

        MovieDirectedNode *movDirecteNode = new MovieDirectedNode();
        movDirecteNode->movieNode = newnode;

        if (currentDirectorNode->data.movieDirectedNodeSart != NULL) 
        {
            MovieDirectedNode *temp = currentDirectorNode->data.movieDirectedNodeSart;

            while (temp->next != NULL) {
                temp = temp->next;
            }

            temp->next  = movDirecteNode;
        }
        else {
            currentDirectorNode->data.movieDirectedNodeSart = movDirecteNode;
        }

    }

    void addReftoActorNode(MovieNode* newnode) {

        if(currActor1 != NULL) addMovsActedNodeOfActors(newnode, currActor1);
        if(currActor2 != NULL) addMovsActedNodeOfActors(newnode, currActor2);
        if(currActor3 != NULL) addMovsActedNodeOfActors(newnode, currActor3);
        
    }

    void addMovsActedNodeOfActors(MovieNode *newnode, ActorNode *actor) {

        MoviesActedInNode *movActedInNode = new MoviesActedInNode();
        movActedInNode->pdata = newnode;

        if(actor->data.movieNodeStart != NULL){
            MoviesActedInNode *temp = new MoviesActedInNode();
            temp = actor->data.movieNodeStart;

            while (temp ->next != NULL) {
                temp = temp->next;
            }
            temp->next = movActedInNode; 
        }
        else {
            actor->data.movieNodeStart = movActedInNode;
        }

    }

    void print(){

        MovieNode *temp = start;
        while(temp != NULL){
            cout << "Movie Title: " << temp->data.movie_Title << "";
            cout << endl;

            cout << "Movie actor1: " << temp->data.actorActedNodeStart->actorNode->data.actor_name << endl;
            if (temp->data.actorActedNodeStart->next != NULL)
                cout << "Movie actor2: " << temp->data.actorActedNodeStart->next->actorNode->data.actor_name << endl;
            if (temp->data.actorActedNodeStart->next->next != NULL)
                cout << "Movie actor3: " << temp->data.actorActedNodeStart->next->next->actorNode->data.actor_name << endl;
                
            cout << endl;
            cout << endl;
            temp = temp->next;
        }

    }

};

   ActorList *actorList = new ActorList(); 
   MovieList *movieList = new MovieList();
   DirectorList *directorList = new DirectorList();
   YearList *yearList = new YearList();
   RatingList *ratingList = new RatingList();
   GenreList *genreList = new GenreList();



/**
 * The controller class raed all the data from the CSV file
 * Its is being used as a parser
 * It is also managing the data of the List created above
 * It helps in maintaining the data of all the above lists.
*/


class Controller{
    

//    ActorsActedList *actorsActedList = new ActorsActedList();


    public:

    void read_record() {
 
        fstream fin;     
        fin.open("IMDB_Top5000-SEECS.csv", ios::in); 

        if(!fin.is_open()){
            cout <<"Eror Reading the file" << endl;
        }

        else{

            vector<string> row; 
            string line, word, temp; 
            int count = 2;

            getline(fin, line);

            while (count < 5000) { 
                row.clear(); 
         
                getline(fin, line); 
                stringstream s(line); 
                
    	        getline(s, word, '/'); // for recognizing the movie column
                row.push_back(word); 
                
    	        getline(s, word, ',');

                while (getline(s, word, ',')) 
                { 
                    row.push_back(word); 
                } 
            
                // cout << "Line No: " << count << endl;

                if(!isAlpha(row[3])) ratingList->insertSorted(makeRatingNode(row[3]));
                yearList->insertSorted(makeYearNode(row[2]));
                directorList->addDirectorNode(makeDirectorNode(row));
                movieList->addMovieNode(movieData(row));
                genreList->addingGenreNodes(row[1]);
                

                count++;
                
            } 
        }
    } 




    MovieNode* movieData(vector<string> row) {

        Movie mov;

        mov.movie_Title = row[0];
        mov.director = currentDirectorNode;
        if(isNumber(row[2])) mov.title_year = stoi(row[2]);
        mov.actorActedNodeStart = makeActorActedNode(row);
        if(!isAlpha(row[3])) mov.imdb_score = strToFlt(row[3]);
        mov.genre = row[1];
        if(isNumber(row[7]))mov.duration =  stoi(row[7]);
        if(isNumber(row[15]))mov.num_voted_users =  stoi(row[15]);
        if(isNumber(row[16]))mov.cast_total_facebook_likes =  stoi(row[16]);
        if(isNumber(row[17]))mov.facenumber_in_poster =  stoi(row[17]);
        mov.plot_keywords = row[18];
        mov.movie_imdb_link = row[19];
        if(isNumber(row[20])) mov.num_voted_users = strToInt(row[20]);

        mov.language = row[21];
        mov.country = row[22];
        mov.content_rating = row[23];
        if(isNumber(row[24])) mov.budget = strToInt(row[24]);
        mov.aspect_ratio = strToFlt(row[25]);
        if(isNumber(row[26])) mov.movie_facebook_likes = strToInt(row[26]);
        
        // if(isAlpha(row[27])) {
        //     if(row[27] == "Color") mov.color = true;
        //     else mov.color = false;
        // }
        
        MovieNode *movieNode = new MovieNode;
        movieNode->data = mov;

        return movieNode;
    }




    RatingNode* makeRatingNode(string rating) {
        RatingNode* ratingNode = new RatingNode();
        ratingNode->data.rating = strToFlt(rating);
        return ratingNode;
    }


    YearNode* makeYearNode(string year_str) {
        YearNode* yearNode = new YearNode();
        if(isNumber(year_str)){
            yearNode->data.year = stoi(year_str);        
        }
        else { 
            yearNode = NULL;
        }
        return yearNode;
    }


    DirectorNode* makeDirectorNode(vector<string> row) {
        DirectorNode *director = new DirectorNode();

        director->data.director_name = row[4];
        director->data.director_facebook_likes = row[5];
        director->data.movieDirectedNodeSart = NULL;

        // directorList->addDirectorNode(director);

        return director;
    }

    /**
     * actors in a movie node will creat a list that will
     * that will creat a list of at the most three actors
     * -->> also these actos will also be added to the "Actor list"
     * so the actors list will also gonna be populated side by side
     * the movie list
    */

    ActorInMovieNode* makeActorActedNode(vector<string> row) {

        // this list will contian the data of the three actrors acted

        ActorsActedList *actorsActedList = new ActorsActedList();

        // if actors data is not available then dont make a node
        for (int i = 8; i < 13 && !row[i].empty(); i += 2)
        {
            ActorNode *tempActorNode = new ActorNode();

            tempActorNode->data.actor_name = row[i];
            tempActorNode->data.actr_facebook_likes =row[i + 1];


            // adding actor to the "Actor list"
            if(!row[i].empty())
            {
                actorList->addActorNode(tempActorNode);
                if (i == 8) currActor1 = currentActor;
                if (i == 10) currActor2 = currentActor;
                if (i == 12) currActor3 = currentActor;
            }
            // adding actor to the acted list
            actorsActedList->addNode(tempActorNode);
        }


        return actorsActedList->getStart();
    }




};


/**
 * Operator class contain all the
 * functions related to the above implemeted
 * lists
*/



class Operations {

    public:


    /**
     * 
     * -->> FUNCTIONS RELATED TO THE ACTOR LIST
     * 
    */

    void searchActor(string actor_name) {
        
        ActorNode *temp = actorList->start;
        while (temp->data.actor_name != actor_name && temp->next != NULL) {
            temp = temp->next;
        }
        cout << "Actor name : " << temp->data.actor_name << endl;
        printMoviesActed(temp);

    }

   
    void actorActedTogether(string actor1, string actor2) {

        ActorNode *temp = actorList->start;
        while (temp->data.actor_name != actor1 && temp->next != NULL) {
            temp = temp->next;
        }

        // cout << "actor found : " << temp->data.actor_name << endl;

        
        MoviesActedInNode *movs = new MoviesActedInNode;
        movs = temp->data.movieNodeStart;
        MoviesActedInNode *temp2 = movs;
        cout << temp2->pdata->data.movie_Title << endl;

        int count = 0;
        while(temp2 != NULL){
            ActorInMovieNode *temp3 = temp2->pdata->data.actorActedNodeStart;
            while(temp3 != NULL){
                if(temp3->actorNode->data.actor_name == actor2){
                    count++;
                    cout << actor1 << " and " << actor2 << " acted in -->" << temp2->pdata->data.movie_Title<< endl; 
                }
                temp3 = temp3->next;
            }
            
            temp2 = temp2->next;
        }
        if(count == 0){
                cout << "They never acted together!" << endl;
        }

    }


   void printMoviesActed(ActorNode *actor) {
       MoviesActedInNode *temp = actor->data.movieNodeStart;
        cout << "Movies Acted In: " << endl;
        int count = 0;
       do {
            cout << "\tYear : " << temp->pdata->data.title_year ;
            cout << "  ->   Movie Title: " << temp->pdata->data.movie_Title << endl;
            temp = temp->next;
            count++;

       } while(temp != NULL);
       cout << "No Movies Acted In: " << count << endl;
   }

   void searchCoactors(string actor_name) {
       ActorNode *temp = actorList->start;
        while (temp->data.actor_name != actor_name && temp->next != NULL) {
            temp = temp->next;
        }
        cout << "Actor name : " << temp->data.actor_name << endl;

        printCoactors(temp);

   }
   

   void printCoactors(ActorNode* actor) {
       MoviesActedInNode* movies = actor->data.movieNodeStart;
       MoviesActedInNode* temp = movies;
       while(temp != NULL){
           printCoactorOfMovie(temp->pdata, actor->data.actor_name );
           cout << endl;
           temp = temp->next;
       }
   }

   void printCoactorOfMovie(MovieNode* movie, string actor_name) {

        ActorInMovieNode* actorsInMov =  movie->data.actorActedNodeStart;
        ActorInMovieNode* temp =  actorsInMov;
        

        while(temp != NULL) {
            if (actor_name != temp->actorNode->data.actor_name){
                cout << left << setw(20) << "\tCo-Actor : " << temp->actorNode->data.actor_name ;
                cout << right << setw(20) << "--> Movie: " <<movie->data.movie_Title << endl;
                // printCo_CoActors(temp->actorNode->data.movieNodeStart->pdata, temp->actorNode->data.actor_name );
            }
            temp = temp->next;
        }

   }

    void printCo_CoActors(MovieNode* movie, string actor_name) {

       ActorInMovieNode* actorInMov  = new ActorInMovieNode;
       actorInMov = movie->data.actorActedNodeStart;
       ActorInMovieNode* temp  = actorInMov;
        
        while(temp != NULL){
            if(actor_name != temp->actorNode->data.actor_name)
                cout << "\t\t|-----> Co-CoActor: " << temp->actorNode->data.actor_name << endl;
            temp = temp->next;
        }
   }

  /**
     * 
     * -->> FUNCTIONS RELATED TO THE DIRECTOR LIST
     * 
    */


  void searchDirector(string director_name) {
        
        DirectorNode *temp = directorList->start;
        while (temp->data.director_name != director_name && temp->next != NULL) {
            temp = temp->next;
        }
        cout << "Director Name : " << temp->data.director_name << endl;
        printMoviesDirected(temp);
    }
   
   void printMoviesDirected(DirectorNode *director) {
       MovieDirectedNode *temp = director->data.movieDirectedNodeSart;
        cout << "Movies Directed: " << endl;

       do {
           cout << " -> "<< temp->movieNode->data.movie_Title<< endl;
           temp = temp->next;
       } while(temp != NULL);
   }

  /**
     * 
     * -->> FUNCTIONS RELATED TO THE GENRE LIST
     * 
    */

    GenreNode* searchGenreWise(string genre) {
        
        GenreNode *temp = genreList->start;
        while (temp->data.genre != genre && temp->next != NULL) {
            temp = temp->next;
        }
        cout << "Genre : " << temp->data.genre << endl;
        // printMoviesInGenre(temp);
        return temp;
    }
   
   void printMoviesInGenre(string genre_str) {
       
        MoviesUnderGnere *temp = searchGenreWise(genre_str)->data.movieNodeStart;
        cout << "Movies under this Genre: " << endl;

       do {
           cout << "-> "<< temp->movieNode->data.movie_Title << endl;
           temp = temp->next;
       } while(temp != NULL);
   }

    void printDirectorsofGnere(string genre_str){
        MoviesUnderGnere *temp = searchGenreWise(genre_str)->data.movieNodeStart;
        cout << "Directors under this Genre: " << endl;

       do {
           cout << "-> "<< temp->movieNode->data.director->data.director_name << endl;
           temp = temp->next;
       } while(temp != NULL);
    }

  /**
     * 
     * -->> FUNCTIONS RELATED TO THE MOVIE LIST
     * 
    */
    void searchMovie(string movie) {
        MovieNode* temp = new MovieNode();
        temp = movieList->start;

        list<MovieNode*> smilar;
        int movCount = 0;
        
        while (temp != NULL) {
            string  str = temp->data.movie_Title;
            if(strstr(str.c_str(),movie.c_str())) {
                smilar.push_back(temp);
                movCount++;
            }
            temp = temp->next;
        }

        if(movCount > 0){
            cout  << "\t\t----------  " << movCount<< " SMILAR MOVIES HAVE FOUND!   ----------" << endl;
            for (std::list<MovieNode*>::iterator it=smilar.begin(); it != smilar.end(); ++it){
                printMovieDeatils(*it);
            }
        }
        else{
            cout << movCount << "NO SMILAR MOVIESFOUND!" << endl;
        }
        
    }

    void printMovieDeatils(MovieNode* temp) {

        cout << endl;
        cout << endl;
        cout << "\t\t------------   MOVIE DETAILS    ------------" << endl;
        cout << endl;
        cout << "\t\tMovie Title: " << temp->data.movie_Title << endl;
        cout << "\t\tRelease Year: " << temp->data.title_year << endl;
        cout << "\t\tGenre : " << temp->data.genre << endl;
        cout << "\t\tRating: " << temp->data.imdb_score << endl;
        cout << "\t\tDirector Name: " << temp->data.director->data.director_name << endl;
        // cout << "Actor Name: " << temp->data.actorActedNodeStart->actorNode->data.actor_name;
        cout << endl;
        cout << "\t\t------------   ACTORS ACTED    ------------" << endl;
        cout << endl;

        printMovieAcotrs(temp->data.actorActedNodeStart);
        cout << endl;
        cout << "\t\t------------   INFO    ------------" << endl;
        cout << endl;
        cout << "\t\tGross: " << temp->data.gross << endl;
        cout << "\t\tNum Voted Users: " << temp->data.num_voted_users << endl;
        cout << "\t\tCast Total FB Likes: " << temp->data.cast_total_facebook_likes << endl;
        cout << "\t\tFacenumber in Poster: " << temp->data.facenumber_in_poster << endl;
        cout << "\t\tPlot KeyWords: " << temp->data.plot_keywords << endl;
        cout << "\t\tMovie IMBD Link: " << temp->data.movie_imdb_link << endl;
        cout << "\t\tNum Users For Revies: " << temp->data.num_user_for_reviews << endl;
        cout << "\t\tLanguage: " << temp->data.language << endl;
        cout << "\t\tCountry: " << temp->data.country << endl;
        cout << "\t\tContent Rating: " << temp->data.content_rating << endl;
        cout << "\t\tBudget: " << temp->data.budget << endl;
        cout << "\t\tAspect Ratio: " << temp->data.aspect_ratio << endl;
        cout << "\t\tMovie FB Likes: " << temp->data.movie_facebook_likes << endl;


    }

    void printMovieAcotrs(ActorInMovieNode* actoracted) {
        ActorInMovieNode *temp = actoracted;
        while (temp != NULL)
        {
            cout << "\t\tActor Name: " << temp->actorNode->data.actor_name << endl;
            cout << "\t\tActor FB Likes: " << temp->actorNode->data.actr_facebook_likes << endl;
            cout << endl;
            temp = temp->next;
        }
        
    }


    /**
     * 
     * -->> FUNCTIONS RELATED TO THE YEAR LIST
     * 
    */
  YearNode* searchYearWise(int year) {
        
        YearNode *temp = yearList->start;
        while (temp->data.year != year && temp->next != NULL) {
            temp = temp->next;
        }
        cout << "Year : " << temp->data.year << endl;
        // printMoviesInYear(temp);
        return temp;
    }
   
   void printMoviesInYear(int year) {
       MoviesInThatYear *temp = searchYearWise(year)->data.moviesInYearStart;
        cout << "Movies Directed in given Year: " << endl;

       do {
           cout << "-> "<< temp->movieNode->data.movie_Title << endl;
           temp = temp->next;
       } while(temp != NULL);
   }

   void printYearInc() {
       YearNode* temp = yearList->start;

       while(temp != NULL){
           printMoviesInYear(temp);
           temp = temp->next;
       }
   }

   void printYearDec() {
        reverseRecusrion(yearList->start);
   }

   void reverseRecusrion(YearNode *node){
        if(node){
            reverseRecusrion(node -> next);
            printMoviesInYear(node);
        }
    }
     void printMoviesInYear(YearNode* year) {
        MoviesInThatYear *temp = year->data.moviesInYearStart;
        cout << "----------------------------" << endl;
        cout << "Movies Directed in Year: " << year->data.year << endl;
       do {
           cout << "-> "<< temp->movieNode->data.movie_Title << endl;
           temp = temp->next;
       } while(temp != NULL);
   }

  /**
     * 
     * -->> FUNCTIONS RELATED TO THE RATING LIST
     * 
    */
 
   void searchRatingWise(float rating) {
        
        RatingNode *temp = ratingList->start;
        while (temp->data.rating != rating && temp->next != NULL) {
            temp = temp->next;
        }
        cout << "Rating : " << temp->data.rating << endl;
        printMoviesInRating(temp);
    }
   
   void printMoviesInRating(RatingNode *rating) {
       MoviesInThatRating *temp = rating->data.moviesRatingStart;
        cout << "Movies under this Rating: " << endl;

       do {
           cout << "-> "<< temp->movieNode->data.movie_Title << endl;
           temp = temp->next;
       } while(temp != NULL);
   }
    

};


int main() {
    
    Controller cntrl;
    Operations opr;
    cntrl.read_record();

    // opr.searchMovie("Harry Potter");  // This will Print All movies related to Pirates
    opr.searchActor("Ruth Wilson");
    // opr.searchActor("Johnny Depp");
    // opr.searchCoactors("Ruth Wilson");
    // opr.actorActedTogether("Johnny Depp", "Michiel Huisman");
    // opr.actorActedTogether("Johnny Depp", "Jack Davenport");
    // opr.searchDirector("Gore Verbinski");
    // opr.printDirectorsofGnere("Family");
    // opr.printMoviesInYear(2012);
    // opr.printYearDec();
    // opr.printYearInc();
    // opr.printMoviesInGenre("Family");
    // opr.searchRatingWise(7.2);

}



