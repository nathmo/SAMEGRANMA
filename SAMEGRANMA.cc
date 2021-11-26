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

int lecture(vector<DictStruct> &listOfDictStruct, bool exitIfError);

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

bool isCaps(string stringEntree);

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

void insert(int index, DictStruct structToInsert
				, vector<DictStruct> &listOfDictStruct);

vector<string> sort(vector<string> list);
string sort(string list);
vector<DictStruct> sort(vector<DictStruct> &list);
/*
 * compute different value related to the string (nbT, nbD, alpha)
 * 
 * Entrée : string
 * Sortie : structDict
 * 
*/
DictStruct computeString(string word);

int computeNbD(string sortedCharWord);

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

void displayAnagram(vector<string> &listOfString);

void permutation(vector<string> &listOfString, vector<vector<string>> &permut,int z=0);

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
		if(positionInIndex >= listOfDictStruct.size())
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
  dictEntry.sortedStringOnChar = sort(word);
	dictEntry.nbD = computeNbD(dictEntry.sortedStringOnChar);

	return dictEntry;
}

DictStruct computeString(vector <string> Multiword)
{
  //clock_t tStart = clock();
	//cout << "TimeCompute1 : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	DictStruct dictEntry;
	dictEntry.IsComposed = true;
	dictEntry.subwords = Multiword;
	string word;
  //cout << "TimeCompute2 : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	for(unsigned int i = 0;i < Multiword.size(); ++i)
	{
		word = word + Multiword[i];
	}
  //cout << "TimeCompute3 : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	dictEntry.mot = word;
  //cout << "TimeCompute4 : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	dictEntry.nbT = word.length();
  //cout << "TimeCompute5 : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
  dictEntry.sortedStringOnChar = sort(word);
  //cout << "TimeCompute6 : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
  dictEntry.nbD = computeNbD(dictEntry.sortedStringOnChar);
  //cout << "TimeCompute7 : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
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

void computeMultiverseFromDict(vector<DictStruct> &listOfStruct)
{
	//clock_t tStart = clock();
	//cout << "Time : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	vector<string> multiverseSeed;
	// extract each word from the dictionnary
	for(unsigned i = 0; i<listOfStruct.size(); ++i)
	{
		multiverseSeed.push_back(listOfStruct[i].mot);
	}
	vector<vector<string>> multiverse = combination(multiverseSeed);
	for(unsigned i = 0; i<multiverse.size(); ++i)
	{ // insert earh wor(l)d from the multiverse in the dictionnary
		//clock_t tStart = clock();
		DictStruct wordFromDict = computeString(multiverse[i]);
		//optimize compute and sort
		//cout << "TimeA : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
		//int index = findPos(wordFromDict, listOfStruct);
		//cout << "TimeB : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
		//insert(index, wordFromDict, listOfStruct);
		listOfStruct.push_back(wordFromDict);
		//cout << "TimeC : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	}
  //cout << "TimeSub : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
}

void computeMultiverseFromDictOld(vector<DictStruct> &listOfStruct)
{
	//clock_t tStart = clock();
	//cout << "Time : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
	vector<string> multiverseSeed;
	// extract each word from the dictionnary
	for(unsigned i = 0; i<listOfStruct.size(); ++i)
	{
		multiverseSeed.push_back(listOfStruct[i].mot);
	}
	vector<vector<string>> multiverse = combination(multiverseSeed);
	for(unsigned i = 0; i<multiverse.size(); ++i)
	{ // insert earh wor(l)d from the multiverse in the dictionnary
	//clock_t tStart = clock();
		DictStruct wordFromDict = computeString(multiverse[i]);
		//optimize compute and sort
		//cout << "TimeA : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
		int index = findPos(wordFromDict, listOfStruct);
		//cout << "TimeB : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
		insert(index, wordFromDict, listOfStruct);
		//cout << "TimeC : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
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
		for(unsigned int i = (listOfString[j].size()-1); i>0 ; --i)
		{
      cout << listOfString[j][i] + " ";
		}
    cout << listOfString[j][0] + "\n";
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
			unsigned int factoria = factorial(sublist.size());
			permutation(sublist, permut, z+i*factoria);
			for(unsigned int j = 0; j<factoria; ++j)
			{
				permut[j+i*factoria+z].push_back(listOfString[i]);
			}
		}
	}
}

int compare(DictStruct structA, DictStruct structB)
{
		if(structA.nbT > structB.nbT)
		{
			return 1;
		}
		else if(structA.nbT == structB.nbT)
		{
			if(structA.nbD > structB.nbD)
			{
				return 1;
			}
			else if (structA.nbD == structB.nbD)
			{
				if(structA.sortedStringOnChar > structB.sortedStringOnChar)
				{
					return 1;
				}
				else if(structA.sortedStringOnChar == structB.sortedStringOnChar)
				{
			return 0;
				}
				else
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
}

vector<int> findAm(DictStruct structStringEntree, vector<DictStruct> &listOfDictStruct)
{
	vector<int> positionInIndex = {};
	for(unsigned int i = 0; i<listOfDictStruct.size();++i)
	{
		int result = compare(structStringEntree,listOfDictStruct[i]);
		if(result == 0)
		{
			positionInIndex.push_back(i);
		}
	}
	return positionInIndex;
}

vector<int> findAmOld(DictStruct structStringEntree, vector<DictStruct> &listOfDictStruct)
{
	vector<int> positionInIndex = {};
	for(unsigned int i = 0; i<listOfDictStruct.size();++i)
	{
		int result = compare(structStringEntree,listOfDictStruct[i]);
		if(result == 0)
		{
			positionInIndex.push_back(i);
		}
		else if(result<0)
		{
			break;
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
	//vector<DictStruct> test = {anagrammeStruct};
	//displayDebug(test);
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
				permuta.reserve(factorial(dict[anaIndex].subwords.size())); //space for the vec
				vector<string> wordSetComposingAnagram = sortnbTAlpha(dict[anaIndex].subwords);
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
	ios_base::sync_with_stdio(false); // faster displaying
	clock_t tStart = clock();
	//cout << "Time : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
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
    cout << "Time : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
		//optimize compute multiverse sorting find smart trick to not sort it.
		computeMultiverseFromDict(dictionnary);
    cout << "Time : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
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
					cout << "Time : "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< endl;
				}
			}
		}
	}
	return 0;
}
