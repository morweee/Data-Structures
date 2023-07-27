#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdbool.h>
#include <vector>
#include <algorithm>
#include <chrono>

class book
{
    private:
        int isbn;
        std::string language, type;
    public:
        book(int isbn_value, std::string lang_value, std::string type_value)
        {
            isbn = isbn_value;
            language = lang_value;
            type = type_value;
        };
        int get_isbn(){ return isbn; }
        std::string get_lang(){ return language; }
        std::string get_type(){ return type; }
        // overloading < operator to compare book objects with rules:
        // smaller isbn number returns true, if isbn numbers are same
        // compare type with order new<used<digital
        // if types are same, compare language alphabetically
        bool operator< (book &book2)
        {
            if(isbn < book2.get_isbn()) return true;
            else if(isbn > book2.get_isbn()) return false;
            else if(isbn == book2.get_isbn())
            {
                if(type=="new" && book2.get_type()!=type) return true;
                else if (type=="used" && book2.get_type()=="new") return false;
                else if (type=="used" && book2.get_type()=="digital") return true;
                else if (type=="digital" && book2.get_type()!=type) return false;
                else if (type == book2.get_type())
                {
                    if(language.compare(book2.get_lang()) < 0) return true;
                }
            }
            return false;
        }
        bool operator== (book &book2)
        {
            if(isbn==book2.get_isbn() && language == book2.get_lang() 
                                      && type == book2.get_type()) return true;
            return false;
        }
};

// generic function to parse data from two input files
void parse_file(std::vector<book> &book_list, std::ifstream &input)
{
    std::string line;
    int line_no = 1;
    for (; std::getline(input, line); ++line_no)
    {   
        // first parse the isbn, language, and type from line, and store all in a vector
        std::vector<std::string> temp;
        std::string data;
        std::stringstream sstr(line);
        while (getline(sstr, data, ',')) temp.push_back(data);
        // store all info as a book object and push_back to the vector
        int isbn_num = std::stoi(temp[0]);
        book book_i(isbn_num, temp[1], temp[2]);
        book_list.push_back(book_i);
    }
}

// the iterative binary search function returns the number of req_book appears in the new_book
int BinarySearch(std::vector<book> &new_book, std::vector<book> &req_book)
{
    int count = 0;
    sort(new_book.begin(), new_book.end());
    for (auto &j: req_book)
    {
        unsigned int lower = 0;
        unsigned int upper = new_book.size()-1;
        unsigned int mid;
        while(lower<=upper)
        {
            mid = (lower+upper)/2;
            if(j == new_book.at(mid))
            {
                ++count;
                break;
            }
            if(j < new_book.at(mid)) upper = mid-1;
            else lower = mid+1;
        }
    }
    return count;
}

// the recursive binary search returns the number of req_book appears in the new_book
bool Recursive_BinarySearch(std::vector<book> &new_book, unsigned int lower,
    unsigned int upper, book &j)
{
    if(lower<=upper)
    {
        unsigned mid=(lower+upper)/2;
        if(j == new_book.at(mid)) 
        {
            return true;
        }
        if(j < new_book.at(mid)) return Recursive_BinarySearch(new_book, lower, mid-1, j);
        else return Recursive_BinarySearch(new_book, mid+1, upper, j);
    }
    return false;
}

int main(int argc, char** argv)
{   
    // argv[1] and argv[2] are "newbooks.dat" and "request.dat" respectively
    // argv[3],  or "found.dat" in default, stores the result output
    std::ifstream input_file1(argv[1]);
    std::ifstream input_file2(argv[2]);
    std::ofstream output_file(argv[3]);
    if (argc<4) std::ofstream output_file("found.dat");
    // error handling: 
    // if the wrong number of arguments is given or if the file(s) can't be opened,
    // exit the program and allow user to input again
    if (argc<3)
    {
        std::cerr << "Usage: .SearchNewBooks <newBooks.dat> <request.dat> <result_file.dat>\n";
        return -1;
    }
    if ( !input_file1 || !input_file2)
    {   
        if ( !input_file1) std::cerr << "Error: cannot open file " << argv[1] << std::endl;
        if ( !input_file2) std::cerr << "Error: cannot open file " << argv[2] << std::endl;
        return -1;
    }

    // create two vectors of book objects to store information of two input files
    std::vector<book> new_book;
    std::vector<book> req_book;

    // call the generic data parsing function for two input files
    parse_file(new_book, input_file1);
    parse_file(req_book, input_file2);
    
    // ask for user input to apply different search methods
    // input will be asked again if it's invalid
    char user_input;
    bool status=false;
    std::cout << "Choice of search method ([l]inear, [b]inary, [r]ecursiveBinary)?" << std::endl;
    while(status==false)
    {
        std::cin>>user_input;
        std::chrono::high_resolution_clock::time_point start;
        start = std::chrono::high_resolution_clock::now();
        switch(user_input)
        {
            //linear search
            case 'l':
            {
                int count=0;
                for(auto & i : req_book)
                {
                    for (auto & j: new_book)
                    {
                        if(i==j) ++count;
                    }
                }
                output_file << count << std::endl;
                status=true;
            }
            break;

            //binary search
            case 'b':
            {

                output_file << BinarySearch(new_book, req_book) << std::endl;
                status=true;
            }
            break;

            // recursive binary search
            case 'r':
            {
                int count=0;
                sort(new_book.begin(), new_book.end());
                for (auto &j: req_book)
                {
                    if(Recursive_BinarySearch(new_book, 0, new_book.size()-1, j)) count++;
                }
                output_file << count << std::endl;
                status=true;
            }    
            break;
        default:
            std::cerr<<"Incorrect choice"<<std::endl;
        }
        //after the switch statement, stop the time measurement
        //display the time in microseconds after compuataion is done
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed_us = std::chrono::duration<double, std::micro>(end - start).count();
        if(status) std::cout << "CPU time: " << elapsed_us << " microseconds"<<std::endl;
    }
}