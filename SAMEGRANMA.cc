/*
 * Titre : 296190.cc
 * Auteur : Nathann Morand
 * Date 22.11.2021
 * Version 1.1
*/

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>
#include <bitset>

using namespace std;

#define GREATER 1
#define LOWER -1
#define EQUAL 0
#define ENDOFENTRY 3
#define CAPITALIZATIONERROR 1
#define REDUNDANTERROR 2
#define OK 0

struct DictStruct {
    unsigned int nbT; // nombre de lettre dans mot (accèlere la recherche)
    unsigned int nbD; // nombre de lettre unique dans le mots (accèlere la recherche)
    bool IsComposed = false; // indique si le mots est une liste de mot
    string mot; // stock la string du mots lui meme pour être affiche si nécéssaire
    string sortedStringOnChar; //alpha, permet de checker si deux mots sont anagramme
    vector<string> subwords; // si le mots est une liste de mots alors stocké ici
};

const string NOT_IN_CAPITAL_LETTERS("The word is not purely in capital letters");
const string DOUBLE_WORD("Each word can be present only once");
const string EMPTY_DICO("The dictionnary cannot be empty");
const string NO_ANAGRAM("There is no anagram for this message and this dictionary");

int lecture(vector<DictStruct> &dictionnary, bool exitIfError);
void manageException(int code, bool exitIfError, bool &firstNotInCapital);
void displayDict(vector<DictStruct> &dictionnary);
void displayDebug(vector<DictStruct> &dictionnary);
/*
 * test que la string respectent les conditions demandé
 * (en majuscule et pas dans le dictionnaire)
 *
 * Entree : Adresse tableau struct
 * Sortie : code d'erreur
 * OK = l'entrée respecte les conditions
 * CAPITALIZATIONERROR = not fully in capital
 * REDUNDANTERROR = the word is aldready in the dictionnary
*/
int test(DictStruct structStringToTest,int index,vector<DictStruct> &dictionnary);
bool isCaps(string structStringToTest);
bool inDict(DictStruct structStringToTest,int index,
            vector<DictStruct> &dictionnary);
/*
 * trouve la position ou se trouve vs devrait se trouver une string
 * dans le dictionnaire / liste de string et retourne son index
 * (dans le cas ou il n'est pas dans le
 * dico/liste de string, l'index représente la position
 * où il devrais être rangé.
 *
 * Entrée : String à tester, structure dictionnaire / liste de string
 * Sortie : int de l'index
 *
*/
int findPos(DictStruct structStringToTest, vector<DictStruct> &dictionnary);
int findPosAlphabet(string stringEntree, vector<string> &listOfString);

void insert(int index, DictStruct structToInsert,vector<DictStruct> &dictionnary);
void insert(int index, string stringToInsert, vector<string> &listOfString);
vector<string> sortnbTAlpha(vector<string> list);
vector<string> sort(vector<string> list);
string sort(string list);
/*
 * compute different value related to the string (nbT, nbD, alpha)
 *
 * Entrée : string
 * Sortie : structDict
 *
*/
DictStruct computeString(string word);
DictStruct computeStringFast(vector <string> multiword);
int computeNbD(string sortedCharWord);
/*
 * compute every plausible combination of the words in the dict
 * and update the anaSpace with the permutation
 * so we have a nice and fast lookup table of every possible anagram
 *
 * Entrée : listOfStruct (must be sorted on char (alpha))
 * Sortie : rien (update via référence)
 *
*/
void computeMultiverseFromDict(vector<DictStruct> &dico,
                               vector<DictStruct> &anaSpace, DictStruct anagram);
vector<vector<string>> combination(vector<string> listOfString, unsigned int size);
void displayAnagram(vector<string> &listOfString);
int factorial(int n);
void permutation(vector<string> &listOfString,
                 vector<vector<string>> &permutList,int z=0);
int compare(DictStruct structA, DictStruct structB);
int compareFast(DictStruct structA, DictStruct structB);
vector<int> findAm(DictStruct anagram,vector<DictStruct> &dictionnary);
DictStruct concatenate(vector<DictStruct>const &list);
/*
 * Anagram, retourne les anagramme possible en fonction du dico et des
 * string fournit en entrée
 *
 * Entrée : listOfStruct listOfString
 * Sortie : rien, affiche les anagrammes
 *
*/
void anagram(vector<DictStruct> dict, DictStruct anagrammeStruct);
unsigned int word2BinVect(string word);
vector<DictStruct> trimDict(vector<DictStruct> dict, DictStruct anagram);

int lecture(vector<DictStruct> &dictionnary, bool exitIfError)
{
    string entree;
    cin >> entree;
    int status = OK;
    bool firstNotInCapital = true;
    while(entree != "." and entree != "*")
    {
        DictStruct wordCandidate = computeString(entree);
        int index = findPos(wordCandidate, dictionnary);
        status = test(wordCandidate, index, dictionnary);
        if(status == OK)
        {
            insert(index, wordCandidate, dictionnary);
        }
        else
        {
            manageException(status, exitIfError, firstNotInCapital);
        }
        cin >> entree;
    }
    if((dictionnary.size() == 0) and exitIfError)
    {
        cout << EMPTY_DICO << endl;
        exit(0);
    }
    if(entree == "*")
    {
        exit(0);
    }
    if(entree == ".")
    {
        return ENDOFENTRY;
    }
    return status;
}

void manageException(int status, bool exitIfError, bool &firstNotInCapital)
{
    if(status == CAPITALIZATIONERROR)
    {
        if(firstNotInCapital and not exitIfError)
        {
            cout << endl;
            firstNotInCapital = false;
        }
        cout << NOT_IN_CAPITAL_LETTERS<< endl;
        if(exitIfError)
        {
            exit(0); // quit if not fully in caps and reading dict
        }
    }
    else if((status == REDUNDANTERROR) and exitIfError)
    {
        cout << DOUBLE_WORD << endl;
        exit(0); // quit if aldready in dict (dont apply for anagra)
    }
}

void displayDict(vector<DictStruct> &dictionnary)
{
    for(unsigned int i = 0; i < dictionnary.size(); ++i)
    {
        cout << dictionnary[i].mot << endl;
    }
}

void displayDebug(vector<DictStruct> &dictionnary)
{
    for(unsigned int i = 0; i < dictionnary.size(); ++i)
    {
        cout <<"mot " << dictionnary[i].mot << endl;
        cout <<"sorted " << dictionnary[i].sortedStringOnChar << endl;
        cout <<"nbT " << dictionnary[i].nbT << endl;
        cout <<"nbD " << dictionnary[i].nbD << endl;
        cout << "---------" << endl;
    }
}

int test(DictStruct structStringToTest, int index,
            vector<DictStruct> &dictionnary)
{
    if(not isCaps(structStringToTest.mot))
    {
        return CAPITALIZATIONERROR;
    }
    else if(inDict(structStringToTest, index, dictionnary))
    {
        return REDUNDANTERROR;
    }
    else
    {
        return OK;
    }
}

bool isCaps(string structStringToTest)
{
    for(unsigned int charIndex = 0; charIndex<structStringToTest.length(); charIndex++)
    {
        if(structStringToTest.at(charIndex)<65 or structStringToTest.at(charIndex)>90)
        {
            return false;
        }
    }
    return true;
}

bool inDict(DictStruct structStringToTest, int index,
            vector<DictStruct> &dictionnary)
{
    if(index == 0)
    {
        return false;
    }
    if(dictionnary[index].mot == structStringToTest.mot)
    { // check si la string est la meme -> dans le dico
        return true;
    } // sinon c'est l'adresse a l'aquelle il devra etre rangé
    return false;
}

int findPos(DictStruct structStringToTest, vector<DictStruct> &dictionnary)
{
    unsigned int i = 0;
    bool wordDontMatch = true;
    while(wordDontMatch)
    {
        if(i >= dictionnary.size())
        {
            return (i); // prevent segfault
        }
        int resultCompairaison = compare(dictionnary[i], structStringToTest);
        if (resultCompairaison == 0)
        {
            string word = dictionnary[i].mot;
            if(structStringToTest.mot > word)
            {
                ++i;
            }
            else
            {
                wordDontMatch = false;
            }
        }
        else if (resultCompairaison > 0)
        {
            wordDontMatch = false;
        }
        else if (resultCompairaison < 0)
        {
            ++i;
        }
    }
    return i;
}

int findPosAlphabet(string stringEntree, vector<string> &listOfString)
{
    unsigned int positionInIndex = 0;
    bool wordIsLower = true;
    // tant que la lettre est pas la meme que dans le dico, regarder
    // le mot suivant
    while(wordIsLower)
    {
        if(listOfString.size() <= positionInIndex)
        { // si l'index sors de la taille de la liste on sors ici
            return positionInIndex;
        }
        string StringFromIndex = listOfString[positionInIndex];
        if(stringEntree > StringFromIndex)
        {
            ++positionInIndex;
        }
        else
        {
            wordIsLower = false;
        }
    }
    return positionInIndex;
}

void insert(int index, DictStruct structToInsert,vector<DictStruct> &dictionnary)
{
    DictStruct swapA = structToInsert;
    unsigned int idx(index);
    // attention <= car taille apres ajout = taille + 1
    if(idx < dictionnary.size())
    {
        swapA = dictionnary[idx];
        dictionnary[idx] = structToInsert;
        for(unsigned int i = idx+1; i<dictionnary.size(); ++i)
        {
            DictStruct swapB = dictionnary[i];
            dictionnary[i] = swapA;
            swapA = swapB;
        }
    }
    dictionnary.push_back(swapA);
}

void insert(int index, string stringToInsert, vector<string> &listOfString)
{
    unsigned int idx(index);
    string buffer = stringToInsert;
    if(idx < listOfString.size())
    {
        buffer = listOfString[idx];
        listOfString[idx] = stringToInsert;
        for(unsigned int i = idx+1; i<listOfString.size(); ++i)
        {
            string subBuffer = listOfString[i];
            listOfString[i] = buffer;
            buffer = subBuffer;
        }
    }
    listOfString.push_back(buffer);
}

vector<string> sortnbTAlpha(vector<string> list)
{
    vector<DictStruct> sortedDictList;
    vector<string> sortedList;
    for(unsigned int elemIndex = 0; elemIndex < list.size(); ++elemIndex)
    {
        DictStruct computedString = computeString(list[elemIndex]);
        unsigned int newIndex = findPos(computedString, sortedDictList);
        insert(newIndex, computedString, sortedDictList);
        insert(newIndex, computedString.mot, sortedList);
    }
    return sortedList;
}

vector<string> sort(vector<string> list)
{
    vector<string> sortedList;
    for(unsigned int elemIndex = 0; elemIndex < list.size(); ++elemIndex)
    {
        unsigned int newIndex = findPosAlphabet(list[elemIndex], sortedList);
        insert(newIndex, list[elemIndex], sortedList);
    }
    return sortedList;
}

string sort(string list)
{
    // convertit la string en liste de string et appelle la fonction
    // qui trie des liste de string alphabétiquement
    vector<string> stringAsVectorOfString;
    for (unsigned int i = 0; i < list.length(); ++i)
    {
        stringAsVectorOfString.push_back(string(1,list.at(i)));
    }
    vector<string> VectorOfString = sort(stringAsVectorOfString);
    string output;
    // remet le tout dans une string
    for (unsigned int i = 0; i < VectorOfString.size(); ++i)
    {
        output = output + VectorOfString[i];
    }
    return output;
}

DictStruct computeString(string word)
{
    DictStruct dictEntry;
    dictEntry.mot = word;
    dictEntry.nbT = word.length();
    dictEntry.sortedStringOnChar = sort(word);
    dictEntry.nbD = computeNbD(dictEntry.sortedStringOnChar);
    return dictEntry;
}

DictStruct computeStringFast(vector <string> multiword)
{
    DictStruct dictEntry;
    dictEntry.IsComposed = true;
    dictEntry.subwords = multiword;
    string word;
    for(unsigned int i = 0;i < multiword.size(); ++i)
    {
        word = word + multiword[i];
    }
    dictEntry.sortedStringOnChar = sort(word);
    //dictEntry.nbD = computeNbD(dictEntry.sortedStringOnChar);
    return dictEntry;
}

int computeNbD(string sortedCharWord)
{
    int nbD = 1;
    if(sortedCharWord.length() == 0)
    { // check for edgecase where string is empty
        return 0;
    }
    for(unsigned int IndexChar = 1; IndexChar < sortedCharWord.length(); ++IndexChar)
    {
        if(sortedCharWord.at(IndexChar) != sortedCharWord.at(IndexChar-1))
        {
            ++nbD;
        }
    }
    return nbD;
}

void computeMultiverseFromDict(vector<DictStruct> &dico,
                               vector<DictStruct> &anaSpace, DictStruct anagram)
{
    vector<string> multiverseSeed;
    for(unsigned i = 0; i<dico.size(); ++i)
    {// extract each word from the dictionnary
        multiverseSeed.push_back(dico[i].mot);
    }// compute all combiniation of thoses words 2^n
    vector<vector<string>> multiverse = combination(multiverseSeed, anagram.nbT);
    anaSpace = dico;
    anaSpace.reserve(pow(2, multiverseSeed.size()));
    for(unsigned i = 0; i<multiverse.size(); ++i)
    { // compute each combination and add it to the anagramme space of possibility
        DictStruct wordFromDict = computeStringFast(multiverse[i]);
        anaSpace.push_back(wordFromDict);
    }
}

vector<vector<string>> combination(vector<string> listOfString, unsigned int size)
{
    vector<vector<string>> output;
    output.reserve(pow(2, listOfString.size()));
    for(unsigned int i = 1; i < pow(2, listOfString.size()); ++i)
    {
        vector<string> subEntry;
        for(unsigned int j = 0; j < listOfString.size(); ++j)
        {
            if((i >> j) % 2)
            {
                subEntry.push_back(listOfString[j]);
            }
        }
        if(subEntry.size()>1)
        {
            unsigned int totalLenght = 0;
            for(auto word : subEntry)
            {
                totalLenght = totalLenght + word.length();
            }
            if(totalLenght == size)
            {
                output.push_back(subEntry);
            }
        }
    }
    return output;
}

void displayAnagram(vector<vector<string>> &listOfString)
{
    for(unsigned int j = 0; j<listOfString.size(); ++j)
    {
        for(unsigned int i = (listOfString[j].size()-1); i>0 ; --i)
        {
            cout << listOfString[j][i] + " ";
        }
        cout << listOfString[j][0] + "\n";
    }
}

int factorial(int n)
{
    if (n<=1)
    {
        return 1;
    }
    else if(n<10)
    {
        switch(n)
        {
            case 2  :
                return 2;
            case 3  :
                return 6;
            case 4  :
                return 24;
            case 5  :
                return 120;
            case 6  :
                return 720;
            case 7  :
                return 5040;
            case 8  :
                return 40320;
            case 9  :
                return 362880;
        }
    }
    return factorial(n-1)*n;
}

void permutation(vector<string> &listOfString, vector<vector<string>> &permutList, int z)
{
    if(listOfString.size() == 2)
    {
        permutList.push_back({listOfString[1],listOfString[0]});
        permutList.push_back({listOfString[0],listOfString[1]});
    }
    else if(listOfString.size() > 2)
    {
        for(unsigned int i = 0; i<listOfString.size(); ++i)
        {
            vector<string> sublist = listOfString;
            for(unsigned int j = i; j<(listOfString.size()-1); ++j)
            {// reconstruit la liste en enlevant le ieme element
                sublist[j]=listOfString[j+1];
            }
            sublist.pop_back();
            unsigned int factoria = factorial(sublist.size());
            permutation(sublist, permutList, z+i*factoria);
            for(unsigned int j = 0; j<factoria; ++j)
            {
                permutList[j+i*factoria+z].push_back(listOfString[i]);
            }
        }
    }
}

int compare(DictStruct structA, DictStruct structB)
{
        if(structA.nbT > structB.nbT)
        {
            return GREATER;
        }
        else if(structA.nbT == structB.nbT)
        {
            if(structA.nbD > structB.nbD)
            {
                return GREATER;
            }
            else if (structA.nbD == structB.nbD)
            {
                if(structA.sortedStringOnChar > structB.sortedStringOnChar)
                {
                    return GREATER;
                }
                else if(structA.sortedStringOnChar == structB.sortedStringOnChar)
                {
                    return EQUAL;
                }
                else
                {
                    return LOWER;
                }
            }
            else
            {
                return LOWER;
            }
        }
        else
        {
            return LOWER;
        }
}

int compareFast(DictStruct structA, DictStruct structB)
{
    if(structA.sortedStringOnChar == structB.sortedStringOnChar)
    {
        return EQUAL;
    }
    return GREATER;
}

vector<int> findAm(DictStruct anagram, vector<DictStruct> &dictionnary)
{
    vector<int> positionInIndex = {};
    for(unsigned int i = 0; i<dictionnary.size();++i)
    {
        int result = compareFast(anagram,dictionnary[i]);
        if(result == OK)
        {
            positionInIndex.push_back(i);
        }
    }
    return positionInIndex;
}

DictStruct concatenate(vector<DictStruct>const &list)
{
    string concatenated;
    for(unsigned int i = 0; i<list.size(); ++i)
    {
        concatenated = concatenated + list[i].mot;
    }
    return computeString(concatenated);
}

void anagram(vector<DictStruct> dict, DictStruct anagrammeStruct)
{
    vector<int> index = (findAm(anagrammeStruct, dict));
    cout << endl;
    for(unsigned int i=index.size()-1;i<index.size();i--)
    {
        int anaIndex = index[i];
        if(dict[anaIndex].sortedStringOnChar == anagrammeStruct.sortedStringOnChar)
        {
            if(dict[anaIndex].IsComposed)
            {
                vector<vector<string>> permuta;
                permuta.reserve(factorial(dict[anaIndex].subwords.size()));
                vector<string> sortedSubDict = dict[anaIndex].subwords;
                vector<string> wordSetComposingAnagram = sortnbTAlpha(sortedSubDict);
                permutation(wordSetComposingAnagram, permuta);
                displayAnagram(permuta);
            }
            else
            {
                vector<vector<string>> permuta = {{dict[anaIndex].mot}};
                displayAnagram(permuta);
            }
        }
    }
    if(index.size() == 0)
    {
        cout << NO_ANAGRAM << endl;
    }
}

unsigned int word2BinVect(string word)
{
    vector <bool> alphabet = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned int binVec = 0;
    for(unsigned int i = 0; i<word.length(); i++)
    {//for each letters, set to true the right letter in the vec (65= 'A')
        alphabet[(word.at(i)-65)] = true;
    }
    for(auto bit : alphabet)
    {
        binVec = binVec + bit; // add letter state as LSB 
        binVec = binVec << 1; // shift one by one every letter
    }//A is MSB, Z is LSB
    return binVec;
}

vector<DictStruct> trimDict(vector<DictStruct> dict, DictStruct anagram)
{
    unsigned int codex = word2BinVect(anagram.mot);
    vector<DictStruct> trimmedDict;
    for(auto entry : dict)
    {
        unsigned int word2test = word2BinVect(entry.mot);
        unsigned int intersection = word2test & codex; // intersection of both charset
        if((intersection & word2test) == word2test) // remove word that contain char
        { // that is not in the other word.
            trimmedDict.push_back(entry);
        }
    }
    return trimmedDict;
}

int main()
{
    //clock_t tStart = clock();
    ios_base::sync_with_stdio(false); // faster displaying
    vector<DictStruct> dictionnary;
    vector<DictStruct> anaSpace;
    lecture(dictionnary, true);
    displayDict(dictionnary);
    while(true)
    {
        vector<DictStruct> anagrammeSentence;
        DictStruct anagramme;
        int status = lecture(anagrammeSentence, false);
        anagramme = concatenate(anagrammeSentence);
        vector<DictStruct> trimmedDict = trimDict(dictionnary, anagramme);
        computeMultiverseFromDict(trimmedDict, anaSpace, anagramme);
        if(status == ENDOFENTRY)
        {
            if(anagramme.nbT != 0)
            {
                anagram(anaSpace, anagramme);
                //cout << "Time "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
            }
        }
    }
    return 0;
}
