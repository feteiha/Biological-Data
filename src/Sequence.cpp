#include "Sequence.h"

using namespace std;

/**
        NOTES:-
                '\0' --> Put the end of the sequence
**/


Sequence::Sequence()
{
    /** Default constructor to initialize length of the sequence with zero **/
    this->seq_length = 0;
    this->valid = false;
}

Sequence::~Sequence()
{
    /** Default destructor to deallocate the memory of the sequence array **/
    if ( this->seq != nullptr )
    {
        //delete[] this->seq;
    }
}

Sequence::Sequence(int length)
{
    this->valid = false;
    /** parameterize constructor to initialize the length of the sequence **/
    this->seq = new char[ length ];
    this->seq_length = length;
}

Sequence::Sequence( char* SEQ , int sequence_length )
{
    /** parameterize constructor to initialize the sequence and it's length **/

    this->valid = false;
    this->seq_length = sequence_length;

    this->seq  = new char [ this->seq_length ];
    for ( int i = 0; i < this->seq_length; i++ )
        this->seq[ i ] = SEQ[ i ];

    this->seq[ this->seq_length ] = '\0';
}

Sequence::Sequence(Sequence& rhs)
{
    /** constructor to make DEEP copy of another object **/
    this->seq_length = rhs.getLength(); /// set length
    this->seq = new char [ this->seq_length ]; /// set size
    this->valid = rhs.getValid();       /// set valid flag


    /// deep copy process
    char * deepCopy_Seq = rhs.getSequence();
    for ( int i = 0; i < this->seq_length; i++ )
        this->seq[ i ] = deepCopy_Seq[ i ];
}

/** Setters and Getters **/
bool Sequence :: getValid()
{
        return this->valid;
}

char * Sequence :: getSequence()
{
    return this->seq;
}

int Sequence :: getLength()
{
    return this->seq_length;
}

int Sequence :: getStrLen()
{
    return strlen( this->seq );
}

/**
        Function to take input from user OR from file and calculate its length , then set the actual input
        after call valid function to validate the inpute before set it;
**/
void Sequence :: input()
{
    string choice = "" , fileName = "";
    bool stop = true;

    /// Check for input choice
    do
    {
        try
        {
            cout << "You need to enter the sequence from file or manually ?";
            cout << "\nfile(1) , manually(2) :  ";
            cin  >> choice;

            if ( choice != "1" && choice != "2" )
                throw "Invalid input";

            if      ( choice == "1" )       /// take the input from file
            {
                bool true_fileSyntax = true;

                do
                {
                    try
                    {
                        cout << "Enter the name of the file :  ";
                        cin  >> fileName;

                        if ( !isValid_fileSyntax( fileName ) )  /// check for file syntax is valid
                            throw "invalid syntax of the file name";

                        fstream infile;
                        infile.open( fileName.c_str() , ios::in );

                        if ( infile.fail() )                    /// check if file not open
                            throw "File not founded";

                        true_fileSyntax = false;
                    }
                    catch ( const char * file_error_message )
                    {
                        cout << "ERROR : " << file_error_message << endl;
                    }
                }
                while( true_fileSyntax == true );

                this->create_inputFile_from_file( fileName );
            }
            else if ( choice == "2" )       /// take the input form console screen
            {
                string SequenceStr = "" , typeStr = "";

                cout << "Enter the sequence :  " ;
                cin  >> SequenceStr;

                cout << "Enter the type     :  ";
                cin  >> typeStr;

                this->create_inputFile_from_input( SequenceStr , typeStr );
            }
            this->calculate_inputFile_length();     /// calculate length of the input
            this->readFromFile();                   /// virtual function to Set the input and valid it

            remove( this->tempFileName.c_str() );   /// delete the software default file
            stop = false;
        }
        catch ( const char * message )
        {
            cout << "ERROR : " << message << endl;
        }
    }
    while ( stop == true );

}
// pure virtual function that should be overridden because every
// type of sequence has its own details to be printed

///virtual void Print()= 0;

// friend function that will find the LCS (longest common
// subsequence) between 2 sequences of any type, according to
// polymorphism

/// validate functions
/**
        Valid function to check if the file name is suitable with the windows rules or not
        and check if the user input the file name with (.txt) extension
**/
bool Sequence :: isValid_fileSyntax( string& fileName )
{
    bool isValid = true;
    for ( int i = 0; i < (int)fileName.length(); i++ )
    {
        if ( fileName[i] == '\\' ) isValid = false;
        if ( fileName[i] == '/'  ) isValid = false;
        if ( fileName[i] == '*'  ) isValid = false;
        if ( fileName[i] == ':'  ) isValid = false;
        if ( fileName[i] == '?'  ) isValid = false;
        if ( fileName[i] == '<'  ) isValid = false;
        if ( fileName[i] == '>'  ) isValid = false;
        if ( fileName[i] == '"'  ) isValid = false;
        if ( fileName[i] == '|'  ) isValid = false;
    }

    if      ( (int)fileName.length() <  4 )
        fileName += ".txt";
    else if ( (int)fileName.length() == 4 )
    {
        if ( fileName == ".txt" )
            return false;
        else
            fileName += ".txt";
    }
    else
    {
        string temp = "";
        int len = (int)fileName.length();

        for ( int i =  len-4 ; i < len; i++ )
            temp += fileName[ i ];

        temp == ".txt" ? 0 : fileName += ".txt";
    }
    return isValid;
}

/**
        this is funtion to put the input data in default file to calculate the length of it
**/
void Sequence :: create_inputFile_from_input( string sequenceStr , string typeStr )
{
    sequenceStr = deleteSpace( sequenceStr ); /// delete all spaces if user enter

    bool isDigit_flag = true;
    for ( int i = 0; i < (int)sequenceStr.length(); i++ )
    {
        if ( isdigit( sequenceStr[i] ) == false )
            isDigit_flag = false;
    }

    ofstream tempFile;
    tempFile.open( this->tempFileName.c_str() , ios::out | ios::trunc );

    if ( isDigit_flag == true )
    {

        if ( (int)sequenceStr.length() % 2 == true )
        {
            sequenceStr += "000";
        }

        tempFile << typeStr << endl;
        for ( int i = 0; i < (int)sequenceStr.length(); i += 2 )
        {
            tempFile << sequenceStr[i] << sequenceStr[i+1] << " ";
        }

    }
    else
    {
        tempFile << typeStr << endl << sequenceStr;
    }
    tempFile.close();
}

/**
        this is funtion to put the input data took from file in default file to calculate the length of it
**/
void Sequence :: create_inputFile_from_file( string fileName )
{
    fstream infile;                             /// open the input file
    infile.open( fileName.c_str() , ios::in );

    fstream tempFile;                           /// open the default file
    tempFile.open( this->tempFileName.c_str() , ios::out | ios::trunc );

    try
    {
        if ( infile.fail() )                    /// if file not open throw error
            throw "file not founded";

        string inputStr = "";

        getline( infile , inputStr );           /// take all line from input file
        tempFile << inputStr << endl;

        getline( infile , inputStr );           /// Set all line in default file
        inputStr = deleteSpace( inputStr );

        bool isDigit_flag = true;               /// validate area for any length error ( for templates )
        for ( int i = 0; i < (int)inputStr.length(); i++ )
        {
            if ( isdigit( inputStr[i] ) == false )
                isDigit_flag = false;
        }

        if ( isDigit_flag == true )
        {
            if ( (int)inputStr.length() % 2 == true )
            {
                inputStr += "000";
            }

            for ( int i = 0; i < (int)inputStr.length(); i += 2 )
            {
                tempFile << inputStr[i] << inputStr[i+1] << " ";
            }
        }
        else
        {
            tempFile << inputStr;
        }

        infile.close();
        tempFile.close();
    }
    catch ( const char* openFile_error_message )
    {
        cout << "ERROR :  " << openFile_error_message << endl;
    }
}

/**
        function open the default file and loop on it + calculate the length of the input
        then set the length in the object to use it in array size
**/
void Sequence :: calculate_inputFile_length()
{
    fstream infile;
    infile.open( this->tempFileName.c_str() , ios::in );

    string input = ""; int counter = 0;
    getline( infile , input ); /// get type

    char inputSeq;
    while ( !infile.eof() )
    {
        infile >> inputSeq;
        counter++;
    }
    counter--;

    infile.close();
    this->seq_length = counter;                 /// set the length
    this->seq = new char [ this->seq_length ];  /// set size of char array
}

/**
        function for delete any spaces if the user enter it
**/
string Sequence :: deleteSpace( string inputStr )
{
    string str = "";
    for ( int i = 0; i < (int)inputStr.length(); i++ )
    {
        if ( inputStr[i] != ' ' )
            str += inputStr[i];
    }
    return str;
}

/// operator over loaded for printing
/**
    this operator call function print that's take ostream (COUT) as a default type or another type ( file name )
    and print the data
**/
ostream& operator << ( ostream& out , Sequence * s )
{
    s->Print( out );
    return out;
}

/// operator over loaded for input
/**
    this operator take the input form the user and set it in the default file , calculate the length
    of the input , then set it in the object after validate it;
**/
istream& operator >> ( istream& in  , Sequence * s )
{
    string typeStr = "" , sequenceStr = "";

    cout << "Enter the type of the sequence :  ";
    in >> typeStr;

    cout << "Enter the sequence : ";
    in >> sequenceStr;

    system( "CLS" );

    s->create_inputFile_from_input( sequenceStr , typeStr );
    s->calculate_inputFile_length();
    s->readFromFile();
    return in;
}

char* Align(Sequence * s1, Sequence * s2)
{
    int row = s1->seq_length +1;
    int col = s2->seq_length +1;
    int table[row][col] , k;

    for (int i=0; i < row; i++)
    {
        for (int j=0; j< col; j++)
        {
            if ( i==0 || j==0 ) //initiates first row and column to zero
            {
                table[i][j] = 0;
                //checker[i][j] = 'o';
            }
            else if (s1->seq[i-1] == s2->seq[j-1]) //checks if the letters match
            {
                table[i][j] = table[i-1][j-1] + 1;
            }
            else //put the greatest number from top cell or left cell
            {
                table[i][j] = max(table[i-1][j], table[i][j-1]); //puts the greater value between the top and the left cell in current cell
                /*if (table[i-1][j] > table [i][j-1])//top cell is greater than left cell
                {
                    table[i][j] = table [i-1][j]; //current cell is equal to top cell
                }
                else
                {
                    table[i][j] = table [i][j-1]; //if top cell isnt bigger then current cell is equal to left cell
                }
                */
            }
        }
    }
    int first_seq = s1->seq_length; //sets first_seq to the length of sequence
    int second_seq = s2->seq_length; //sets second_seq to the length of sequence
    k = table[first_seq][second_seq]; //sets k to the biggest value in the table which is the last
    char * LCS = new char[k+1];
    LCS[k] = '\0';
    while (first_seq > 0 && second_seq >0)
    {
        if (s1->seq[first_seq-1] == s2->seq[second_seq-1])
        {
            LCS[k-1]= s1->seq[first_seq-1];
            k--;
            first_seq--;
            second_seq--;
        }
        else if(table[first_seq-1][second_seq] > table[first_seq][second_seq-1])
        {
            first_seq--;
        }
        else
        {
            second_seq--;
        }
    }

    return LCS;
}


char* Sequence :: get_upper( char * seq , int len )
{
    for ( int i = 0; i < len; i++ )
    {
        if ( islower( seq[i] ) )
        {
            seq[i] = char( seq[i] - 32 );
        }
    }
    return seq;
}
