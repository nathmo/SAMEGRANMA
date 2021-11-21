#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>
using namespace std;

const string NOT_IN_CAPITAL_LETTERS("The word is not purely in capital letters");
const string DOUBLE_WORD("Each word can be present only once");
const string EMPTY_DICO("The dictionnary cannot be empty");
const string NO_ANAGRAM("There is no anagram for this message and this dictionary");

struct DictStruct {
	unsigned int nbT;
	unsigned int nbD;
	bool IsComposed = false;
	string mot;
	string sortedStringOnChar;
	vector<string> subwords;
};

/*
 * Lis les paramètre sur l'entrée standard (cin)
 * 
 * Entree : Adresse tableau struct
 * Sortie : code de status (affiche d'éventuelle code d'erreur)
*/
int lecture(vector<DictStruct> &listOfDictStruct, bool exitIfError);

/*
 * affiche le dictionnaire
 * 
 * Entree : Adresse tableau struct
 * Sortie : rien, affiche le dictionnaire
*/
void displayDict(vector<DictStruct> &dictionnary);
void displayDebug(vector<DictStruct> &dictionnary);

/*
 * test que la string respectent les conditions demandé
 * (en majuscule et pas dans le dictionnaire)
 * 
 * Entree : Adresse tableau struct
 * Sortie : code d'erreur
 * 0 = l'entrée respecte les conditions
 * 1 = not fully in capital
 * 2 = the word is aldready in the dictionnary
*/
int test(DictStruct structStringEntree, int &index,
			vector<DictStruct> &listOfDictStruct);

/*
 * test si une string est entièrement en majuscule
 * 
 * Entree : String à tester
 * Sortie : vrai si entièrement en majuscule sinon faux
*/
bool isCaps(string stringEntree);

/*
 * test si une string est déja dans le dictionnaire
 * et retourne par référence son index pour pas le chercher deux fois
 * (dans le cas ou il n'est pas dans le dico, l'index représente la
 * position ou il devrais être rangé
 * 
 * Entrée : String à tester
 * Sortie : Booléen vrai si le mots n'est pas dans l'index, faux sinon
 * 
*/
bool notInDict(DictStruct structStringEntree, int &index,
				vector<DictStruct> &listOfDictStruct);


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
int findPos(DictStruct structStringEntree, vector<DictStruct> &listOfDictStruct);
int findPos(string stringEntree, vector<string> &listOfString);

/*
 * Insere le struct donné a la position donné dans la liste de struct
 * passé par référence
 * 
 * Entrée : index, structToInsert, &listOfDictStruct
 * Sortie : rien
 * 
*/
void insert(int index, DictStruct structToInsert
				, vector<DictStruct> &listOfDictStruct);

/*
 * Trie la string / liste de string
 * par ordre alphabétique (sur  les char dans le cas de la string
 * 
 * Entrée : list de DictStruct /liste de string / string
 * Sortie : sorted List de string / string
 * 
*/
vector<string> sort(vector<string> list);
string sort(string list);
vector<DictStruct> sort(vector<DictStruct> &list);
/*
 * compute different value related to the string
 * 
 * Entrée : string
 * Sortie : structDict
 * 
*/
DictStruct computeString(string word);

/*
 * computeNbD
 * 
 * Entrée : string
 * Sortie : nombreDeCharUnique
 * 
*/
int computeNbD(string word);

/*
 * compute every combination of the words in the dict
 * and update the dictionnary with the permutation
 * so we have a nice and fast lookup table of every possible anagram
 * 
 * Entrée : listOfStruct
 * Sortie : rien (update via référence)
 * 
*/
void computeMultiverseFromDict(vector<DictStruct> &listOfStruct);

/*
 * Anagram, retourne les anagramme possible en fonction du dico et des
 * string fournit en entrée
 * 
 * Entrée : listOfStruct listOfString
 * Sortie : rien, affiche les anagrammes
 * 
*/
void Anagram(vector<DictStruct> dict, vector<DictStruct> anagram);

/*
 * affiche les permutation d'une liste de string
 * 
 * Entrée : listOfString
 * Sortie : rien (affiche les resultats)
 * 
*/
void displayAnagram(vector<string> &listOfString);

/*
 * compute les permutations des elements d'une liste
 * 
 * Entrée : liste
 * Sortie : liste de permutation(list) (par référence)
 * 
*/
void permutation(vector<string> &listOfString, vector<vector<string>> &permut, int z=0);

/*
 * compute les combinaisons des elements d'une liste
 * 
 * Entrée : liste
 * Sortie : liste de combinaison(list)
 * 
*/
vector<vector<string>> combination(vector<string> listOfString);

int factorial(int n);

int lecture(vector<DictStruct> &listOfDictStruct, bool exitIfError)
{
	string entree;
	cin >> entree;
	int status = 0;
	bool firstNotInCapital = true;
	while(entree != "." and entree != "*")
	{
		int index = 0;
		DictStruct wordFromDict = computeString(entree);
		status = test(wordFromDict, index, listOfDictStruct);
		if(status == 0)
		{
			insert(index, wordFromDict, listOfDictStruct);
		}
		else if(status == 1)
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
		else if((status == 2) and exitIfError)
		{
			cout << DOUBLE_WORD << endl;
			exit(0); // quit if aldready in dict (dont apply for anagra)
		}
		cin >> entree;
	}
	if(entree == "*")
	{
		exit(0);
	}
	if(entree == ".")
	{
		return 3;
	}
	return status;
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

int test(DictStruct structStringEntree, int &index,
			vector<DictStruct> &listOfDictStruct)
{
	if(not isCaps(structStringEntree.mot))
	{
		return 1;
	}
	else if(not notInDict(structStringEntree, index, listOfDictStruct))
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

bool isCaps(string stringEntree)
{
	for(unsigned int charIndex = 0; charIndex<stringEntree.length(); charIndex++)
	{
		if(stringEntree.at(charIndex)<65 or stringEntree.at(charIndex)>90)
		{
			return false;
		}
	}
	return true;
}

bool notInDict(DictStruct structStringEntree, int &index,
				vector<DictStruct> &listOfDictStruct)
{
	index = findPos(structStringEntree, listOfDictStruct);
	if(index == 0)
	{
		return true;
	}
	if(listOfDictStruct[index].mot == structStringEntree.mot)
	{ // check si la string est la meme -> dans le dico
		return false;
	} // sinon c'est l'adresse a l'aquelle il devra etre rangé et
	// retour par référence
	return true;	
}

int findPos(DictStruct structStringEntree, vector<DictStruct> &listOfDictStruct)
{
	unsigned int positionInIndex = 0;
	bool wordDontMatch = true;
	while(wordDontMatch)
	{
		if(listOfDictStruct.size() <= positionInIndex)
		{
			return (positionInIndex); // prevent segfault
		}
		unsigned int nbT = listOfDictStruct[positionInIndex].nbT;
		unsigned int nbD = listOfDictStruct[positionInIndex].nbD;
		string word = listOfDictStruct[positionInIndex].mot;
		string alpha = listOfDictStruct[positionInIndex].sortedStringOnChar;
		if(structStringEntree.nbT > nbT)
		{
			++positionInIndex;
		}
		else if(structStringEntree.nbT == nbT)
		{
			if(structStringEntree.nbD > nbD)
			{
				++positionInIndex;
			}
			else if (structStringEntree.nbD == nbD)
			{
				if(structStringEntree.sortedStringOnChar > alpha)
				{
					++positionInIndex;
				}
				else if(structStringEntree.sortedStringOnChar == alpha)
				{
					if(structStringEntree.mot > word)
					{
						++positionInIndex;
					}
					else
					{
						wordDontMatch = false;
					}
				}
				else
				{
					wordDontMatch = false;
				}
			}
			else
			{
				wordDontMatch = false;
			}
		}
		else
		{
			wordDontMatch = false;
		}
	}
	return positionInIndex;
}

int findPos(string stringEntree, vector<string> &listOfString)
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

void insert(int index, DictStruct structToInsert
				, vector<DictStruct> &listOfDictStruct)
{
	vector<DictStruct> tempListOfDictStruct;
	DictStruct tempA = structToInsert;
	unsigned int idx(index);
	// attention <= car taille apres ajout = taille + 1
	if(idx < listOfDictStruct.size())
	{
		for(unsigned int i = idx; i<(listOfDictStruct.size()); ++i)
		{
			if(i == idx)
			{
				tempA = listOfDictStruct[i];
				listOfDictStruct[i] = structToInsert;
			}
			else
			{
				DictStruct tempB = listOfDictStruct[i];
				listOfDictStruct[i] = tempA;
				tempA = tempB;
			}
		}
	}
	listOfDictStruct.push_back(tempA);
	return;
}

void insert(int index, string stringToInsert
				, vector<string> &listOfString)
{
	vector<string> tempListOfString;
	unsigned int idx(index);
	for(unsigned int i = 0; i<=listOfString.size(); ++i)
	{
		if(i < idx)
		{
			tempListOfString.push_back(listOfString[i]);
		}
		else if(i == idx)
		{
			tempListOfString.push_back(stringToInsert);
		}
		else if(i > idx)
		{
			tempListOfString.push_back(listOfString[i-1]);
		}
	}
	listOfString = tempListOfString;
}

vector<DictStruct> sort(vector<DictStruct> &list)
{
	// UTILISER TRI DOCHOTOMIQUE
	clock_t tStart = clock();
	cout << "Time01 taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	vector<DictStruct> sortedList;
	for(unsigned int elemIndex = 0; elemIndex < list.size(); ++elemIndex)
	{
		cout << "Time00 taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
		unsigned int newIndex = findPos(list[elemIndex], sortedList); // 1.2 ms
		cout << "Time01 taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
		insert(newIndex, list[elemIndex], sortedList); // 9.3 ms
	}
	cout << "Time02 taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	return sortedList;
}

vector<string> sort(vector<string> list)
{
	vector<string> sortedList;
	for(unsigned int elemIndex = 0; elemIndex < list.size(); ++elemIndex)
	{
		unsigned int newIndex = findPos(list[elemIndex], sortedList);
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
	dictEntry.nbD = computeNbD(word);
	dictEntry.sortedStringOnChar = sort(word);

	return dictEntry;
}

DictStruct computeString(vector <string> Multiword)
{
	DictStruct dictEntry;
	dictEntry.IsComposed = true;
	dictEntry.subwords = Multiword;
	string word;
	for(unsigned int i = 0;i < Multiword.size(); ++i)
	{
		word = word + Multiword[i];
	}
	dictEntry.mot = word;
	dictEntry.nbT = word.length();
	dictEntry.nbD = computeNbD(word);
	dictEntry.sortedStringOnChar = sort(word);

	return dictEntry;
}

int computeNbD(string word)
{
	int nbD = 1;
	string sortedString(sort(word));
	if(sortedString.length() == 0)
	{ // check for edgecase where string is empty
		return 0;
	}
	for(unsigned int IndexChar = 1; IndexChar < sortedString.length(); ++IndexChar)
	{
		if(sortedString.at(IndexChar) != sortedString.at(IndexChar-1))
		{
			++nbD;
		}
	}
	return nbD;	
}

void computeMultiverseFromDict(vector<DictStruct> &listOfStruct)
{
	vector<string> multiverseSeed;
	// extract each word from the dictionnary
	for(unsigned i = 0; i<listOfStruct.size(); ++i)
	{
		multiverseSeed.push_back(listOfStruct[i].mot);
	}
	vector<vector<string>> multiverse = combination(multiverseSeed);
	for(unsigned i = 0; i<multiverse.size(); ++i)
	{ // insert earh wor(l)d from the multiverse in the dictionnary
		DictStruct wordFromDict = computeString(multiverse[i]);
		int index = findPos(wordFromDict, listOfStruct);
		insert(index, wordFromDict, listOfStruct);
	}
}

vector<vector<string>> combination(vector<string> listOfString)
{
	vector<vector<string>> output;
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
			output.push_back(subEntry);
		}
	}
	return output;
}

void displayAnagram(vector<vector<string>> &listOfString)
{
	for(unsigned int j = 0; j<listOfString.size(); ++j)
	{
		string stringToOut;
		for(unsigned int i = 0; i<(listOfString[j].size()-1); ++i)
		{
			stringToOut = stringToOut + listOfString[j][(listOfString[j].size()-1)-i] + " ";
		}
		stringToOut = stringToOut + listOfString[j][0] + '\n';
		cout << stringToOut;
	}
}

int factorial(int n) {
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

void permutation(vector<string> &listOfString, vector<vector<string>> &permut, int z)
{
	if(listOfString.size() == 2)
	{
		permut.push_back({listOfString[1],listOfString[0]});
		permut.push_back({listOfString[0],listOfString[1]});
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
			permutation(sublist, permut, z+i*factorial(sublist.size()));
			for(unsigned int j = 0; j<unsigned(factorial(sublist.size())); ++j)
			{
				permut[j+i*factorial(sublist.size())+z].push_back(listOfString[i]);
			}
		}
	}
}

vector<int> findAm(DictStruct structStringEntree, vector<DictStruct> &listOfDictStruct)
{
	vector<int> positionInIndex = {};
	for(unsigned int i = 0; i<listOfDictStruct.size();++i)
	{
		unsigned int nbT = listOfDictStruct[i].nbT;
		string alpha = listOfDictStruct[i].sortedStringOnChar;
		if(structStringEntree.nbT == nbT)
		{
			if(structStringEntree.sortedStringOnChar == alpha)
			{
				positionInIndex.push_back(i);
			}
		}
	}
	return positionInIndex;
}

void Anagram(vector<DictStruct> dict, vector<DictStruct> anagram)
{
	string anagrammeInputConcat;
	for(unsigned int i = 0; i<anagram.size(); ++i)
	{
		anagrammeInputConcat = anagrammeInputConcat + anagram[i].mot;
	}
	DictStruct anagrammeStruct = computeString(anagrammeInputConcat);
	vector<DictStruct> debugPLZDeleteMe = {anagrammeStruct};
	vector<int> index = (findAm(anagrammeStruct, dict));
	cout << endl;
	for(auto anaIndex : index)
	{
		if(dict[anaIndex].sortedStringOnChar == anagrammeStruct.sortedStringOnChar)
		{
			if(dict[anaIndex].IsComposed)
			{
				vector<vector<string>> permuta;
				permuta.reserve(factorial(dict[anaIndex].subwords.size()));
				vector<string> wordSetComposingAnagram = sort(dict[anaIndex].subwords);
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
		cout << NO_ANAGRAM<< endl;
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	//clock_t tStart = clock();
	vector<DictStruct> dictionnary;
	int status = lecture(dictionnary, true);
	if(status==3)
	{
		if(dictionnary.size() == 0)
		{
			cout << EMPTY_DICO << endl;
			return 0;
		}
		displayDict(dictionnary);
		computeMultiverseFromDict(dictionnary);
		int status = 0;
		while(status !=4)
		{
			vector<DictStruct> anagramme;
			status = lecture(anagramme, false);
			if(status == 3)
			{
				if(anagramme.size() != 0)
				{
					Anagram(dictionnary, anagramme);
					//cout << "Total Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
				}
			}
		}
	}
	return 0;
}
